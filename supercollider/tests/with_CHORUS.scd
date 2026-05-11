Server.default.options.outDevice = nil;
Server.default.options.numOutputBusChannels = 2;
Server.default.options.memSize = 8192 * 16;

s.boot;

s.waitForBoot({

    MIDIClient.init;
    MIDIIn.connectAll;

    "SuperCollider server booted".postln;

    // =========================
    // GLOBAL STATE
    // =========================

    ~notes = Array.newClear(128);
    ~releasedNotes = Array.newClear(128);
    ~sustainActive = false;

    ~lastGestureTime = AppClock.seconds;

    ~gestureData = (
        openness: 0.0,
        thumb: 0.0
    );

    // =========================
    // SYNTHDEF (PAD + CHORUS)
    // =========================

    SynthDef(\fatPad, {
        |freq = 440, amp = 0.3, gate = 1,
         cutoff = 1000, rq = 0.5,

         chorusMix = 0.0,
         chorusDepth = 0.5,
         chorusRate = 0.15|

        var osc1, osc2, signal, env, cutoffCtrl;

        // Chorus vars
        var lfo1, lfo2, lfo3;
        var d1, d2, d3;
        var chorusSignal;

        env = EnvGen.kr(
            Env.adsr(0.05, 0.4, 0.6, 0.8),
            gate,
            doneAction: 2
        );

        osc1 = LFSaw.ar(freq);
        osc2 = LFSaw.ar(freq * 1.005);

        signal = Mix([osc1, osc2]) * 0.5;

        signal = signal + DelayC.ar(
            signal,
            0.02,
            SinOsc.kr(0.1).range(0.01, 0.02)
        );

        cutoffCtrl = Lag.kr(cutoff.clip(20, 20000), 0.05);
        signal = RLPF.ar(signal, cutoffCtrl, rq);

        // ================= CHORUS =================

        lfo1 = SinOsc.kr(chorusRate, 0).range(0, chorusDepth);
        lfo2 = SinOsc.kr(chorusRate, 2pi/3).range(0, chorusDepth);
        lfo3 = SinOsc.kr(chorusRate, 4pi/3).range(0, chorusDepth);

        d1 = lfo1.linlin(0, 1, 0.01, 0.03);
        d2 = lfo2.linlin(0, 1, 0.01, 0.03);
        d3 = lfo3.linlin(0, 1, 0.01, 0.03);

        chorusSignal = Mix([
            DelayC.ar(signal, 0.05, d1),
            DelayC.ar(signal, 0.05, d2),
            DelayC.ar(signal, 0.05, d3)
        ]) / 3;

        signal = XFade2.ar(
            signal,
            chorusSignal,
            chorusMix.linlin(0, 1, -1, 1)
        );

        Out.ar(0, Pan2.ar(signal * env * amp, 0));

    }).add;

    s.sync;

    // =========================
    // MIDI
    // =========================

    MIDIdef.cc(\sustainPedal, { |val, num|
        if (num == 64) {
            if (val >= 64) {
                ~sustainActive = true;
            } {
                ~sustainActive = false;
                ~releasedNotes.do { |synth, i|
                    if (synth.notNil) {
                        synth.set(\gate, 0);
                        ~releasedNotes[i] = nil;
                    }
                };
            };
        };
    });

    MIDIdef.noteOn(\noteOnPad, { |vel, num|
        if (~notes[num].notNil) { ~notes[num].set(\gate, 0) };
        if (~releasedNotes[num].notNil) {
            ~releasedNotes[num].set(\gate, 0);
            ~releasedNotes[num] = nil;
        };

        ~notes[num] = Synth(\fatPad, [
            \freq, num.midicps,
            \amp, vel.linlin(0, 127, 0, 0.4),
            \cutoff, vel.linexp(0, 127, 300, 7000),
            \rq, 0.5
        ]);
    });

    MIDIdef.noteOff(\noteOffPad, { |vel, num|
        if (~sustainActive) {
            ~releasedNotes[num] = ~notes[num];
            ~notes[num] = nil;
        } {
            ~notes[num].set(\gate, 0);
            ~notes[num] = nil;
        };
    });

    // =========================
    // OSC
    // =========================

    OSCdef(\leftHandOpen, { |msg|
        ~gestureData[\openness] = msg[1].asFloat.clip(0, 1);
        ~lastGestureTime = AppClock.seconds;
    }, '/hand/left/open', recvPort: 57130);

    OSCdef(\leftThumb, { |msg|
        ~gestureData[\thumb] = msg[1].asFloat.clip(0, 1);
        ~lastGestureTime = AppClock.seconds;
    }, '/hand/left/thumb', recvPort: 57130);

    // =========================
    // CONTROL ROUTINE
    // =========================

    ~controlRoutine = Routine({
        loop {
            var elapsed, cutoff, thumb;
            var mix, depth, rate;

            elapsed = AppClock.seconds - ~lastGestureTime;

            if (elapsed > 0.3) {
                cutoff = 200;
                thumb = 0;
            } {
                cutoff = ~gestureData[\openness].linexp(0, 1, 200, 8000);
                thumb = ~gestureData[\thumb];
            };

            mix   = thumb;
            depth = thumb;
            rate  = thumb.linexp(0, 1, 0.08, 0.3);

            (~notes ++ ~releasedNotes).do { |synth|
                if (synth.notNil) {
                    synth.set(
                        \cutoff, cutoff,
                        \chorusMix, mix,
                        \chorusDepth, depth,
                        \chorusRate, rate
                    );
                };
            };

            0.02.wait;
        }
    }).play(AppClock);

});
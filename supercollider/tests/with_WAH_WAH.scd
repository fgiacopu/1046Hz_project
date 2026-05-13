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
    // SYNTHDEF (PAD + WAH)
    // =========================

    SynthDef(\fatPad, {
        |freq = 440, amp = 0.3, gate = 1,

         // Base tone
         cutoff = 1000,

         // WAH parameters
         wahFreq = 800,
         wahRQ = 0.25|

        var osc1, osc2, signal, env;
        var cutoffCtrl, wahFreqCtrl, wahRQCtrl;

        env = EnvGen.kr(
            Env.adsr(0.05, 0.4, 0.6, 0.8),
            gate,
            doneAction: 2
        );

        osc1 = LFSaw.ar(freq);
        osc2 = LFSaw.ar(freq * 1.005);

        signal = Mix([osc1, osc2]) * 0.5;

        // Gentle movement / analog feel
        signal = signal + DelayC.ar(
            signal,
            0.02,
            SinOsc.kr(0.1).range(0.01, 0.02)
        );

        // Base tone control (hand openness)
        cutoffCtrl = Lag.kr(cutoff.clip(50, 12000), 0.08);
        signal = LPF.ar(signal, cutoffCtrl);

        // ================= WAH FILTER =================

        wahFreqCtrl = Lag.kr(wahFreq.clip(200, 6000), 0.03);
        wahRQCtrl   = Lag.kr(wahRQ.clip(0.1, 0.9), 0.03);

        signal = RLPF.ar(
            signal,
            wahFreqCtrl,
            wahRQCtrl
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
            \cutoff, vel.linexp(0, 127, 300, 7000)
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
            var wahFreq, wahRQ;

            elapsed = AppClock.seconds - ~lastGestureTime;

            if (elapsed > 0.3) {
                cutoff = 300;
                thumb = 0;
            } {
                cutoff = ~gestureData[\openness].linexp(0, 1, 300, 8000);
                thumb = ~gestureData[\thumb];
            };

            // WAH mapping (very expressive)
            wahFreq = thumb.linexp(0, 1, 300, 5000);
            wahRQ   = thumb.linlin(0, 1, 0.15, 0.8);

            (~notes ++ ~releasedNotes).do { |synth|
                if (synth.notNil) {
                    synth.set(
                        \cutoff, cutoff,
                        \wahFreq, wahFreq,
                        \wahRQ, wahRQ
                    );
                };
            };

            0.02.wait;
        }
    }).play(AppClock);

});
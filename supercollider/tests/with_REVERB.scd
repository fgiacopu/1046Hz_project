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
    // SYNTHDEF (PAD + REVERB)
    // =========================

    SynthDef(\fatPad, {
        |freq = 440, amp = 0.3, gate = 1,
         cutoff = 1000, rq = 0.5,

         reverbMix = 0.0,
         reverbRoom = 0.5,
         reverbDamp = 0.5|

        var osc1, osc2, signal, env, cutoffCtrl;
        var wet;

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

        // ================= REVERB =================

        wet = FreeVerb.ar(
            signal,
            mix: reverbMix,
            room: reverbRoom,
            damp: reverbDamp
        );

        Out.ar(0, Pan2.ar(wet * env * amp, 0));

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
            var mix, room, damp;

            elapsed = AppClock.seconds - ~lastGestureTime;

            if (elapsed > 0.3) {
                cutoff = 200;
                thumb = 0;
            } {
                cutoff = ~gestureData[\openness].linexp(0, 1, 200, 8000);
                thumb = ~gestureData[\thumb];
            };

            mix  = thumb.linlin(0, 1, 0.0, 0.6);   // mai 100% wet
            room = thumb.linlin(0, 1, 0.2, 0.9);
            damp = thumb.linlin(0, 1, 0.3, 0.8);

            (~notes ++ ~releasedNotes).do { |synth|
                if (synth.notNil) {
                    synth.set(
                        \cutoff, cutoff,
                        \reverbMix, mix,
                        \reverbRoom, room,
                        \reverbDamp, damp
                    );
                };
            };

            0.02.wait;
        }
    }).play(AppClock);

});
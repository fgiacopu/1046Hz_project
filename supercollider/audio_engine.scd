Server.default.options.outDevice = nil;
Server.default.options.numOutputBusChannels = 2;
Server.default.options.memSize = 8192 * 16;

s.boot;

s.waitForBoot({

    MIDIClient.init;
    MIDIIn.connectAll;

    "SuperCollider server booted".postln;

    ~notes = Array.newClear(128);
    ~releasedNotes = Array.newClear(128);
    ~sustainActive = false;

    ~gestureData = (
        openness: 0.0,
        thumb: 0.0,
        rightOpen: 0.0,
        rThumb: 0.0,

        attack: 0.05,
        decay: 0.4,
        sustaain: 0.6,
        releasee: 0.8
    );

    SynthDef(\fatPad, {
        |freq = 440, amp = 0.3, gate = 1,
         cutoff = 1000, rq = 0.5,

         attack = 0.05, decay = 0.4, sustaain = 0.6, releasee = 0.8,

         flangerMix = 0.0, flangerDepth = 0.5, flangerRate = 0.2,
         chorusMix = 0.0, chorusDepth = 0.5, chorusRate = 0.15,

         distMix = 0.0, distAmount = 1.0|

        var osc1, osc2, signal, env;
        var cutoffCtrl;

        var lfoF, delayTimeF, flanged;
        var lfo1, lfo2, lfo3, d1, d2, d3, chorusSignal;

        var drive, distorted;

        env = EnvGen.kr(
            Env.adsr(attack, decay, sustaain, releasee),
            gate,
            doneAction: 2
        );

        osc1 = LFSaw.ar(freq);
        osc2 = LFSaw.ar(freq * 1.005);
        signal = Mix([osc1, osc2]) * 0.5;

        signal = signal + DelayC.ar(signal, 0.02, SinOsc.kr(0.1).range(0.01, 0.02));

        cutoffCtrl = Lag.kr(cutoff.clip(20, 20000), 0.05);
        signal = RLPF.ar(signal, cutoffCtrl, rq);

        // FLANGER
        lfoF = SinOsc.kr(flangerRate).range(0, flangerDepth);
        delayTimeF = lfoF.linlin(0, 1, 0.001, 0.008);
        flanged = DelayC.ar(signal, 0.01, delayTimeF);
        signal = XFade2.ar(signal, flanged, flangerMix.linlin(0, 1, -1, 1));

        // CHORUS
        lfo1 = SinOsc.kr(chorusRate).range(0, chorusDepth);
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

        signal = XFade2.ar(signal, chorusSignal, chorusMix.linlin(0, 1, -1, 1));

		  // DISTORTION
        drive = distAmount;

        distorted = signal * (drive * 2);

        distorted = tanh(distorted);

        distorted = (distorted * 3).clip2(0.7);

        distorted = (distorted * 1.5).fold2(0.8);

        distorted = LPF.ar(distorted, cutoffCtrl * 1.3);

        signal = XFade2.ar(
            signal,
            distorted,
            distMix.linlin(0, 1, -1, 1)
        );

        Out.ar(0, Pan2.ar(signal * env * amp, 0));

    }).add;

    s.sync;

    // MIDI
	 ~processing = NetAddr("127.0.0.1", 9001);

	 MIDIdef.noteOn(\noteOnPad, { |vel, num|
		  ~processing.sendMsg("/noteON", num, vel);
        if (~notes[num].notNil) { ~notes[num].set(\gate, 0) };
        if (~releasedNotes[num].notNil) {
            ~releasedNotes[num].set(\gate, 0);
            ~releasedNotes[num] = nil;
        };

        ~notes[num] = Synth(\fatPad, [
            \freq, num.midicps,
            \amp, vel.linlin(0, 127, 0, 0.4),
            \cutoff, vel.linexp(0, 127, 300, 7000),

            \attack, ~gestureData[\attack],
            \decay, ~gestureData[\decay],
            \sustaain, ~gestureData[\sustaain],
            \releasee, ~gestureData[\releasee]
        ]);
    });

    MIDIdef.noteOff(\noteOffPad, { |vel, num|
        ~notes[num].set(\gate, 0);
        ~notes[num] = nil;
    });

    // OSC
	 OSCdef(\attack, { |msg| ~gestureData[\attack] = msg[1].asFloat; }, "/attack", recvPort: 57130);
    OSCdef(\decay, { |msg| ~gestureData[\decay] = msg[1].asFloat; }, "/decay", recvPort: 57130);
    OSCdef(\sustaain, { |msg| ~gestureData[\sustaain] = msg[1].asFloat; }, "/sustaain", recvPort: 57130);
    OSCdef(\releasee, { |msg| ~gestureData[\releasee] = msg[1].asFloat; }, "/releasee", recvPort: 57130);

    OSCdef(\leftHandOpen, { |msg|
        ~gestureData[\openness] = msg[1].clip(0, 1);
    }, '/hand/left/open', recvPort: 57130);

    OSCdef(\leftThumb, { |msg|
        ~gestureData[\thumb] = msg[1].clip(0, 1);
    }, '/hand/left/thumb', recvPort: 57130);

    OSCdef(\rightHandOpen, { |msg|
        ~gestureData[\rightOpen] = msg[1].clip(0, 1);
    }, '/hand/right/open', recvPort: 57130);

    OSCdef(\rightThumb, { |msg|
        ~gestureData[\rThumb] = msg[1].clip(0, 1);
    }, '/hand/right/thumb', recvPort: 57130);

    // CONTROL
    ~controlRoutine = Routine({
        loop {
            var cutoff, thumb, right, rThumb;
            var fMix, fDepth, fRate;
            var cMix, cDepth, cRate;
            var dMix, dAmt;

            cutoff = ~gestureData[\openness].linexp(0, 1, 200, 8000);

            thumb  = ~gestureData[\thumb];
            right  = ~gestureData[\rightOpen];
            rThumb = ~gestureData[\rThumb];

            fMix   = thumb;
            fDepth = thumb;
            fRate  = thumb.linexp(0, 1, 0.05, 0.5);

            cMix   = right;
            cDepth = right;
            cRate  = right.linexp(0, 1, 0.08, 0.3);

            // DIST
            dMix = rThumb.pow(0.8);
            dAmt = rThumb.linexp(0, 1, 1, 12);

            (~notes).do { |synth|
                if (synth.notNil) {
                    synth.set(
                        \cutoff, cutoff,

                        \flangerMix, fMix,
                        \flangerDepth, fDepth,
                        \flangerRate, fRate,

                        \chorusMix, cMix,
                        \chorusDepth, cDepth,
                        \chorusRate, cRate,

                        \distMix, dMix,
                        \distAmount, dAmt,

						      \attack, ~gestureData[\attack],
                        \decay, ~gestureData[\decay],
                        \sustaain, ~gestureData[\sustaain],
                        \releasee, ~gestureData[\releasee]
                    );
                };
            };

            0.02.wait;
        }
    }).play(AppClock);

});
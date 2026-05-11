Server.default.options.outDevice = nil;          // Disable explicit audio output device selection
Server.default.options.numOutputBusChannels = 2; // Force the server to allocate exactly 2 output audio channels
Server.default.options.memSize = 8192 * 16;      // Increase server memory

s.boot;

s.waitForBoot({ // Execute the following block only after the server has fully booted

	MIDIClient.init;     // Initialize the MIDI subsystem
	MIDIIn.connectAll;   // Connect all detected MIDI inputs automatically

	"SuperCollider server booted".postln;

   //	Global container for continuous gesture data received via OSC.
   // Values are updated asynchronously and read by control routines.

	// Array indexed by MIDI note number, stores active Synth instances
	~notes = Array.newClear(128);

	// True while sustain pedal (CC 64) is pressed
	~sustainActive = false;

	// Notes released during sustain, freed when pedal is lifted
	~releasedNotes = Array.newClear(128);

	~lastGestureTime = AppClock.seconds;

	// Shared state for continuous gesture data received via OSC
	~gestureData = (
		openness: 0.0
	);

   // Pad-like synthesizer based on detuned saw oscillators,
   // low-pass filtering and slow modulation for movement.

	SynthDef(\fatPad, {
		|freq = 440, amp = 0.3, gate = 1, cutoff = 1000, rq = 0.5|

		var osc1, osc2, signal, env, cutoffCtrl;

		env = EnvGen.kr(Env.adsr(0.05, 0.4, 0.6, 0.8), gate, doneAction: 2);

		osc1 = LFSaw.ar(freq);
		osc2 = LFSaw.ar(freq * 1.005);

		signal = Mix([osc1, osc2]) * 0.5;
		signal = signal + DelayC.ar(signal, 0.02, SinOsc.kr(0.1).range(0.01, 0.02));

		cutoffCtrl = Lag.kr(cutoff.clip(20, 20000), 0.05);

		signal = RLPF.ar(signal, cutoffCtrl, rq);

		Out.ar(0, Pan2.ar(signal * env * amp, 0));
	}).add;

	s.sync; // Wait until SynthDef is fully registered on the server

	// MIDI note management with sustain pedal support.
   // Notes released during sustain are stored and closed when the pedal is lifted.

	// 1. Sustain pedal handling (CC 64)
	MIDIdef.cc(\sustainPedal, {
		|val, num, chan, src|
		if (num == 64) {
			if (val >= 64) {
				~sustainActive = true;
				//"Sustain ON".postln;
			} {
				~sustainActive = false;
				//"Sustain OFF".postln;
				// When the pedal is released, stop all notes that were held by sustain
				~releasedNotes.do { |synth, i|
					if (synth.notNil) {
						synth.set(\gate, 0);
						~releasedNotes[i] = nil;
					}
				};
			};
		};
	});

	// 2. Note ON handling
	MIDIdef.noteOn(\noteOnPad, { |vel, num, chan, src|
		// If the note is already active (e.g. due to sustain), stop it before retriggering
		if (~notes[num].notNil) { ~notes[num].set(\gate, 0) };
		if (~releasedNotes[num].notNil) {~releasedNotes[num].set(\gate, 0); ~releasedNotes[num] = nil; };

		~notes[num] = Synth(\fatPad, [
			\freq, num.midicps,
			\amp, vel.linlin(0, 127, 0, 0.4),
			\cutoff, vel.linexp(0, 127, 300, 7000),
			\rq, 0.5
		]);
	});

	// 3. Note OFF handling
	MIDIdef.noteOff(\noteOffPad, { |vel, num, chan, src|
		if (~sustainActive) {
			// If sustain pedal is down, move the synth to the held-notes buffer
			~releasedNotes[num] = ~notes[num];
			~notes[num] = nil;
		} {
			// If sustain pedal is up, release the note immediately
			~notes[num].set(\gate, 0);
			~notes[num] = nil;
		};
	});

	// OSC receivers compatible with Python-based hand tracking.
   // Hand openness and rotation are streamed independently.



	OSCdef(\leftHandOpen, { |msg|
		~gestureData[\openness] = msg[1].asFloat.clip(0.0, 1.0);
		~lastGestureTime = AppClock.seconds;
		msg.postln;
	}, '/hand/left/open', recvPort: 57130);
	// recvPort: 57130 for juce implementation | 9000 for python

	// Control routine that maps gesture data to continuous synth parameters.

	~controlRoutine = Routine({
    loop {
        var elapsed = AppClock.seconds - ~lastGestureTime;
        var cutoff;

        cutoff = if (elapsed > 0.3) {
            200
        } {
            ~gestureData[\openness].linexp(0, 1, 200, 8000)
        };

        // Update active notes
			~notes.do { |synth|
				if (synth.notNil) {
					synth.set(\cutoff, cutoff);
				};
			};

			~releasedNotes.do { |synth|
				if (synth.notNil) {
					synth.set(\cutoff, cutoff);
				};
			};


        0.02.wait;
    }
}).play(AppClock);

});
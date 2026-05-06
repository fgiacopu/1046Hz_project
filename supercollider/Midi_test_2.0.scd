MIDIClient.init;
MIDIIn.connectAll;
s.boot;
s.reset;
s.scope;

(
SynthDef(\fatPad, { |freq = 440, amp = 0.3, gate = 1, cutoff = 1000, rq = 0.5|
    var osc1, osc2, signal, env, filterEnv;

    // Inviluppo dell'ampiezza (ADSR)
    env = EnvGen.kr(Env.adsr(0.05, 0.4, 0.6, 0.8), gate, doneAction: 2);

    // Due oscillatori a dente di sega, leggermente scordati tra loro
    osc1 = LFSaw.ar(freq);
    osc2 = LFSaw.ar(freq * 1.005); // Detune per l'effetto "fat"

    signal = Mix([osc1, osc2]) * 0.5;

    // Un po' di Chorus/movimento usando un LFO sul ritardo
    signal = signal + DelayC.ar(signal, 0.02, SinOsc.kr(0.1).range(0.01, 0.02));

    // Filtro Passa-Basso (RLPF)
    // Più alta è la velocity, più il filtro si sposta verso l'alto
    signal = RLPF.ar(signal, cutoff.clip(20, 20000), rq);

    Out.ar(0, Pan2.ar(signal * env * amp, 0));
}).add;
)


(
var notes = Array.newClear(128);
var sustainActive = false;
var releasedNotes = Array.newClear(128); // Note rilasciate ma tenute attive dal pedale

// 1. Gestione del Pedale Sustain (CC 64)
MIDIdef.cc(\sustainPedal, { |val, num, chan, src|
    if (num == 64) {
        if (val >= 64) {
            sustainActive = true;
            //"Sustain ON".postln;
        } {
            sustainActive = false;
            //"Sustain OFF".postln;
            // Quando rilascio il pedale, spengo tutte le note che erano state rilasciate
            releasedNotes.do { |synth, i|
                if (synth.notNil) {
                    synth.set(\gate, 0);
                    releasedNotes[i] = nil;
                }
            };
        };
    };
});

// 2. Gestione Note ON
MIDIdef.noteOn(\noteOnPad, { |vel, num, chan, src|
    // Se la nota è già attiva (es. dal pedale), la spegniamo prima di riattivarla
    if (notes[num].notNil) { notes[num].set(\gate, 0) };
    if (releasedNotes[num].notNil) { releasedNotes[num].set(\gate, 0); releasedNotes[num] = nil; };

    notes[num] = Synth(\fatPad, [
        \freq, num.midicps,
        \amp, vel.linlin(0, 127, 0, 0.4),
        \cutoff, vel.linexp(0, 127, 300, 7000),
        \rq, 0.5
    ]);
});

// 3. Gestione Note OFF
MIDIdef.noteOff(\noteOffPad, { |vel, num, chan, src|
    if (sustainActive) {
        // Se il pedale è giù, sposta il synth nell'array di "attesa"
        releasedNotes[num] = notes[num];
        notes[num] = nil;
    } {
        // Se il pedale è su, spegni normalmente
        notes[num].set(\gate, 0);
        notes[num] = nil;
    };
});
)

// --- Esegui questo per ATTIVARE il riverbero ---
(
~myReverb = {
    var in = In.ar(0, 2);
    FreeVerb.ar(in, mix: 0.3, room: 0.8, damp: 0.6);
}.play(addAction: \addAfter);
)

// --- Esegui questo per TOGLIERE il riverbero ---
~myReverb.free;
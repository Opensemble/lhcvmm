//-----------------------------------------------------------------
// EJEMPLO: \inharmonic1
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(
SynthDef.new(\inharmonic1, {
  arg freq=400, amp=0.3, gate = 1, bend=0, attack = 0.01, sustain = 0,release = 0.01, curve = 0,
	spectrum_plenitude = 1,
	spectrum_concentration = 1,
	spectrum_interstices = 1,
	spectrum_crossover = 1,
	spectrum_density = 1;

    var sig, env, freq_base, partials;

	sig = Mix.fill(8, { SinOsc.ar(freq + 500.0.rand, 0, 0.05) });

	//sig = LFTri.ar(spectrum_occupation);
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7, curve),doneAction:2);
    sig = sig * env * amp;
  Out.ar(0,sig);
}).add;


~opensemble.register(\inharmonic1, [\inharmonic, \root, \canopy, \centre],
	{ arg suf;

	var att, occ, den, func;

	//simple example

	func = switch (suf.function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	occ = switch (suf.spectrum_occupation)
	{\root} {100}
	{\centre} {500}
	{\canopy} {2000};

	den = switch (suf.spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};

x = Synth(\inharmonic1, [\freq, occ, \attack, suf.duration, \curve, func] );
} );

)





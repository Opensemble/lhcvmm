/*

HOW THIS WORKS
==============

The file framework.scd contains a global ~interpreter object and a ~sound_unit_fase object to be extended by users.
Besides, there is an OSCresponderNode object that takes care of arriving OSC messages ( and will be soon included as a member of ~interpreter object)

The workflow is the following:

1) execute the main block of framework.scd to get env vars defined and OSC listener ready.
2) in a separeted file, define your SynthDefs and register your playable functions matching spectromorfological keywords (see how to register your playable functions).

You can stop OSC messages by executing o.remove;
You can start over by executing the main block of framework.scd again.



HOW TO REGISTER YOUR PLAYABLE FUNCTIONS
=======================================

Walking through an example will be easier.

*/

(
// Declare your vars ( you wont even need it, you'll see)
var dense_inharmonic;

// Firstly, lets clear previously registered functions
// note: only call this if you want to redefine your playable functions
// without re-executing the main block of framework.scd
~interpreter.clearTable();

// Then, define your SynthDefs and stuff you need available on server.
SynthDef(\smooth, { |freq = 440, sustain = 1, amp = 0.5|
    var sig;
    sig = SinOsc.ar(freq, 0, amp) * EnvGen.kr(Env.linen(0.05, sustain, 0.1), doneAction: 2);
    Out.ar(0, sig ! 2)
}).add;


// And this is how define and register a particular sound unit fase
// Dont worry: I'll show you a shorcut later, but it is easier to explain this way step by step.


// Define an empty Event
dense_inharmonic = ();

// Set parent to global ~sound_unit_fase to "inherit" its members and functions
dense_inharmonic.parent = ~sound_unit_fase;

// Implement the playEvent function
dense_inharmonic.playEvent = {
	|self|
	/*
	argument 'self' is the Event, and since its parent was set to
	~sound_unit_fase you have access to all its members within this function
	which are filled with incoming OSC data


	here's a list of them

	self.function
	self.duration
	self.unidirectional_motion
	self.reciprocal_motion
	self.cyclic_motion
	self.characteristic_motion
	self.texture_motion_relationship
	self.texture_motion_continuity
	self.texture_motion_periodic
	self.texture_motion_acceleration
	self.spectrum_type
	self.spectrum_occupation
	self.spectrum_plenitude
	self.spectrum_concentration
	self.spectrum_interstices
	self.spectrum_crossover
	self.spectrum_density

	*/


	Synth(\smooth, [freq: 330, sustain: self.duration]);
};

// Finally, register it passing an array of spectromorfological keywords
// Whenever the ~interpreter receives and OSC message, it will look
// through all registered function and guess which one fits better
// based on the number of keywords occurences on message parameters
dense_inharmonic.register( [\harmonic, \inharmonic, \canopy, \centre] );

// That's it.

// And now that it is explained, just use this shorcut:
(
	parent: ~sound_unit_fase,
	playEvent:  {
		|self|
		Synth(\smooth, [freq: 440, sustain: self.duration]);
	}

).register( [\harmonic, \inharmonic, \canopy, \centre] );



)





//-----------------------------------------------------------------
// EJEMPLO
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(
SynthDef.new(\harmonic2, {
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
)


// Funcion - Interface
// this way you define and register a particular sound unit fase
(

(
	parent: ~sound_unit_fase,
	playEvent:  { arg self;

	var att, occ, den, func;

	//simple example

	func = switch (self.function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	occ = switch (self.spectrum_occupation)
	{\root} {100}
	{\centre} {500}
	{\canopy} {2000};

	den = switch (self.spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};

x = Synth(\harmonic2, [\freq, occ, \attack, self.duration, \curve, func] );
}

).register( [\harmonic, \inharmonic, \root, \canopy, \centre] );


)





//-----------------------------------------------------------------
// EJEMPLO2
// Descripción:
//     Colaborador john lennon
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(
SynthDef.new(\granular, {
	arg freq=800, amp=0.3, gate = 1, bend=0, attack = 0.01, sustain=0,release = 0.01 ;
	var env,sig;

	sig = GrainFM.ar(2, Impulse.kr(10), 0.1, 440, 200, LFNoise1.kr.range(1, 10));
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7),doneAction:2);
	sig = sig * env;
	Out.ar(0,sig);
}).add;

SynthDef.new(\saturated, {
	arg freq=200, amp=0.6, gate = 1, bend=0, attack = 0.01, sustain=0,release = 0.01 ;
	var env,sig;
	sig = PinkNoise.ar!2;
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7),doneAction:2);
	sig = sig * env * amp;
	Out.ar(0,sig);
}).add;
)




// this way you define and register a particular sound unit fase
(

(
	parent: ~sound_unit_fase,
	playEvent:  { arg self;
	var att, occ, den, func, spectrum;

	//simple example

	func = switch (self.function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	den = switch (self.spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};


	x = Synth(self.spectrum_type.asSymbol, [\freq, occ, \attack, self.duration, \curve, func] );
}

).register( [\saturated, \granular] );


)





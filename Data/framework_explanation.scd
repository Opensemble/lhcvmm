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
	|self,
	function,
	duration,
	unidirectional_motion,
	reciprocal_motion,
	cyclic_motion,
	characteristic_motion,
	texture_motion_relationship,
	texture_motion_continuity,
	texture_motion_periodic,
	texture_motion_acceleration,
	spectrum_type,
	spectrum_occupation,
	spectrum_plenitude,
	spectrum_concentration,
	spectrum_interstices,
	spectrum_crossover,
	spectrum_density |

	/*
	First argument ('self') is the Event itself, and since its parent was set to
	~sound_unit_fase you have access to all its members which were filled with incoming OSC data.

	You can choose to receive only 'self' argument and access members
	through dot notation (ex.: self.duration ), or to receive all arguments
	and use them directly with the arg name you prefer (but respecting args order)
	It is just a matter of taste.


	here's a list of all members:

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


	Synth(\smooth, [freq: 330, sustain: duration]); // or self.duration
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


(
// And now that it is explained, just use this shorcut:
~interpreter.clearTable();

(
	parent: ~sound_unit_fase,
	playEvent:  {
		|self, function,duration,unidirectional_motion|

	duration.postln;
		Synth(\smooth, [freq: 440, sustain: self.duration]);
	}

).register( [\centre] );

)

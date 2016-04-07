// At a glance: simple code example first; details later.
(
SynthDef(\pulse, { |freq = 440, sustain = 1|
    Out.ar(0, SinOsc.ar(freq) * EnvGen.kr(Env.perc(0.01,sustain), doneAction: 2)! 2);
}).add;

~opensemble.register(\hard_attack, [\attack, \centre], {
	|suf| //(sound unit fase)
	Synth(\pulse, [freq: 400, sustain: suf.duration]);
});
)

/*

HOW THIS WORKS
==============

The file framework.scd contains a global ~opensemble var that takes care of
registering/unregistering functions, and selecting the best match for
arriving OSC messages.

The workflow is the following:

1) Execute the main block of framework.scd to get opensemble env ready.
   Files under contributions folder will be loaded.
   You'll notice a smal GUI at bottom right
2) Define your SynthDefs and register your playable functions
   matching spectromorfological keywords (see how to register your playable functions).

Each contribution must live in a separated file inside contributions folder, as
files inside this folder will be loaded at the end of framework.scd main block.

Nevertheless, you can reevaluate your single contribution file without reloading
the framework when prototyping without worryng if your function key remains the same
since it will be replaced upon reevaluation.


HOW TO REGISTER YOUR PLAYABLE FUNCTIONS
=======================================

Walking through an example will be easier.

Define your SynthDefs and stuff you need available on server.
*/

(
SynthDef(\smooth, { |freq = 440, sustain = 1, amp = 0.5|
    var sig;
    sig = SinOsc.ar(freq, 0, amp) * EnvGen.kr(Env.linen(0.05, sustain, 0.1), doneAction: 2);
    Out.ar(0, sig ! 2)
}).add;
)

/*

Register your playable function executing

~opensemble.register( key , keywords ,  playFunc );

where 'key' must be unique as with SynthDefs
      'keywords' is an array of spectromorfological keywords (as symbols)
  and 'playFunc' is your function,

'playFunc' receives a single parameter which is a sound unit fase (onset, continuant, termination)
filled with incoming OSC data.

You can access 'suf' (sound unit fase) members through dot notation (ex.: suf.duration ).

	Here's a list of all members, so far:

	suf.function
	suf.duration
	suf.unidirectional_motion
	suf.reciprocal_motion
	suf.cyclic_motion
	suf.characteristic_motion
	suf.texture_motion_relationship
	suf.texture_motion_continuity
	suf.texture_motion_periodic
	suf.texture_motion_acceleration
	suf.spectrum_type
	suf.spectrum_occupation
	suf.spectrum_plenitude
	suf.spectrum_concentration
	suf.spectrum_interstices
	suf.spectrum_crossover
	suf.spectrum_density



For example:

*/
(
~opensemble.register(\dense_inharmonic, [\inharmonic, \canopy, \centre], {
	|suf|
	Synth(\smooth, [freq: 400, sustain: suf.duration]);
});

)

/*

That's it.

If you wish, for testing purposes, to unregister this function, then call ~opensemble.unregister(key)
*/
~opensemble.unregister(\dense_inharmonic);

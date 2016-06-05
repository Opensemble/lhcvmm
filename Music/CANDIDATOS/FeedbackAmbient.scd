//-----------------------------------------------------------------
// EJEMPLO: \inharmonic1
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(


SynthDef(\feedbackAmbient, {
	var src, loc, freq, rt;
	rt = 0.25;
	freq = Duty.ar(16/rt,0,Dseq([60,68,67,59,61,64,65].midicps,inf));

	src = Saw.ar(freq)*Decay.ar(Impulse.ar(rt),1/rt,0.25);
	//src = SoundIn.ar(0);
	loc = LocalIn.ar(2)+src;
	loc = FreqShift.ar(loc,-1);
	loc = loc+DelayC.ar(loc,0.2,freq.reciprocal);
	loc = DelayC.ar(loc,4,LFNoise1.ar(rt!2).range(0.25,2));
	loc = DelayC.ar(loc,4,2);
	//loc = (loc*250).tanh;
	//loc = LPF.ar(loc,2500);
	loc = loc+AllpassC.ar(loc,0.1,LFNoise0.ar(rt!2).range(0.05,0.1),4);
	loc = HPF.ar(loc,100);

	LocalOut.ar(loc*1.75);

	Out.ar(0,Limiter.ar(loc)*0.5);
}).add;


~opensemble.register(\feedbackAmbient, [\harmonic, \canopy, \centre],
	{ arg suf;

	Synth(\feedbackAmbient);

} );

)





//-----------------------------------------------------------------
// EJEMPLO: \inharmonic1
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(



SynthDef(\ruido,

{// Noise BPF - Creative Use
	var freq = 100;
	var rt = 0.5;
	var src = PinkNoise.ar(1!2);
	4.do{src=BRF.ar(src,rrand(100,500).round(10)+(freq*LFNoise1.ar(rt.rand!2)),0.1)};
	4.do{src=BRF.ar(src,rrand(1000,2500).round(10)+(freq*LFNoise1.ar(rt.rand!2)),0.2)};
	4.do{src=BRF.ar(src,rrand(250,750).round(10)+(freq*LFNoise1.ar(rt.rand!2)),0.1)};
	4.do{src=BRF.ar(src,rrand(2500,5000).round(10)+(freq*LFNoise1.ar(rt.rand!2)),0.2)};
	src=FreqShift.ar(src,Duty.ar(8,0,Dwhite(-1*freq*10,freq*10).round(freq/2)),0,0.25)+src;
	src=CombC.ar(src,1,1,8);
		Out.ar(0, Limiter.ar(LPF.ar(src,2500+LFNoise1.ar(rt.rand!2,1250))));
}
). add;

~opensemble.register(\ruido, [\saturated, \granular, \root, \centre],
	{ arg suf;

	Synth(\ruido);

} );

)





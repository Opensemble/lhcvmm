//-----------------------------------------------------------------
// EJEMPLO: \inharmonic1
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(



SynthDef(\darkambience,
{
	a=HPF.ar(ar(PinkNoise,5e-3),10)*Line.kr(0,1,9);
	Out.ar (0, ar(GVerb,({|i|ar(Ringz,a*LFNoise1.kr(0.05+0.1.rand),55*i+60,0.2)}!99).sum,70,99).tanh);

}
).add;


~opensemble.register(\darkambience, [\saturated, \root, \centre],
	{ arg suf;

Synth(\darkambience);} );

)





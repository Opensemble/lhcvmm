//-----------------------------------------------------------------
// EJEMPLO: \inharmonic1
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs
(
SynthDef(\chapa,

{
	//Sines
		var songLength = 3;
	var sineLine1 = Line.kr(600, 210, songLength);
	var sineLine2 = Line.kr(100, 210, songLength);
	var sines = SinOsc.ar(sineLine1) * SinOsc.ar(sineLine2);
	//Filter
	var filterLineLFO = SinOsc.ar(0.2);
	var filterLine = Line.kr(200, 6000, songLength, doneAction: 2) * filterLineLFO;
	var filtLFO = SinOsc.kr(0.4).range(60, filterLine);
	var filter = filtLFO;
	//Comb
	var comb = CombL.ar(filter, 1, 0.3, 1);
	var mix1 = (comb + GVerb.ar(filter)) * 0.1;
	mix1 = (CombL.ar(mix1, 1, 1, 6) * 0.3) + mix1;
	Out.ar(0, HPF.ar(mix1, 100) * 0.5);
}).add;

~opensemble.register(\chapa, [\granular],
	{ arg suf;

	Synth(\chapa);

} );

)





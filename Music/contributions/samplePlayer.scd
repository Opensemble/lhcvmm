//-----------------------------------------------------------------
// EJEMPLO
// Descripción:
//     Colaborador mz
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

// \samplePlayer
(
// buffer player with done action and control of envelope and panning
SynthDef(\samplePlayer, {
	arg out=0, buf = 0, rate = 1, at =0.01, rel= 0.1, pos = 0, pSpeed = 0, lev = 1, fc = 0;
	var sample, panT,  amp, aux;

	sample = PlayBuf.ar(2, buf, rate*BufRateScale.kr(buf), 1, pos, 0);
	panT= FSinOsc.kr(pSpeed);
	amp = EnvGen.ar(Env.perc(at, rel, lev), doneAction: 2);
	//amp = CombC.ar(amp, 0.4, fc, 2);
	Out.ar(out, Pan2.ar(sample, panT, amp));
}).add;


//----------------------------------------------------
// initializaition function?
// could return an error code for succeess or failure

//~samplePlayer_init = {

	// Buffer Allocation
	~pathLittle1 = "C:/Users/matias/Desktop/backup compu mechi/radiohead sound objects/Little1.wav";
	~bufferLittle1 = Buffer.read(s, ~pathLittle1);

	// Pdef definitionn

//};
//----------------------------------------------------,

~samplePlayer_play = {
		Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf,  ~bufferLittle1,
		\at, 0.04,
		\rel, 0.1,
		\pSpeed, 0,
		\pos, 1300000 + Pseries(0, 1000, inf),
		\lev, 1,
		\dur, Pseq([Pgeom(0.02, 1.03, 60), Pgeom(0.09, 0.97, 200)])
	    ).play;
};

~samplePlayer_keywords = [\granular, \root, \centre, \parabola, \attack, \upbeat,\downbeat];

~opensemble.register(\samplePlayer, ~samplePlayer_keywords, ~samplePlayer_play);

)

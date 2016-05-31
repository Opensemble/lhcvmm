//GESTO 1

// Cargamos buffer
(
~pathLittle1 = "C:/Users/matias/Desktop/backup compu mechi/radiohead sound objects/Little1.wav";
~bufferLittle1 = Buffer.read(s, ~pathLittle1);
//{PlayBuf.ar(2, ~bufferLittle1, BufRateScale.kr(~bufferLittle1))}.play;
)

// Creamos \samplePlayer
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
)

// Creamos los Pdef \x, \y, \z
(
Pdef(\x,
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
	)
);

Pdef(\y,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf, ~bufferLittle1,
		\at, 0.03,
		\rel, 0.1,
		\lev, 0.9,
		\pos, Pseries(0, 1000, inf),
		\dur, Pseq([Pgeom(0.02, 1.05, 55), Pgeom(0.1, 0.97, 200)])
	)
);

~stutPatt = Pseq([Pgeom(0.01, 1.1707, 28), Pn(0.1, 1),Pgeom(0.1, 0.94, 200) ]);

Pdef(\z,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf,  ~bufferLittle1,
		\at, 0.01,
		\rel, ~stutPatt,
		\lev, 0.8,
		\pos, 700000,
		\dur, ~stutPatt
	)
);

// Creamos el Pdef \gesto1
Pdef(\gesto1, Psym(Pseq([\x, \z, \y], 1)));
)

// tests-individuales
Pdef(\x).play
Pdef(\y).play
Pdef(\z).play

// test gesto 1
Pdef(\gesto1).play;
Pdef(\gesto1).stop;

// ------------------------------------------------------------
// ------------------------------------------------------------
// GESTO 2
// ------------------------------------------------------------
// ------------------------------------------------------------

// Carga los buffers
(
~pathLittle2 = "C:/Users/matias/Desktop/backup compu mechi/radiohead sound objects/Little2.wav";
~bufferLittle2 = Buffer.read(s, ~pathLittle2);
)

// Con Little2, variar pos entre 16, 15 y 17
(
Pdef(\a,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf, ~bufferLittle2,
		\at, 0.1,
		\rel, Pseq([Pgeom(0.01, 1.05, 100)]),
		\lev, 0.3,
		\pSpeed, 0.5,
		\pos, 1300000 + Pseries(0, 0, inf),
		\dur, Pseq([Pgeom(0.008, 1.01, 100)])
	)
);

Pdef(\b,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf, ~bufferLittle2,
		\at, 0.05,
		\rel, 0.1,
		\pSpeed, 0.5,
		\pos, Pseries(0, 1000, inf),
		\dur, Pseq([Pgeom(0.02, 1.05, 55)])
	)
);

// define gesto 2
Pdef(\gesto2, Psym(Pseq([\a, \b], 1)));
)

// tests-individuales
Pdef(\a).play
Pdef(\b).play

// test gesto 2
Pdef(\gesto2).play;

~bufferLittle2.free;

// ------------------------------------------------------------
// ------------------------------------------------------------
// GESTO 3
// ------------------------------------------------------------
// ------------------------------------------------------------

// Carga el Buffer
(
~pathLotus1 = "C:/Users/matias/Desktop/backup compu mechi/radiohead sound objects/Lotus1.wav";
~bufferLotus1 = Buffer.read(s, ~pathLotus1);
)

(
// Con Little2, variar pos entre 16, 15 y 17
Pdef(\m,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf,  ~bufferLotus1,
		\at, 0.07,
		\rel, Pseq([Pgeom(0.01, 1.05, 100)]),
		\lev, 0.9,
		\pSpeed, 0.5,
		\pos, 1400000 + Pseries(0, 1000, inf),
		\dur, Pseq([Pgeom(0.008, 1.01, 90)])
	)
);

Pdef(\n,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf, ~bufferLotus1,
		\at, 0.05,
		\rel, 0.05,
		\pSpeed, 0.5,
		\lev, 0.7,
		\pos, 3000000 + Pseries(0, 1000, inf),
		\dur, Pseq([Pgeom(0.02, 1.05, 60)])
	)
);

// define gesto 3
Pdef(\gesto3, Psym(Pseq([\m, \n], 1)));
)

// tests-individuales
Pdef(\m).play
Pdef(\n).play

// test gesto 3
Pdef(\gesto3).play;

~bufferLotus1.free;


// TEST DE MOVIMIENTO

Pdef(\gestott, Psym(Pseq([\x, \m4a], 1)));



// ------------------------------------------------------------
// ------------------------------------------------------------
// GESTO 4
// ------------------------------------------------------------
// ------------------------------------------------------------

// EN ESTE MOVIMIENTO PROBAMOS OTRO TIPO DE GESTO. MÁS SUTIL, PARA PODER LUEGO INTERCALARLOS CON LOS DEL MOV 1

(
~pathGhost1 = "C:/Users/matias/Desktop/backup compu mechi/radiohead sound objects/Ghost1.wav";
~bufferGhost1 = Buffer.read(s, ~pathGhost1);
)

//play({PlayBuf.ar(2, ~bufferGhost1, BufRateScale.kr(~bufferGhost1))});

//~bufferGhost1.free;

(
// buffer player with done action and control of envelope and panning
SynthDef(\samplePlayer2,
	{ arg out=0, buf = 0, rate = 1, at =0.01, rel= 0.1, pos = 0, pSpeed = 0, lev = 0.5, fc = 0;
	var sample, panT,  amp, aux;

	sample = PlayBuf.ar(2, buf, rate*BufRateScale.kr(buf), 1, pos, 0);
panT= FSinOsc.kr(pSpeed);
amp = EnvGen.ar(Env.perc(at, rel, lev), doneAction: 2);
		// amp = BPF.ar(amp, 4000, 8, 0.5);
		// amp = HPF.ar(amp, 100, 1);
Out.ar(out, Pan2.ar(sample, panT, amp));
}).add;
)

//Synth(\samplePlayer2, [\buf,~bufferGhost1]);

//SUB GESTO 1

// elegir entre uno o el otro. con el 0.08 ,se hace muy distinto
Pdef(\m2a, Pbind(\instrument, \samplePlayer2, \buf, ~bufferGhost1, \lev, 0.7, \pos, 1715000 + Pseries(0, 100, inf), \dur, Prand([0.01, 0.02, 0.05], inf), \at, Pseries(0.0001, 0.0001, inf), \rel, 10*Prand([0.01, 0.02, 0.05], inf)));

//Pbind(\instrument, \samplePlayer2, \buf, ~bufferGhost1, \pos, Pseries(0, 100, inf), \dur, Prand([0.01, 0.02, 0.05, 0.08], inf), \at, Pseries(0, 0.0001, inf), \rel, 0.1, \rate, 3).play

Pdef(\m2a).play
Pdef(\m2a).stop

//SUB GESTO 2

// elegir entre uno o el otro. con el 0.08 ,se hace muy distinto
Pdef(\m2b, Pbind(\instrument, \samplePlayer2, \buf, ~bufferGhost1, \lev, 0.5, \pos, 1915000+Pseries(0,10,inf), \dur,  10, \at, 0.001, \rel, 10));

Pdef(\m2b).play;
Pdef(\m2b).stop;

// ------------------------------------------------------------
// ------------------------------------------------------------
// GESTO 5
// ------------------------------------------------------------
// ------------------------------------------------------------


(
~pathCodex2 = "C:/Users/matias/Desktop/backup compu mechi/radiohead sound objects/Lotus1.wav";
~bufferCodex2 = Buffer.read(s, ~pathCodex2);
)

//~buf1.numChannels
{PlayBuf.ar(2, ~bufferCodex2, BufRateScale.kr(~bufferCodex2))}.play;

(
// buffer player with done action and control of envelope and panning
SynthDef(\samplePlayer, { arg out=0, buf = 0,
rate = 1, at =0.01, rel= 0.1, pos = 0, pSpeed = 0, lev = 0.5, fc = 0;
var sample, panT,  amp, aux;
sample = PlayBuf.ar(2, buf, rate*BufRateScale.kr(buf), 1, pos, 0);
panT= FSinOsc.kr(pSpeed);
amp = EnvGen.ar(Env.perc(at, rel, lev), doneAction: 2);
//amp = CombC.ar(amp, 0.4, 0.2, 3);
Out.ar(out, Pan2.ar(sample, panT, amp));
}).add;
)

Pdef(\m4a,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf, ~bufferCodex2,
		\at, 0.001,
		\dur, Pwhite(0.05, 0.07, inf),
		\rel, Pseries(0.07, 0.05, 100),
		\lev, 1.5,
		\rate, Pwhite(-1.5, 1.5, inf),
		\pos, Pwhite(0, ~bufferCodex2.numFrames, 30)
)).play

Pdef(\m4b,
	Pbind(
		\instrument, \samplePlayer,
		\out, 0,
		\buf, ~bufferCodex2,
		\at, 0.01,
		\dur, Pseries(0.1,-0.004, inf),
		\rel, Pseries(0.1,0.003, inf),
		\lev, 1.4,
		\rate, 1,
		\pos, Pwhite(4500000, ~bufferCodex2.numFrames, 80)
)).play


// tests-individuales
Pdef(\m4a).play
Pdef(\m4b).play

Pdef(\m4a).stop
Pdef(\m4b).stop


// gesto 1
Pdef(\gesto4, Psym(Pseq([\m4a, \m4b], 1)));  // así no funciona bueno
Pdef(\gesto4).play;

b.free;

// ------------------------------------------------------------
// DEFINIR TODOS LOS GESTOS Y TEXTURAS ANTES DE PROBAR ESTO

(
Pdef(\movimiento1, Psym(Pseq([\x, \y, \m4a, \z, \gesto2, [\gesto3, \m2a]], 1))).play;
t = PdefAllGui();
)

Pdef(\movimiento1).stop;

//m3a se cuelga, pero podemos arreglarlo
// ------------------------------------------------------------



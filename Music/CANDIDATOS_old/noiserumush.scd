// Rumush
// https://mycelialcordsblog.wordpress.com/
// https://fungorumush.bandcamp.com/releases

// Simple FM Noise Saw
({a=VarSaw;b=SyncSaw;c=a.ar([60.rand,60.001], 0, [0.3,0.4], 0.5)+b.ar([59.9,59.91.rand], [60.01,60.0001], 0.5);d=Saw.ar(Line.ar(0, 1, 20,1,0,2)*10000.rand+(c*1000.rand), 0.5)}.play;)

// Simple FM Noise SinOsc
({a=VarSaw;b=SyncSaw;c=a.ar([6000.rand,6000.001.rand], 0, [0.3,0.4], 0.5)+b.ar([590.9,590.91], [600.01,600.0001], 0.5);d=SinOsc.ar(Line.ar(0, 1, 8,1,0,2)*10000.rand+(c*10000.rand), 0, 0.5)}.play;)

// Semi Random Saw Rhythms - c = fundFreq; Run it a couple of times
({a=VarSaw;b=SyncSaw;c=1;d=a.ar([c*50.rand,c*5.rand], 0, 0.5, 0.5)+b.ar([c*5.rand,c*50.rand], [c*500.rand,c*500.rand], 0.5);e=Saw.ar(Line.ar(1, 5, 20,1,0,2)*100+(d*500), 0.5)}.play;)

// AM and FM Function - change 'b' and Mix.fill 1st argument
({a=SinOsc;b=0.05;Mix.fill(32,{Saw.ar(1000.rand!2,0.125)*(a.ar(b+b.rand*(a.ar(b,0,1000)))*a.ar(b+b.rand))})}.play;)

// Noisy Detuned SinOsc
({a=LFPulse;b=SinOsc;r=[0.125, 0.25, 0.5];f=[1, 3, 5, 8]*10;Mix.fill(8, {b.ar(f.choose!2*Lag.ar(a.ar([r.choose+0.1.rand, r.choose+0.1.rand], add:4),0.5.rand),0, a.ar([r.choose+0.1.rand, r.choose+0.1.rand]*50)).tanh})}.play;)

// Welcome to space
({m={r;LFNoise0.ar(r, add:1.1)*0.5};Mix.fill(2,{AllpassC.ar(SinOscFB.ar(15000.rand!2*m.(1+0.5.rand),m.(0.5+0.5.rand))*2, 1, [1*m.(0.25+0.5.rand), 1*m.(0.25+0.5.rand)], 4,0.125)})}.play;)
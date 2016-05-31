// this is the new version!
(
s = Server.default;
s.boot;
)
(
SynthDef(\bglass, {

	|t_trig = 1.0, audioOutBus = 0|

	var major_hit_times, major_hit_deltas, major_hit_levels, major_hit_impulse_levels, major_hit_impulses;
	var major_hit_amp, major_hit_envGen, major_hit_out;
	var klank_freq_array, klank_out;
	var noise, noise_out;
	var additional_tinkles;
	var initial_impact_pitch_envGen, initial_impact_amp_envGen, initial_impact;
	var comb_out, output;

	var last_time;

	major_hit_times = [0.02, 0.1, 0.21, 0.28, 0.32, 0.48, 0.6, 0.69];
	major_hit_deltas = [];
	last_time = 0.0;
	major_hit_times.size.do { |i|
		major_hit_deltas = major_hit_deltas ++
			[
			SampleDur.ir,
			((major_hit_times[i] - last_time) * TRand.kr(0.4, 1.6, t_trig)) - SampleDur.ir
			];

		last_time = major_hit_times[i];
	};
	major_hit_levels = [1.0, 0.3, 0.7, 0.4, 0.65, 0.87, 0.27, 0.4];
	major_hit_levels.size.do { |i|

		major_hit_levels[i] = major_hit_levels[i] * TRand.kr(0.7, 1.0, t_trig);

	};
	major_hit_impulse_levels = [major_hit_levels, 0 ! major_hit_times.size].lace(major_hit_times.size * 2);
	major_hit_impulses = Duty.ar(
		Dseq(major_hit_deltas, 1), K2A.ar(t_trig), Dseq(major_hit_impulse_levels, 1)
		);
	major_hit_amp = Gate.ar(major_hit_impulses, major_hit_impulses);
	major_hit_envGen = EnvGen.ar(Env.perc(0.0, 0.03, 1, -9), major_hit_impulses) * major_hit_amp;
	major_hit_out = major_hit_envGen * WhiteNoise.ar * 0.6;
	major_hit_out = major_hit_out + major_hit_impulses;

	klank_freq_array = [1383, 2831, 3252, 3446, 4547, 4600, 4832, 5863, 6813, 8683, 11196];
	klank_freq_array.size.do { |i|

		klank_freq_array[i] = klank_freq_array[i] * TRand.kr(0.8, 1.2,  Impulse.kr(20));
	};
	klank_out = DynKlank.ar(`[klank_freq_array, nil, 0.2], major_hit_out * 0.05);
	klank_out = DelayC.ar(klank_out, 0.2, 0.009);

	noise = BrownNoise.ar + (WhiteNoise.ar * 0.3) + (PinkNoise.ar * 0.6);
	noise = noise * 0.1;
	noise = noise + Dust.ar(70, 1);
	noise_out = noise * LagUD.ar(major_hit_impulses, 0.0, 0.842);

	additional_tinkles =
	DynKlank.ar(`[ Array.series(8, 1200, 179), nil, 0.7], Dust.ar(14), TRand.kr(2.9, 3.1, Impulse.kr(28)) )
	+
	DynKlank.ar(`[ Array.series(8, 1200, 179), nil, 0.13], Dust.ar(11, 0.7), TRand.kr(1.4, 2.2, Impulse.kr(15)) );

	additional_tinkles = additional_tinkles *
		EnvGen.ar(Env.linen(0.15, 0.3, 0.3, 1.0, -2), t_trig, timeScale: TRand.kr(0.9, 1.12, t_trig));

	initial_impact_pitch_envGen = EnvGen.ar(Env.perc(0.001, 0.03, 1.0, -7), t_trig);
	initial_impact_amp_envGen = EnvGen.ar(Env.perc(0.0, 0.04, 1.0, -9), t_trig);
	initial_impact = SinOsc.ar(initial_impact_pitch_envGen.exprange(20, 4000) * TRand.kr(0.96, 1.03, t_trig)) * 0.5;
	initial_impact = initial_impact + LFNoise1.ar(6800, 1.0) * initial_impact_amp_envGen;
	initial_impact = HPF.ar(initial_impact, 100);
	initial_impact = initial_impact + CombC.ar(initial_impact, 0.2, 0.04, 0.2, 0.12);
	output = (klank_out * 0.5) + (major_hit_out * 0.45);
	output = (output * 0.86) + DelayC.ar(output, 0.2, 0.01);
	output = output + (noise_out * 0.32) + (additional_tinkles * 0.044) + (initial_impact * 0.2);

	comb_out = CombC.ar(output, 0.1, [0.028, 0.031], 0.52, 0.23);
	comb_out = LPF.ar(comb_out, 3000);
	comb_out = HPF.ar(comb_out, 110);
	output = output + comb_out;
	Out.ar(audioOutBus, output);

}).send(s);

)
x = Synth(\bglass, [\audioOutBus, 0], s);
x.set(\t_trig, 1.0);



///////////////////////////////////////////////////////////////////////////


// and now the previous attempt
(
s = Server.default;
s.boot;
)

(



SynthDef(\bglass, {

	|t_trig = 1.0, audioOutBus = 0|

	var major_hit_times, major_hit_deltas, major_hit_levels, major_hit_impulse_levels, major_hit_impulses;
	var major_hit_amp, major_hit_envGen, major_hit_out;
	var klank_freq_array, klank_out;
	var noise, noise_out;
	var additional_tinkles;
	var initial_impact_pitch_envGen, initial_impact_amp_envGen, initial_impact;
	var output;

	var last_time;

	major_hit_times = [0.02, 0.1, 0.21, 0.28, 0.32, 0.48, 0.6, 0.69];
	major_hit_deltas = [];
	last_time = 0.0;
	major_hit_times.size.do { |i|
		major_hit_deltas = major_hit_deltas ++
			[
			SampleDur.ir,
			(major_hit_times[i] - last_time) - SampleDur.ir
			];

		last_time = major_hit_times[i];
	};

	major_hit_levels = [1.0, 0.3, 0.7, 0.4, 0.65, 0.87, 0.27, 0.4];
	major_hit_impulse_levels = [major_hit_levels, 0 ! major_hit_times.size].lace(major_hit_times.size * 2);

	major_hit_impulses = Duty.ar(
		Dseq(major_hit_deltas, 1), K2A.ar(t_trig), Dseq(major_hit_impulse_levels, 1)
		);

	major_hit_amp = Gate.ar(major_hit_impulses, major_hit_impulses);

	major_hit_envGen = EnvGen.ar(Env.perc(0.0, 0.03, 1, -9), major_hit_impulses) * major_hit_amp;
	major_hit_out = major_hit_envGen * WhiteNoise.ar * 0.6;

	major_hit_out = major_hit_out + major_hit_impulses;

	klank_freq_array = [1383, 2831, 3252, 3446, 4547, 4600, 4832, 5863, 6813, 8683, 11196];
	klank_freq_array.size.do { |i|

		klank_freq_array[i] = klank_freq_array[i] * TRand.kr(0.9, 1.1,  Impulse.kr(20));
	};
	klank_out = DynKlank.ar(`[klank_freq_array, nil, 0.2], major_hit_out * 0.05);
	klank_out = DelayC.ar(klank_out, 0.2, 0.009);

	noise = BrownNoise.ar + (WhiteNoise.ar * 0.3) + (PinkNoise.ar * 0.6);
	noise = noise * 0.1;
	noise = noise + Dust.ar(70, 1);
	noise_out = noise * LagUD.ar(major_hit_impulses, 0.0, 0.842);

	additional_tinkles = DynKlank.ar(`[ Array.series(8, 1200, 179), nil, 0.7], Dust.ar(18), TRand.kr(2.9, 3.1, Impulse.kr(20)) );

	additional_tinkles = additional_tinkles * EnvGen.ar(Env.linen(0.15, 0.3, 0.3, 1.0, -2), t_trig);

	initial_impact_pitch_envGen = EnvGen.ar(Env.perc(0.001, 0.03, 1.0, -7), t_trig);
	initial_impact_amp_envGen = EnvGen.ar(Env.perc(0.0, 0.04, 1.0, -9), t_trig);
	initial_impact = SinOsc.ar(initial_impact_pitch_envGen.exprange(20, 4000)) * 0.5;
	initial_impact = initial_impact + LFNoise1.ar(6800, 1.0) * initial_impact_amp_envGen;
	initial_impact = HPF.ar(initial_impact, 100);
	initial_impact = initial_impact + CombC.ar(initial_impact, 0.2, 0.04, 0.2, 0.12);
	output = (klank_out * 0.5) + (major_hit_out * 0.45) + (noise_out * 0.12) + (additional_tinkles * 0.044) + (initial_impact * 0.2);

	output = output ! 2;
	Out.ar(audioOutBus, output);

}).send(s);


)


x = Synth(\bglass, [\audioOutBus, 0], s);
x.set(\t_trig, 1.0);
//-----------------------------------
// FRAMEWORK: ETAPA DE INICIALIZACIÓN
//-----------------------------------

(

// Function Descriptor Table (por ahora es una lista de descriptores)
~funcDescTable = List.new;

)

//-----------------------------------
// FRAMEWORK: MÉTODO PARA REGISTRAR FUNCIONES
//-----------------------------------
(

~funcRegistration = {|func, description|
	var desc;

	// Set with spectromorfological words describing the new function
	desc = Set.newFrom(description);

	// A descriptor is a Dictionary with function + description
	~functionDescriptor = Dictionary.new; //there is a better way, I know
	~functionDescriptor.putAll(Dictionary[\function -> func, \description -> desc]);

	// Add the new descriptor to the Table
	~funcDescTable.add(~functionDescriptor);
};

)

//-----------------------------------
// FRAMEWORK: REGISTRAMOS FUNCIONES DE USUSARIOS
//-----------------------------------

(
// NOTAS: Todas las que queramos, pero sin repetir.

~funcRegistration.value(~f1, Array[\harmonic, \inharmonic, \root, \canopy, \centre]);
~funcRegistration.value(~f2, Array[\saturated, \granular]);

)

//--------------------------------------------------
// FRAMEWORK: REGISTRAMOS EL LISTENER OSC Y FRAMEWORK
//--------------------------------------------------

(
//creates a simple synth and attachs an OSC listener
o.remove;

//-----------------------------------
// BUSCA EN LA BASE DE DATOS DE USUARIOS, recibe el evento y la estructura de func desc
~getUserFunction = {|ev|
//-----------------------------------

// ev es el dictionary de la temporal phase, por ejemplo es un onset

//requerimientos de ejemplo, extraidos del evento:
//~rr = Array[\harmonic, \emergence];
~rr = List.new;
//~rr.add(ev.at(\function));
~rr.add(ev.at(\spectrum_type));   // por ahora sólo este así suena algo
//~rr.add(ev.at(\spectrum_occupation));

//~rr

//Devolvemos una lista de funciones
~ll = List.new;

//operacion include

	~funcDescTable.do({|i|

	if ((i.at(\description)).includesAll(~rr))
	{~ll.add(i.at(\function)); "match".postln;};

	});

	// por ahora sólo devolvemos la primera, luego devolvemos la lista y
	// se elige una por algún criterio a definir.
	~ll[0];

};

~makeOnset = {|event|

	var ev = event[0], //onset dictionary
	ff; // user function;

	// here we pick the user funcion

	ff = ~getUserFunction.value(ev);

	// here we call the user function
	ff.value(ev.at(\function), ev.at(\duration), ev.at(\spectrum_type),  ev.at(\spectrum_occupation), ev.at(\spectrum_plenitude), ev.at(\spectrum_concentration), ev.at(\spectrum_interstices), ev.at(\spectrum_crossover), ev.at(\spectrum_density));

	// the 'wait' should go outside the function ?

	ev.at(\duration).postln;
	ev.at(\spectrum_type).postln;
	ev.at(\duration).wait;
};

// implement
~makeContinuant;
~makeTermination;


// this function will be a thread.
// so, every SoundUnit will run in parallel

~makeSoundUnit = { |event|
	// makePreparation
	// relationship among temporal fases
	~makeOnset.value(event);
	~makeContinuant.value(event);
	~makeTermination.value(event);

};

//-----------------------------------
//-----------------------------------

o = OSCresponderNode(nil, '/sound_unit', {
	|x, r, msg|
	var onset, continuant, termination, event;
	onset = msg[1..17] ;
	continuant = msg[18..34] ;
	termination = msg[35..51] ;

	// onset, continuant, termination
	// duration
	// unidirectional_motion
	// reciprocal_motion
	// cyclic_motion
	// characteristic_motion
	// texture motion relationship
	// texture motion continuity-discontinuity
 	// texture motion periodic:  periodic - aperiodic erratic
	// texture motion acceleration:  accelerating - decelerating - flux
	// spectrum type
	// spectral occupation type
	// spectral qualifiers: emptiness - plenitude
	// spectral qualifiers: diffuseness - concentration
	// spectral qualifiers: streams - interstices
	// spectral qualifiers: overlap - crossover
	// spectral density


e = Dictionary.new;
c = Dictionary.new;
t = Dictionary.new;

fork{

e.put(\function, onset[0]);
e.put(\duration, onset[1]);
e.put(\unidirectional_motion, onset[2]);
e.put(\reciprocal_motion, onset[3]);
e.put(\cyclic_motion, onset[4]);
e.put(\characteristic_motion, onset[5]);
e.put(\texture_motion_relationship, onset[6]);
e.put(\texture_motion_continuity, onset[7]);
e.put(\texture_motion_periodic, onset[8]);
e.put(\texture_motion_acceleration, onset[9]);
e.put(\spectrum_type, onset[10]);
e.put(\spectrum_occupation, onset[11]);
e.put(\spectrum_plenitude, onset[12]);
e.put(\spectrum_concentration, onset[13]);
e.put(\spectrum_interstices, onset[14]);
e.put(\spectrum_crossover, onset[15]);
e.put(\spectrum_density, onset[16]);

/*
c.put(\function, continuant[0]);
c.put(\duration, continuant[1]);
c.put(\unidirectional_motion, continuant[2]);
c.put(\reciprocal_motion, continuant[3]);
c.put(\cyclic_motion, continuant[4]);
c.put(\characteristic_motion, continuant[5]);
c.put(\texture_motion_relationship, continuant[6]);
c.put(\texture_motion_continuity, continuant[7]);
c.put(\texture_motion_periodic, continuant[8]);
c.put(\texture_motion_acceleration, continuant[9]);
c.put(\spectrum_type, continuant[10]);
c.put(\spectrum_occupation, continuant[11]);
c.put(\spectrum_plenitude, continuant[12]);
c.put(\spectrum_concentration, continuant[13]);
c.put(\spectrum_interstices, continuant[14]);
c.put(\spectrum_crossover, continuant[15]);
c.put(\spectrum_density, continuant[16]);

t.put(\function, termination[0]);
t.put(\duration, termination[1]);
t.put(\unidirectional_motion, termination[2]);
t.put(\reciprocal_motion, termination[3]);
t.put(\cyclic_motion, termination[4]);
t.put(\characteristic_motion, termination[5]);
t.put(\texture_motion_relationship, termination[6]);
t.put(\texture_motion_continuity, termination[7]);
t.put(\texture_motion_periodic, termination[8]);
t.put(\texture_motion_acceleration, termination[9]);
t.put(\spectrum_type, termination[10]);
t.put(\spectrum_occupation, termination[11]);
t.put(\spectrum_plenitude, termination[12]);
t.put(\spectrum_concentration, termination[13]);
t.put(\spectrum_interstices, termination[14]);
t.put(\spectrum_crossover, termination[15]);
t.put(\spectrum_density, termination[16]);
*/

event = [e, c, t];

//------------------------------------------------------------
// acá debemos invocar al framework, quien será el encargado
// de hacer las llamadas a las funciones registradas.

// llega event
// makeSoundUnit(event)
	// (this function can relate temporal stages and make preparations
	// makeOnset(event)
	// makeContinuation(event)
	// makeTermination(event)
		// these makeSomething will find an appropiate user function

// We can implement user function registration with a Set of key words and a function reference. Then we will find the usefull functions asking: does this function implement 'harmonic', continuant', 'turbulence', etc. These will be symbols.

// After we have an array of useful functions, we will choose random or ranking based.

		// f.value(e.at(\function), e.at(\duration), e.at(\spectrum_occupation),
		// e.at(\spectrum_plenitude), e.at(\spectrum_concentration), e.at(\spectrum_interstices), e.at(\spectrum_crossover), e.at(\spectrum_density));
		// e.at(\duration).postln;
		// e.at(\duration).wait;


~makeSoundUnit.value(event);

} // fin del fork

}).add; // add el OSC listener

) // fin del bloque


//----------------------------------------------------------------------------
s.freeAll;
//to remove listener
o.remove;






import time
import sys
from thread import start_new_thread
from OSC import  OSCClient, OSCClientError, OSCBundle, OSCMessage
from random import random, choice, uniform
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('-s','--simulate', help='Simulate events.', action='store_true')
parser.add_argument('-r','--random', help='Send random events. (will use a list otherwise)', action='store_true')

args = parser.parse_args()




osc_port = 57120 #default SuperCollider port (must be open before executing this program)
interval = 1.12 # time between events, in seconds
# just some easy ansi colors printing: 'o' for ok, 'w' for warning, 'e' for error.
def printc(t, c='o'):
    print '\033[9' + {'o': '2m','w': '3m','e': '1m'}[c] + t + '\033[0m'

onsets = ['','departure','emergence','anacrusis','attack','upbeat','downbeat']
continuants = ['','passage','transition','prolongation','maintenance','statement']
terminations = ['','arrival','disappearence','closure','release','resolution','plane']
morfological_functions = [onsets,continuants,terminations]
duration_ranges = [ [0.01, 0.3], [0.4, 2], [0.2, 0.5]]

unidirectional_motions = ['','ascent','plane','descent']
reciprocal_motions = ['','parabola','oscilation','ondulation']
cyclic_motions = ['','rotation','spiral','spin','vortex','pericentrality','centrifugal']
characteristic_motions = ['','push','flow','rise','throw','drift','float','fly','plummet','dive','fall','tumble','sink','glide','slump']

texture_motion_relationships = ['','streaming', 'flocking', 'convolution','turbulence']

spectrums = ['harmonic', 'inharmonic','granular','saturated']
spectral_occupation_types = ['canopy', 'centre', 'root']
spectral_densities = ['filled', 'packed', 'opaque','translucent','transparent','empty']




# test osc messages:

# onset, continuant, termination
# duration
# unidirectional_motions
# reciprocal_motions
# cyclic_motions
# characteristic_motions
# texture motion
# continuity-discontinuity (sustained -> granular -> iterative)
# movement:  periodic - aperiodic erratic
# movement:  accelerating - decelerating - flux
# spectrum type
# spectral occupation type
# emptiness - plenitude
# diffuseness - concentration
# streams - interstices
# overlap - crossover
# spectral density
'departure', 0.2, 'ascent','parabola','rotation','push', 'flocking', 0.5, 0.5, 0.5, 'harmonic', 'canopy', 0.5, 0.5, 0.5, 0.5, 'filled'



msg.append(choice(spectral_densities))




def send_random_event():
    try:
        bundle = OSCBundle()
        msg = OSCMessage("/sound_unit")

        for i in range(3):
            #onset, continuant, termination
            msg.append(choice(morfological_functions[i]))
            #duration
            msg.append(uniform(duration_ranges[i][0],duration_ranges[i][1]))
            # unidirectional_motions
            msg.append(choice(unidirectional_motions))
            # reciprocal_motions
            msg.append(choice(reciprocal_motions))
            # cyclic_motions
            msg.append(choice(cyclic_motions))
            # characteristic_motions
            msg.append(choice(characteristic_motions))

            # texture motion
            msg.append(choice(texture_motion_relationships))
            msg.append(uniform(0, 1)) # continuity-discontinuity (sustained -> granular -> iterative)
            msg.append(uniform(0, 1)) # movement:  periodic - aperiodic erratic
            msg.append(uniform(0, 1)) # movement:  accelerating - decelerating - flux

            # spectrum type
            msg.append(choice(spectrums))

            #spectral occupation type
            msg.append(choice(spectral_occupation_types))
            #spectral qualifiers
            msg.append(uniform(0, 1)) # emptiness - plenitude
            msg.append(uniform(0, 1)) # diffuseness - concentration
            msg.append(uniform(0, 1)) # streams - interstices
            msg.append(uniform(0, 1)) # overlap - crossover

            #spectral density
            msg.append(choice(spectral_densities))


        bundle.append(msg)
        client.send(bundle)

    except OSCClientError, e:
        printc( "\OSCClientError: Connection refused on port %s." % osc_port, 'e')

# Define a function for the thread
def send_events_periodically():
    print "Start Sending events every %s seconds." % interval
    print "Each * printed represents an event sent."
    print "Press Ctrl + C to finalize."

    starttime=time.time()

    while 1:
        # restart * printing every 10 events
        sys.stdout.write('\r----------\r')
        for i in range(0, 10):
            if args.simulate:
                if args.random:
                    send_random_event()
                    time_to_sleep = interval - ((time.time() - starttime) % interval)
                else:
                    send_random_event()
                    time_to_sleep = interval - ((time.time() - starttime) % interval)

            sys.stdout.write('*')
            sys.stdout.flush()
            time.sleep(time_to_sleep)


try:
    #initialize osc client
    client = OSCClient()
    client.connect(('127.0.0.1', osc_port))   # connect to SuperCollider

    start_new_thread( send_events_periodically, ())

    while 1:
        pass

except OSCClientError:
    printc( "\OSCClientError: Connection refused on port %s." % osc_port, 'e')

except KeyboardInterrupt:
    print "\nProgram terminated."


'''


Sound Unit

	Morfological framework
		Function: Onsets, Continuants and Terminations
		Description (palabras para cada categoria)
		Duration (float, en segundos)
		Motion and Growth
			Motion: unidirectional, reciprocal and cyclic/centric;
			Growth: bi/multidirectional
			Characteristics of motion
		Texture motion :
            relationship: streaming, flocking, convolution and turbulence.
			internal consistency: continuity-discontinuity continuum
								(sustained -> granular -> iterative)
			movement: 	o periodic - aperiodic erratic
						o accelerating - decelerating - flux
						o grouping patterns
		Behaviour
			motion coordination (vertical): loose-tight continuum
			motion passage (Causality, horiz): voluntary-pressured cont.
			relations: dominance/subordination and conflict/coexistence.

	Spectral framework
        Note to Noise continuum (armonico, inarmonico, granular, saturado)
                                (desarrollar esto)
        Occupancy of Spectral Space
	       type:  canopy, centre and root
           qualifiers:
		        o emptiness - plenitude
		        o diffuseness - concentration
		        o streams - interstices
		        o overlap - crossover
		Spectral Density (filled, packed/compressed, opaque, translucent, transparent and empty)



'''

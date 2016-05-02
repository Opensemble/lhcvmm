import time
import sys
from thread import start_new_thread
from OSC import  OSCClient, OSCClientError, OSCBundle, OSCMessage
from random import random, choice, uniform
import argparse
import csv

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('-c','--csv', help='Load events from csv.', action='store_false')
parser.add_argument('-r','--random', help='Send random events. (will use a list otherwise)', action='store_true')
parser.add_argument('-l','--loop', help='Loop events sequence.', action='store_true')

args = parser.parse_args()

csv_data = []
# columns that must be converted to float
float_indexes = [0,2,8,9,10,13,14,15,16,19,25,26,27,30,31,32,33,36,42,43,44,47,48,49,50]
with open('data_files/smalley.csv', 'rb') as csvfile:
    csv = csv.reader(csvfile, delimiter=',', quotechar='"')
    csv.next() # first row
    csv.next() # and second row are headers
    for row in csv:
        for i, val in enumerate(row):
            if i in float_indexes:
                try:
                    row[i] = float(row[i].replace(",", "."))
                except:
                    row[i] = 0

        csv_data.append(row)


osc_port = 57120 #default SuperCollider port (must be open before executing this program)
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

# https://docs.google.com/feeds/download/spreadsheets/Export?key=16FjhAmzO4CZyGDEJfdMguKVnG9FgMQe8M04X5MLigR4&exportFormat=csv



def send_event(data):
    try:
        bundle = OSCBundle()
        msg = OSCMessage("/sound_unit")
        for d in data:
            msg.append(d)

        bundle.append(msg)
        client.send(bundle)

    except OSCClientError, e:
        printc( "\OSCClientError: Connection refused on port %s." % osc_port, 'e')


def send_random_event():
    data = []

    for i in range(3):
        #onset, continuant, termination
        data.append(choice(morfological_functions[i]))
        #duration
        data.append(uniform(duration_ranges[i][0],duration_ranges[i][1]))
        # unidirectional_motions
        data.append(choice(unidirectional_motions))
        # reciprocal_motions
        data.append(choice(reciprocal_motions))
        # cyclic_motions
        data.append(choice(cyclic_motions))
        # characteristic_motions
        data.append(choice(characteristic_motions))

        # texture motion
        data.append(choice(texture_motion_relationships))
        data.append(uniform(0, 1)) # continuity-discontinuity (sustained -> granular -> iterative)
        data.append(uniform(0, 1)) # movement:  periodic - aperiodic erratic
        data.append(uniform(0, 1)) # movement:  accelerating - decelerating - flux

        # spectrum type
        data.append(choice(spectrums))

        #spectral occupation type
        data.append(choice(spectral_occupation_types))
        #spectral qualifiers
        data.append(uniform(0, 1)) # emptiness - plenitude
        data.append(uniform(0, 1)) # diffuseness - concentration
        data.append(uniform(0, 1)) # streams - interstices
        data.append(uniform(0, 1)) # overlap - crossover

        #spectral density
        data.append(choice(spectral_densities))

    send_event(data)


try:
    #initialize osc client
    client = OSCClient()
    client.connect(('127.0.0.1', osc_port))   # connect to SuperCollider

    starttime=time.time()

    if args.random:
        interval = 1.12 # time between events, in seconds
        print "Start Sending events every %s seconds." % interval
        print "Each * printed represents an event sent."
        print "Press Ctrl + C to finalize."
        while 1:
            # restart * printing every 10 events
            sys.stdout.write('\r----------\r')
            for i in range(0, 10):
                send_random_event()
                time_to_sleep = interval - ((time.time() - starttime) % interval)
                sys.stdout.write('*')
                sys.stdout.flush()
                time.sleep(time_to_sleep)

    if args.csv:
        print "Start Sending events from csv file."
        print "Each * printed represents an event sent."
        print "Press Ctrl + C to finalize."

        while 1:

            csv_data_index = 0

            sys.stdout.write('\r')
            for i in range(len(csv_data)):
                sys.stdout.write('-')
            sys.stdout.write('\r')

            for row in csv_data:
                send_event(row[1:])
                sys.stdout.write('*')
                sys.stdout.flush()

                time_to_sleep = row[0]
                if time_to_sleep > 0:
                    time_to_sleep = time_to_sleep - ((time.time() - starttime) % time_to_sleep)
                    time.sleep(time_to_sleep)

            if not args.loop:
                break


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

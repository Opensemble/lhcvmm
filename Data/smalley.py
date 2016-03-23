import time
import sys
from thread import start_new_thread
from OSC import  OSCClient, OSCClientError, OSCBundle, OSCMessage
from random import random, choice, uniform

osc_port = 57120 #default SuperCollider port (must be open before executing this program)
interval = 1.12 # time between events, in seconds
# just some easy ansi colors printing: 'o' for ok, 'w' for warning, 'e' for error.
def printc(t, c='o'):
    print '\033[9' + {'o': '2m','w': '3m','e': '1m'}[c] + t + '\033[0m'

onset_types = ['','departure','emergence','anacrusis','attack','upbeat','downbeat']
continuant_types = ['','passage','transition','prolongation','maintenance','statement']
termination_types = ['','arrival','disappearence','closure','release','resolution','plane']
spectrum_types = ['harmonic', 'inharmonic','granular','saturated']

def send_event():
    try:
        bundle = OSCBundle()
        msg = OSCMessage("/entry")
        #onset
        msg.append(choice(onset_types))
        msg.append(uniform(0.01, 0.3)) #duration
        msg.append(choice(spectrum_types))
        #continuant
        msg.append(choice(continuant_types))
        msg.append(uniform(0.4, 2)) #duration
        msg.append(choice(spectrum_types))
        #termination
        msg.append(choice(termination_types))
        msg.append(uniform(0.2, 0.5)) #duration
        msg.append(choice(spectrum_types))

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
            send_event()
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

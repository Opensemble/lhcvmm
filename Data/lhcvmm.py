import time
import sys
from OSC import  OSCClient, OSCClientError, OSCBundle, OSCMessage
import argparse
import json

data_file = None
tree = None

parser = argparse.ArgumentParser(description='Process ATLAS data and streams through OSC.')
parser.add_argument('-d','--datafile', help='Load events from csv.', default='data_files/data_Egamma_A.root')
parser.add_argument('--host', help='OSC host.', default='127.0.0.1')
parser.add_argument('--port', help='OSC port.', default=57120, type=int)
parser.add_argument('-m','--messagename', help='OSC message name.', default='sound_unit')
parser.add_argument('-r','--rate', help='Rate at which events will be read.', default=1.0, type=float)
parser.add_argument('-l','--loop', help='Loop events sequence.', default=True, type=bool)
#parser.add_argument('--offset', help='Offset of read data.', default=0, type=int)
parser.add_argument('--limit', help='Limit the amount of events to read.', type=int)

args = parser.parse_args()

# just some easy ansi colors printing: 'o' for ok, 'w' for warning, 'e' for error.
def printc(t, c='o'):
    print '\033[9' + {'o': '2m','w': '3m','e': '1m'}[c] + t + '\033[0m'

def mapValue(value, leftMin, leftMax, rightMin, rightMax):
    if value < leftMin:
        return rightMin
    if value > leftMax:
        return rightMax
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin
    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)
    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)

def send_event_old():
    spectral_densities = ['filled', 'packed', 'opaque','translucent','transparent','empty']

    data = ['']*17*3
    #onset, continuant, termination
    data[0] = 'attack'

    #elegimos el de mayor momento transversal
    i = [l for l in tree.lep_pt].index(max(tree.lep_pt))

    #duration, based on momento transversal .. lep_pt
    data[1] = mapValue(tree.lep_pt[i],0,100000,0.1,10)
    #Spectrum types: electrones : inarmonico  ,  muones:  granular
    data[10] = 'inharmonic' if tree.lep_type[i] == 11 else 'granular'
    #Spectrum occupation: angulo
    data[11] = 'center'
    #Spectrum density: lepton energy .. lep_E
    data[16] = spectral_densities[int(mapValue(tree.lep_E[i],0,100000,0,5))]

    bundle = OSCBundle()
    msg = OSCMessage("/"+args.messagename)
    for d in data:
        msg.append(d)
    bundle.append(msg)
    client.send(bundle)

def send_event():

    data = {}

    data['pvxp_n'] = tree.pvxp_n
    data['mu'] = tree.mu
    data['met_phi'] = tree.met_phi
    data['met_et'] = tree.met_et
    #leptons
    data['leps'] = []
    for i in range(tree.lep_n):
        lep = {}
        lep['lep_pt'] = tree.lep_pt[i]
        lep['lep_phi'] = tree.lep_phi[i]
        lep['lep_eta'] = tree.lep_eta[i]
        lep['lep_E'] = tree.lep_E[i]
        data['leps'].append(lep)

    #jets
    data['jets'] = []
    for i in range(tree.jet_n):
        jet = {}
        jet['jet_pt'] = tree.jet_pt[i]
        jet['jet_phi'] = tree.jet_phi[i]
        jet['jet_eta'] = tree.jet_eta[i]
        jet['jet_E'] = tree.jet_E[i]
        jet['jet_m'] = tree.jet_m[i]
        data['jets'].append(jet)

    json_data = json.dumps(data)

    bundle = OSCBundle()
    msg = OSCMessage("/raw_data")
    msg.append(json_data)
    bundle.append(msg)
    client.send(bundle)



try:
    from ROOT import TFile

    #initialize osc client
    client = OSCClient()
    client.connect((args.host, args.port))   # connect to SuperCollider

    # open test data file
    data_file = TFile.Open(args.datafile, "read")
    #gathers mini ttree
    tree = data_file.Get("mini")

    printc( "TTree 'mini' loaded from %s containing %s entries." % (args.datafile, tree.GetEntriesFast()),'o')
    print "Limit events: %s." % args.limit
    print "Loop enabled: %s." % args.loop
    print "Start Sending events every %s seconds." % args.rate
    print "Each * printed represents an event sent."
    print "Press Ctrl + C to terminate."

    count = tree.GetEntriesFast()
    if args.limit and args.limit < count:
        count = args.limit
    starttime=time.time()

    while 1:
        for i in range(0, count):
            if(i%10==0):
                # restart * printing every 10 events
                sys.stdout.write('\r----------\r')

            # copy next entry into memory
            tree.GetEntry(i)
            send_event()

            time_to_sleep = args.rate - ((time.time() - starttime) % args.rate)
            sys.stdout.write('*')
            sys.stdout.flush()
            time.sleep(time_to_sleep)

        if not args.loop:
            break

except OSCClientError:
    printc( "\OSCClientError: Connection refused to %s on port %s." % args.host, args.port, 'e')
except KeyboardInterrupt:
    if data_file is not None:
        data_file.Close()
    print "\nProgram terminated."
except ReferenceError:
    printc( "Error: %s file not found. Data will be simulated." % args.datafile, 'e')
    printc( "Download it from https://tripiana.web.cern.ch/tripiana/openensemble/ATLAS_data/data_Egamma_A.root\n", 'w')
except AttributeError:
    printc( "Error: Expected TTree 'mini' in %s file.\n" % args.datafile, 'e')
except ImportError:
    printc( "Error: Unable to load ROOT.\n", 'e')
    printc( "Checkout https://github.com/Opensemble/lhcvmm/tree/master/Data#installing-root for further instructions.\n",'e')




'''
From varlist.pdf sent by Martin Tripiana.

Proposed content of slimmed down version of the a data sample for educational purposes.
The content presented is a further tuple produced by AnalysisTop.


+==========================+===============+====================================================================================+
| BRANCH NAME              | TYPE          | DESCRIPTION                                                                        |
+==========================+===============+====================================================================================+
| runNumber                | int           | runNumber                                                                          |
+==========================+===============+====================================================================================+
| eventNumber              | int           | eventNumber                                                                        |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| channelNumber            | int           | channelNumber                                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lbNumber                 | int           | lbNumber                                                                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| rndRunNumber             | int           | randomized run number mimicking run number distribution in data                    |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| mu                       | float         | average interactions per bunch crossing                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| mcWeight                 | float         | weight of an MC event                                                              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| pvxp_n                   | int           | number of primary vertices                                                         |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| isGoodEvent              | int           | summary of diverse quality flags like hfor                                         |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor              | float         | overall scale factor for the preselected event                                     |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| trigE                    | bool          | boolean whether a standard trigger has fired in the egamma stream                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| trigM                    | bool          | boolean whether a standard trigger has fired in the muon stream                    |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| passGRL                  | bool          | signifies whether event passes the GRL may be put in isGoodEvent                   |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| hasGoodVertex            | bool          | signifies whether the event has at least one good vertex                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_n                    | int           | number of preselected leptons                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_truthMatched         | vector<bool>  | boolean indicating whether the lepton is matched to a truth lepton                 |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_trigMatched          | vector<bool>  | boolean signifying whether the lepton is the one triggering the event              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_pt                   | vector<float> | transverse momentum of the lepton                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_eta                  | vector<float> | pseudo-rapidity of the lepton                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_phi                  | vector<float> | azimuthal angle of the lepton                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_E                    | vector<float> | energy of the lepton                                                               |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_z0                   | vector<float> | z-coordinate of the track associated to the lepton wrt.                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_charge               | vector<float> | the primary vertex charge of the lepton                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_isTight              | vector<bool>  | boolean indicating whether the lepton is of tight quality                          |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_flag                 | vector<int>   | bitmask implementing object cuts of the top group                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_type                 | vector<int>   | number signifying the lepton type (e, mu, tau) of the lepton                       |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_ptcone30             | vector<float> | ptcone30 isolation for the lepton                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_etcone20             | vector<float> | etcone20 isolation for the lepton                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_trackd0pvunbiased    | vector<float> | d0 of the track associated to the lepton at the point of closest approach (p.o.a.) |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_tracksigd0pvunbiased | vector<float> | d0 signifcance of the track associated to the lepton at the p.o.a.                 |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| met_et                   | float         | Transverse energy of the missing momentum vector                                   |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| met_phi                  | float         | Azimuthal angle of the missing momentum vector                                     |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_n                    | int           | number of selected jets                                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_pt                   | vector<float> | transverse momentum of the jet                                                     |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_eta                  | vector<float> | pseudorapidity of the jet                                                          |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_phi                  | vector<float> | azimuthal angle of the jet                                                         |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_E                    | vector<float> | energy of the jet                                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_m                    | vector<float> | invariant mass of the jet                                                          |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_jvf                  | vector<float> | JetVertexFraction of the jet                                                       |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_flag                 | vector<int>   | bitmask implementing object cuts of the top group                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_trueflav             | vector<int>   | true flavor of the jet                                                             |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_truthMatched         | vector<int>   | information whether the jet matches a jet on truth level                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_SV0                  | vector<float> | SV0 weight of the jet                                                              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_MV1                  | vector<float> | MV1 weight of the jet                                                              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_BTAG         | float         | scalefactor for btagging                                                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_ELE          | float         | scalefactor for electron eciency                                                   |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_JVFSF        | float         | scalefactor for jet vertex fraction                                                |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_MUON         | float         | scalefactor for muon eciency                                                       |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_PILEUP       | float         | scalefactor for pileup reweighting                                                 |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_TRIGGER      | float         | scalefactor for trigger                                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_ZVERTEX      | float         | scalefactor for z-vertex reweighting                                               |
+==========================+===============+====================================================================================+


'''

#!/usr/local/bin/python

import time
import sys
from OSC import  OSCClient, OSCClientError, OSCBundle, OSCMessage
import argparse
import json
import os

data_file = None
tree = None

parser = argparse.ArgumentParser(description='Process ATLAS data and streams through OSC.')
parser.add_argument('-d','--datafile', help='Load events from csv.', default='data_files/data_Egamma_A.root')
parser.add_argument('--host', help='OSC host.', default='127.0.0.1')
parser.add_argument('--port', help='OSC port.', default=57120, type=int)
parser.add_argument('-r','--rate', help='Rate at which events will be read.', default=1.0, type=float)
parser.add_argument('-l','--loop', help='Loop events sequence.', default=True, type=bool)
parser.add_argument('-o','--offset', help='Offset of read data.', default=0, type=int)
parser.add_argument('-c','--count', help='Num of events to read.', type=int)

args = parser.parse_args()

# just some easy ansi colors printing: 'o' for ok, 'w' for warning, 'e' for error.
def printc(t, c='o'):
    print '\033[9' + {'o': '2m','w': '3m','e': '1m'}[c] + t + '\033[0m'

def send_event( event_index, events_count,events_rate, elapsed_time):

    data = {}
    data['event_index'] = event_index
    data['events_count'] = events_count
    data['events_rate'] = events_rate
    data['elapsed_time'] = elapsed_time

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
    dir = os.path.dirname(__file__)
    filename = os.path.join(dir, args.datafile)
    data_file = TFile.Open(filename, "read")
    #gathers mini ttree
    tree = data_file.Get("mini")

    printc( "TTree 'mini' loaded from %s containing %s entries." % (args.datafile, tree.GetEntriesFast()),'o')
    print "Events count: %s." % args.count
    print "Loop enabled: %s." % args.loop
    print "Offset : %s." % args.offset
    print "Start Sending events every %s seconds." % args.rate
    print "Each * printed represents an event sent."
    print "Press Ctrl + C to terminate."

    count = tree.GetEntriesFast()
    if args.count:
        count = args.limit
    start_time=time.time()

    while 1:
        for i in range(args.offset, args.offset + count):
            # copy next entry into memory
            tree.GetEntry(i)
            elapsed_time = time.time()-start_time
            send_event(i,count,args.rate,elapsed_time)

            #visual feedback
            sys.stdout.write('\r' + '-' * (i%10) + '\033[92m' + '*' + '\033[0m' + '-' * (9 - i%10) + '{0:>10}'.format(i) )
            sys.stdout.flush()

            #accurate sleeping
            time_to_sleep = args.rate - ((time.time() - start_time) % args.rate)
            time.sleep(time_to_sleep)

        if not args.loop:
            break

except OSCClientError:
    printc("\nOSCClientError: Connection refused to {0} on port {1}.".format(args.host, args.port), 'e')
except KeyboardInterrupt:
    if data_file is not None:
        data_file.Close()
    print "\nProgram terminated."
except ReferenceError:
    printc( "\nError: {0} file not found.".format(args.datafile), 'e')
    printc( "Download it from https://tripiana.web.cern.ch/tripiana/openensemble/ATLAS_data/data_Egamma_A.root\n", 'w')
except AttributeError:
    printc( "Error: Expected TTree 'mini' in %s file.\n" % args.datafile, 'e')
except ImportError:
    printc( "Error: Unable to load ROOT.\n", 'e')
    printc( "Checkout https://github.com/Opensemble/lhcvmm/tree/master/Data#installing-root for further instructions.\n",'e')

except:
    print "Unexpected error:", sys.exc_info()[0]
    raise


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

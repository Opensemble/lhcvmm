
#include "EventStream.h"

void EventStream::setup(vector<Datafile> *dataFiles, OscMultiClient *oscMultiClient, int channel){
    this->channel = channel;
    this->oscMultiClient = oscMultiClient;
    this->dataFiles = dataFiles;
    parameters.setName("Event stream #" + std::to_string(channel));
  
    
  parameters.add(dataFileIndex.set("select file",0,0,(*dataFiles).size()-1));
  parameters.add(fileToAdd.set("file", (*dataFiles)[dataFileIndex].name));

  parameters.add(enabled.set("enabled", false));
  parameters.add(rate.set("rate", 1.0f, 0.1f, 5.0f));
  parameters.add(randomness.set("randomness", 0.0f, 0.0f, 1.0f));
  parameters.add(loop.set("loop", true));
  parameters.add(offset.set("offset", 0, 0, 1000));
  parameters.add(count.set("count", 16, 1, 1000));

  dataFileIndex.addListener(this, &EventStream::dataFileIndexChanged);
  enabled.addListener(this, &EventStream::enabledChanged);
}

void EventStream::update(){

}

void EventStream::dataFileIndexChanged(int & val){
  fileToAdd = (*dataFiles)[dataFileIndex].name;
  enabled = false; //when file is changed, it must be manually re-enabled
}


void EventStream::enabledChanged(bool & val){
  if(val){
    loadTTree();
    startThread(true);    // blocking, non verbose
  }
  else{
    stopThread();
      if(data_file!= NULL && data_file->IsOpen())
          data_file->Close();
    data_file = NULL;
    tree = NULL;
  }
  //ofLog(OF_LOG_NOTICE) << "enabledChanged" << val;

}

void EventStream::loadTTree(){
  data_file = TFile::Open((*dataFiles)[dataFileIndex].path.c_str());
  tree = (TTree*)data_file->Get("mini");
  events_count = tree->GetEntriesFast();

  tree->SetBranchAddress("pvxp_n", &data.pvxp_n);
  tree->SetBranchAddress("mu", &data.mu);
  tree->SetBranchAddress("met_phi", &data.met_phi);
  tree->SetBranchAddress("met_et", &data.met_et);

  tree->SetBranchAddress("lep_n", &data.lep_n);
  tree->SetBranchAddress("lep_pt", &data.lep_pt);
  tree->SetBranchAddress("lep_phi", &data.lep_phi);
  tree->SetBranchAddress("lep_eta", &data.lep_eta);
  tree->SetBranchAddress("lep_E", &data.lep_E);

    tree->SetBranchAddress("jet_n", &data.jet_n);
    tree->SetBranchAddress("jet_pt", &data.jet_pt);
    tree->SetBranchAddress("jet_phi", &data.jet_phi);
    tree->SetBranchAddress("jet_eta", &data.jet_eta);
    tree->SetBranchAddress("jet_E", &data.jet_E);
    tree->SetBranchAddress("jet_m", &data.jet_m);

  offset.setMax(events_count);
  count.setMax(events_count);

}



EventStream::EventStream(){
}

EventStream::~EventStream(){
	waitForThread(true);
}



void EventStream::threadedFunction() {
    float startTime = ofGetElapsedTimef();
  while(1) {

    for(currentEventIndex = offset;currentEventIndex< (offset + count); currentEventIndex++){
      if(!isThreadRunning())
        return;



      // copy next entry into memory
      tree->GetEntry(currentEventIndex);

      //calculate rate and elapsed time
      float r = rate * ( 1.0f +  randomness * ofRandomf() );
        currentEventTime= ofGetElapsedTimef();
        nextEventTime = currentEventTime+r;

      stringstream json;
      json << "{";
      json << "\"channel\":" << channel << ",";
      json << "\"event_index\":" << currentEventIndex << ",";
      json << "\"events_count\":" << count << ",";
      json << "\"events_rate\":" << rate << ",";
      json << "\"elapsed_time\":" << (currentEventTime-startTime) << ",";

      json << "\"pvxp_n\":" << data.pvxp_n << ",";
      json << "\"mu\":" << data.mu << ",";
      json << "\"met_phi\":" << data.met_phi << ",";
      json << "\"met_et\":" << data.met_et << ",";

      //leptons
      json << "\"leps\": [ ";
      for(int j= 0;j< data.lep_n && j< MAX_EVENT_ARRAY;j++){
          if(j>0)
              json << ",";
        json << "{";
        json << "\"lep_pt\":" << data.lep_pt[j] << ",";
        json << "\"lep_phi\":" << data.lep_phi[j] << ",";
        json << "\"lep_eta\":" << data.lep_eta[j] << ",";
        json << "\"lep_E\":" << data.lep_E[j];
        json << "}";
      }
        json << "],";
     
      //jets
      json << "\"jets\": [ ";
      for(int j= 0;j< data.jet_n && j< MAX_EVENT_ARRAY;j++){
          if(j>0)
              json << ",";
          json << "{";
        json << "\"jet_pt\":" << data.jet_pt[j] << ",";
        json << "\"jet_phi\":" << data.jet_phi[j] << ",";
        json << "\"jet_eta\":" << data.jet_eta[j] << ",";
        json << "\"jet_E\":" << data.jet_E[j] << ",";
        json << "\"jet_m\":" << data.jet_m[j];
        json << "}";
      }
      json << "]";

      json << "}";


      oscMultiClient->sendMessage(json.str());
        
      //send_event(i,count,rate,elapsed_time)
      //ofLog(OF_LOG_NOTICE) << r;
      sleep(r*1000);
    }

    if(!loop)
      return;

  }

}

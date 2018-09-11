#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>

#include "PDAnalyzer.h"

#include "TDirectory.h"
#include "TBranch.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TFile.h"

// additional features
#include "PDSecondNtupleWriter.h"   // second ntuple
//#include "DataSetFilter.cc"       // dataset filter
#include "PDMuonVar.cc"
#include "PDSoftMuonMvaEstimator.cc"
#include "AlbertoUtil.cc"

using namespace std;

/*
pdTreeAnalyze /lustre/cmswork/abragagn/ntuList/MC2016Lists/BsToJpsiPhi_BMuonFilter_2016_DCAP.list hist.root -v histoMode RECREATE -v use_gen t -n 10000
*/

PDAnalyzer::PDAnalyzer() {

    std::cout << "new PDAnalyzer" << std::endl;

    // user parameters are set as names associated to a string, 
    // default values can be set in the analyzer class contructor

    setUserParameter( "verbose", "f" );
    setUserParameter( "process", "BsJPsiPhi" );
    setUserParameter( "useTightSel", "f" );
    setUserParameter( "useHLT", "f" );

    setUserParameter( "ptCut", "40.0" ); //needed for paolo's code for unknow reasons

}


PDAnalyzer::~PDAnalyzer() {
}



void PDAnalyzer::beginJob() {

    PDAnalyzerUtil::beginJob();

    // user parameters are retrieved as strings by using their names;
    // numeric parameters ( int, float or whatever ) can be directly set
    // by passing the corresponding variable,
    // e.g. getUserParameter( "name", x )

    getUserParameter( "verbose", verbose );
    getUserParameter( "process", process );
    getUserParameter( "useTightSel", useTightSel );
    getUserParameter( "useHLT", useHLT );

    getUserParameter( "ptCut", ptCut ); //needed for paolo's code for unknow reasons

// to skim the N-tuple "uncomment" the following lines
    dropBranch( "*tau*" ); 
    dropBranch( "*ele*" ); 
    initWSkim( new TFile( "skim.root", "RECREATE" ) );


// additional features
//  DataSetFilter::beginJob();

    SetBpMassRange(5.15, 5.40);

    return;

}


void PDAnalyzer::book() {

    // putting "autoSavedObject" in front of the histo creation 
    // it's automatically marked for saving on file; the option 
    // is uneffective when not using the full utility

    return;

}


void PDAnalyzer::reset() {
// automatic reset
    autoReset();
    return;
}


bool PDAnalyzer::analyze( int entry, int event_file, int event_tot ) {

    if ( verbose ) {
        cout << " +++++++++++++++++++++++++++ " << endl;
        cout << "entry: "
                 << entry << " " << event_file << " " << event_tot << endl;
        cout << "run: " << runNumber << " , "
                 << "evt: " << eventNumber << endl;
    }
    else {

        if ( (!(event_tot%10) && event_tot<100 ) || 
     (!(event_tot %100) && event_tot<1000 ) || 
     (!(event_tot %1000) && event_tot<10000 ) || 
     (!(event_tot %10000) && event_tot<100000 ) || 
     (!(event_tot %100000) && event_tot<1000000 ) || 
     (!(event_tot %1000000) && event_tot<10000000 ) )
            cout << " == at event " << event_file << " " << event_tot << endl;
    }


    // flag to be set "true" or "false" for events accepted or rejected

    int whichHLT = 0;
    if(useHLT && (process=="BsJPsiPhi")){
        bool hltFlag = false;
        for(int i=0; i<nHLTStatus; ++i){
            if(!hltAccept->at( i )) continue;
            if((hltPath->at( i ) == PDEnumString::HLT_Dimuon0_Jpsi3p5_Muon2_v) || (hltPath->at( i ) == PDEnumString::HLT_Dimuon0_Jpsi_Muon_v)) {hltFlag = true; whichHLT += pow(2, 1);}
            if(hltPath->at( i ) == PDEnumString::HLT_DoubleMu4_JpsiTrkTrk_Displaced_v) {hltFlag = true; whichHLT += pow(2, 2);}
            if(hltPath->at( i ) == PDEnumString::HLT_DoubleMu4_JpsiTrk_Displaced_v) {hltFlag = true; whichHLT += pow(2, 3);}
        }
        if(!hltFlag) return false;
    }

    if(useHLT && (process=="BuJPsiK")){
        bool hltFlag = false;
        for(int i=0; i<nHLTStatus; ++i){
            if(!hltAccept->at( i )) continue;
            if(hltPath->at( i ) == PDEnumString::HLT_DoubleMu4_JpsiTrk_Displaced_v) {hltFlag = true; whichHLT += pow(2, 3);}
        }
        if(!hltFlag) return false;
    }

    if(nMuons<3) return false;

    if( GetCandidate(process, useTightSel)<0 ) return false;

    fillSkim();

    return true;

}


void PDAnalyzer::endJob() {
// to skim the N-tuple "uncomment" the following line
  closeSkim();


    return;
}


void PDAnalyzer::save() {
#   if UTIL_USE == FULL
    // explicit saving not necessary for "autoSavedObjects"
    autoSave();
#elif UTIL_USE == BARE
    // explicit save histos when not using the full utility

#endif

    return;
}


// to plot some histogram immediately after the ntuple loop
// "uncomment" the following lines
/*
void PDAnalyzer::plot() {
    TCanvas* can = new TCanvas( "muoPt", "muoPt", 800, 600 );
    can->cd();
    can->Divide( 1, 2 );
    can->cd( 1 );
    hptmumax->Draw();
    hptmu2nd->Draw();
    return;
}
*/


// ======MY FUNCTIONS===============================================================================
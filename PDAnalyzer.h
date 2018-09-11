#ifndef PDAnalyzer_H
#define PDAnalyzer_H

#include "TH1.h"
#include "PDAnalyzerUtil.h"
#include "PDAnalysis/Ntu/interface/PDGenHandler.h"
#include "PDMuonVar.h"
#include "TopDecayClassifier.h"
#include "PDSoftMuonMvaEstimator.h"
#include "AlbertoUtil.h"

// to skim the N-tuple "uncomment" the following line
#include "NtuTool/Common/interface/TreeFilter.h"

// additional features
//#include "DataSetFilter.h"
class PDSecondNtupleWriter;

// to skim the N-tuple replace the the following line
// with the "commented" ones
class PDAnalyzer:   public virtual PDAnalyzerUtil
,                   public virtual PDGenHandler
,                   public virtual PDMuonVar
,                   public virtual PDSoftMuonMvaEstimator
,                   public virtual AlbertoUtil

// additional features
//,                              public virtual DataSetFilter    // dataset filter
// to skim the N-tuple "uncomment" the following line
,                              public virtual TreeFilter
 {

 public:

    PDAnalyzer();
    virtual ~PDAnalyzer();

    // function called before starting the analysis
    virtual void beginJob();

    // functions to book the histograms
    void book();

    // functions called for each event
    // function to reset class content before reading from file
    virtual void reset();
    // function to do event-by-event analysis,
    // return value "true" for accepted events
    virtual bool analyze( int entry, int event_file, int event_tot );

    // function called at the end of the analysis
    virtual void endJob();

    // functions called at the end of the event loop
// to plot some histogram immediately after the ntuple loop
// "uncomment" the following line
//  virtual void plot();     // plot the histograms on the screen
    virtual void save();     // save the histograms on a ROOT file


 protected:

    double ptCut; //needed for paolo's code for unknow reasons

    bool verbose;
    bool useHLT;
    bool useTightSel;
    TString process;


    TH1D* hmass_JPsi;
    TH1D* hmass_Bs;

// additional features: second ntuple
    PDSecondNtupleWriter* tWriter;                               // second ntuple

 private:


    // dummy copy constructor and assignment
    PDAnalyzer ( const PDAnalyzer& );
    PDAnalyzer& operator=( const PDAnalyzer& );

};


#endif


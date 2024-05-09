#ifndef _calol1_ctp7_CTP7Readout_h_
#define _calol1_ctp7_CTP7Readout_h_


// Swatch Headers
#include "swatch/processor/ReadoutInterface.hpp"

// calol1
#include "UCT2016Layer1CTP7.hh"


namespace calol1 {

  typedef UCT2016Layer1CTP7::DAQStatus DAQStatus;
  typedef UCT2016Layer1CTP7::TTCBGoCmdCnt TTCBGoCmdCnt;

  class CTP7Readout : public swatch::processor::ReadoutInterface {
  public:
    CTP7Readout(UCT2016Layer1CTP7* const client);
    ~CTP7Readout();

    void retrieveMetricValues();


  private:
    // no copy constructor
    CTP7Readout(const CTP7Readout&);
    // no assignment operator
    const CTP7Readout& operator=(const CTP7Readout&);

    UCT2016Layer1CTP7* const client_;
  };

} // calol1

 
#endif // header guard

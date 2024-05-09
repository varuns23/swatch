#ifndef _calol1_ctp7_CicadaReadout_h_
#define _calol1_ctp7_CicadaReadout_h_


// Swatch Headers
#include "swatch/processor/ReadoutInterface.hpp"

// calol1
#include "UCT2023SummaryCTP7.hh"


//namespace calol1 {

namespace swatch {
namespace dummy {

  //typedef UCT2016Layer1CTP7::DAQStatus DAQStatus; //TO DO
  //typedef UCT2016Layer1CTP7::TTCBGoCmdCnt TTCBGoCmdCnt; //TO DO

  class CicadaReadout : public swatch::processor::ReadoutInterface {
  public:
    CicadaReadout(UCT2023SummaryCTP7* const client);
    ~CicadaReadout();

    void retrieveMetricValues();


  private:
    // no copy constructor
    CicadaReadout(const CicadaReadout&);
    // no assignment operator
    const CicadaReadout& operator=(const CicadaReadout&);

    UCT2023SummaryCTP7* const client_;
  };

}} // calol1

 
#endif // header guard

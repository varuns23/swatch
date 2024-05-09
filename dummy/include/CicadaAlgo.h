#ifndef _calol1_ctp7_CicadaAlgo_h_
#define _calol1_ctp7_CicadaAlgo_h_

#include <map>
#include <vector>
#include <string>
#include <time.h>
#include <memory>

// Swatch and xdaq headers
#include "swatch/processor/AlgoInterface.hpp"
#include "xdata/UnsignedInteger.h"
#include "xdata/UnsignedInteger64.h"
#include "xdata/Table.h"
#include "xdata/Vector.h"

// CaloL1 headers
#include "UCT2023SummaryCTP7.hh"

//namespace calol1 {
namespace swatch {
namespace dummy {


  class CicadaAlgo : public swatch::processor::AlgoInterface {
  public:
    CicadaAlgo(UCT2023SummaryCTP7* const client,
             const unsigned int phi);
    virtual ~CicadaAlgo() ;
   

  protected:
    virtual void retrieveMetricValues();

  private:
    // no copy constructor
    CicadaAlgo(const CicadaAlgo&);
    // no assignment operator
    const CicadaAlgo& operator=(const CicadaAlgo&);

    UCT2023SummaryCTP7* const client_;

    unsigned int phi_;
    
    swatch::core::SimpleMetric<bool>& mMetricAlgoWorking; 

  };
 
}} // namespace calol1

#endif


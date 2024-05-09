#ifndef _calol1_ctp7_CTP7TTC_h_
#define _calol1_ctp7_CTP7TTC_h_

#include "swatch/processor/TTCInterface.hpp"
#include "swatch/core/SimpleMetric.hpp"
#include "swatch/core/MetricConditions.hpp"

#include "UCT2016Layer1CTP7.hh"


namespace calol1
{
  typedef UCT2016Layer1CTP7::TTCStatus TTCStatus;
  typedef UCT2016Layer1CTP7::TTCBGoCmdCnt TTCBGoCmdCnt;

  class CTP7TTC : public swatch::processor::TTCInterface
    {
    public:
      CTP7TTC(UCT2016Layer1CTP7* const client);
      ~CTP7TTC();

      void retrieveMetricValues();


    private:
      // no copy constructor
      CTP7TTC(const CTP7TTC&);
      // no assignment operator
      const CTP7TTC& operator=(const CTP7TTC&);

      UCT2016Layer1CTP7* const client_;

      swatch::core::SimpleMetric<uint32_t>& mMetricEventCounter;
      swatch::core::SimpleMetric<uint32_t>& mMetricUnlockedBC0Counter;
    };

}


#endif // header guard

#ifndef _calol1_ctp7_CicadaTTC_h_
#define _calol1_ctp7_CicadaTTC_h_

#include "swatch/processor/TTCInterface.hpp"
#include "swatch/core/SimpleMetric.hpp"
#include "swatch/core/MetricConditions.hpp"

#include "UCT2023SummaryCTP7.hh"


//namespace calol1
//{

namespace swatch {
namespace dummy {

  typedef UCT2023SummaryCTP7::TTCStatus TTCStatus;
//  typedef UCT2016Layer1CTP7::TTCBGoCmdCnt TTCBGoCmdCnt; // TO DO

  class CicadaTTC : public swatch::processor::TTCInterface
    {
    public:
      CicadaTTC(UCT2023SummaryCTP7* const client);
      ~CicadaTTC();

      void retrieveMetricValues();


    private:
      // no copy constructor
      CicadaTTC(const CicadaTTC&);
      // no assignment operator
      const CicadaTTC& operator=(const CicadaTTC&);

      UCT2023SummaryCTP7* const client_;
      
      swatch::core::SimpleMetric<uint32_t>& mMetricEventCounter;
      swatch::core::SimpleMetric<uint32_t>& mMetricUnlockedBC0Counter;

    };

}}


#endif // header guard

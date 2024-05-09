#include "CicadaTTC.h"

#include <vector>

//using namespace calol1;

namespace swatch {
namespace dummy {


CicadaTTC::CicadaTTC(UCT2023SummaryCTP7* const client) :
  client_(client),
  mMetricEventCounter(registerMetric<uint32_t>("EC0Count")),
  mMetricUnlockedBC0Counter(registerMetric<uint32_t>("UnlockedBX0Count",
                                                    swatch::core::GreaterThanCondition<uint32_t>(100),
                                                    swatch::core::RangeCondition<uint32_t>(10,100)))
{
}


CicadaTTC::~CicadaTTC() {}

void CicadaTTC::retrieveMetricValues() 
{
   TTCStatus status;
   bool success = client_->getTTCStatus(status);
   // TTCBGoCmdCnt counters; //TO DO: to be implemented 
   // success &= client_->getTTCBGoCmdCnt(counters);  // TO DO: to be implemented

   if(success)
     {
      uint32_t tmpValue = 555; 
      setMetricValue<uint32_t>(mMetricBunchCounter, tmpValue); //TO DO: setMetricValue<uint32_t>(mMetricBunchCounter, counters.BX0);
      setMetricValue<uint32_t>(mMetricOrbitCounter, tmpValue); //TO DO: setMetricValue<uint32_t>(mMetricOrbitCounter, counters.OC0);
      setMetricValue<uint32_t>(mMetricEventCounter, tmpValue); //TO DO: setMetricValue<uint32_t>(mMetricEventCounter, counters.EC0); 
      setMetricValue<uint32_t>(mMetricSingleBitErrors,
                               status.TTCDecoderSingleError);
      setMetricValue<uint32_t>(mMetricDoubleBitErrors,
                               status.TTCDecoderDoubleError);
      setMetricValue<uint32_t>(mMetricUnlockedBC0Counter, status.BX0UnlockedCnt);
      setMetricValue<uint32_t>(mMetricL1ACounter, tmpValue); //TO DO: setMetricValue<uint32_t>(mMetricL1ACounter, counters.L1A);
      // in CTP7, locked and running are synonymous
      setMetricValue<bool>(mMetricIsClock40Locked, bool(status.BCClockLocked));
      setMetricValue<bool>(mMetricHasClock40Stopped, 
                           !bool(status.BCClockLocked)); 
      setMetricValue<bool>(mMetricIsBC0Locked, bool(status.BX0Locked));
     }
  else
     {
     // something went wrong, set to error values
      setMetricValue<uint32_t>(mMetricBunchCounter, 0);
      setMetricValue<uint32_t>(mMetricOrbitCounter, 0);
      setMetricValue<uint32_t>(mMetricEventCounter, 0);
      setMetricValue<uint32_t>(mMetricSingleBitErrors, 9999);
      setMetricValue<uint32_t>(mMetricDoubleBitErrors, 9999);
      setMetricValue<uint32_t>(mMetricUnlockedBC0Counter, 9999);
      setMetricValue<uint32_t>(mMetricL1ACounter, 0);
      // in CTP7, locked and running are synonymous
      setMetricValue<bool>(mMetricIsClock40Locked, false);
      setMetricValue<bool>(mMetricHasClock40Stopped, true);
      setMetricValue<bool>(mMetricIsBC0Locked, false);
     }

}

}}

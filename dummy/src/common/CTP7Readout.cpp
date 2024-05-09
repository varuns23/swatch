#include "CTP7Readout.h"

#include "swatch/core/TTSUtils.hpp"

#include<sstream>

//using namespace calol1;

namespace swatch {
namespace dummy {


CTP7Readout::CTP7Readout(UCT2016Layer1CTP7* const client) :
  client_(client)
{}


CTP7Readout::~CTP7Readout() {}


void CTP7Readout::retrieveMetricValues()
{
  TTCBGoCmdCnt ttcStatus;
  bool ttcSuccess = client_->getTTCBGoCmdCnt(ttcStatus);

  DAQStatus daqStatus;
  bool daqSuccess = client_->getDAQStatus(daqStatus);

  if(ttcSuccess && daqSuccess)
    {
      setMetricValue<swatch::core::tts::State>(mMetricTTS, swatch::core::tts::State(daqStatus.TTS) );
      setMetricValue<bool>(mMetricAMCCoreReady, daqStatus.amcCoreReady);
      setMetricValue<uint32_t>(mMetricEventCounter, ttcStatus.L1ID);
    }
  else
    {
      // something went wrong, set to error values

      setMetricValue<swatch::core::tts::State>(mMetricTTS, swatch::core::tts::State::kError);
      setMetricValue<bool>(mMetricAMCCoreReady, false);
      setMetricValue<uint32_t>(mMetricEventCounter, 9999);
    }
}
 
}}

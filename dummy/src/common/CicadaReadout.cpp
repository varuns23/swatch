#include "CicadaReadout.h"

#include "swatch/core/TTSUtils.hpp"

#include<sstream>

//using namespace calol1;

namespace swatch {
namespace dummy {


CicadaReadout::CicadaReadout(UCT2023SummaryCTP7* const client) :
  client_(client)
{}


CicadaReadout::~CicadaReadout() {}


void CicadaReadout::retrieveMetricValues()
{
   // TTCBGoCmdCnt ttcStatus; // TO DO: to be implemented
   // bool ttcSuccess = client_->getTTCBGoCmdCnt(ttcStatus); //TO DO: to be implemented
   bool ttcSuccess = true;

   // DAQStatus daqStatus; // TO DO: to be implemented
   // bool daqSuccess = client_->getDAQStatus(daqStatus);
   bool daqSuccess = true;

   if(ttcSuccess && daqSuccess)
    {
      setMetricValue<swatch::core::tts::State>(mMetricTTS, swatch::core::tts::State::kReady); //TO DO: setMetricValue<swatch::core::tts::State>(mMetricTTS, swatch::core::tts::State(daqStatus.TTS) );
      setMetricValue<bool>(mMetricAMCCoreReady, true); //TO DO: setMetricValue<bool>(mMetricAMCCoreReady, daqStatus.amcCoreReady);
      setMetricValue<uint32_t>(mMetricEventCounter, 555); //TO DO: setMetricValue<uint32_t>(mMetricEventCounter, ttcStatus.L1ID);
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

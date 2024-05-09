#include "CTP7LinkInterface.h"

//using namespace calol1;

namespace swatch {
namespace dummy {



CTP7InputLinkInterface::CTP7InputLinkInterface(UCT2016Layer1CTP7* const client) :
  client_(client)
{
}

CTP7InputLinkInterface::~CTP7InputLinkInterface() {}


void CTP7InputLinkInterface::retrieveMetricValues()
{
  updateLinkStatuses();
}


void CTP7InputLinkInterface::distributeLinkStatuses(const std::vector<LinkStatus>& inputsPos,
                                                    const std::vector<LinkStatus>& inputsNeg)
{
  size_t iPort = 0;
  
  for(auto iStatus = inputsPos.begin(); 
      iStatus != inputsNeg.end();
      ++iStatus)
    {
      if(iStatus == inputsPos.end())
        iStatus = inputsNeg.begin();

      dynamic_cast<CTP7Input*>(getPorts().at(iPort))->setStatus(*iStatus);
      ++iPort;
    }
}


void CTP7InputLinkInterface::updateLinkStatuses()
{
  std::vector<LinkStatus> inputLinkStatusesPos;
  bool successEtaPos = client_->getInputLinkStatus(false, inputLinkStatusesPos);
  std::vector<LinkStatus> inputLinkStatusesNeg;
  bool successEtaNeg = client_->getInputLinkStatus(true, inputLinkStatusesNeg);

  if(!(successEtaPos && successEtaNeg))
    {
      LinkStatus allBad;
      allBad.bx0ErrorCount = 9999;
      allBad.checkSumErrorCount = 9999;
      allBad.linkGotAligned = false;
      allBad.linkLocked = false;
      allBad.towerMask = 0x7ff;
      allBad.linkUp = 0;
      allBad.rawLinkStatus = 0;
      allBad.linkFifoEmpty = 0;
      allBad.bx0Latency = 9999;
      allBad.alignmentMask = 0;

      inputLinkStatusesPos.clear();
      inputLinkStatusesPos.resize(32, allBad);
      inputLinkStatusesNeg.clear();
      inputLinkStatusesNeg.resize(32, allBad);
    }

  distributeLinkStatuses(inputLinkStatusesPos, inputLinkStatusesNeg);
}


CTP7OutputLinkInterface::CTP7OutputLinkInterface(UCT2016Layer1CTP7* const client) :
  client_(client)
{
}

CTP7OutputLinkInterface::~CTP7OutputLinkInterface() {}


void CTP7OutputLinkInterface::retrieveMetricValues()
{
  updateLinkStatuses();
}


void CTP7OutputLinkInterface::distributeLinkStatuses(const std::vector<LinkStatus>& outputsPos,
                                                     const std::vector<LinkStatus>& outputsNeg)
{
  std::vector<LinkStatus>::const_iterator iOutStatus = outputsPos.begin();
  
  for(auto iOut = getPorts().begin(); iOut != getPorts().end(); ++iOut)
    {
      if(iOutStatus == outputsPos.end())
        iOutStatus = outputsNeg.begin();
  
      dynamic_cast<CTP7Output*>(*iOut)->setStatus(*iOutStatus);
      
      ++iOutStatus;
    }
}


void CTP7OutputLinkInterface::updateLinkStatuses()
{
  // Default for now
  LinkStatus defStatus;
  defStatus.linkLocked = 1;
  for(auto i = getPorts().begin(); i != getPorts().end(); ++i)
    dynamic_cast<CTP7Output*>(*i)->setStatus(defStatus);

  // Output link status not currently available in FW. The following will 
  // work when it's available
  // std::vector<LinkStatus> outputLinkStatusesPos;
  // client_->getOutputLinkStatus(false, outputLinkStatusesPos);
  // std::vector<LinkStatus> outputLinkStatusesNeg;
  // client_->getOutputLinkStatus(true, outputLinkStatusesNeg);

  // distributeLinkStatuses(outputLinkStatusesPos, outputLinkStatusesNeg);
}

}}

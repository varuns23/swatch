#include "CTP7Input.h"

#include<sstream>
#include<iomanip>

//using namespace calol1;

namespace swatch {
namespace dummy {


CTP7Input::CTP7Input(const std::string& aID,
                     const uint32_t number) :
  swatch::processor::InputPort(aID),
  negativeEta_(aID[0] == 'N' || aID[0] == 'n'),
  id_(getID(number)),
  name_(aID),
  statusSet_(false),
  mMetricBX0Errors(registerMetric<uint32_t>("BX0Errors",
                                            swatch::core::GreaterThanCondition<uint32_t>(5000),
                                            swatch::core::RangeCondition<uint32_t>(1000,5000))),
  mMetricTowerMask(registerMetric<std::string>("TowerMask")),
  mMetricIsUp(registerMetric<bool>("Linkisup",
                                   swatch::core::EqualCondition<bool>(false)))
{
  setErrorCondition(mMetricCRCErrors, 
                    swatch::core::GreaterThanCondition<uint32_t>(30000));
  setWarningCondition(mMetricCRCErrors, 
                      swatch::core::RangeCondition<uint32_t>(15000,30000));

}


CTP7Input::~CTP7Input() {}

void CTP7Input::retrieveMetricValues() 
{
  if(!statusSet_)
    return;

  setMetricValue<uint32_t>(mMetricBX0Errors, status_.bx0ErrorCount);
  setMetricValue<uint32_t>(mMetricCRCErrors, status_.checkSumErrorCount);
  setMetricValue<bool>(mMetricIsAligned, bool(status_.linkGotAligned));
  setMetricValue<bool>(mMetricIsLocked, bool(status_.linkLocked));
  setMetricValue<bool>(mMetricIsUp, bool(status_.linkUp));

  std::stringstream towerMaskStream;
  towerMaskStream << std::showbase << std::hex << std::uppercase 
                  << status_.towerMask;
  setMetricValue<std::string>(mMetricTowerMask, towerMaskStream.str());
}


const InputLink CTP7Input::getID(const unsigned int linkNum,
                                 const bool isHCAL,
                                 const bool isHF) const
{
  unsigned int n = linkNum;

  if(isHCAL && !isHF)
      n += 0x10;

  if(isHF)
    n += 0x1e;

  return InputLink(n);
}


void CTP7Input::setStatus(const LinkStatus status)
{
  status_ = status;

  statusSet_ = true;
}

}}

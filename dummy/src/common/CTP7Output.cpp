#include "CTP7Output.h"


//using namespace calol1;

namespace swatch {
namespace dummy {


CTP7Output::CTP7Output(const std::string& aID,
                       const uint32_t number) :
  swatch::processor::OutputPort(aID),
  negativeEta_(aID[0] == 'N' || aID[0] == 'n'),
  id_(getID(number)),
  name_(aID),
  statusSet_(false)
{}


CTP7Output::~CTP7Output() {}

void CTP7Output::retrieveMetricValues() 
{
  if(!statusSet_)
    return;
  
  setMetricValue<bool>(mMetricIsOperating, bool(status_.linkLocked));
}


void CTP7Output::setStatus(const LinkStatus status)
{
  status_ = status;
  statusSet_ = true;
}
}}

#include "CicadaInput.h"

#include<sstream>
#include<iomanip>

//using namespace calol1;

namespace swatch {
namespace dummy {


CicadaInput::CicadaInput(const std::string& aID,
                     const uint32_t number) :
  swatch::processor::InputPort(aID),
  mMetricIsUp(registerMetric<bool>("Linkisup",
                                   swatch::core::EqualCondition<bool>(false)))
{
}


CicadaInput::~CicadaInput() {}

void CicadaInput::retrieveMetricValues() 
{
  setMetricValue<bool>(mMetricIsUp, true);
  setMetricValue<uint32_t>(mMetricCRCErrors, 0);
  setMetricValue<bool>(mMetricIsAligned, true);
  setMetricValue<bool>(mMetricIsLocked, true);

}


}}

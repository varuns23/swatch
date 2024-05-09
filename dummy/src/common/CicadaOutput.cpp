#include "CicadaOutput.h"


//using namespace calol1;

namespace swatch {
namespace dummy {


CicadaOutput::CicadaOutput(const std::string& aID,
                       const uint32_t number) :
  swatch::processor::OutputPort(aID)
{}


CicadaOutput::~CicadaOutput() {}

void CicadaOutput::retrieveMetricValues() 
{
  setMetricValue<bool>(mMetricIsOperating, true);
}

}}

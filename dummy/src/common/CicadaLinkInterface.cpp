#include "CicadaLinkInterface.h"

//using namespace calol1;

namespace swatch {
namespace dummy {



CicadaInputLinkInterface::CicadaInputLinkInterface(UCT2023SummaryCTP7* const client) :
  client_(client)
{
}

CicadaInputLinkInterface::~CicadaInputLinkInterface() {}


void CicadaInputLinkInterface::retrieveMetricValues()
{
}



CicadaOutputLinkInterface::CicadaOutputLinkInterface(UCT2023SummaryCTP7* const client) :
  client_(client)
{
}

CicadaOutputLinkInterface::~CicadaOutputLinkInterface() {}


void CicadaOutputLinkInterface::retrieveMetricValues()
{
}


}}

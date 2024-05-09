#include "CicadaAlgo.h"

#include <stdexcept>
#include <sstream>

#include "swatch/core/MetricConditions.hpp"

//using namespace calol1;

namespace swatch {
namespace dummy {


CicadaAlgo::CicadaAlgo(UCT2023SummaryCTP7* const client, 
                   const unsigned int phi) :
  client_(client),
  phi_(phi),
  mMetricAlgoWorking(registerMetric<bool>("AlgoWorking", 
                                          swatch::core::EqualCondition<bool>(false)))
{
}

CicadaAlgo::~CicadaAlgo()
{
}


void CicadaAlgo::retrieveMetricValues()
{
   setMetricValue<bool>(mMetricAlgoWorking, true);
}

}}



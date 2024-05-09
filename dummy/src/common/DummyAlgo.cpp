
#include "swatch/dummy/DummyAlgo.hpp"


#include <string>

#include "swatch/core/MetricConditions.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


DummyAlgo::DummyAlgo(DummyProcDriver& aDriver) :
  AlgoInterface(),
  mDriver(aDriver),
  mRateCounterA(registerMetric<float>("rateCounterA", core::GreaterThanCondition<float>(80e3), core::GreaterThanCondition<float>(40e3))),
  mRateCounterB(registerMetric<float>("rateCounterB", core::GreaterThanCondition<float>(80e3), core::GreaterThanCondition<float>(40e3)))
{
  for (size_t i = 0; i < 500; i++)
    registerMetric<float>("rate_counter_" + std::to_string(i));
}


DummyAlgo::~DummyAlgo()
{
}


void DummyAlgo::retrieveMetricValues()
{
  DummyProcDriver::AlgoStatus lStatus = mDriver.getAlgoStatus();

  setMetricValue(mRateCounterA, lStatus.rateCounterA);
  setMetricValue(mRateCounterB, lStatus.rateCounterB);
}

} // namespace dummy
} // namespace swatch

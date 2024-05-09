
#include "swatch/dummy/DummyAMC13Interfaces.hpp"


// SWATCH headers
#include "swatch/core/MetricConditions.hpp"
#include "swatch/dummy/DummyAMC13Driver.hpp"


namespace swatch {
namespace dummy {


AMC13BackplaneDaqPort::AMC13BackplaneDaqPort(uint32_t aSlot, DummyAMC13Driver& aDriver) :
  dtm::AMCPort(aSlot),
  mDriver(aDriver),
  mOOS(registerMetric<bool>("outOfSync", core::EqualCondition<bool>(true))),
  mTTSWarning(registerMetric<bool>("ttsWarning")),
  mAMCEventCount(registerMetric<uint64_t>("amcEventCount"))
{
  setWarningCondition<>(mTTSWarning, core::EqualCondition<bool>(true));
}

AMC13BackplaneDaqPort::~AMC13BackplaneDaqPort()
{
}

void AMC13BackplaneDaqPort::retrieveMetricValues()
{
  DummyAMC13Driver::AMCPortStatus lStatus = mDriver.readAMCPortStatus(getSlot());

  setMetricValue<>(mOOS, lStatus.outOfSync);
  setMetricValue<>(mTTSWarning, lStatus.ttsWarning);
  setMetricValue<>(mAMCEventCount, lStatus.amcEventCount);
}


//--------------------------------------------------------------------

AMC13EventBuilder::AMC13EventBuilder(DummyAMC13Driver& aDriver) :
  mDriver(aDriver),
  mOOS(registerMetric<bool>("outOfSync", core::EqualCondition<bool>(true))),
  mTTSWarning(registerMetric<bool>("ttsWarning")),
  mL1ACount(registerMetric<uint64_t>("l1aCount"))
{
  setWarningCondition<>(mTTSWarning, core::EqualCondition<bool>(true));
}

AMC13EventBuilder::~AMC13EventBuilder()
{
}

void AMC13EventBuilder::retrieveMetricValues()
{
  DummyAMC13Driver::EventBuilderStatus lStatus = mDriver.readEvbStatus();

  setMetricValue<>(mOOS, lStatus.outOfSync);
  setMetricValue<>(mTTSWarning, lStatus.ttsWarning);
  setMetricValue<>(mL1ACount, lStatus.l1aCount);
}


//--------------------------------------------------------------------

AMC13SLinkExpress::AMC13SLinkExpress(uint32_t aSfpId, DummyAMC13Driver& aDriver) :
  dtm::SLinkExpress(aSfpId),
  mDriver(aDriver),
  mCoreInitialised(registerMetric<bool>("coreInitialised", core::EqualCondition<bool>(false))),
  mBackPressure(registerMetric<bool>("backPressure")),
  mWordsSent(registerMetric<uint32_t>("wordsSent")),
  mPacketsSent(registerMetric<uint32_t>("packetsSent"))
{
  setWarningCondition<>(mBackPressure, core::EqualCondition<bool>(true));
}

AMC13SLinkExpress::~AMC13SLinkExpress()
{
}

void AMC13SLinkExpress::retrieveMetricValues()
{
  DummyAMC13Driver::SLinkStatus lStatus = mDriver.readSLinkStatus();

  setMetricValue<>(mCoreInitialised, lStatus.coreInitialised);
  setMetricValue<>(mBackPressure, lStatus.backPressure);
  setMetricValue<>(mWordsSent, lStatus.wordsSent);
  setMetricValue<>(mPacketsSent, lStatus.packetsSent);
}


//--------------------------------------------------------------------

AMC13TTC::AMC13TTC(DummyAMC13Driver& aDriver) :
  mDriver(aDriver),
  mClockFreq(registerMetric<double>("clockFreq", core::InvRangeCondition<double>(39.9e6, 40.1e6))),
  mBC0Counter(registerMetric<uint32_t>("bc0Counter")),
  mErrCountBC0(registerMetric<uint32_t>("errCountBC0", core::GreaterThanCondition<uint32_t>(0))),
  mErrCountSingleBit(registerMetric<uint32_t>("errCountSingleBit", core::GreaterThanCondition<uint32_t>(0))),
  mErrCountDoubleBit(registerMetric<uint32_t>("errCountDoubleBit", core::GreaterThanCondition<uint32_t>(0))),
  mWarningSign(registerMetric<bool>("warningSign"))
{
  setWarningCondition(mWarningSign, core::EqualCondition<bool>(true));
}

AMC13TTC::~AMC13TTC()
{
}

void AMC13TTC::retrieveMetricValues()
{
  DummyAMC13Driver::TTCStatus lStatus = mDriver.readTTCStatus();

  setMetricValue<>(mClockFreq, lStatus.clockFreq);
  setMetricValue<>(mBC0Counter, lStatus.bc0Counter);
  setMetricValue<>(mErrCountBC0, lStatus.errCountBC0);
  setMetricValue<>(mErrCountSingleBit, lStatus.errCountSingleBit);
  setMetricValue<>(mErrCountDoubleBit, lStatus.errCountDoubleBit);
  setMetricValue<>(mWarningSign, lStatus.warningSign);
}


} // namespace dummy
} // namespace swatch


#include "swatch/dummy/DummyProcessor7.hpp"

// SWATCH headers
#include "swatch/action/CommandSequence.hpp"
#include "swatch/action/StateMachine.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/MetricSnapshot.hpp"
#include "swatch/dummy/DummyAlgo.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"
#include "swatch/dummy/DummyProcessorCommands7.hpp"
#include "swatch/dummy/DummyReadout.hpp"
#include "swatch/dummy/DummyRxPort.hpp"
#include "swatch/dummy/DummyTTC.hpp"
#include "swatch/dummy/DummyTxPort.hpp"
#include "swatch/dummy/utilities.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Vector.h"

// C++ Headers
#include <iomanip>


SWATCH_REGISTER_CLASS(swatch::dummy::DummyProcessor7)


namespace swatch {
namespace dummy {


DummyProcessor7::DummyProcessor7(const swatch::core::AbstractStub& aStub) :
  Processor(aStub),
  mDriver(new DummyProcDriver())
{
  // 1) Interfaces
  registerInterface(new DummyTTC(*mDriver));
  registerInterface(new DummyReadoutInterface(*mDriver));
  registerInterface(new DummyAlgo(*mDriver));
  registerInterface(new processor::InputPortCollection());
  registerInterface(new processor::OutputPortCollection());

  const processor::ProcessorStub& stub = getStub();

  for (auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    getInputPorts().addPort(new DummyRxPort(it->id, it->number, *mDriver));
  for (auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    getOutputPorts().addPort(new DummyTxPort(it->id, it->number, *mDriver));

  // 2) Monitoring
  std::vector<core::MonitorableObject*> lInputPorts;
  std::vector<core::AbstractMetric*> lCRCErrorMetrics;
  for (auto lIt = getInputPorts().getPorts().begin(); lIt != getInputPorts().getPorts().end(); lIt++) {
    lCRCErrorMetrics.push_back(&(*lIt)->getMetric(processor::InputPort::kMetricIdCRCErrors));
    lInputPorts.push_back(*lIt);
  }
  registerComplexMetric<uint32_t>("totalCRCErrors", lCRCErrorMetrics.begin(), lCRCErrorMetrics.end(), core::ComplexMetric<uint32_t>::CalculateFunction_t(&sumUpCRCErrors), &filterOutMaskedPorts);
  registerComplexMetric<uint32_t>("portsInError", lInputPorts.begin(), lInputPorts.end(), core::ComplexMetric<uint32_t>::CalculateFunction2_t(&countObjectsInError), &filterOutMaskedPorts);

  // 3) Commands
  action::Command& reboot7 = registerCommand<DummyResetCommand7>("reboot7");
  action::Command& reset7 = registerCommand<DummyResetCommand7>("reset7");
  action::Command& cfgTx7 = registerCommand<DummyConfigureTxCommand7>("configureTx7");
  action::Command& cfgRx7 = registerCommand<DummyConfigureRxCommand7>("configureRx7");
  action::Command& cfgDaq7 = registerCommand<DummyConfigureDaqCommand7>("configureDaq7");
  action::Command& cfgAlgo7 = registerCommand<DummyConfigureAlgoCommand7>("configureAlgo7");

  registerCommand<DummyProcessorForceClkTtcStateCommand7>("forceClkTtcState7");
  registerCommand<DummyProcessorForceRxPortsStateCommand7>("forceRxPortsState7");
  registerCommand<DummyProcessorForceTxPortsStateCommand7>("forceTxPortsState7");
  registerCommand<DummyProcessorForceReadoutStateCommand7>("forceReadoutState7");
  registerCommand<DummyProcessorForceAlgoStateCommand7>("forceAlgoState7");

  // 4) Command sequences
  action::CommandSequence& cfgSeq7 = registerSequence("configPartA", reset7).then(cfgDaq7).then(cfgTx7);
  registerSequence("fullReconfigure", reset7).then(cfgDaq7).then(cfgAlgo7).then(cfgRx7).then(cfgTx7);

  // 5) State machines
  processor::RunControlFSM& lFSM = getRunControlFSM();
  lFSM.coldReset.add(reboot7);
  lFSM.setup.add(cfgSeq7);
  lFSM.configure.add(cfgAlgo7);
  lFSM.align.add(cfgRx7);
  lFSM.fsm.addTransition("dummyNoOp", processor::RunControlFSM::kStateAligned, processor::RunControlFSM::kStateInitial);
}


DummyProcessor7::~DummyProcessor7()
{
}


std::string DummyProcessor7::firmwareInfo() const
{
  return "none";
}


void DummyProcessor7::retrieveMetricValues()
{
  setMetricValue<uint64_t>(mMetricFirmwareVersion, mDriver->getFirmwareVersion());
}


}
}

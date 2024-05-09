
#include "swatch/dummy/DummyProcessorCommands7.hpp"


// XDAQ headers
#include "xdata/String.h"
#include "xdata/Vector.h"

// SWATCH headers
#include "swatch/dummy/DummyProcDriver.hpp"
#include "swatch/dummy/DummyProcessor7.hpp"
#include "swatch/processor/Port.hpp"
#include "swatch/processor/PortCollection.hpp"

#include <iostream>


namespace swatch {
namespace dummy {


//////////////////////////
/*  DummyRebootCommand7  */

DummyRebootCommand7::DummyRebootCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyRebootCommand7::~DummyRebootCommand7()
{
}

void DummyRebootCommand7::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  lDriver.reboot();
}


/////////////////////////
/*  DummyResetCommand7  */

DummyResetCommand7::DummyResetCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyResetCommand7::~DummyResetCommand7()
{
}

void DummyResetCommand7::runAction(bool aGoIntoError)
{
std::cout << " Reset 7 " << std::endl ;
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  if (!aGoIntoError)
    lDriver.reset();
}


///////////////////////////////
/*  DummyConfigureTxCommand7  */

DummyConfigureTxCommand7::DummyConfigureTxCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureTxCommand7::~DummyConfigureTxCommand7()
{
}

void DummyConfigureTxCommand7::runAction(bool aGoIntoError)
{
  DummyProcessor7& lProc = getActionable<DummyProcessor7>();

  xdata::Vector<xdata::String> lPorts;
  for (auto lIt = lProc.getOutputPorts().getPorts().begin(); lIt != lProc.getOutputPorts().getPorts().end(); lIt++)
    lPorts.push_back((*lIt)->getId());
  addExecutionDetails("ports", lPorts);

  DummyProcDriver& lDriver = lProc.getDriver();
  if (!aGoIntoError)
    lDriver.configureTxPorts();
}


///////////////////////////////
/*  DummyConfigureRxCommand7  */

DummyConfigureRxCommand7::DummyConfigureRxCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureRxCommand7::~DummyConfigureRxCommand7()
{
}

void DummyConfigureRxCommand7::runAction(bool aGoIntoError)
{
  DummyProcessor7& lProc = getActionable<DummyProcessor7>();
std::cout << " configure 7 " << std::endl ;

  xdata::Vector<xdata::String> lPorts;
  for (auto lIt = lProc.getInputPorts().getPorts().begin(); lIt != lProc.getInputPorts().getPorts().end(); lIt++) {
    if (!(*lIt)->isMasked())
      lPorts.push_back((*lIt)->getId());
  }
  addExecutionDetails("ports", lPorts);

  DummyProcDriver& lDriver = lProc.getDriver();
  if (!aGoIntoError)
    lDriver.configureRxPorts();
}


////////////////////////////////
/*  DummyConfigureDaqCommand7  */

DummyConfigureDaqCommand7::DummyConfigureDaqCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureDaqCommand7::~DummyConfigureDaqCommand7()
{
}

void DummyConfigureDaqCommand7::runAction(bool aGoIntoError)
{

  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  if (!aGoIntoError)
    lDriver.configureReadout();
}


/////////////////////////////////
/*  DummyConfigureAlgoCommand7  */

DummyConfigureAlgoCommand7::DummyConfigureAlgoCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureAlgoCommand7::~DummyConfigureAlgoCommand7()
{
}

void DummyConfigureAlgoCommand7::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  if (!aGoIntoError)
    lDriver.configureAlgo();
}


/////////////////////////////////////////////
/*  DummyProcessorForceClkTtcStateCommand7  */

DummyProcessorForceClkTtcStateCommand7::DummyProcessorForceClkTtcStateCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceClkTtcStateCommand7::~DummyProcessorForceClkTtcStateCommand7()
{
}

action::Command::State DummyProcessorForceClkTtcStateCommand7::code(const core::ParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  lDriver.forceClkTtcState(parseState(aParamSet));
  return kDone;
}


//////////////////////////////////////////////
/*  DummyProcessorForceRxPortsStateCommand7  */

DummyProcessorForceRxPortsStateCommand7::DummyProcessorForceRxPortsStateCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceRxPortsStateCommand7::~DummyProcessorForceRxPortsStateCommand7()
{
}

action::Command::State DummyProcessorForceRxPortsStateCommand7::code(const core::ParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  lDriver.forceRxPortsState(parseState(aParamSet));
  return kDone;
}


//////////////////////////////////////////////
/*  DummyProcessorForceTxPortsStateCommand7  */

DummyProcessorForceTxPortsStateCommand7::DummyProcessorForceTxPortsStateCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceTxPortsStateCommand7::~DummyProcessorForceTxPortsStateCommand7()
{
}

action::Command::State DummyProcessorForceTxPortsStateCommand7::code(const core::ParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  lDriver.forceTxPortsState(parseState(aParamSet));
  return kDone;
}


//////////////////////////////////////////////
/*  DummyProcessorForceReadoutStateCommand7  */

DummyProcessorForceReadoutStateCommand7::DummyProcessorForceReadoutStateCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceReadoutStateCommand7::~DummyProcessorForceReadoutStateCommand7()
{
}

action::Command::State DummyProcessorForceReadoutStateCommand7::code(const core::ParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  lDriver.forceReadoutState(parseState(aParamSet));
  return kDone;
}


///////////////////////////////////////////
/*  DummyProcessorForceAlgoStateCommand7  */

DummyProcessorForceAlgoStateCommand7::DummyProcessorForceAlgoStateCommand7(const std::string& aId, action::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceAlgoStateCommand7::~DummyProcessorForceAlgoStateCommand7()
{
}

action::Command::State DummyProcessorForceAlgoStateCommand7::code(const core::ParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor7>().getDriver();
  lDriver.forceAlgoState(parseState(aParamSet));
  return kDone;
}



} // namespace dummy
} // namespace swatch

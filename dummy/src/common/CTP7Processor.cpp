// SWATCH includes
#include "swatch/core/Factory.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/action/CommandSequence.hpp"
#include "swatch/action/StateMachine.hpp"

// CaloL1 includes
#include "CTP7Processor.h"
#include "CTP7TTC.h"
#include "CTP7Algo.h"
#include "CTP7Input.h"
#include "CTP7Output.h"
#include "CTP7Readout.h"
#include "CTP7LinkInterface.h"
#include "CTP7Commands.h"

// UCT2016Layer1
#include "UCT2016Layer1CTP7.hh"

// STL includes
#include <sstream>
#include <stdexcept>

//SWATCH_REGISTER_CLASS(calol1::CTP7Processor);
//using namespace calol1;

SWATCH_REGISTER_CLASS(swatch::dummy::CTP7Processor);

namespace swatch {
namespace dummy {


CTP7Processor::CTP7Processor(const swatch::core::AbstractStub& aStub) :
  swatch::processor::Processor(aStub),
  phi_(stoi(getStub().uri)),
  client_(getNewClient(phi_)),
  clientInError_(false),
  metricFirmwareBuildTimestamp_(registerMetric<uint32_t>("FirmwareBuildTimestamp")),
  metricFirmwareGitHash_(registerMetric<uint32_t>("FirmwareGitHashCode")),
  metricFirmwareGitHashDirty_(registerMetric<bool>("FirmwareGitHashDirty")),
  metricFirmwareUptime_(registerMetric<uint32_t>("FirmwareUptime")),
  metricClientInError_(registerMetric<bool>("Clientinerror",
                                            swatch::core::EqualCondition<bool>(true)))
 {
  const swatch::processor::ProcessorStub& stub = getStub();

  registerInterface(new CTP7TTC(client_));
  registerInterface(new CTP7Algo(client_, phi_));
  registerInterface(new CTP7Readout(client_));
  registerInterface(new CTP7InputLinkInterface(client_));
  registerInterface(new CTP7OutputLinkInterface(client_));
  
  for(auto i = stub.rxPorts.begin(); i != stub.rxPorts.end(); ++i)
    getInputPorts().addPort(new CTP7Input(i->id, i->number));
  for(auto i = stub.txPorts.begin(); i != stub.txPorts.end(); ++i)
    getOutputPorts().addPort(new CTP7Output(i->id, i->number));

  // Individual registered functions, see CTP7Commands.cc/h
  registerCommand<CTP7AlignInputLinks>("AlignInputLinks");
  registerCommand<CTP7AlignOutputLinks>("AlignOutputLinks");
  registerCommand<CTP7SetTowerMask>("SetTriggerTowerMask");
  registerCommand<CTP7SetInputLinkAlignmentMask>("SetInputLinkAlignmentMask");
  registerCommand<CTP7ResetInputLinkDecoders>("ResetInputLinkDecoders");
  registerCommand<CTP7ResetInputLinkChecksumErrorCounters>("ResetInputLinkCECs");
  registerCommand<CTP7ResetInputLinkBX0ErrorCounters>("ResetInputLinkBX0ErrorCounters");
  registerCommand<CTP7GetInputLinkAlignmentStatus>("GetInputLinkAlignmentStatus");
  //registerCommand<CTP7SetInputLinkLUT>("SetInputLinkLUTs");
  registerCommand<CTP7AlignTTCDecoder>("AlignTTCDecoder");
  registerCommand<CTP7SetDAQConfig>("SetDAQConfig");
 // action::Command& reset = registerCommand<DummyResetCommand>("reset");
  action::Command& SetRunNumber = registerCommand<CTP7SetRunNumber>("SetRunNumber");
 //  registerCommand<CTP7SetRunNumber>("SetRunNumber");
  registerCommand<CTP7Sleep>("Sleep");
  registerCommand<CTP7SetTMTCycle>("TMTCycleConfig");
  registerCommand<CTP7CheckBCClockAndBX0Lock>("CheckBCClockandBX0Lock");
  registerCommand<LoadLUTs>("LoadLUTs");
  registerCommand<CheckLUTs>("ForceLUTCheck");
  registerCommand<CheckLUT2>("ForceSecond_StageLUTCheck");
  registerCommand<CheckLUT3>("ForceHCalFBLUTCheck");
  registerCommand<HardReset>("HardReset");

  registerSequence("hardReset", "HardReset").run("HardReset");
  registerSequence("resetErrors", "ResetInputLinkCECs")
    .run("ResetInputLinkBX0ErrorCounters");
  registerSequence("TMTCycleConfigure", "TMTCycleConfig");
  swatch::action::CommandSequence& configCardSeq = registerSequence("checkBCClockAndBX0Lock", 
                                                                    "CheckBCClockandBX0Lock"
                                                                    )
    .run("SetTriggerTowerMask")
    .run("SetInputLinkAlignmentMask")
    .run("TMTCycleConfig");
//    .run("LoadLUTs");
  swatch::action::CommandSequence& alignCardSeq = registerSequence("alignCard", 
                                                                   "ResetInputLinkDecoders"
                                                                   )
    .run("AlignInputLinks")
    .run("Sleep")
    .run("ResetInputLinkCECs")
    .run("ResetInputLinkBX0ErrorCounters")
    .run("GetInputLinkAlignmentStatus")
    .run("AlignOutputLinks");
  swatch::action::CommandSequence& setupSeq = registerSequence("setup", 
                                                               SetRunNumber
                                                               )
    .then("SetDAQConfig")
    .then("AlignTTCDecoder");
  swatch::action::CommandSequence& alignTTCDecoderSeq = registerSequence("alignTTCDecoder", 
                                                                         "AlignTTCDecoder");

  // Finite State Machine
  swatch::processor::RunControlFSM& lFSM = getRunControlFSM();
  lFSM.setup.add( setupSeq );
  lFSM.configure.add( configCardSeq );
  lFSM.align.add( alignCardSeq );
  //lFSM.start.add(getCommand("ForceLUTCheck"));
  lFSM.stopFromAligned //.add(getCommand("ForceLUTCheck"))
    .add( alignTTCDecoderSeq ).add( configCardSeq );
  lFSM.stopFromRunning //.add(getCommand("ForceLUTCheck"))
    .add( alignTTCDecoderSeq ).add( configCardSeq );
}

void CTP7Processor::retrieveMetricValues()
{
  FWInfo fwInfo;
  
  if(client_->getFWInfo(fwInfo))
    {
      setClientErrorStatus(false);

      setMetricValue<uint64_t>(mMetricFirmwareVersion, fwInfo.version);
      setMetricValue<uint32_t>(metricFirmwareBuildTimestamp_, fwInfo.buildTimestamp);
      setMetricValue<uint32_t>(metricFirmwareGitHash_, fwInfo.gitHashCode);
      setMetricValue<bool>(metricFirmwareGitHashDirty_, bool(fwInfo.gitHashDirty));
      setMetricValue<uint32_t>(metricFirmwareUptime_, fwInfo.uptime);
    }
  else
    {
      setClientErrorStatus(true);

      setMetricValue<uint64_t>(mMetricFirmwareVersion, 0);
      setMetricValue<uint32_t>(metricFirmwareBuildTimestamp_, 0);
      setMetricValue<uint32_t>(metricFirmwareGitHash_, 0);
      setMetricValue<bool>(metricFirmwareGitHashDirty_, true);
      setMetricValue<uint32_t>(metricFirmwareUptime_, 0);      
    }

  setMetricValue<bool>(metricClientInError_, clientInError_);
}


UCT2016Layer1CTP7* const
CTP7Processor::getNewClient(unsigned short iPhi) const
{
  UCT2016Layer1CTP7* out = 0;
  try
    {
      out = new UCT2016Layer1CTP7(iPhi, 
                                  "CTP7map.xml", 
                                  UCT2016Layer1CTP7::CONNECTSTRING_PHIMAPXML);

      if(!out->checkConnection())
        throw std::runtime_error(std::string("Connection check failed."));

      return out;
    } 
  catch (std::runtime_error& e)
    {
      delete out;

      std::stringstream exMsg;
      
      exMsg << "Failed to connect to CTP7 " << iPhi << " "
            << "(" << getStub().crate << " slot " << getStub().slot << ")!\n"
            << "Client connection failed with message:\n    "
            << e.what();

      throw std::runtime_error(exMsg.str());
    }
  catch(...)
    {
      delete out;
      throw;
    }
}


void CTP7Processor::reportClientError(const std::string& failingTask)
{
  std::stringstream s;
  
  s << "The CTP7 client for iPhi=" << phi()
    << " failed to connect while attempting task: "
    << failingTask;

  reportError("Client Error", s.str());
}

}}

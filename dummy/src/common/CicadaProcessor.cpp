// SWATCH includes
#include "swatch/core/Factory.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/action/CommandSequence.hpp"
#include "swatch/action/StateMachine.hpp"

// CaloL1 includes
#include "CicadaProcessor.h"
#include "CicadaTTC.h"
#include "CicadaAlgo.h"
#include "CicadaInput.h"
#include "CicadaOutput.h"
#include "CicadaReadout.h"
#include "CicadaLinkInterface.h"
#include "CicadaCommands.h"

// UCT2023Summary
#include "UCT2023SummaryCTP7.hh"

// STL includes
#include <sstream>
#include <stdexcept>

//SWATCH_REGISTER_CLASS(calol1::CicadaProcessor);
//using namespace calol1;

SWATCH_REGISTER_CLASS(swatch::dummy::CicadaProcessor);

namespace swatch {
namespace dummy {


CicadaProcessor::CicadaProcessor(const swatch::core::AbstractStub& aStub) :
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

  registerInterface(new CicadaTTC(client_));
  registerInterface(new CicadaAlgo(client_, phi_));
  registerInterface(new CicadaReadout(client_));
  registerInterface(new CicadaInputLinkInterface(client_));
  registerInterface(new CicadaOutputLinkInterface(client_));
  
  for(auto i = stub.rxPorts.begin(); i != stub.rxPorts.end(); ++i)
    getInputPorts().addPort(new CicadaInput(i->id, i->number));
  for(auto i = stub.txPorts.begin(); i != stub.txPorts.end(); ++i)
    getOutputPorts().addPort(new CicadaOutput(i->id, i->number));

  // Individual registered functions, see CicadaCommands.cc/h
  registerCommand<CicadaAlignInputLinks>("AlignInputLinks");
  registerCommand<CicadaSleep>("Sleep");
  registerCommand<CicadaAlignTTCDecoder>("AlignTTCDecoder") ;
  registerCommand<CicadaSetBackPlaneInputLinkMask>("SetBackPlaneInputLinkMask") ;
  registerCommand<CicadaSetOpticalInputLinkMask>("SetOpticalInputLinkMask") ;
  registerCommand<CicadaEnableTXCXP0CXP1Links>("EnableTXCXP0CXP1Links") ;
  registerCommand<CicadaResetBackPlaneMGTs>("ResetBackPlaneMGTs") ;
  registerCommand<CicadaResetTxCXP1>("ResetTxCXP1") ;
  registerCommand<CicadaResetRxCXP1>("ResetRxCXP1") ;
  registerCommand<CicadaResetInput8b10bErrorCounters>("ResetInput8b10bErrorCounters") ;


  swatch::action::CommandSequence& configCardSeq = registerSequence("config","SetBackPlaneInputLinkMask") 
    .run("SetOpticalInputLinkMask") ;
  swatch::action::CommandSequence& alignCardSeq = registerSequence("alignCard","AlignInputLinks")  
    .run("Sleep") ;
  swatch::action::CommandSequence& setupSeq = registerSequence("setup","AlignTTCDecoder")
    .run("EnableTXCXP0CXP1Links")
    .run("ResetBackPlaneMGTs")
    .run("ResetTxCXP1")
    .run("ResetRxCXP1")
    .run("ResetInput8b10bErrorCounters")
    .run("AlignTTCDecoder") ; 
  swatch::action::CommandSequence& alignTTCDecoderSeq = registerSequence("alignTTCDecoder","AlignTTCDecoder") ;

  // Finite State Machine
  swatch::processor::RunControlFSM& lFSM = getRunControlFSM();
  lFSM.setup.add( setupSeq );
  lFSM.configure.add( configCardSeq );
  lFSM.align.add( alignCardSeq );
  lFSM.stopFromAligned 
    .add( alignTTCDecoderSeq ).add( configCardSeq );
  lFSM.stopFromRunning 
    .add( alignTTCDecoderSeq ).add( configCardSeq );
}

void CicadaProcessor::retrieveMetricValues()
{
      //setClientErrorStatus(true);

      setMetricValue<uint64_t>(mMetricFirmwareVersion, 0);
      setMetricValue<uint32_t>(metricFirmwareBuildTimestamp_, 0);
      setMetricValue<uint32_t>(metricFirmwareGitHash_, 0);
      setMetricValue<bool>(metricFirmwareGitHashDirty_, true);
      setMetricValue<uint32_t>(metricFirmwareUptime_, 0);      
    

   setMetricValue<bool>(metricClientInError_, clientInError_);
}


UCT2023SummaryCTP7* const
CicadaProcessor::getNewClient(unsigned short iPhi) const
{
  UCT2023SummaryCTP7* out = 0;
  try
    {
      out = new UCT2023SummaryCTP7(iPhi, 
                                  "CTP7map.xml", 
                                  UCT2023SummaryCTP7::CONNECTSTRING_PHIMAPXML);

      if(!out->checkConnection())
        throw std::runtime_error(std::string("Connection check failed."));

      return out;
    } 
  catch (std::runtime_error& e)
    {
      delete out;

      std::stringstream exMsg;
      
      exMsg << "Failed to connect to Cicada " << iPhi << " "
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


void CicadaProcessor::reportClientError(const std::string& failingTask)
{
  std::stringstream s;
  
  s << "The Cicada client for iPhi=" << phi()
    << " failed to connect while attempting task: "
    << failingTask;

  reportError("Client Error", s.str());
}

}}

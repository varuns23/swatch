#include "CTP7Commands.h"
#include "log4cplus/loggingmacros.h"

// CTP7 client
#include "UCT2016Layer1CTP7.hh"

// CaloL1 
#include "CTP7Processor.h"
#include "CTP7Input.h"
#include "CTP7LinkInterface.h"
#include "CTP7Algo.h"

// boost
#include "boost/chrono.hpp"
#include <boost/thread/thread.hpp>

// SWATCH 
#include "swatch/processor/PortCollection.hpp"
#include "swatch/system/System.hpp"
#include "swatch/system/Service.hpp"

// xdaq/TS
#include "xdata/Vector.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/Integer.h"
#include "xdata/Boolean.h"
#include "xdata/String.h"
#include "xdata/Float.h"
#include "xdata/Table.h"

// stl
#include <stdio.h>
#include <iostream>

//using namespace calol1;

namespace swatch {
namespace dummy {


CTP7SetTowerMask::CTP7SetTowerMask(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
  // does xdata::Vector really not provide a constructor from std::vector?
  xdata::Vector<xdata::UnsignedInteger> zeros;
  zeros.resize(32);
  for(size_t i = 0; i < zeros.size(); ++i)
    zeros.at(i) = xdata::UnsignedInteger(0x0);

  registerParameter("towerMaskPosEta", zeros);
  registerParameter("towerMaskNegEta", zeros);
}

CTP7SetTowerMask::~CTP7SetTowerMask() {
}

swatch::action::Command::State CTP7SetTowerMask::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Set tower mask command started");

  // automatic cast of xdata::UnsignedInteger to uint32_t doesn't work :(
  const xdata::Vector<xdata::UnsignedInteger>& maskPosRaw = 
    params.get<xdata::Vector<xdata::UnsignedInteger> >("towerMaskPosEta");
  const xdata::Vector<xdata::UnsignedInteger>& maskNegRaw = 
    params.get<xdata::Vector<xdata::UnsignedInteger> >("towerMaskNegEta");
  std::vector<uint32_t> maskPos = std::vector<uint32_t>(maskPosRaw.size());
  std::vector<uint32_t> maskNeg = std::vector<uint32_t>(maskNegRaw.size());
  for(size_t i = 0; i < maskPos.size(); ++i)
    maskPos.at(i) = uint32_t(maskPosRaw.at(i).value_);
  for(size_t i = 0; i < maskNeg.size(); ++i)
    maskNeg.at(i) = uint32_t(maskNegRaw.at(i).value_);
  
  bool successPos = proc.client().setInputLinkTowerMask(false, maskPos);
  bool successNeg = proc.client().setInputLinkTowerMask(true,  maskNeg);

  if(successPos && successNeg)
    {
      setStatusMsg("Tower masks set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Tower masks not set, something is wrong.");

  proc.reportClientError("tower masking");

  return swatch::action::Command::State::kError;
}


CTP7SetInputLinkAlignmentMask::CTP7SetInputLinkAlignmentMask(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
}

CTP7SetInputLinkAlignmentMask::~CTP7SetInputLinkAlignmentMask() {
}

swatch::action::Command::State CTP7SetInputLinkAlignmentMask::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Set Input Links Alignment Mask command started");

  // uint vector to send to CTP7 (one per link)
  std::vector<uint32_t> maskPos = std::vector<uint32_t>(32, 0x0);
  std::vector<uint32_t> maskNeg = std::vector<uint32_t>(32, 0x0);
  
  for(auto iLink = proc.getInputPorts().getPorts().begin();
      iLink != proc.getInputPorts().getPorts().end();
      iLink++)
    {
      CTP7Input* link = dynamic_cast<CTP7Input*>(*iLink);

      if(link->isMasked())
        {
          if(link->isNegativeEta())
            maskNeg.at(link->getLinkNum()) = 0x1;
          else
            maskPos.at(link->getLinkNum()) = 0x1;
        }
    }

  const bool successEtaPos = proc.client().setInputLinkAlignmentMask( false, maskPos );
  const bool successEtaNeg = proc.client().setInputLinkAlignmentMask( true, maskNeg );
  
  if(successEtaNeg && successEtaPos)
    {
      setStatusMsg("Set Input Links Alignment Mask command completed");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Failed to set link alignment mask");
  
  proc.reportClientError("input link masking");

  return swatch::action::Command::State::kError;
}


CTP7AlignInputLinks::CTP7AlignInputLinks(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) {
  registerParameter("alignBX", xdata::UnsignedInteger(3563));
  registerParameter("alignSubBX", xdata::UnsignedInteger(0));
}

CTP7AlignInputLinks::~CTP7AlignInputLinks() {
}

swatch::action::Command::State CTP7AlignInputLinks::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Align Input Links command just started");

  const uint32_t alignBX(params.get<xdata::UnsignedInteger>("alignBX").value_);
  const uint32_t alignSubBX(params.get<xdata::UnsignedInteger>("alignSubBX").value_);
  
  if (alignBX > 3563) {
    setStatusMsg("Bunch Crossings value can not be above 3563. Talk to Layer1 on call for help.");
    proc.reportError("Bad align BX", "CTP7 at iPhi=" + std::to_string(static_cast<long long unsigned int>(proc.phi())) + " has invalid alignBX");
    return swatch::action::Command::State::kError;
  }
  if (alignSubBX >= 6) {
    setStatusMsg("Bunch Crossings value can not be above 5. Talk to Layer1 on call for help.");
    proc.reportError("Bad align subBX", "CTP7 at iPhi=" + std::to_string(static_cast<long long unsigned int>(proc.phi())) + " has invalid alignSubBX");
    return swatch::action::Command::State::kError;
  }

  const bool successEtaPos = proc.client().alignInputLinks( false, alignBX, alignSubBX, true );
  const bool successEtaNeg = proc.client().alignInputLinks( true,  alignBX, alignSubBX, true );
  
  if(successEtaPos && successEtaNeg)
    {
      setStatusMsg("Align Input Links command completed");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Align Input Links command failed");

  proc.reportClientError("input link alignment");

  return swatch::action::Command::State::kError;
}




CTP7AlignOutputLinks::CTP7AlignOutputLinks(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) {
  registerParameter("alignBX", xdata::UnsignedInteger(3563));
  registerParameter("alignSubBX", xdata::UnsignedInteger(0));
}

CTP7AlignOutputLinks::~CTP7AlignOutputLinks() {
}

swatch::action::Command::State CTP7AlignOutputLinks::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Align Output Links command just started");

  const uint32_t alignBX(params.get<xdata::UnsignedInteger>("alignBX").value_);
  const uint32_t alignSubBX(params.get<xdata::UnsignedInteger>("alignSubBX").value_);
  
  if (alignBX > 3563) {
    setStatusMsg("Bunch Crossings value can not be above 3563. Talk to Layer1 on call for help.");
    proc.reportError("Bad align BX", "CTP7 at iPhi=" + std::to_string(static_cast<long long unsigned int>(proc.phi())) + " has invalid alignBX");
    return swatch::action::Command::State::kError;
  }
  if (alignSubBX >= 6) {
    setStatusMsg("Bunch Crossings value can not be above 5. Talk to Layer1 on call for help.");
    proc.reportError("Bad align subBX", "CTP7 at iPhi=" + std::to_string(static_cast<long long unsigned int>(proc.phi())) + " has invalid alignSubBX");
    return swatch::action::Command::State::kError;
  }

  const bool successEtaPos = proc.client().alignOutputLinks( false, alignBX, alignSubBX );
  const bool successEtaNeg = proc.client().alignOutputLinks( true,  alignBX, alignSubBX );
  
  if(successEtaPos && successEtaNeg)
    {
      setStatusMsg("Align Output Links command completed");
      return swatch::action::Command::State::kDone;
    }

    setStatusMsg("Align Output Links command failed for negative eta!");

    proc.reportClientError("output link alignment");

    return swatch::action::Command::State::kError;
}




CTP7ResetInputLinkDecoders::CTP7ResetInputLinkDecoders(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) {
}

CTP7ResetInputLinkDecoders::~CTP7ResetInputLinkDecoders() {
}

swatch::action::Command::State CTP7ResetInputLinkDecoders::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Reset Input Link Decoders command just started");

  const bool successEtaPos = proc.client().resetInputLinkDecoders( false );
  const bool successEtaNeg = proc.client().resetInputLinkDecoders( true );  
  
  if(successEtaPos && successEtaNeg)
    {
      setStatusMsg("Reset Input Link Decoders command completed");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Reset Input Link Decoders command failed for negative eta!");

  proc.reportClientError("input link decoder reset");

  return swatch::action::Command::State::kError;
}




CTP7ResetInputLinkChecksumErrorCounters::CTP7ResetInputLinkChecksumErrorCounters(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) {
}

CTP7ResetInputLinkChecksumErrorCounters::~CTP7ResetInputLinkChecksumErrorCounters() {
}

swatch::action::Command::State CTP7ResetInputLinkChecksumErrorCounters::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Reset Input Link CECs command just started");

  const bool successEtaPos = proc.client().resetInputLinkChecksumErrorCounters( false );
  const bool successEtaNeg = proc.client().resetInputLinkChecksumErrorCounters( true );

  if(successEtaPos && successEtaNeg)
    {
      setStatusMsg("Reset Input Link CECs command completed");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Reset Input Link Checksum Errors command failed");

  proc.reportClientError("input link CEC reset");

  return swatch::action::Command::State::kError;
}




CTP7ResetInputLinkBX0ErrorCounters::CTP7ResetInputLinkBX0ErrorCounters(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) {
}

CTP7ResetInputLinkBX0ErrorCounters::~CTP7ResetInputLinkBX0ErrorCounters() {
}

swatch::action::Command::State CTP7ResetInputLinkBX0ErrorCounters::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Reset Input Link BX0 Error Counters command just started");

  const bool successEtaPos = proc.client().resetInputLinkBX0ErrorCounters( false );
  const bool successEtaNeg = proc.client().resetInputLinkBX0ErrorCounters( true );
  
  if(successEtaPos && successEtaNeg)
    {
      setStatusMsg("Reset Input Link BX0 Error Counters command completed");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Reset Input Link BX0 Errors command failed for negative eta!");

  proc.reportClientError("input link BX0 error counter reset");

  return swatch::action::Command::State::kError;
}




CTP7GetInputLinkAlignmentStatus::CTP7GetInputLinkAlignmentStatus(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) 
{
  registerParameter("allowBadLinks", xdata::UnsignedInteger(2));
}

CTP7GetInputLinkAlignmentStatus::~CTP7GetInputLinkAlignmentStatus() {
}

swatch::action::Command::State CTP7GetInputLinkAlignmentStatus::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Get Input Link Alignment Status command just started");

  uint32_t align_status_eta_pos;
  uint32_t align_status_eta_neg;
  
  bool successEtaPos = proc.client().getInputLinkAlignmentStatus( true, align_status_eta_pos );
  bool successEtaNeg = proc.client().getInputLinkAlignmentStatus( false, align_status_eta_neg );

  if(!(successEtaPos && successEtaNeg))
    {
      setStatusMsg("Failed to retrieve link alignment status! Check CTP7");
      return swatch::action::Command::State::kError;
    }

  if((align_status_eta_pos + align_status_eta_neg) == 0)
    {
      setStatusMsg("Input links look good");
      return swatch::action::Command::State::kDone;
    }

  // if something is bad, give a good message about what
  unsigned nBad = 0;

  CTP7InputLinkInterface& links = 
    static_cast<CTP7InputLinkInterface&>(proc.getInputPorts());
  links.updateLinkStatuses();

  std::stringstream msg;
  msg << "\nCTP7 at iPhi " << proc.phi() 
      << " input links did not properly align!\n"
      << "Failing links:\n";

  for(size_t iLink = 0;
      iLink < links.getNumPorts();
      ++iLink)
    {
      CTP7Input* link = dynamic_cast<CTP7Input*>(links.getPorts().at(iLink));
      if(link->getStatus().linkLocked == 0 && !link->isMasked())
        {
          nBad++;

          msg << "    " << link->getName() << " ("
              << (link->isNegativeEta() ? "negative" : "positive")
              << " eta)\n";
        }
    }

  setStatusMsg(msg.str());

  const unsigned allowBadLinks(params.get<xdata::UnsignedInteger>("allowBadLinks").value_);

  // break if too many links are bad
  if(nBad <= allowBadLinks)
    {
      proc.reportError("Link failure warning", msg.str());
      return swatch::action::Command::State::kWarning;
    }
      
  proc.reportError("Link failure ERROR", msg.str());
  return swatch::action::Command::State::kError;
}




CTP7AlignTTCDecoder::CTP7AlignTTCDecoder(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
}

CTP7AlignTTCDecoder::~CTP7AlignTTCDecoder() {
}

swatch::action::Command::State CTP7AlignTTCDecoder::code(const swatch::core::ParameterSet& params)
{
  log4cplus::Logger calol1Logger = log4cplus::Logger::getInstance("CHECK-LOGS");
  LOG4CPLUS_INFO(calol1Logger, "#1 -- Entering the [CTP7AlignTTCDecoder] command ");

  CTP7Processor& proc = getActionable<CTP7Processor>();

  LOG4CPLUS_INFO(calol1Logger, "#2 -- Starting the [CTP7AlignTTCDecoder] command ");

  setStatusMsg("Align TTC Decoder command just started");

  if(!proc.client().alignTTCDecoder())
    {
      setStatusMsg("Align TTC Decoder command failed");
      proc.reportClientError("TTC decoder alignment");
      return swatch::action::Command::State::kError;
    }
 
  setStatusMsg("Align TTC Decoder command completed");
  return swatch::action::Command::State::kDone;
}




CTP7SetDAQConfig::CTP7SetDAQConfig(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
  registerParameter("DAQDelayLineDepth", xdata::UnsignedInteger( 0 ));
}

CTP7SetDAQConfig::~CTP7SetDAQConfig() {
}

swatch::action::Command::State CTP7SetDAQConfig::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Set DAQ Config command just started");

  const uint32_t delayLineDepth(params.get<xdata::UnsignedInteger>("DAQDelayLineDepth").value_);

  UCT2016Layer1CTP7::DAQConfig daqConfig;

  daqConfig.DAQDelayLineDepth = delayLineDepth;

  if(!proc.client().setDAQConfig( daqConfig ))
    {
      setStatusMsg("Set DAQ Config command failed!");
      proc.reportClientError("DAQ configuration");
      return swatch::action::Command::State::kError;
    }

  setStatusMsg("Set DAQ Config command completed");
  return swatch::action::Command::State::kDone;
}



CTP7SetRunNumber::CTP7SetRunNumber(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
  registerParameter("runNumber", xdata::UnsignedInteger( 999999 ));
}

CTP7SetRunNumber::~CTP7SetRunNumber() {
}

swatch::action::Command::State CTP7SetRunNumber::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Set Run Number command just started");
std::cout << " Set Run Number command just started " << std::endl ;

  const uint32_t runNumber = params.get<xdata::UnsignedInteger>("runNumber").value_;
std::cout << " Set Run Number command just started " << runNumber << std::endl ;

  if(!proc.client().setRunNumber( runNumber ))
    {
      setStatusMsg("Set Run Number command failed!");
      proc.reportClientError("setting run number");
      return swatch::action::Command::State::kError;
    }

  setStatusMsg("Set Run Number command completed");
  return swatch::action::Command::State::kDone;
}



CTP7CheckBCClockAndBX0Lock::CTP7CheckBCClockAndBX0Lock(const std::string& aId, 
                                                       swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{}

CTP7CheckBCClockAndBX0Lock::~CTP7CheckBCClockAndBX0Lock() 
{}

swatch::action::Command::State 
CTP7CheckBCClockAndBX0Lock::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();

  setStatusMsg("Checking to make sure BX0 and BC clock are locked");

  UCT2016Layer1CTP7::TTCStatus status;

  if(!proc.client().getTTCStatus(status))
    {
      setStatusMsg("Failed to retrieve TTC Status!");
      proc.reportClientError("TTC status check");
      return swatch::action::Command::State::kError;
    }

  bool allGood = true;

  if(status.BCClockLocked == 0)
    {
      setStatusMsg("BC clock is not locked!");
      allGood = false;
    }
  if(status.BX0Locked == 0)
    {
      setStatusMsg("BX0 is not locked!");
      allGood = false;
    }

  if(!allGood)
    {
      proc.reportError("TTC Check Failed", 
                       "BC clock and/or BX0 unlocked in CTP7 at iPhi=" + 
                       std::to_string(static_cast<long long unsigned int>(proc.phi())));
      return swatch::action::Command::State::kError;
    }

  setStatusMsg("BX0 and BC clock locked. Moving on.");
  return swatch::action::Command::State::kDone;
}





// Includes specific to Sleep
// Sleep command is used in configuration sequences before checking status
// of CTP7s.  Sleep is necessary to make sure the system has had time to
// fully configure before checking status.
CTP7Sleep::CTP7Sleep(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
  registerParameter("millisecToSleep", xdata::UnsignedInteger(1000));
}

CTP7Sleep::~CTP7Sleep() {
}

swatch::action::Command::State CTP7Sleep::code(const swatch::core::ParameterSet& params)
{
  setStatusMsg("Dummy sleep command just started");

  unsigned int millisecToSleep(params.get<xdata::UnsignedInteger>("millisecToSleep").value_);
  
  boost::this_thread::sleep_for(boost::chrono::milliseconds(millisecToSleep));
    
  setStatusMsg("Sleep command completed");
  return swatch::action::Command::State::kDone;
}


// SetTMTCycle is used to set which links on each CTP7
// are the active links sending data.  Different cycles
// can be used if an optical transmitter fails.
CTP7SetTMTCycle::CTP7SetTMTCycle(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
  registerParameter("tmtCycleConfig", xdata::UnsignedInteger(0));
}

CTP7SetTMTCycle::~CTP7SetTMTCycle() {
}

swatch::action::Command::State CTP7SetTMTCycle::code(const swatch::core::ParameterSet& params)
{
  setStatusMsg("Starting TMT Cycle Reconfiguration");

  CTP7Processor& proc = getActionable<CTP7Processor>();

  uint32_t tmtCycleConfig(params.get<xdata::UnsignedInteger>("tmtCycleConfig").value_);
  if (tmtCycleConfig < 0 || tmtCycleConfig > 11)
  {
    std::cout<<"Invalid tmtCycleConfig number.  Config should be in [0,11]. \
	Given value: "<<tmtCycleConfig<<std::endl;
  }

  // Get the associated mapping of links for a given config value
  std::vector<uint32_t> tmt_cycle;

  const uint32_t config_map[10][12] =
    {
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 11, 11},
      {11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 11},
      {0, 11, 1, 2, 3, 4, 5, 6, 7, 8, 11, 11},
      {0, 1, 11, 2, 3, 4, 5, 6, 7, 8, 11, 11},
      {0, 1, 2, 11, 3, 4, 5, 6, 7, 8, 11, 11},
      {0, 1, 2, 3, 11, 4, 5, 6, 7, 8, 11, 11},
      {0, 1, 2, 3, 4, 11, 5, 6, 7, 8, 11, 11},
      {0, 1, 2, 3, 4, 5, 11, 6, 7, 8, 11, 11},
      {0, 1, 2, 3, 4, 5, 6, 11, 7, 8, 11, 11},
      {0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 11, 11}
    };

  tmt_cycle.empty();

  for (int i = 0; i < 12; i++)
  {
          tmt_cycle.push_back( config_map [ tmtCycleConfig ][ i ]);
  }

  for (int link_pair = 0; link_pair < 12; link_pair++)
  {
    if(!proc.client().setTMTCycle( false,
                                   static_cast<UCT2016Layer1CTP7::OutputTMTLinkPair>( link_pair ),
                                   tmt_cycle[link_pair])) 
      {
        setStatusMsg("Set TMT Cycle Config command failed");
        proc.reportClientError("TMT cycle configuration");
        return swatch::action::Command::State::kError; 
      }
    if(!proc.client().setTMTCycle( true,
                                   static_cast<UCT2016Layer1CTP7::OutputTMTLinkPair>( link_pair ),
                                   tmt_cycle[link_pair])) 
      {
        setStatusMsg("Set TMT Cycle Config command failed");
        proc.reportClientError("TMT cycle configuration");
        return swatch::action::Command::State::kError; 
      }
  }

  setStatusMsg("Set DAQ TMT Cycle command completed");
  return swatch::action::Command::State::kDone;
}	


LoadLUTs::LoadLUTs(const std::string& aId, 
                   swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
  registerParameter("ECALLUTPlus", xdata::Table());
  registerParameter("ECALLUTMinus", xdata::Table());
  registerParameter("HCALLUTPlus", xdata::Table());
  registerParameter("HCALLUTMinus", xdata::Table());
  registerParameter("HFLUTPlus", xdata::Table());
  registerParameter("HFLUTMinus", xdata::Table());
  registerParameter("HCALFBLUTPlus", xdata::Table());
  registerParameter("HCALFBLUTMinus", xdata::Table());
  registerParameter("md5checksum", xdata::String("XXXFIXMEXXX"));
  registerParameter("LUTVerificationPeriodInSeconds", xdata::Float(900.));
  registerParameter("layer1SecondStageLUT", xdata::Vector<xdata::UnsignedInteger>());
}

LoadLUTs::~LoadLUTs() {
}

swatch::action::Command::State 
LoadLUTs::code(const swatch::core::ParameterSet& params)
{
  CTP7Processor& proc = getActionable<CTP7Processor>();
  CTP7Algo& algo = 
    static_cast<CTP7Algo&>(proc.getAlgo());

  double lutVerificationPeriod = 
    params.get<xdata::Float>("LUTVerificationPeriodInSeconds").value_;
  algo.setLUTVerificationPeriod(lutVerificationPeriod);
  
  std::string lutVersion = params.get<xdata::String>("md5checksum").toString();
  
  setStatusMsg("Caching LUTs");
  algo.cacheLUT(UCT2016Layer1CTP7::ECAL, 
                params.get<xdata::Table>("ECALLUTPlus"),
                lutVersion);
  algo.cacheLUT(UCT2016Layer1CTP7::ECAL, 
                params.get<xdata::Table>("ECALLUTMinus"),
                lutVersion);
  algo.cacheLUT(UCT2016Layer1CTP7::HCAL, 
                params.get<xdata::Table>("HCALLUTPlus"),
                lutVersion);
  algo.cacheLUT(UCT2016Layer1CTP7::HCAL, 
                params.get<xdata::Table>("HCALLUTMinus"),
                lutVersion);
  algo.cacheLUT(UCT2016Layer1CTP7::HF, 
                params.get<xdata::Table>("HFLUTPlus"),
                lutVersion);
  algo.cacheLUT(UCT2016Layer1CTP7::HF, 
                params.get<xdata::Table>("HFLUTMinus"),
                lutVersion);
  algo.cacheLUT2(params.get<xdata::Vector<xdata::UnsignedInteger> >("layer1SecondStageLUT"),
                 lutVersion);
  algo.cacheLUT3(params.get<xdata::Table>("HCALFBLUTPlus"),
                 lutVersion);
  algo.cacheLUT3(params.get<xdata::Table>("HCALFBLUTMinus"),
                 lutVersion);
  if(algo.lutVersionsMatch())
    {
      setStatusMsg("LUTs already loaded to CTP7, no action needed");
    }
  else
    {
      setStatusMsg("Loading LUTs to CTP7");

      if(!algo.loadLUTs())
        {
          setStatusMsg("LUT loading failed, check the CTP7");
          proc.reportClientError("LUT loading");
          return swatch::action::Command::State::kError;
        }
      setStatusMsg("LUTs successfully loaded");
    }

  return swatch::action::Command::State::kDone;
}


CheckLUTs::CheckLUTs(const std::string& aId, 
                     swatch::action::ActionableObject& aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CheckLUTs::~CheckLUTs()
{
}

swatch::action::Command::State 
CheckLUTs::code(const swatch::core::ParameterSet& params)
{
  CTP7Algo& algo = 
    static_cast<CTP7Algo&>(getActionable<CTP7Processor>().getAlgo());

  algo.forceNextVerification();

  return swatch::action::Command::State::kDone;
}


CheckLUT2::CheckLUT2(const std::string& aId, 
                     swatch::action::ActionableObject& aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CheckLUT2::~CheckLUT2()
{
}

swatch::action::Command::State 
CheckLUT2::code(const swatch::core::ParameterSet& params)
{
  CTP7Algo& algo = 
    static_cast<CTP7Algo&>(getActionable<CTP7Processor>().getAlgo());

  algo.forceNextVerificationLUT2();

  return swatch::action::Command::State::kDone;
}

CheckLUT3::CheckLUT3(const std::string& aId, 
                     swatch::action::ActionableObject& aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CheckLUT3::~CheckLUT3()
{
}

swatch::action::Command::State 
CheckLUT3::code(const swatch::core::ParameterSet& params)
{
  CTP7Algo& algo = 
    static_cast<CTP7Algo&>(getActionable<CTP7Processor>().getAlgo());

  algo.forceNextVerificationLUT3();

  return swatch::action::Command::State::kDone;
}

HardReset::HardReset(const std::string& aId, 
                     swatch::action::ActionableObject& aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

HardReset::~HardReset()
{
}

swatch::action::Command::State 
HardReset::code(const swatch::core::ParameterSet& params)
{
  if(!getActionable<CTP7Processor>().client().hardReset("ctp7_v7_stage2"))
    {
      setStatusMsg("Hard reset failed. Check CTP7.");
      getActionable<CTP7Processor>().reportClientError("hard reset");
      return swatch::action::Command::State::kError;
    }
  setStatusMsg("Hard reset done");

  return swatch::action::Command::State::kDone;
}


}}

#include "CicadaCommands.h"

// Cicada client
#include "UCT2023SummaryCTP7.hh"

// CaloL1 
#include "CicadaProcessor.h"
#include "CicadaInput.h"
#include "CicadaLinkInterface.h"
#include "CicadaAlgo.h"

// boost
#include "boost/chrono.hpp"
#include <boost/thread/thread.hpp>

// SWATCH 
#include "swatch/processor/PortCollection.hpp"

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


CicadaSetBackPlaneInputLinkMask::CicadaSetBackPlaneInputLinkMask(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
  xdata::Vector<xdata::UnsignedInteger> zeros;
  zeros.resize(32);
  for(size_t i = 0; i < zeros.size(); ++i)
    zeros.at(i) = xdata::UnsignedInteger(0x0);

  registerParameter("BackPlaneInputLinkMask", zeros);
}

CicadaSetBackPlaneInputLinkMask::~CicadaSetBackPlaneInputLinkMask() {
}

swatch::action::Command::State CicadaSetBackPlaneInputLinkMask::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("Set BackPlaneInputLinkMask command started");

  const xdata::Vector<xdata::UnsignedInteger>& mask = 
    params.get<xdata::Vector<xdata::UnsignedInteger> >("BackPlaneInputLinkMask");

  std::vector<uint32_t> maskIn = std::vector<uint32_t>(mask.size());
  
for(size_t i = 0; i < mask.size(); ++i)
    maskIn.at(i) = uint32_t(mask.at(i).value_);
  
  bool success = proc.client().setBackPlaneInputLinkMask(maskIn);

  if(success)
    {
      setStatusMsg("BackPlaneInputLink masks set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("BackPlaneInputLink not set, something is wrong.");

  proc.reportClientError("BackPlaneInputLinkMask erro");

  return swatch::action::Command::State::kError;
}

CicadaSetOpticalInputLinkMask::CicadaSetOpticalInputLinkMask(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
  xdata::Vector<xdata::UnsignedInteger> zeros;
  zeros.resize(32);
  for(size_t i = 0; i < zeros.size(); ++i)
    zeros.at(i) = xdata::UnsignedInteger(0x0);

  registerParameter("OpticalInputLinkMask", zeros);
}

CicadaSetOpticalInputLinkMask::~CicadaSetOpticalInputLinkMask() {
}

swatch::action::Command::State CicadaSetOpticalInputLinkMask::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("Set OpticalInputLinkMask command started");

  const xdata::Vector<xdata::UnsignedInteger>& mask = 
    params.get<xdata::Vector<xdata::UnsignedInteger> >("OpticalInputLinkMask");

  std::vector<uint32_t> maskIn = std::vector<uint32_t>(mask.size());
  
for(size_t i = 0; i < mask.size(); ++i)
    maskIn.at(i) = uint32_t(mask.at(i).value_);
  
  bool success = proc.client().setOpticalInputLinkMask(maskIn);

  if(success)
    {
      setStatusMsg("OpticalInputLink masks set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("OpticalInputLink not set, something is wrong.");

  proc.reportClientError("OpticalInputLinkMask erro");

  return swatch::action::Command::State::kError;
}

CicadaEnableTXCXP0CXP1Links::CicadaEnableTXCXP0CXP1Links(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CicadaEnableTXCXP0CXP1Links::~CicadaEnableTXCXP0CXP1Links() {
}

swatch::action::Command::State CicadaEnableTXCXP0CXP1Links::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("Set EnableTXCXP0CXP1Links command started");

  bool success = proc.client().enableTXCXP0CXP1Links();

  if(success)
    {
      setStatusMsg("enableTXCXP0CXP1Links set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("enableTXCXP0CXP1Links not set, something is wrong.");

  proc.reportClientError("enableTXCXP0CXP1Links erro");

  return swatch::action::Command::State::kError;
}

CicadaResetInput8b10bErrorCounters::CicadaResetInput8b10bErrorCounters(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CicadaResetInput8b10bErrorCounters::~CicadaResetInput8b10bErrorCounters() {
}

swatch::action::Command::State CicadaResetInput8b10bErrorCounters::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("ResetInput8b10bErrorCounters command started");

  bool success = proc.client().resetInput8b10bErrorCounters();

  if(success)
    {
      setStatusMsg("ResetInput8b10bErrorCounters set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("ResetInput8b10bErrorCounters not set, something is wrong.");

  proc.reportClientError("ResetInput8b10bErrorCounters erro");

  return swatch::action::Command::State::kError;
}

CicadaResetTxCXP1::CicadaResetTxCXP1(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CicadaResetTxCXP1::~CicadaResetTxCXP1() {
}

swatch::action::Command::State CicadaResetTxCXP1::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("ResetTxCXP1 command started");

  bool success = proc.client().resetTxCXP1();

  if(success)
    {
      setStatusMsg("ResetTxCXP1 set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("ResetTxCXP1 not set, something is wrong.");

  proc.reportClientError("ResetTxCXP1 erro");

  return swatch::action::Command::State::kError;
}

CicadaResetRxCXP1::CicadaResetRxCXP1(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CicadaResetRxCXP1::~CicadaResetRxCXP1() {
}

swatch::action::Command::State CicadaResetRxCXP1::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("ResetRxCXP1 command started");

  bool success = proc.client().resetRxCXP1();

  if(success)
    {
      setStatusMsg("ResetRxCXP1 set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("ResetRxCXP1 not set, something is wrong.");

  proc.reportClientError("ResetRxCXP1 erro");

  return swatch::action::Command::State::kError;
}

CicadaResetBackPlaneMGTs::CicadaResetBackPlaneMGTs(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) 
{
}

CicadaResetBackPlaneMGTs::~CicadaResetBackPlaneMGTs() {
}

swatch::action::Command::State CicadaResetBackPlaneMGTs::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("resetBackPlaneMGTs command started");

  bool success = proc.client().resetBackPlaneMGTs();

  if(success)
    {
      setStatusMsg("resetBackPlaneMGTs set");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("resetBackPlaneMGTs not set, something is wrong.");

  proc.reportClientError("resetBackPlaneMGTs erro");

  return swatch::action::Command::State::kError;
}

CicadaAlignInputLinks::CicadaAlignInputLinks(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer()) {
  registerParameter("alignBX", xdata::UnsignedInteger(3563));
  registerParameter("alignSubBX", xdata::UnsignedInteger(0));
}

CicadaAlignInputLinks::~CicadaAlignInputLinks() {
}

swatch::action::Command::State CicadaAlignInputLinks::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

  setStatusMsg("Align Input Links command just started");

  const uint32_t alignBX(params.get<xdata::UnsignedInteger>("alignBX").value_);
  const uint32_t alignSubBX(params.get<xdata::UnsignedInteger>("alignSubBX").value_);
  
  if (alignBX > 3563) {
    setStatusMsg("Bunch Crossings value can not be above 3563. Talk to Layer1 on call for help.");
    proc.reportError("Bad align BX", "Cicada at iPhi=" + std::to_string(static_cast<long long unsigned int>(proc.phi())) + " has invalid alignBX");
    return swatch::action::Command::State::kError;
  }
  if (alignSubBX >= 6) {
    setStatusMsg("Bunch Crossings value can not be above 5. Talk to Layer1 on call for help.");
    proc.reportError("Bad align subBX", "Cicada at iPhi=" + std::to_string(static_cast<long long unsigned int>(proc.phi())) + " has invalid alignSubBX");
    return swatch::action::Command::State::kError;
  }

  const bool success = proc.client().alignInputLinks(alignBX, alignSubBX);
  
  if(success)
    {
      setStatusMsg("Align Input Links command completed");
      return swatch::action::Command::State::kDone;
    }

  setStatusMsg("Align Input Links command failed");

  proc.reportClientError("input link alignment");

  return swatch::action::Command::State::kError;
}




CicadaAlignTTCDecoder::CicadaAlignTTCDecoder(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
}

CicadaAlignTTCDecoder::~CicadaAlignTTCDecoder() {
}

swatch::action::Command::State CicadaAlignTTCDecoder::code(const swatch::core::ParameterSet& params)
{
  CicadaProcessor& proc = getActionable<CicadaProcessor>();

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




CicadaSleep::CicadaSleep(const std::string& aId, swatch::action::ActionableObject&  aActionable) :
  swatch::action::Command(aId, aActionable, xdata::Integer(-33)) {
  registerParameter("millisecToSleep", xdata::UnsignedInteger(1000));
}

CicadaSleep::~CicadaSleep() {
}

swatch::action::Command::State CicadaSleep::code(const swatch::core::ParameterSet& params)
{
  setStatusMsg("Dummy sleep command just started");

  unsigned int millisecToSleep(params.get<xdata::UnsignedInteger>("millisecToSleep").value_);
  
  boost::this_thread::sleep_for(boost::chrono::milliseconds(millisecToSleep));
    
  setStatusMsg("Sleep command completed");
  return swatch::action::Command::State::kDone;
}


}}

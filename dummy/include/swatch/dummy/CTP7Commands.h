#ifndef _calol1_ctp7_CTP7Commands_h_
#define _calol1_ctp7_CTP7Commands_h_

// SWATCH to CTP7 command interface .h
// U.Wisconsin - Tyler Ruggles and Nate Woods, September 2015

#include "swatch/action/Command.hpp"
#include "swatch/core/ParameterSet.hpp"

namespace calol1
{

  class CTP7SetTowerMask: public swatch::action::Command {
  public:
    CTP7SetTowerMask( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7SetTowerMask();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7SetInputLinkAlignmentMask: public swatch::action::Command {
  public:
    CTP7SetInputLinkAlignmentMask( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7SetInputLinkAlignmentMask();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7AlignInputLinks: public swatch::action::Command {
  public:
    CTP7AlignInputLinks( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7AlignInputLinks();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7AlignOutputLinks: public swatch::action::Command {
  public:
    CTP7AlignOutputLinks( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7AlignOutputLinks();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7ResetInputLinkDecoders: public swatch::action::Command {
  public:
    CTP7ResetInputLinkDecoders( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7ResetInputLinkDecoders();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7ResetInputLinkChecksumErrorCounters: public swatch::action::Command {
  public:
    CTP7ResetInputLinkChecksumErrorCounters( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7ResetInputLinkChecksumErrorCounters();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7ResetInputLinkBX0ErrorCounters: public swatch::action::Command {
  public:
    CTP7ResetInputLinkBX0ErrorCounters( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7ResetInputLinkBX0ErrorCounters();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7GetInputLinkAlignmentStatus: public swatch::action::Command {
  public:
    CTP7GetInputLinkAlignmentStatus( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7GetInputLinkAlignmentStatus();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7AlignTTCDecoder: public swatch::action::Command {
  public:
    CTP7AlignTTCDecoder( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7AlignTTCDecoder();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7SetDAQConfig: public swatch::action::Command {
  public:
    CTP7SetDAQConfig( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7SetDAQConfig();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7SetRunNumber: public swatch::action::Command {
  public:
    CTP7SetRunNumber( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7SetRunNumber();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7CheckBCClockAndBX0Lock: public swatch::action::Command {
  public:
    CTP7CheckBCClockAndBX0Lock( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7CheckBCClockAndBX0Lock();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CTP7Sleep: public swatch::action::Command {
  public:
    CTP7Sleep( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7Sleep();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };

  class CTP7SetTMTCycle: public swatch::action::Command {
  public:
    CTP7SetTMTCycle( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CTP7SetTMTCycle();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class LoadLUTs : public swatch::action::Command {
  public:
    LoadLUTs( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~LoadLUTs();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CheckLUTs : public swatch::action::Command {
  public:
    CheckLUTs( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CheckLUTs();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CheckLUT2 : public swatch::action::Command {
  public:
    CheckLUT2( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CheckLUT2();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };

  class CheckLUT3 : public swatch::action::Command {
  public:
    CheckLUT3( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CheckLUT3();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class HardReset : public swatch::action::Command {
  public:
    HardReset( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~HardReset();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };

} // namespace

#endif // header guard

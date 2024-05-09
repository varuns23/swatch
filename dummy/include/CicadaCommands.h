#ifndef _calol1_ctp7_CicadaCommands_h_
#define _calol1_ctp7_CicadaCommands_h_

#include "swatch/action/Command.hpp"
#include "swatch/core/ParameterSet.hpp"

//namespace calol1
//{

namespace swatch {
namespace dummy {



  class CicadaAlignInputLinks: public swatch::action::Command {
  public:
    CicadaAlignInputLinks( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaAlignInputLinks();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };

  class CicadaSetBackPlaneInputLinkMask: public swatch::action::Command {
  public:
    CicadaSetBackPlaneInputLinkMask( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaSetBackPlaneInputLinkMask();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CicadaSetOpticalInputLinkMask: public swatch::action::Command {
  public:
    CicadaSetOpticalInputLinkMask( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaSetOpticalInputLinkMask();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CicadaEnableTXCXP0CXP1Links: public swatch::action::Command {
  public:
    CicadaEnableTXCXP0CXP1Links( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaEnableTXCXP0CXP1Links();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CicadaResetBackPlaneMGTs: public swatch::action::Command {
  public:
    CicadaResetBackPlaneMGTs( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaResetBackPlaneMGTs();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };



  class CicadaResetTxCXP1: public swatch::action::Command {
  public:
    CicadaResetTxCXP1( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaResetTxCXP1();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };

  class CicadaResetRxCXP1: public swatch::action::Command {
  public:
    CicadaResetRxCXP1( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaResetRxCXP1();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CicadaResetInput8b10bErrorCounters: public swatch::action::Command {
  public:
    CicadaResetInput8b10bErrorCounters( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaResetInput8b10bErrorCounters();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CicadaAlignTTCDecoder: public swatch::action::Command {
  public:
    CicadaAlignTTCDecoder( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaAlignTTCDecoder();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


  class CicadaSleep: public swatch::action::Command {
  public:
    CicadaSleep( const std::string& aId, swatch::action::ActionableObject&  aActionable );

    virtual ~CicadaSleep();

    virtual swatch::action::Command::State code(const swatch::core::ParameterSet& params); 
  };


}} // namespace

#endif // header guard

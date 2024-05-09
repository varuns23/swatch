
#ifndef __SWATCH_DUMMY_DUMMYAMC13MANAGERCOMMANDS_HPP__
#define __SWATCH_DUMMY_DUMMYAMC13MANAGERCOMMANDS_HPP__


#include "swatch/dummy/AbstractConfigureCommand.hpp"
#include "swatch/dummy/AbstractForceStateCommand.hpp"


namespace swatch {
namespace dummy {


class DummyAMC13RebootCommand : public AbstractConfigureCommand {
public:
  DummyAMC13RebootCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13RebootCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13ResetCommand : public AbstractConfigureCommand {
public:
  DummyAMC13ResetCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ResetCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13ConfigureEvbCommand : public AbstractConfigureCommand {
public:
  DummyAMC13ConfigureEvbCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ConfigureEvbCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13ConfigureSLinkCommand : public AbstractConfigureCommand {
public:
  DummyAMC13ConfigureSLinkCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ConfigureSLinkCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13ConfigureAMCPortsCommand : public AbstractConfigureCommand {
public:
  DummyAMC13ConfigureAMCPortsCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ConfigureAMCPortsCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13StartDaqCommand : public AbstractConfigureCommand {
public:
  DummyAMC13StartDaqCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13StartDaqCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13StopDaqCommand : public AbstractConfigureCommand {
public:
  DummyAMC13StopDaqCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13StopDaqCommand();

private:
  void runAction(bool aGoIntoError);
};


class DummyAMC13ForceClkTtcStateCommand : public AbstractForceStateCommand {
public:
  DummyAMC13ForceClkTtcStateCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ForceClkTtcStateCommand();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyAMC13ForceEvbStateCommand : public AbstractForceStateCommand {
public:
  DummyAMC13ForceEvbStateCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ForceEvbStateCommand();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyAMC13ForceSLinkStateCommand : public AbstractForceStateCommand {
public:
  DummyAMC13ForceSLinkStateCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ForceSLinkStateCommand();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyAMC13ForceAMCPortStateCommand : public AbstractForceStateCommand {
public:
  DummyAMC13ForceAMCPortStateCommand(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyAMC13ForceAMCPortStateCommand();

private:
  State code(const core::ParameterSet& aParamSet);
};


} // namespace dummy
} // namespace swatch

#endif /* __SWATCH_DUMMY_DUMMYAMC13MANAGERCOMMAND_HPP__ */

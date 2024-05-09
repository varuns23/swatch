/**
 * @file    DummyProcessorCommands.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_DUMMY_DUMMYPROCESSORCOMMANDS7_HPP__
#define __SWATCH_DUMMY_DUMMYPROCESSORCOMMANDS7_HPP__


#include "swatch/dummy/AbstractConfigureCommand.hpp"
#include "swatch/dummy/AbstractForceStateCommand.hpp"


namespace swatch {
namespace dummy {


class DummyRebootCommand7 : public AbstractConfigureCommand {
public:
  DummyRebootCommand7(const std::string& aId, swatch::action::ActionableObject& aActionable);
  ~DummyRebootCommand7();

private:
  void runAction(bool aErrorOccurs);
};

class DummyResetCommand7 : public AbstractConfigureCommand {
public:
  DummyResetCommand7(const std::string& aId, swatch::action::ActionableObject& aActionable);
  ~DummyResetCommand7();

private:
  void runAction(bool aErrorOccurs);
};

class DummyConfigureTxCommand7 : public AbstractConfigureCommand {
public:
  DummyConfigureTxCommand7(const std::string& aId, swatch::action::ActionableObject& aActionable);
  ~DummyConfigureTxCommand7();

private:
  void runAction(bool aErrorOccurs);
};

class DummyConfigureRxCommand7 : public AbstractConfigureCommand {
public:
  DummyConfigureRxCommand7(const std::string& aId, swatch::action::ActionableObject& aActionable);
  ~DummyConfigureRxCommand7();

private:
  void runAction(bool aErrorOccurs);
};


class DummyConfigureDaqCommand7 : public AbstractConfigureCommand {
public:
  DummyConfigureDaqCommand7(const std::string& aId, swatch::action::ActionableObject& aActionable);
  ~DummyConfigureDaqCommand7();

private:
  void runAction(bool aErrorOccurs);
};


class DummyConfigureAlgoCommand7 : public AbstractConfigureCommand {
public:
  DummyConfigureAlgoCommand7(const std::string& aId, swatch::action::ActionableObject& aActionable);
  ~DummyConfigureAlgoCommand7();

private:
  void runAction(bool aErrorOccurs);
};


class DummyProcessorForceClkTtcStateCommand7 : public AbstractForceStateCommand {
public:
  DummyProcessorForceClkTtcStateCommand7(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyProcessorForceClkTtcStateCommand7();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyProcessorForceRxPortsStateCommand7 : public AbstractForceStateCommand {
public:
  DummyProcessorForceRxPortsStateCommand7(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyProcessorForceRxPortsStateCommand7();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyProcessorForceTxPortsStateCommand7 : public AbstractForceStateCommand {
public:
  DummyProcessorForceTxPortsStateCommand7(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyProcessorForceTxPortsStateCommand7();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyProcessorForceReadoutStateCommand7 : public AbstractForceStateCommand {
public:
  DummyProcessorForceReadoutStateCommand7(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyProcessorForceReadoutStateCommand7();

private:
  State code(const core::ParameterSet& aParamSet);
};


class DummyProcessorForceAlgoStateCommand7 : public AbstractForceStateCommand {
public:
  DummyProcessorForceAlgoStateCommand7(const std::string& aId, action::ActionableObject& aActionable);
  ~DummyProcessorForceAlgoStateCommand7();

private:
  State code(const core::ParameterSet& aParamSet);
};



} // namespace dummy
} // namespace swatch

#endif /* __SWATCH_DUMMY_DUMMYPROCESSORCOMMAND7_HPP__ */


#include "swatch/dummy/AbstractConfigureCommand.hpp"


#include <thread>

#include "xdata/Boolean.h"
#include "xdata/String.h"
#include "xdata/UnsignedInteger.h"


namespace swatch {
namespace dummy {


AbstractConfigureCommand::AbstractConfigureCommand(const std::string& aId, action::ActionableObject& aActionable) :
  Command(aId, aActionable, xdata::String("Dummy command's default result!"))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(5));
  registerParameter("returnWarning", xdata::Boolean(false));
  registerParameter("returnError", xdata::Boolean(false));
  registerParameter("throw", xdata::Boolean(false));
}


AbstractConfigureCommand::~AbstractConfigureCommand()
{
}


action::Command::State AbstractConfigureCommand::code(const core::ParameterSet& aParams)
{
  const size_t lNrSeconds = aParams.get<xdata::UnsignedInteger>("cmdDuration").value_;

  State lState = kDone;
  if (aParams.get<xdata::Boolean>("throw").value_)
    lState = kError;
  else if (aParams.get<xdata::Boolean>("returnError").value_)
    lState = kError;
  else if (aParams.get<xdata::Boolean>("returnWarning").value_)
    lState = kWarning;

  for (size_t i = 0; i < (lNrSeconds * 4); i++) {
    std::ostringstream lMsg;
    lMsg << "Done " << i << " of " << (lNrSeconds * 4) << " things";
    setProgress(float(i) / float(lNrSeconds * 4), lMsg.str());
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }

  this->runAction(lState == kError);

  if (aParams.get<xdata::Boolean>("throw").value_)
    SWATCH_THROW(core::RuntimeError("An exceptional error occurred!"));
  return lState;
}

} // end ns: dummy
} // end ns: swatch

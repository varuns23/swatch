
#ifndef __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__
#define __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__


#include <memory>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"


namespace swatch {
namespace dummy {


class DummyAMC13Driver;

class DummyAMC13Manager : public dtm::DaqTTCManager {
public:
  DummyAMC13Manager(const swatch::core::AbstractStub& aStub);

  virtual ~DummyAMC13Manager();

  DummyAMC13Driver& getDriver()
  {
    return *mDriver;
  }

private:
  virtual void retrieveMetricValues();

  std::unique_ptr<DummyAMC13Driver> mDriver;
};


} // namespace dummy
} // namespace swatch


#endif /* __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__ */

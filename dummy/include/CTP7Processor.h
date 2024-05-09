#ifndef _calol1_ctp7_CTP7Processor_h_
#define _calol1_ctp7_CTP7Processor_h_

/*****************************************************************************
*                                                                            *
* CTP7Processor.h                                                            *
*     A basic skeleton for a SWATCH processor for the stage 2 upgrade        *
*     layer-1 calorimeter trigger's CTP7s.                                   *
*                                                                            *
* Nate Woods, U. Wisconsin                                                   *
*                                                                            *
*****************************************************************************/

#include "swatch/processor/Processor.hpp"
#include "swatch/core/SimpleMetric.hpp"

#include "UCT2016Layer1CTP7.hh"

#include <string>
#include <map>

//namespace calol1

namespace swatch {
namespace dummy {


  typedef UCT2016Layer1CTP7::FWInfo FWInfo;

  class CTP7Processor : public swatch::processor::Processor
    {
    public:
      CTP7Processor(const swatch::core::AbstractStub& stub);

      ~CTP7Processor()
        {
          delete client_;
        }

      void retrieveMetricValues();

      UCT2016Layer1CTP7& client() const { return *client_; }

      const unsigned short phi() const { return phi_; }

      void setClientErrorStatus(bool status) { clientInError_ = status; }
      bool clientInError() const { return clientInError_; }

      void reportError(const std::string& errorType,
                       const std::string& errorMessage)
      {errorLog_[errorType] = errorMessage;}

      const std::map<std::string, std::string>& getReportedErrors() const
      {return errorLog_;}

      void clearReportedErrors() {errorLog_.clear();}

      // Client errors are common enough they get their own function
      void reportClientError(const std::string& failingTask);

    private:
      // make and check a (heap allocated) CTP7 client
      UCT2016Layer1CTP7* const getNewClient(unsigned short iPhi) const;

      // no copy constructor
      CTP7Processor(const CTP7Processor&);
      // no assignment operator
      const CTP7Processor& operator=(const CTP7Processor&);

      const unsigned short phi_;

      UCT2016Layer1CTP7 * const client_;
      bool clientInError_;

      swatch::core::SimpleMetric<uint32_t>& metricFirmwareBuildTimestamp_;
      swatch::core::SimpleMetric<uint32_t>& metricFirmwareGitHash_;
      swatch::core::SimpleMetric<bool>& metricFirmwareGitHashDirty_;
      swatch::core::SimpleMetric<uint32_t>& metricFirmwareUptime_;
      swatch::core::SimpleMetric<bool>& metricClientInError_;

      std::map<std::string, std::string> errorLog_;
    };

}}

#endif // header guard

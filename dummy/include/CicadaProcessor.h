#ifndef _calol1_ctp7_CicadaProcessor_h_
#define _calol1_ctp7_CicadaProcessor_h_


#include "swatch/processor/Processor.hpp"
#include "swatch/core/SimpleMetric.hpp"

#include "UCT2023SummaryCTP7.hh"

#include <string>
#include <map>

//namespace calol1

namespace swatch {
namespace dummy {

  class CicadaProcessor : public swatch::processor::Processor
    {
    public:
      CicadaProcessor(const swatch::core::AbstractStub& stub);

      ~CicadaProcessor()
        {
          delete client_;
        }

      void retrieveMetricValues();

      UCT2023SummaryCTP7& client() const { return *client_; }

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
      // make and check a (heap allocated) Cicada client
      UCT2023SummaryCTP7* const getNewClient(unsigned short iPhi) const;

      // no copy constructor
      CicadaProcessor(const CicadaProcessor&);
      // no assignment operator
      const CicadaProcessor& operator=(const CicadaProcessor&);

      const unsigned short phi_;

      UCT2023SummaryCTP7 * const client_;
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

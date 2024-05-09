#ifndef _calol1_ctp7_CicadaInput_h_
#define _calol1_ctp7_CicadaInput_h_

// SWATCH
#include "swatch/processor/Port.hpp"
#include "swatch/core/SimpleMetric.hpp"
#include "swatch/core/MetricConditions.hpp"

// calol1
#include "UCT2023SummaryCTP7.hh"

// standard stuff
#include <string>
#include <vector>


//namespace calol1
//{

namespace swatch {
namespace dummy {


  class CicadaInput : public swatch::processor::InputPort
    {
    public:
      // If the first letter of aID is "N" or "n", this link is on negative
      // eta side. Link ID is obtained from number.
      CicadaInput(const std::string& aID, const uint32_t number);

      ~CicadaInput();

      // retrieveMetricValues() assumes status has already been set 
      // with setStatus()
      void retrieveMetricValues();

    private:
      // no copy constructor
      CicadaInput(const CicadaInput&);
      // no assignment operator
      const CicadaInput& operator=(const CicadaInput&);

      swatch::core::SimpleMetric<bool>& mMetricIsUp;
    };


}}


#endif // header guard

#ifndef _calol1_ctp7_CicadaOutput_h_
#define _calol1_ctp7_CicadaOutput_h_

// SWATCH
#include "swatch/processor/Port.hpp"

// calol1
#include "UCT2023SummaryCTP7.hh"

// standard stuff
#include <string>
#include <vector>


//namespace calol1
//{

namespace swatch {
namespace dummy {


  class CicadaOutput : public swatch::processor::OutputPort
    {
    public:
      // If the first letter of aID is "N" or "n", this link is on negative
      // eta side. Link ID is obtained from number.
      CicadaOutput(const std::string& aID, const uint32_t number);
      ~CicadaOutput();

      // assumes status has already been set by setStatus()
      void retrieveMetricValues();

    private:
  
    };


}}


#endif // header guard

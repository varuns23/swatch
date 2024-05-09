#ifndef _calol1_ctp7_CTP7Output_h_
#define _calol1_ctp7_CTP7Output_h_

// SWATCH
#include "swatch/processor/Port.hpp"

// calol1
#include "UCT2016Layer1CTP7.hh"

// standard stuff
#include <string>
#include <vector>


//namespace calol1
//{

namespace swatch {
namespace dummy {

  typedef UCT2016Layer1CTP7::OutputLink OutputLink;
  typedef UCT2016Layer1CTP7::LinkStatusSummary LinkStatus;

  class CTP7Output : public swatch::processor::OutputPort
    {
    public:
      // If the first letter of aID is "N" or "n", this link is on negative
      // eta side. Link ID is obtained from number.
      CTP7Output(const std::string& aID, const uint32_t number);
      ~CTP7Output();

      // assumes status has already been set by setStatus()
      void retrieveMetricValues();

      void setStatus(const LinkStatus status);
      const LinkStatus& getStatus() const {return status_;}
      const std::string& getName() const {return name_;}

    private:
      // if isHCAL and isHF are both true, it's HF (if neither, it's ECAL)
      const OutputLink getID(const uint32_t num) const 
      {return OutputLink(num);}

      const bool negativeEta_;
      const OutputLink id_;
      const std::string name_;
      LinkStatus status_;
      bool statusSet_;
  
    };


}}


#endif // header guard

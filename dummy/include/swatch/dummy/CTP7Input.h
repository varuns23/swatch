#ifndef _calol1_ctp7_CTP7Input_h_
#define _calol1_ctp7_CTP7Input_h_

// SWATCH
#include "swatch/processor/Port.hpp"
#include "swatch/core/SimpleMetric.hpp"
#include "swatch/core/MetricConditions.hpp"

// calol1
#include "UCT2016Layer1CTP7.hh"

// standard stuff
#include <string>
#include <vector>


namespace calol1
{
  typedef UCT2016Layer1CTP7::InputLink InputLink;
  typedef UCT2016Layer1CTP7::LinkStatusSummary LinkStatus;

  class CTP7Input : public swatch::processor::InputPort
    {
    public:
      // If the first letter of aID is "N" or "n", this link is on negative
      // eta side. Link ID is obtained from number.
      CTP7Input(const std::string& aID, const uint32_t number);

      ~CTP7Input();

      // retrieveMetricValues() assumes status has already been set 
      // with setStatus()
      void retrieveMetricValues();

      void setStatus(const LinkStatus status);
      const LinkStatus& getStatus() const {return status_;}
      bool isStatusSet() const {return statusSet_;}

      InputLink getLinkID() const {return id_;}
      const size_t getLinkNum() const {return size_t(id_);}
      const bool isNegativeEta() const {return negativeEta_;}
      const std::string& getName() const {return name_;}

    private:
      // no copy constructor
      CTP7Input(const CTP7Input&);
      // no assignment operator
      const CTP7Input& operator=(const CTP7Input&);

      // if isHCAL and isHF are both true, it's HF (if neither, it's ECAL)
      const InputLink getID(const unsigned int linkNum,
                            const bool isHCAL, 
                            const bool isHF) const;
      const InputLink getID(const uint32_t num) const {return InputLink(num);}

      const bool negativeEta_;
      const InputLink id_;
      const std::string name_;
      LinkStatus status_;
      bool statusSet_;  

      swatch::core::SimpleMetric<uint32_t>& mMetricBX0Errors;
      swatch::core::SimpleMetric<std::string>& mMetricTowerMask;
      swatch::core::SimpleMetric<bool>& mMetricIsUp;
    };


}


#endif // header guard

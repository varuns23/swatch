#ifndef _calol1_ctp7_CTP7Algo_h_
#define _calol1_ctp7_CTP7Algo_h_

#include <map>
#include <vector>
#include <string>
#include <time.h>
#include <memory>

// Swatch and xdaq headers
#include "swatch/processor/AlgoInterface.hpp"
#include "xdata/UnsignedInteger.h"
#include "xdata/UnsignedInteger64.h"
#include "xdata/Table.h"
#include "xdata/Vector.h"

// CaloL1 headers
#include "UCT2016Layer1CTP7.hh"

namespace calol1 {

  // enum LUTType 
  // {
  //   kECAL = UCT2016Layer1CTP7::ECAL,
  //   kHCAL = UCT2016Layer1CTP7::HCAL,
  //   kHF   = UCT2016Layer1CTP7::HF
  // };
  typedef UCT2016Layer1CTP7::LUTType LUTType;
  typedef std::map<size_t, std::vector<uint32_t> > LUT;
  typedef std::map<LUTType, std::unique_ptr<LUT> > LUTMap;

  // Second stage LUT the same for all ieta, don't need full table
  typedef std::vector<uint32_t> LUT2;

  // HCal FB LUT stored 64b words instead of 32b
  typedef std::map<size_t, std::vector<uint64_t> > LUT3;

  class CTP7Algo : public swatch::processor::AlgoInterface {
  public:
    CTP7Algo(UCT2016Layer1CTP7* const client,
             const unsigned int phi);
    ~CTP7Algo() {}
   
    //! Implemented as a no-op for the moment
    virtual void reset();

    // Is there a LUT of this type stored in this object yet?
    bool isLUTCached(const LUTType type) const
    {
      return luts_.find(type) != luts_.end() && bool(luts_.at(type)->size());
    }
    bool isLUT2Cached() const
    {
      return bool(lut2_) && bool(lut2_->size());
    }
    bool isLUT3Cached() const
    {
      return bool(lut3_) && bool(lut3_->size()); 
    }

    // Get version string for LUTs currently on CTP7
    std::string loadedLUTVersion() const;

    // Get version string for cached LUTs
    const std::string& cachedLUTVersion() const { return lutVersion_; }
    // For LUT2
    const std::string& cachedLUT2Version() const { return lut2Version_; }
    // For HCal FB LUT
    const std::string& cachedLUT3Version() const { return lut3Version_; }
    
    // Check to see if the loaded and cached version strings match
    bool lutVersionsMatch() const 
    {
      return cachedLUTVersion().compare(loadedLUTVersion()) == 0;
    }
    bool lut2VersionsMatch() const 
    {
      return cachedLUT2Version().compare(loadedLUTVersion()) == 0;
    }
    bool lut3VersionsMatch() const 
    {
      return cachedLUT3Version().compare(loadedLUTVersion()) == 0;
    }

    // Cache LUT created from xdata Table
    void cacheLUT(const LUTType type, const xdata::Table& lut, 
                  const std::string& version);
    void cacheLUT2(const xdata::Vector<xdata::UnsignedInteger>& lut,
                   const std::string& version);
    void cacheLUT3(const xdata::Table& lut, 
                  const std::string& version);

    // Load all LUTs onto CTP7 from cached versions
    bool loadLUTs() const;

    // Verify that the first stage LUTs on the CTP7 are the same as those cached
    bool verifyLUTs() const;
    // Verify second-stage LUT
    bool verifyLUT2() const;
    // Verify HCal FB LUT
    bool verifyLUT3() const;

    // Set or get how often the LUTs are verified (in seconds)
    void setLUTVerificationPeriod(double t) { lutVerificationPeriod_ = t; }
    double getLUTVerificationPeriod() const { return lutVerificationPeriod_; }
    // Offset between first- and second-stage LUT verification cycles
    void setLUT2VerificationOffset(double t) { lut2VerificationOffset_ = t; }
    double getLUT2VerificationOffset() const { return lut2VerificationOffset_; }
    // Offset between first-stage and HCal FB LUT verification cycles
    void setLUT3VerificationOffset(double t) { lut3VerificationOffset_ = t; }
    double getLUT3VerificationOffset() const { return lut3VerificationOffset_; }

    // Require LUT check in next monitoring update
    void forceNextVerification() { forceVerification_ = true; }
    void forceNextVerificationLUT2() { forceVerificationLUT2_ = true; }
    void forceNextVerificationLUT3() { forceVerificationLUT3_ = true; }

  protected:
    virtual void retrieveMetricValues();

  private:
    // no copy constructor
    CTP7Algo(const CTP7Algo&);
    // no assignment operator
    const CTP7Algo& operator=(const CTP7Algo&);

    // Make a LUT from a xdata Table. 
    std::unique_ptr<LUT> makeLUT(const xdata::Table& in) const;
    std::unique_ptr<LUT2> makeLUT2(const xdata::Vector<xdata::UnsignedInteger>& in) const;
    std::unique_ptr<LUT3> makeLUT3(const xdata::Table& in) const;

    // Load single LUT onto CTP7 from cached version
    bool loadLUT(const LUTType type) const;
    bool loadLUT2() const;
    bool loadLUT3() const;

    UCT2016Layer1CTP7* const client_;

    unsigned int phi_;

    LUTMap luts_;
    bool lutsCorrupted_;
    std::string lutVersion_;
    double lutVerificationPeriod_;
    time_t lastLUTVerification_;
    bool forceVerification_;

    swatch::core::SimpleMetric<std::string>& mMetricLUTVersion;
    swatch::core::SimpleMetric<float>& mMetricTimeSinceLUTVerification;
    swatch::core::SimpleMetric<bool>& mMetricLUTsCorrupted;

    std::unique_ptr<LUT2> lut2_;
    bool lut2Corrupted_;
    std::string lut2Version_;
    double lut2VerificationOffset_;
    time_t lastLUT2Verification_;
    bool forceVerificationLUT2_;

    swatch::core::SimpleMetric<std::string>& mMetricLUT2Version;
    swatch::core::SimpleMetric<float>& mMetricTimeSinceLUT2Verification;
    swatch::core::SimpleMetric<bool>& mMetricLUT2Corrupted;

    std::unique_ptr<LUT3> lut3_;
    bool lut3Corrupted_;
    std::string lut3Version_;
    double lut3VerificationOffset_;
    time_t lastLUT3Verification_;
    bool forceVerificationLUT3_;

    swatch::core::SimpleMetric<std::string>& mMetricLUT3Version;
    swatch::core::SimpleMetric<float>& mMetricTimeSinceLUT3Verification;
    swatch::core::SimpleMetric<bool>& mMetricLUT3Corrupted;
  };
 
} // namespace calol1

#endif

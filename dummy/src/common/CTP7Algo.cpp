#include "CTP7Algo.h"

#include <stdexcept>
#include <sstream>

#include "swatch/core/MetricConditions.hpp"

//using namespace calol1;

namespace swatch {
namespace dummy {


CTP7Algo::CTP7Algo(UCT2016Layer1CTP7* const client, 
                   const unsigned int phi) :
  client_(client),
  phi_(phi),
  luts_(LUTMap()),
  lutsCorrupted_(false),
  lutVersion_(""),
  lutVerificationPeriod_(900.), // 15 minutes by default
  lastLUTVerification_(std::time(NULL) - (time_t(lutVerificationPeriod_) / 18 * phi)), // not technically ok, but probably fine as a temporary measure
  forceVerification_(false),
  mMetricLUTVersion(registerMetric<std::string>("LUTversiononCTP7")),
  mMetricTimeSinceLUTVerification(registerMetric<float>("TsincelastLUTver")),
  mMetricLUTsCorrupted(registerMetric<bool>("LUTcorruptiondetected",
                                            swatch::core::EqualCondition<bool>(true))),
  lut2Corrupted_(false),
  lut2Version_(""),
  lut2VerificationOffset_(lutVerificationPeriod_ / 36), // halfway between checks
  lastLUT2Verification_(lastLUTVerification_+1),
  forceVerificationLUT2_(false),
  mMetricLUT2Version(registerMetric<std::string>("SecondstageLUTversion")),
  mMetricTimeSinceLUT2Verification(registerMetric<float>("Tsince2ndStageLUTver")),
  mMetricLUT2Corrupted(registerMetric<bool>("SecondstageLUTcorruption",
                                            swatch::core::EqualCondition<bool>(true))),
  lut3Corrupted_(false),
  lut3Version_(""),
  lut3VerificationOffset_(lutVerificationPeriod_ / 36), // halfway between checks
  lastLUT3Verification_(lastLUTVerification_+1),
  forceVerificationLUT3_(false),
  mMetricLUT3Version(registerMetric<std::string>("HCalFBLUTversiononCTP7")),
  mMetricTimeSinceLUT3Verification(registerMetric<float>("TsinceHCalFBLUTver")),
  mMetricLUT3Corrupted(registerMetric<bool>("HCalFBLUTcorruption",
                                            swatch::core::EqualCondition<bool>(true)))
{
}


void CTP7Algo::reset(){
}


std::string CTP7Algo::loadedLUTVersion() const
{
  std::string out;

  if(!client_->getConfiguration(out))
    return "ERROR";

  return out;
}


void CTP7Algo::cacheLUT(const LUTType type, const xdata::Table& lut, 
                        const std::string& version)
{
  if(version.empty())
    throw std::runtime_error("LUT version may not be an empty string");

  if(lutVersion_.empty())
    lutVersion_ = std::string(version); // force copy
  else if(lutVersion_.compare(version))
    throw std::runtime_error("LUT version mismatch");
    
  if(luts_.find(type) != luts_.end()) // should be safe because parameters can't change anyway
    return;

  luts_[type] = makeLUT(lut);
}


void CTP7Algo::cacheLUT2(const xdata::Vector<xdata::UnsignedInteger>& lut,
                         const std::string& version)
{
  if(version.empty())
    throw std::runtime_error("LUT version may not be an empty string");

  if(lut2Version_.empty())
    lut2Version_ = std::string(version); // force copy
  else if(lut2Version_.compare(version))
    throw std::runtime_error("LUT version mismatch");

  if(bool(lut2_)) // should be safe because parameters can't change anyway
    return;

  lut2_ = makeLUT2(lut);
}

void CTP7Algo::cacheLUT3(const xdata::Table& lut, 
                        const std::string& version)
{
  if(version.empty())
    throw std::runtime_error("LUT version may not be an empty string");

  if(lut3Version_.empty())
    lut3Version_ = std::string(version); // force copy
  else if(lut3Version_.compare(version))
    throw std::runtime_error("LUT version mismatch");
    
  if(bool(lut3_)) // should be safe because parameters can't change anyway
    return;

  lut3_ = makeLUT3(lut);
}


std::unique_ptr<LUT> CTP7Algo::makeLUT(const xdata::Table& in) const
{
  std::unique_ptr<LUT> out(new LUT);

  std::vector<std::string> cols = in.getColumns();

  size_t iEtaMin, iEtaMax = 0;

  // make sure the table has acceptable dimensions
  if(cols.size() == 29 && in.getRowCount() == 512) // ECAL/HCAL
    {
      iEtaMin = 1;
      iEtaMax = 28;
    }
  else if(cols.size() == 13 && in.getRowCount() == 1024) // HF
    {
      iEtaMin = 30;
      iEtaMax = 41;
    }
  else
    throw std::runtime_error("Invalid LUT");

  for(size_t iCol = 0; iCol < cols.size(); ++iCol)
    {
      std::string& col = cols.at(iCol);

      if(!col.compare("Input"))
        continue;
      
      size_t iEta = std::stoi(col);
      (*out)[iEta].reserve(in.getRowCount());

      if(iEta < iEtaMin || iEta > iEtaMax)
        {
          std::stringstream ss;
          ss << "Invalid column " << col << " in LUT";
          throw std::runtime_error(ss.str());
        }

      for(size_t idx = 0; idx < in.getRowCount(); ++idx)
        {
          xdata::UnsignedInteger* valRaw = 
            static_cast<xdata::UnsignedInteger*>(in.getValueAt(idx, col));
          uint32_t val = valRaw->value_;
          (*out)[iEta].push_back(val);
        }
    }

  return out;
}


std::unique_ptr<LUT2> 
CTP7Algo::makeLUT2(const xdata::Vector<xdata::UnsignedInteger>& in) const
{
  // make sure the table has acceptable dimensions
  if(in.size() != 8192)
    throw std::runtime_error("Invalid second-stage LUT");

  std::unique_ptr<LUT2> out(new LUT2);

  for(const auto& numIn : in)
    out->push_back(numIn.value_);

  return out;
}

std::unique_ptr<LUT3> CTP7Algo::makeLUT3(const xdata::Table& in) const
{
  std::unique_ptr<LUT3> out(new LUT3);

  std::vector<std::string> cols = in.getColumns();

  size_t iEtaMin, iEtaMax = 0;

  // make sure the table has acceptable dimensions
  if(cols.size() == 28 && in.getRowCount() == 1) // HCAL FB
    {
      iEtaMin = 1;
      iEtaMax = 28;
    }
  else
    throw std::runtime_error("Invalid HCal FB LUT");

  for(size_t iCol = 0; iCol < cols.size(); ++iCol)
    {
      std::string& col = cols.at(iCol);
      
      size_t iEta = std::stoi(col);
      (*out)[iEta].reserve(in.getRowCount());

      if(iEta < iEtaMin || iEta > iEtaMax)
        {
          std::stringstream ss;
          ss << "Invalid column " << col << " in HCal FB LUT";
          throw std::runtime_error(ss.str());
        }

      for(size_t idx = 0; idx < in.getRowCount(); ++idx)
        {
          xdata::UnsignedInteger64* valRaw = 
            static_cast<xdata::UnsignedInteger64*>(in.getValueAt(idx, col));
          uint64_t val = valRaw->value_;
          (*out)[iEta].push_back(val);
        }
    }

  return out;
}


bool CTP7Algo::loadLUT(const LUTType type) const
{
  if(!isLUTCached(type))
    return false;

  bool success = true;

  for(LUT::const_iterator iLUT = luts_.at(type)->begin();
      iLUT != luts_.at(type)->end() && success; ++iLUT)
    {
      size_t iEta = iLUT->first;

      success &= client_->setInputLinkLUT(true, type, // +eta
                                          static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                          iLUT->second);
      success &= client_->setInputLinkLUT(false, type, // -eta
                                          static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                          iLUT->second);
    }

  return success;
}


bool CTP7Algo::loadLUT2() const
{
  if(!isLUT2Cached())
    return false;

  bool success = true;

  for(size_t iEta = 1; iEta <= 28; ++iEta)
    {
      success &= client_->setInputLinkLUT2S(true, // +eta
                                            static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                            *lut2_);
      success &= client_->setInputLinkLUT2S(false, // -eta
                                            static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                            *lut2_);
    }

  return success;
}


bool CTP7Algo::loadLUT3() const
{
  if(!isLUT3Cached())
    return false;

  bool success = true;

  for(size_t iEta = 1; iEta <= 28; ++iEta)
    {
      std::vector<uint32_t> ietaLutVal (2);
      // populate standard uint32_t vector of two elements with lower and upper 64b parts from HCal FB LUT
      ietaLutVal.at(0)= (*lut3_).at(iEta)[0] & 0xFFFFFFFF; // lower 32-bit part
      ietaLutVal.at(1)= (*lut3_).at(iEta)[0] >> 32;        // upper 32-bit part

      success &= client_->setInputLinkLUTHcalFb(true, // +eta
                                          static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                          ietaLutVal);
      success &= client_->setInputLinkLUTHcalFb(false, // -eta
                                          static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                          ietaLutVal);
    }

  return success;
}


bool CTP7Algo::loadLUTs() const
{
  // Set CTP7 version key to garbage while loading
  if(!client_->setConfiguration("GARBAGEGARBAGEGARBAGE"))
    return false;

  if(!(loadLUT(LUTType::ECAL) && loadLUT(LUTType::HCAL) && loadLUT(LUTType::HF)))
    return false;

  if(!loadLUT2())
    return false;

  if(!(loadLUT3()))
    return false;

  // Set CTP7 version to this version
  return client_->setConfiguration(cachedLUTVersion());
}


bool CTP7Algo::verifyLUTs() const
{
  if(!lutVersionsMatch())
    return false;

  for(LUTMap::const_iterator iCalo = luts_.begin();
      iCalo != luts_.end(); ++iCalo)
    {
      LUTType type = iCalo->first;

      for(LUT::const_iterator iLUT = iCalo->second->begin();
          iLUT != iCalo->second->end(); ++iLUT)
        {
          size_t iEta = iLUT->first;
          for(int negEta = -1; negEta <= 1; negEta += 2)
            {
              std::vector<uint32_t> fromCTP7;
              if(!client_->getInputLinkLUT(negEta < 0, type,
                                           static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                           fromCTP7))
                throw std::runtime_error("Failed to recieve LUT from CTP7");

              if(!(iLUT->second == fromCTP7)) // how pythonic
                {
                  client_->setConfiguration("");
                  return false;
                }
            }
        }
    }

  return true;
}


bool CTP7Algo::verifyLUT2() const
{
  if(!lut2VersionsMatch())
    return false;

  for(size_t iEta = 1; iEta <= 28; ++iEta)
    {
      for(int negEta = -1; negEta <= 1; negEta += 2)
        {
          LUT2 fromCTP7;
          if(!client_->getInputLinkLUT2S(negEta < 0,
                                         static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                         fromCTP7))
            throw std::runtime_error("Failed to recieve second-stage LUT from CTP7");

          if(*lut2_ != fromCTP7) // how pythonic
            {
              client_->setConfiguration("");
              return false;
            }
        }
    }

  return true;
}

bool CTP7Algo::verifyLUT3() const
{
  if(!lut3VersionsMatch())
    return false;

  for(size_t iEta = 1; iEta <= 28; ++iEta)
    {
      for(int negEta = -1; negEta <= 1; negEta += 2)
        {
	  std::vector<uint32_t> fromCTP7 (2);
	  // populate standard uint32_t vector of two elements with lower and upper 64b parts of HCal FB LUT from CTP7
          if(!client_->getInputLinkLUTHcalFb(negEta < 0,
                                         static_cast<UCT2016Layer1CTP7::LUTiEtaIndex>(iEta),
                                         fromCTP7))
            throw std::runtime_error("Failed to recieve HCal FB LUT from CTP7");

	  uint32_t lut3_lower = (*lut3_).at(iEta)[0] & 0xFFFFFFFF; // lower 32-bit part
	  uint32_t lut3_upper = (*lut3_).at(iEta)[0] >> 32;        // upper 32-bit part

	  if((lut3_lower != fromCTP7.at(0)) || (lut3_upper != fromCTP7.at(1)))
            {
              client_->setConfiguration("");
              return false;
            }
        }
    }

  return true;
}


void CTP7Algo::retrieveMetricValues() 
{
  time_t now;
  std::time(&now);

  double elapsed = std::difftime(now, lastLUTVerification_);
  double elapsed2 = std::difftime(now, lastLUT2Verification_);
  double elapsed3 = std::difftime(now, lastLUT3Verification_);
  
  if(!cachedLUTVersion().empty())
    {
      if(forceVerification_ || elapsed > lutVerificationPeriod_)
        {
          lutsCorrupted_ = !verifyLUTs();
          std::time(&now);
          lastLUTVerification_ = now;
          elapsed = std::difftime(now, lastLUTVerification_);
          forceVerification_ = false;
        }
      if(forceVerificationLUT2_ || 
         (lastLUTVerification_ > lastLUT2Verification_ && 
          elapsed > lut2VerificationOffset_))
        {
          lut2Corrupted_ = !verifyLUT2();
          std::time(&now);
          lastLUT2Verification_ = now;
          elapsed2 = std::difftime(now, lastLUT2Verification_);
          forceVerificationLUT2_ = false;
        }
      if(forceVerificationLUT3_ || 
         (lastLUTVerification_ > lastLUT3Verification_ && 
          elapsed > lut3VerificationOffset_))
        {
          lut3Corrupted_ = !verifyLUT3();
          std::time(&now);
          lastLUT3Verification_ = now;
          elapsed3 = std::difftime(now, lastLUT3Verification_);
          forceVerificationLUT3_ = false;
        }
    }

  setMetricValue<std::string>(mMetricLUTVersion, loadedLUTVersion());
  setMetricValue<bool>(mMetricLUTsCorrupted, lutsCorrupted_);
  setMetricValue<float>(mMetricTimeSinceLUTVerification, elapsed);
  setMetricValue<std::string>(mMetricLUT2Version, cachedLUT2Version());
  setMetricValue<bool>(mMetricLUT2Corrupted, lut2Corrupted_);
  setMetricValue<float>(mMetricTimeSinceLUT2Verification, elapsed2);
  setMetricValue<std::string>(mMetricLUT3Version, cachedLUT3Version());
  setMetricValue<bool>(mMetricLUT3Corrupted, lut3Corrupted_);
  setMetricValue<float>(mMetricTimeSinceLUT3Verification, elapsed3);
}

}}

#ifndef UCT2016Layer1CTP7_hh
#define UCT2016Layer1CTP7_hh

//
// This header file specifies functional interface to the
// Upgraded Calorimeter Trigger Layer-1 CTP7.
//
// The ZYNQ based softare encapsulates all details of the
// firmware operation so addresses, internal state transition
// details, sequencing of commands, etc. are not exposed.
//

#include <stdint.h>
#include <vector>
#include <string>

#include <wiscrpcsvc.h>

class UCT2016Layer1CTP7
{

public:

	// Each CTP7 is processing calorimeter data corresponding to
	// 20 degree Phi region. phi parameter (0 - 17) is set
	// according to the RCT map:
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/RCTMap

	enum ConnectStringType {
		CONNECTSTRING_PHIMAPXML = 0,
		CONNECTSTRING_IPHOST    = 1
	};
	UCT2016Layer1CTP7(uint32_t phi, const std::string connect_string, enum ConnectStringType connect_type = CONNECTSTRING_PHIMAPXML);

	virtual ~UCT2016Layer1CTP7();

	// Since we don't construct an object of this type there is no
	// constructor specified.  The actual implementation can specify
	// IP address / port parameters in the public constructor

	// We want to routinely make sure that connection to CTP7 is good

	virtual bool checkConnection();

	// Resets
	// These functions reset the CTP7 firmware to a known status

	virtual bool hardReset(std::string bitstream = "ctp7_v7_stage2.bit");
	virtual bool softReset();

         virtual bool setTxPower(bool enabled);
       
       	 virtual bool configRefClk(void);


	//
	virtual bool getPhi(uint32_t &phi)
	{
		phi = this->phi;
		return true;
	};

	// UCT2016 Layer-1 CTP7 boards can be configured to either in normal run
	// mode where data is simply played through, or in playback mode
	// when data from playback buffers is processed and played out
	// There is a special test mode for debugging purpose
	// The idle mode should disable output links
	//

	enum RunMode
	{
		normal   = 0x0,
		input_playBack = 0x1,
		ecal_testMode = 0x2,
		output_pattern = 0x3,
		idle_mode = 0xF,
		unknown  = 0xFF
	};

	virtual bool setRunMode(RunMode mode);
	virtual bool getRunMode(RunMode &mode);


	// Configuration string for conveniently storing user information
	// This is just an arbitrary string that can be set and retrieved
	// and is left to the user to maintain -- it has no bearing on the
	// CTP7 or its functionality

	virtual bool getConfiguration(std::string &user_config_string);
	virtual bool setConfiguration(std::string user_config_string);


	// Link Alignment and status

	virtual bool alignInputLinks(bool negativeEta, uint32_t bx, uint32_t sub_bx, bool alignManual = false);

	virtual bool getInputLinkAlignmentStatus(bool negativeEta, uint32_t &alignStatus);

	// Link Status

	typedef struct LinkStatusSummary
	{
		uint32_t rawLinkStatus;
		uint32_t linkUp;
		uint32_t linkGotAligned;
		uint32_t linkLocked;
		uint32_t linkFifoEmpty;
		uint32_t checkSumErrorCount;
		uint32_t bx0ErrorCount;
		uint32_t bx0Latency;
		uint32_t alignmentMask;
		uint32_t towerMask;
	} LinkStatusSummary;

	virtual bool getInputLinkStatus(bool negativeEta, std::vector<LinkStatusSummary> &linkSummary);

	// No link Summary available in FW currently
	//virtual bool getOutputLinkStatus(std::vector<LinkStatusSummary> &linkSummary);

	// Special functions for controlling input / output capture

	enum CaptureState
	{
		Idle = 0,
		Armed = 1,
		Capturing = 2,
		Done = 3
	};

	typedef struct CaptureStatus
	{
		uint16_t localBXId;
		uint16_t linkBXId;
		CaptureState state;
	} CaptureStatus;

	enum CaptureMode
	{
		linkBXAligned = 0,
		localBXAligned = 1,
		tmtMasterAligned = 2 // Output Only
	};

	typedef struct CaptureBX
	{
		uint32_t bx;
		uint32_t sub_bx;
	} CaptureBX;

	virtual bool captureInputLinks(bool negativeEta, CaptureMode mode);
	virtual bool getInputLinkCaptureStatus(bool negativeEta, std::vector<CaptureStatus> &c);
	virtual bool setInputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> c);
	virtual bool getInputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> &c);

	virtual bool captureOutputLinks(bool negativeEta, CaptureMode mode);
	virtual bool getOutputLinkCaptureStatus(bool negativeEta, std::vector<CaptureStatus> &c);
	virtual bool setOutputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> c);
	virtual bool getOutputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> &c);

	typedef struct CaptureSyncd
	{
		bool waitForSyncOrbitCnt;
		uint32_t syncOrbitCnt;
		uint32_t bx;
	} CaptureSyncd;

	virtual bool captureSyncdInputOutputLinks(bool negativeEta, CaptureSyncd mode);

	// Functions to write / read BRAM

	// Input Capture and Playback BRAM is the same physical BRAM.
	// Functionally we read input capture BRAM and set playback BRAM.
	// There are 16 ECAL, 14 HCAL and 2 HF BRAMs per eta side, each of 1K 32-bit word blocks
	// So these functions get or set 32 blocks of 1K 32-bit words all together
	// or a selected block to avoid unnecessarily large data transport

	enum InputLink
	{
		ECAL_Link_00 = 0x00,
		ECAL_Link_01 = 0x01,
		ECAL_Link_02 = 0x02,
		ECAL_Link_03 = 0x03,
		ECAL_Link_04 = 0x04,
		ECAL_Link_05 = 0x05,
		ECAL_Link_06 = 0x06,
		ECAL_Link_07 = 0x07,
		ECAL_Link_08 = 0x08,
		ECAL_Link_09 = 0x09,
		ECAL_Link_10 = 0x0a,
		ECAL_Link_11 = 0x0b,
		ECAL_Link_12 = 0x0c,
		ECAL_Link_13 = 0x0d,
		ECAL_Link_14 = 0x0e,
		ECAL_Link_15 = 0x0f,

		HCAL_Link_00 = 0x10,
		HCAL_Link_01 = 0x11,
		HCAL_Link_02 = 0x12,
		HCAL_Link_03 = 0x13,
		HCAL_Link_04 = 0x14,
		HCAL_Link_05 = 0x15,
		HCAL_Link_06 = 0x16,
		HCAL_Link_07 = 0x17,
		HCAL_Link_08 = 0x18,
		HCAL_Link_09 = 0x19,
		HCAL_Link_10 = 0x1a,
		HCAL_Link_11 = 0x1b,
		HCAL_Link_12 = 0x1c,
		HCAL_Link_13 = 0x1d,

		HF_Link_0 = 0x1e,
		HF_Link_1 = 0x1f
	};

	enum AlignMode
	{
		runStart = 0,
		immediate = 1,
	};


	virtual bool setLinkAlignmentMode(AlignMode mode);
	virtual bool getLinkAlignmentMode(AlignMode &mode);

	virtual bool resetInputLinkDecoders(bool negativeEta);

	virtual bool resetInputLinkChecksumErrorCounters(bool negativeEta);
	virtual bool resetInputLinkBX0ErrorCounters(bool negativeEta);

	virtual bool setInputLinkAlignmentMask(bool negativeEta, std::vector<uint32_t> mask);
	virtual bool getInputLinkAlignmentMask(bool negativeEta, std::vector<uint32_t> & mask);

	virtual bool setInputLinkTowerMask(bool negativeEta, std::vector<uint32_t> mask);
	virtual bool getInputLinkTowerMask(bool negativeEta, std::vector<uint32_t> & mask);

	virtual bool getInputLinkBuffer(bool negativeEta, InputLink link, std::vector<uint32_t> &selectedData);
	virtual bool setInputLinkBuffer(bool negativeEta, InputLink link, const std::vector<uint32_t> &selectedData);

	// There are 24 output buffers corresponding to the 24 output links each for positive and negative eta
	// Each buffer corresponds to a memory block of 1K 32-bit words

	enum OutputLink
	{
		Link_00 = 0x00,
		Link_01 = 0x01,
		Link_02 = 0x02,
		Link_03 = 0x03,
		Link_04 = 0x04,
		Link_05 = 0x05,
		Link_06 = 0x06,
		Link_07 = 0x07,
		Link_08 = 0x08,
		Link_09 = 0x09,
		Link_10 = 0x0a,
		Link_11 = 0x0b,
		Link_12 = 0x0c,
		Link_13 = 0x0d,
		Link_14 = 0x0e,
		Link_15 = 0x0f,
		Link_16 = 0x10,
		Link_17 = 0x11,
		Link_18 = 0x12,
		Link_19 = 0x13,
		Link_20 = 0x14,
		Link_21 = 0x15,
		Link_22 = 0x16,
		Link_23 = 0x17
	};

	virtual bool getOutputLinkBuffer(bool negativeEta, OutputLink link, std::vector<uint32_t> &data);
	virtual bool setOutputLinkBuffer(bool negativeEta, OutputLink link, const std::vector<uint32_t> &data); // This function is for testing purpose only

	// There are 28x 2kB ECAL and 28x 2kB HCAL LUT BRAMs (one BRAM per iEta, 1-28) in EB+EE / HB+HE region
	// There are 12x 2kB HF LUT BRAMs (one BRAM per iEta, HF_A and HF_B share the same LUT)
	//
	// !! iEta_01 - iEta_28 applicable only for ECAL and HCAL !!
	// !! iEta_30 - iEta_41 applicable only for HF !!

	enum LUTType
	{
		ECAL = 0x01,
		HCAL = 0x02,
		HF   = 0x03
	};

	enum LUTiEtaIndex
	{
		iEta_01 = 0x01,
		iEta_02 = 0x02,
		iEta_03 = 0x03,
		iEta_04 = 0x04,
		iEta_05 = 0x05,
		iEta_06 = 0x06,
		iEta_07 = 0x07,
		iEta_08 = 0x08,
		iEta_09 = 0x09,
		iEta_10 = 0x0a,
		iEta_11 = 0x0b,
		iEta_12 = 0x0c,
		iEta_13 = 0x0d,
		iEta_14 = 0x0e,
		iEta_15 = 0x0f,
		iEta_16 = 0x10,
		iEta_17 = 0x11,
		iEta_18 = 0x12,
		iEta_19 = 0x13,
		iEta_20 = 0x14,
		iEta_21 = 0x15,
		iEta_22 = 0x16,
		iEta_23 = 0x17,
		iEta_24 = 0x18,
		iEta_25 = 0x19,
		iEta_26 = 0x1a,
		iEta_27 = 0x1b,
		iEta_28 = 0x1c,
		// there is no iEta_29
		iEta_30 = 0x1e,
		iEta_31 = 0x1f,
		iEta_32 = 0x20,
		iEta_33 = 0x21,
		iEta_34 = 0x22,
		iEta_35 = 0x23,
		iEta_36 = 0x24,
		iEta_37 = 0x25,
		iEta_38 = 0x26,
		iEta_39 = 0x27,
		iEta_40 = 0x28,
		iEta_41 = 0x29
	};

	virtual bool getInputLinkLUT(bool negativeEta, LUTType type, LUTiEtaIndex iEta, std::vector<uint32_t> &lut);
	virtual bool setInputLinkLUT(bool negativeEta, LUTType type, LUTiEtaIndex iEta, const std::vector<uint32_t> &lut);

	// Transmit side configuration

	virtual bool setOutputLinkPattern(bool negativeEta, const std::vector<uint32_t> value);
	virtual bool getOutputLinkPattern(bool negativeEta, std::vector<uint32_t> &value);

	virtual bool setOutputLinkID(bool negativeEta, const std::vector<uint32_t> value);
	virtual bool getOutputLinkID(bool negativeEta, std::vector<uint32_t> &value);


	enum OutputTMTLinkPair
	{
		LinkPair_00 = 0x00,
		LinkPair_01 = 0x01,
		LinkPair_02 = 0x02,
		LinkPair_03 = 0x03,
		LinkPair_04 = 0x04,
		LinkPair_05 = 0x05,
		LinkPair_06 = 0x06,
		LinkPair_07 = 0x07,
		LinkPair_08 = 0x08,
		LinkPair_09 = 0x09,
		LinkPair_10 = 0x0a,
		LinkPair_11 = 0x0b
	};
 
	virtual bool setTMTCycle(bool negativeEta, OutputTMTLinkPair olPair, uint32_t value);
	virtual bool getTMTCycle(bool negativeEta, OutputTMTLinkPair olPair, uint32_t &value);


	virtual bool alignOutputLinks(bool negativeEta, uint32_t bx = 0, uint32_t sub_bx = 0);


	// TTC Decoder Control, Monitoring and B-Go Commands

	virtual bool alignTTCDecoder(void);

	typedef struct TTCStatus
	{
		uint32_t BCClockLocked;
		uint32_t BX0Locked;
		uint32_t BX0Error;
		uint32_t BX0UnlockedCnt;
		uint32_t TTCDecoderSingleError;
		uint32_t TTCDecoderDoubleError;
	} TTCStatus;

	virtual bool getTTCStatus(TTCStatus &ttcStatus);

	typedef struct TTCBGoCmdCnt
	{
		uint32_t L1A;
		uint32_t BX0;
		uint32_t EC0;
		uint32_t Resync;
		uint32_t OC0;
		uint32_t TestSync;
		uint32_t Start;
		uint32_t Stop;
		uint32_t Orbit;
		uint32_t L1ID;
	} TTCBGoCmdCnt;

	virtual bool getTTCBGoCmdCnt(TTCBGoCmdCnt &ttcBGoCmdCnt);


	typedef struct DAQConfig
	{
		uint32_t DAQDelayLineDepth;
	} DAQConfig;

	virtual bool setDAQConfig(DAQConfig daqConfig);
	virtual bool getDAQConfig(DAQConfig &daqConfig);

	virtual bool setRunNumber(uint32_t runNumber);
	virtual bool getRunNumber(uint32_t &runNumber);
	
	typedef struct DAQStatus
	{
		uint32_t fifoOcc;
		uint32_t fifoOccMax;
		uint32_t fifoEmpty;
		uint32_t CTP77ToAMC13BP;
		uint32_t AMC13ToCTP7BP;
		uint32_t TTS;
		uint32_t internalError;
		uint32_t amcCoreReady;
	} DAQStatus;

	virtual bool setDAQStatusReset(void);
	
	virtual bool getDAQStatus(DAQStatus &daqStatus);

	typedef struct FWInfo
	{
		uint32_t buildTimestamp;
		uint32_t gitHashCode;
		uint32_t gitHashDirty;
		uint32_t version;
		uint32_t projectCode;
		uint32_t uptime;
	} FWInfo;

	// V7 FW Info
	virtual bool getFWInfo(FWInfo &fwInfo);

	// Get a string containing build information about the RPCSvc module.
	virtual bool getModuleBuildInfo(std::string &value);

        // There are 28x 256kB (8k x 32b) Extra Bit LUT2S (Second Stage) BRAMs EB+EE / HB+HE region
        virtual bool getInputLinkLUT2S(bool negativeEta, LUTiEtaIndex iEta, std::vector<uint32_t> &lut);
        virtual bool setInputLinkLUT2S(bool negativeEta, LUTiEtaIndex iEta, const std::vector<uint32_t> &lut);

        virtual bool getInputLinkLUTHcalFb(bool negativeEta, LUTiEtaIndex iEta, std::vector<uint32_t> &lut);
        virtual bool setInputLinkLUTHcalFb(bool negativeEta, LUTiEtaIndex iEta, const std::vector<uint32_t> &lut);

protected:
	uint32_t phi;
	std::string hostname;
	wisc::RPCSvc rpc;

private:
	/* We disable copying and assignment, because it is complex to maintain
	 * an extra internal structure to handle the management of shared socket
	 * connections to the server, and giving each object its own separate
	 * connection will quickly explode unless the library user is
	 * specifically aware of it and careful in their handling.
	 *
	 * Use pointers or single instances.
	 *
	 * It is easy to just allocate with
	 *   UCT2016Layer1CTP7 *card = new UCT2016Layer1CTP7(0);
	 *
	 * or to still use
	 *   UCT2016Layer1CTP7 card(0);
	 * but only pass around pointers or references after that.
	 */
	UCT2016Layer1CTP7(const UCT2016Layer1CTP7 &v);
	UCT2016Layer1CTP7& operator=(const UCT2016Layer1CTP7 &v);
};

#endif



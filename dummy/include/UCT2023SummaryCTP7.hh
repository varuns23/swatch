#ifndef UCT2023SummaryCTP7_hh
#define UCT2023SummaryCTP7_hh

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

class UCT2023SummaryCTP7
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
	UCT2023SummaryCTP7(uint32_t phi, const std::string connect_string, enum ConnectStringType connect_type = CONNECTSTRING_PHIMAPXML);

	virtual ~UCT2023SummaryCTP7();

	// Since we don't construct an object of this type there is no
	// constructor specified.  The actual implementation can specify
	// IP address / port parameters in the public constructor

	// We want to routinely make sure that connection to CTP7 is good

	virtual bool checkConnection();

	// Resets
	// These functions reset the CTP7 firmware to a known status

	virtual bool hardReset(std::string bitstream = "ctp7_v7_stage3.bit");
	
	//
	virtual bool getPhi(uint32_t &phi)
	{
		phi = this->phi;
		return true;
	};


	//***** Commands below are same for all the slot7 cards. They are not distinguished for Readout card.**
	// Here the commands are enabling CXP0 CXP1 links for all the cards.
	// Once enabled, we are checking the status of these links.
	// We run align TTC commands and then check the TTC status.
	virtual bool enableTXCXP0CXP1Links();
	virtual bool getTxInitStatus(std::vector<uint32_t> &mgtStatus);
	virtual bool getRxInitStatus(std::vector<uint32_t> &mgtStatus);
	virtual bool getClockStabilityStatus(std::vector<uint32_t> &mgtStatus);
	virtual bool getGTHAlignStatus(std::vector<uint32_t> &mgtStatus);
	virtual bool getEncoder8b10bStatus(std::vector<uint32_t> &mgtStatus);
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

	typedef struct TTC160Status
	{
		
		uint32_t TTC160Lock;
		uint32_t TTC160Errors;
		uint32_t TTCUnlockedCounter;
		uint32_t TTCUnderflowCounter;
		uint32_t TTCOverflowCounter;
	} TTC160Status;
	virtual bool getTTC160Status(TTC160Status &ttc160Status); 


	virtual bool resetInput8b10bErrorCounters();
	
	virtual bool resetBackPlaneMGTs();

	// Here all commands have a parameter to ascertain where the commands need to execute. For e.g in readout/master card or not.
	//**** Commands have the parameters readout_valid=0x1, to run only in readout/master card
	//**** Commands have the parameters readout_valid=0x0, to run only in non master card only

	//virtual bool setInputLinkAlignmentMask(bool negativeEta, std::vector<uint32_t> mask);
	virtual bool setBackPlaneInputLinkMask(std::vector<uint32_t> mask,uint32_t readoutcard_valid=0x1);
	virtual bool setOpticalInputLinkMask(std::vector<uint32_t> mask,uint32_t readoutcard_valid=0x1);

	
	virtual bool getBackPlaneInputLinkMask(std::vector<uint32_t> &mask,uint32_t readoutcard_valid=0x1);
	virtual bool getOpticalInputLinkMask(std::vector<uint32_t> &mask,uint32_t readoutcard_valid=0x1);
	//virtual bool getInputLinkAlignmentMask(std::vector<uint32_t> & mask);

	
	// Link Alignment and status

	virtual bool alignInputLinks(uint32_t bx, uint32_t sub_bx,uint32_t readoutcard_valid=0x1);
//	virtual bool getInputLinkAlignmentStatus(bool negativeEta, uint32_t &alignStatus);

	virtual bool getInputLinkAlignmentStatus(std::vector<uint32_t> &alignStatus,uint32_t readoutcard_valid=0x1);

	
	
	virtual bool resetTxCXP1(uint32_t readoutcard_valid=0x0);
	virtual bool resetRxCXP1(uint32_t readoutcard_valid=0x0);

	//These commands are executed to configure CXP2 links of the readout card, which are connected to uGT.
	
	virtual bool enableTXCXP2Links(uint32_t readoutcard_valid=0x1);
	virtual bool getTXCXP2Status(uint32_t readoutcard_valid=0x1);

	virtual bool enableRXCXP2Links(uint32_t readoutcard_valid=0x1);
	virtual bool getRXCXP2Status(uint32_t readoutcard_valid=0x1);

	virtual bool enableQPLLReset(uint32_t readoutcard_valid=0x1);
	virtual bool getQPLLStatus(uint32_t readoutcard_valid=0x1);

	// These commands are employed to reset the input to  Algo module and the Gearbox module in the master card.

	virtual bool resetAlgoMod(uint32_t readoutcard_valid=0x1);
	virtual bool resetGearboxMod(uint32_t readoutcard_valid=0x1);
	
	
	
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
	 *   UCT2023SummaryCTP7 *card = new UCT2023SummaryCTP7(0);
	 *
	 * or to still use
	 *   UCT2023SummaryCTP7 card(0);
	 * but only pass around pointers or references after that.
	 */
	UCT2023SummaryCTP7(const UCT2023SummaryCTP7 &v);
	UCT2023SummaryCTP7& operator=(const UCT2023SummaryCTP7 &v);
};

#endif



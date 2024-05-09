#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <wiscrpcsvc.h>
#include "UCT2023SummaryCTP7.hh"

// Including the files included by tinyxml2.h here as well, to make sure we
// don't get them subsequently imported with the wrong visibility and having it
// matter somehow, later.  (Hail header include guards!)
#include <cctype>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#pragma GCC visibility push(hidden)
#include "tinyxml2.h"
#pragma GCC visibility pop

using namespace wisc;
using namespace std;

static const std::string module_name = "UCT2023Summary";
static const std::string module_key = "UCT2023Summary v1.1.0";

#define RPC_CATCH_RETURN catch (wisc::RPCSvc::RPCException &e) {  return false; }

#define TYPE_GENERIC_CALL(rtype, result, rpcreq) \
    do { \
        if (!this->checkConnection()) \
            return false; \
        try { \
            RPCMsg rpcrsp = this->rpc.call_method(rpcreq); \
            if (rpcrsp.get_key_exists("error")) \
                return false; \
            if (rpcrsp.get_key_exists("result")) \
                result = rpcrsp.get_ ## rtype("result"); \
            return true; \
        } \
        RPC_CATCH_RETURN; \
    } while (0)

#define WORD_GENERIC_CALL(result, rpcreq) TYPE_GENERIC_CALL(word, result, rpcreq)
#define GENERIC_CALL(rpcreq) WORD_GENERIC_CALL(__attribute__((unused)) uint32_t discard, rpcreq)

UCT2023SummaryCTP7::UCT2023SummaryCTP7(uint32_t phi, const std::string connect_string, enum ConnectStringType connect_type)
	: phi(phi)
{
	// Find out the correct address to access.

	if (connect_type == CONNECTSTRING_PHIMAPXML) {
		tinyxml2::XMLDocument phiMapDoc;
		if (phiMapDoc.LoadFile(connect_string.c_str()))
			throw std::runtime_error("Unable to read address configuration XML 1");

		tinyxml2::XMLElement *config_elem = phiMapDoc.FirstChildElement("layer1_hw_config");
		if (!config_elem)
			throw std::runtime_error("Unable to read address configuration XML 2");

		this->hostname = "";

		for (tinyxml2::XMLElement *phi_elem = config_elem->FirstChildElement("phi_config"); phi_elem; phi_elem = phi_elem->NextSiblingElement("phi_config"))
		{
			int phival;
			if (phi_elem->QueryIntAttribute("phi", &phival) != 0)
				continue;
			if (phival != phi)
				continue;
			tinyxml2::XMLElement *ba_elem = phi_elem->FirstChildElement("board_address");
			if (!ba_elem)
				continue;
			const char *ba_text = ba_elem->GetText();
			if (!ba_text)
				continue;
			this->hostname = ba_text;
			break;
		}

		if (!this->hostname.size())
			throw std::runtime_error("Unable to locate address configuration for the requested phi");
	}
	else if (connect_type == CONNECTSTRING_IPHOST) {
		this->hostname = connect_string;
	}
	else {
		throw std::runtime_error("Invalid connect_type provided");
	}

	try
	{
		this->rpc.connect(this->hostname);
	
		if (!this->rpc.load_module(module_name, module_key))
			throw std::runtime_error("Unable to load the appropriate rpcsvc module");
	}
	catch (wisc::RPCSvc::RPCException &e)
	{
		throw std::runtime_error(std::string("RPCSvc Exception: ") + e.message);
	}
}


UCT2023SummaryCTP7::~UCT2023SummaryCTP7()
{

}

bool UCT2023SummaryCTP7::checkConnection()
{
	try
	{	
		this->rpc.call_method(RPCMsg(module_name + ".ping"));
		return true;
	}
	catch (wisc::RPCSvc::RPCException &e)
	{
		 //throw std::runtime_error(std::string("RPCSvc Exception: ") + e.message);
	}

	// Bad connection.  Let's try reconnecting.
	this->rpc.disconnect();

	try
	{
		this->rpc.connect(this->hostname);
		if (!this->rpc.load_module(module_name, module_key))
			return false;
		return true;
	}
	catch (wisc::RPCSvc::RPCException &e)
	{
		return false;
	}
}

bool UCT2023SummaryCTP7::hardReset(std::string bitstream)
{
	GENERIC_CALL(RPCMsg(module_name + ".hardReset")
	        .set_string("bitstream", bitstream)
                .set_word("phi", this->phi)
		);
}

bool UCT2023SummaryCTP7::resetTxCXP1(uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetTxCXP1")
				.set_word("readout_card", readoutcard_valid)
	            );
}


bool UCT2023SummaryCTP7::resetRxCXP1(uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetRxCXP1")
				.set_word("readout_card", readoutcard_valid)
	            );
}

bool UCT2023SummaryCTP7::resetAlgoMod(uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetAlgoMod")
				.set_word("readout_card", readoutcard_valid)
	            );
}

bool UCT2023SummaryCTP7::resetGearboxMod(uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetGearboxMod")
				.set_word("readout_card", readoutcard_valid)
	            );
}



bool UCT2023SummaryCTP7::setBackPlaneInputLinkMask(std::vector<uint32_t> mask,uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".setBackPlaneInputLinkMask")
	             .set_word_array("mask", mask)
				 .set_word("readout_card", readoutcard_valid)
	            );
}




bool UCT2023SummaryCTP7::setOpticalInputLinkMask(std::vector<uint32_t> mask,uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".setOpticalInputLinkMask")
	             .set_word_array("mask", mask)
				 .set_word("readout_card", readoutcard_valid)
	            );
}

bool UCT2023SummaryCTP7::enableTXCXP0CXP1Links()
{
	GENERIC_CALL(RPCMsg(module_name + ".enableTXCXP0CXP1Links")
	            );
}

bool UCT2023SummaryCTP7::enableTXCXP2Links(uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".enableTXCXP2Links")	
		 		.set_word("readout_card", readoutcard_valid)  // Use 1 for true
	            );
}

bool UCT2023SummaryCTP7::getTXCXP2Status( uint32_t readoutcard_valid)
{
	if (!this->checkConnection())
		return false;

	try
		{
		uint32_t hostnameInvalid;
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getTXCXP2Status")
							.set_word("readout_card", readoutcard_valid)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		

		if (!rpcrsp.get_key_exists("TXCXP2")) 
			return false;

		if (rpcrsp.get_key_exists("hostnameInvalid"))
		// checking If the command is not executed at right card
		{
			std::cout << "hostname Invalid executed" << std::endl;
			hostnameInvalid =0x1;
		}
		
		std::cout << " To retierive status of thelinks \n" << std::endl;
		std::vector<uint32_t> TxCxp2  = rpcrsp.get_word_array("TXCXP2");

		for (int i = 0; i < TxCxp2.size(); i++)
		{

			uint32_t txcxp2 = (TxCxp2[i]) ;
			if((txcxp2 & 0x7) == 0x7|| hostnameInvalid==0x1)
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false;
}



bool UCT2023SummaryCTP7::enableRXCXP2Links(uint32_t readoutcard_valid) 
{
	GENERIC_CALL(RPCMsg(module_name + ".enableRXCXP2Links")
		 .set_word("readout_card", readoutcard_valid)
	            );
}

bool UCT2023SummaryCTP7::getRXCXP2Status( uint32_t readoutcard_valid)
{
	if (!this->checkConnection())
		return false;
	try
		{
		uint32_t hostnameInvalid;
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getRXCXP2Status")
							.set_word("readout_card", readoutcard_valid)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("RXCXP2")) 
			return false;

		if (rpcrsp.get_key_exists("hostnameInvalid"))
		// checking If the command is not executed at right card
		{
			std::cout << "hostname Invalid executed" << std::endl;
			hostnameInvalid =0x1;
		}
		

		std::vector<uint32_t> RxCxp2  = rpcrsp.get_word_array("RXCXP2");

		for (int i = 0; i < RxCxp2.size(); i++)
		{

			uint32_t rxcxp2 = (RxCxp2[i]) ;
			if((rxcxp2 & 0x7) == 0x7|| hostnameInvalid==0x1)
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false;
}




bool UCT2023SummaryCTP7::enableQPLLReset(uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".enableQPLLReset")
		.set_word("readout_card", readoutcard_valid)
		    );
}

bool UCT2023SummaryCTP7::getQPLLStatus( uint32_t readoutcard_valid)
{
	if (!this->checkConnection())
		return false;
	try
	{
		uint32_t hostnameInvalid;
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getQPLLStatus")
							.set_word("readout_card", readoutcard_valid)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("QPLL")) 
			return false;

		if (rpcrsp.get_key_exists("hostnameInvalid"))
		// checking If the command is not executed at right card
		{
			std::cout << "hostname Invalid executed" << std::endl;
			hostnameInvalid =0x1;
		}
		

		std::vector<uint32_t> QPll  = rpcrsp.get_word_array("QPLL");

		for (int i = 0; i < QPll.size(); i++)
		{

			uint32_t qpll = (QPll[i]) ;
			if((qpll & 0x7) == 0x1|| hostnameInvalid==0x1)
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false;
}



bool UCT2023SummaryCTP7::getBackPlaneInputLinkMask( std::vector<uint32_t> &mask,uint32_t readoutcard_valid)
{
	if (!this->checkConnection())
		return false;
		try
          	{
                uint32_t hostnameInvalid;
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getBackPlaneInputLinkMask")
							.set_word("readout_card", readoutcard_valid)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("BackPlaneMask")) 
			return false;

		if (rpcrsp.get_key_exists("hostnameInvalid"))
		// checking If the command is not executed at right card
		{
			std::cout << "hostname Invalid executed" << std::endl;
			hostnameInvalid =0x1;
		}
		std::vector<uint32_t> BackPlaneMask  = rpcrsp.get_word_array("BackPlaneMask");

		for (int i = 0; i < BackPlaneMask.size(); i++)
		{

			uint32_t backplanemask = (BackPlaneMask[i]) ;
			if((backplanemask == 0x0)||(backplanemask == 0x1 && hostnameInvalid==0x1)) 
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false;
}
bool UCT2023SummaryCTP7::getOpticalInputLinkMask( std::vector<uint32_t> &mask,uint32_t readoutcard_valid)
{
	if (!this->checkConnection())
		return false;
	try
	{
		
        uint32_t hostnameInvalid;
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getOpticalInputLinkMask")
							.set_word("readout_card", readoutcard_valid)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("OpticalMask")) 
			return false;

		if (rpcrsp.get_key_exists("hostnameInvalid"))
		// checking If the command is not executed at right card
		{
			std::cout << "hostname Invalid executed" << std::endl;
			hostnameInvalid =0x1;
		}
		std::vector<uint32_t> OpticalMask  = rpcrsp.get_word_array("OpticalMask");

		for (int i = 0; i < OpticalMask.size(); i++)
		{

			uint32_t opmask = (OpticalMask[i]) ;
			if((opmask == 0x0)||(opmask==0x1 && hostnameInvalid==0x1)) 
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false ;
}
bool UCT2023SummaryCTP7::getTxInitStatus( std::vector<uint32_t> &txinitStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getTxInitStatus")
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("TxInit")) 
			return false;

		std::vector<uint32_t> TxInit  = rpcrsp.get_word_array("TxInit");

		if (TxInit.size() != 36)      return false;

		for (int i = 0; i < TxInit.size(); i++)
		{

			uint32_t txstat = (TxInit[i]) ;
			if((txstat & 0x7) == 0x7) 
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false ;
}

bool UCT2023SummaryCTP7::getRxInitStatus( std::vector<uint32_t> &rxinitStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getRxInitStatus")
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("RxInit"))             return false;

		std::vector<uint32_t> RxInit  = rpcrsp.get_word_array("RxInit");


		for (int i = 0; i < RxInit.size(); i++)
		{

			uint32_t rxstat = (RxInit[i]) ;
			if((rxstat & 0x7) == 0x7) 
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false ;
}

bool UCT2023SummaryCTP7::getClockStabilityStatus( std::vector<uint32_t> &clockstabilityStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getClockStabilityStatus")
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("ClockStability"))      return false;

		std::vector<uint32_t> ClockStability  = rpcrsp.get_word_array("ClockStability");

		for (int i = 0; i < ClockStability.size(); i++)
		{

			uint32_t clkstat = (ClockStability[i]) ;
			if((clkstat & 0x7) == 0x1) 
				return true;
			else
				return false;

		}
		
	}
	RPC_CATCH_RETURN;
return false ;
}

bool UCT2023SummaryCTP7::getGTHAlignStatus( std::vector<uint32_t> &gthalignStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getGTHAlignStatus")
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("GTHAlign"))        return false;

		std::vector<uint32_t> GTHAlign  = rpcrsp.get_word_array("GTHAlign");

		for (int i = 0; i < GTHAlign.size(); i++)
		{

			uint32_t gthstat = (GTHAlign[i]) ;
			if((gthstat & 0x3) == 0x1) 
				return true;
			else
				return false;

		}
			
	}
	RPC_CATCH_RETURN;
return false ;
}

bool UCT2023SummaryCTP7::getEncoder8b10bStatus( std::vector<uint32_t> &encoder8b10bStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getEncoder8b10bStatus")
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("Encoder8b10b")) return false;
		
		std::vector<uint32_t> Encoder8b10b  = rpcrsp.get_word_array("Encoder8b10b");

		for (int i = 0; i < Encoder8b10b.size(); i++)
		{

			uint32_t encoderstat = (Encoder8b10b[i]) ;
			if((encoderstat & 0x18) == 0x0) 
				return true;
			else
				return false;

		}
	}
	RPC_CATCH_RETURN;
return false ;
}


bool UCT2023SummaryCTP7::resetInput8b10bErrorCounters()
{
	GENERIC_CALL(RPCMsg(module_name + ".resetInput8b10bErrorCounters")
	            );
}

bool UCT2023SummaryCTP7::resetBackPlaneMGTs()
{
	GENERIC_CALL(RPCMsg(module_name + ".resetBackPlaneMGTs")
	            );
}


bool UCT2023SummaryCTP7::alignTTCDecoder(void)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".alignTTCDecoder")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		return true;
	}
	RPC_CATCH_RETURN;
}

/* typedef struct TTCStatus {
 *  uint32_t BCClockLocked;
 *  uint32_t BX0Locked;
 *  uint32_t BX0Error;
 *  uint32_t BX0UnlockedCnt;
 *  uint32_t TTCDecoderSingleError;
 *  uint32_t TTCDecoderDoubleError;
 * } TTCStatus;
 */

bool UCT2023SummaryCTP7::getTTCStatus(TTCStatus &ttcStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getTTCStatus")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("BCClockLocked"))         return false;
		if (!rpcrsp.get_key_exists("BX0Locked"))             return false;
		if (!rpcrsp.get_key_exists("BX0Error"))              return false;
		if (!rpcrsp.get_key_exists("BX0UnlockedCnt"))        return false;
		if (!rpcrsp.get_key_exists("TTCDecoderSingleError")) return false;
		if (!rpcrsp.get_key_exists("TTCDecoderDoubleError")) return false;

		ttcStatus.BCClockLocked         = rpcrsp.get_word("BCClockLocked");
		ttcStatus.BX0Locked             = rpcrsp.get_word("BX0Locked");
		ttcStatus.BX0Error              = rpcrsp.get_word("BX0Error");
		ttcStatus.BX0UnlockedCnt        = rpcrsp.get_word("BX0UnlockedCnt");
		ttcStatus.TTCDecoderSingleError = rpcrsp.get_word("TTCDecoderSingleError");
		ttcStatus.TTCDecoderDoubleError = rpcrsp.get_word("TTCDecoderDoubleError");

		return true;
	}
	RPC_CATCH_RETURN;
}


bool UCT2023SummaryCTP7::getTTC160Status(TTC160Status &ttc160Status)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getTTC160Status")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("TTC160Lock"))            return false;
		if (!rpcrsp.get_key_exists("TTC160Errors"))          return false;
		if (!rpcrsp.get_key_exists("TTCUnlockedCounter"))    return false;
		if (!rpcrsp.get_key_exists("TTCUnderflowCounter"))   return false;
		if (!rpcrsp.get_key_exists("TTCOverflowCounter"))    return false;

		ttc160Status.TTC160Lock            = rpcrsp.get_word("TTC160Lock");
		ttc160Status.TTC160Errors          = rpcrsp.get_word("TTC160Errors");
		ttc160Status.TTCUnlockedCounter    = rpcrsp.get_word("TTCUnlockedCounter");
		ttc160Status.TTCUnderflowCounter   = rpcrsp.get_word("TTCUnderflowCounter");
		ttc160Status.TTCOverflowCounter    = rpcrsp.get_word("TTCOverflowCounter");
		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2023SummaryCTP7::alignInputLinks( uint32_t bx, uint32_t sub_bx,uint32_t readoutcard_valid)
{
	GENERIC_CALL(RPCMsg(module_name + ".alignInputLinks")
	             .set_word("bx", bx)
	             .set_word("sub_bx", sub_bx)
				 .set_word("readout_card", readoutcard_valid)
	            );
}

bool UCT2023SummaryCTP7::getInputLinkAlignmentStatus(std::vector<uint32_t> &alignStatus,uint32_t readoutcard_valid)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getInputLinkStatus")
							.set_word("readout_card", readoutcard_valid)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		
        	//std::cout << "hostname Invalid to be checked" << std::endl;
		if (rpcrsp.get_key_exists("hostnameInvalid"))
		// checking If the command is not executed at right card
		{
			std::cout << "hostname Invalid executed" << std::endl;
			return true;
		}

		if (!rpcrsp.get_key_exists("rawLinkStatus"))      return false;
		if (!rpcrsp.get_key_exists("bx0ErrorCount"))      return false;
		if (!rpcrsp.get_key_exists("alignmentMask"))      return false;

	        std::vector<uint32_t> rawLinkStatus = rpcrsp.get_word_array("rawLinkStatus");
		std::vector<uint32_t> alignmentMask = rpcrsp.get_word_array("alignmentMask");
		std::vector<uint32_t> bx0ErrorCount = rpcrsp.get_word_array("bx0ErrorCount");


		if (rawLinkStatus.size() != bx0ErrorCount.size())      return false;
		if (rawLinkStatus.size() != alignmentMask.size())      return false;

                
		for (int i = 0; i < rawLinkStatus.size(); i++)
		{ 

//                    std::cout << "rawlink status=" <<rawLinkStatus[i]<< std::endl;                 
		            if (alignmentMask[i] == 0x1 || ((rawLinkStatus[i] == 0x3) && bx0ErrorCount[i] == 0x0)){
                     
  //                  std::cout << "pushing zero" << std::endl;                 
 					alignStatus.push_back(0);
	                    }
			    
			   else
					alignStatus.push_back(1);
    //                std::cout << "pushing one" << std::endl;                 
				

			   
			// check if we have any bx0 errors or if link is down with non-masked input links
		 }

		for (int i = 0; i < alignStatus.size(); ++i) {
	            if (alignStatus[i] == 1) {
                return false;
        	    }
	        }

        return true;
	}

	RPC_CATCH_RETURN;
}


/* typedef struct LinkStatusSummary {
 *  uint32_t rawLinkStatus;
 *  uint32_t linkUp;
 *  uint32_t linkGotAligned;
 *  uint32_t linkFifoEmpty;
 *  uint32_t linkLocked;
 *  uint32_t checkSumErrorCount;
 *  uint32_t bx0ErrorCount;
 *  uint32_t bx0Latency;
 *  uint32_t alignmentMask;
 *  uint32_t towerMask;
 * } LinkStatusSummary;
 */
/*bool UCT2023SummaryCTP7::getInputLinkStatus(bool negativeEta, std::vector<LinkStatusSummary> &linkSummary)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getInputLinkStatus")
		                    .set_word("negativeEta", negativeEta)
			                              summary.bx0ErrorCount  < 1000 ); // allow certain number of sporadic BX0 errors before declaring link unlocked

			linkSummary.push_back(summary);
		}
		return true;
	}
	RPC_CATCH_RETURN;
}*/










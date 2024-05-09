#include <stdio.h>
#include <stdexcept>
#include <wiscrpcsvc.h>
#include "UCT2016Layer1CTP7.hh"

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

static const std::string module_name = "UCT2016Layer1";
static const std::string module_key = "UCT2016Layer1 v1.1.0";

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

UCT2016Layer1CTP7::UCT2016Layer1CTP7(uint32_t phi, const std::string connect_string, enum ConnectStringType connect_type)
	: phi(phi)
{
	// Find out the correct address to access.

	if (connect_type == CONNECTSTRING_PHIMAPXML) {
		tinyxml2::XMLDocument phiMapDoc;
		if (phiMapDoc.LoadFile(connect_string.c_str()))
			throw std::runtime_error("Unable to read address configuration XML");

		tinyxml2::XMLElement *config_elem = phiMapDoc.FirstChildElement("layer1_hw_config");
		if (!config_elem)
			throw std::runtime_error("Unable to read address configuration XML");

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


UCT2016Layer1CTP7::~UCT2016Layer1CTP7()
{

}

bool UCT2016Layer1CTP7::checkConnection()
{
	try
	{
		this->rpc.call_method(RPCMsg(module_name + ".ping"));
		return true;
	}
	catch (wisc::RPCSvc::RPCException &e)
	{
		//
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

bool UCT2016Layer1CTP7::hardReset(std::string bitstream)
{
	GENERIC_CALL(RPCMsg(module_name + ".hardReset")
	             .set_string("bitstream", bitstream)
	             .set_word("phi", this->phi)
	            );
}

bool UCT2016Layer1CTP7::softReset()
{
	GENERIC_CALL(RPCMsg(module_name + ".softReset"));
}

bool UCT2016Layer1CTP7::setTxPower(bool enabled)
{
	GENERIC_CALL(RPCMsg(module_name + ".setTxPower")
	             .set_word("enabled", enabled)
	            );
}

bool UCT2016Layer1CTP7::configRefClk(void)
{
	GENERIC_CALL(RPCMsg(module_name + ".configRefClk")
	            );
}

bool UCT2016Layer1CTP7::setRunMode(RunMode mode)
{
	GENERIC_CALL(RPCMsg(module_name + ".setRunMode")
	             .set_word("mode", mode)
	            );
}

bool UCT2016Layer1CTP7::getRunMode(RunMode &mode)
{
	RPCMsg rpcreq(module_name + ".getRunMode");
	rpcreq.set_word("mode", mode);

	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(rpcreq);
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (rpcrsp.get_key_exists("result"))
			mode = static_cast<UCT2016Layer1CTP7::RunMode>(rpcrsp.get_word("result"));
		return true;
	}
	RPC_CATCH_RETURN;
}


bool UCT2016Layer1CTP7::getConfiguration(std::string &output)
{
	TYPE_GENERIC_CALL(string, output, RPCMsg(module_name + ".getConfiguration"));
}

bool UCT2016Layer1CTP7::setConfiguration(std::string input)
{
	GENERIC_CALL(RPCMsg(module_name + ".setConfiguration")
	             .set_string("input", input)
	            );
}

bool UCT2016Layer1CTP7::setLinkAlignmentMode(AlignMode mode)
{
	GENERIC_CALL(RPCMsg(module_name + ".setLinkAlignmentMode")
	             .set_word("mode", mode)
	            );
}

bool UCT2016Layer1CTP7::getLinkAlignmentMode(AlignMode &mode)
{
	RPCMsg rpcreq(module_name + ".getLinkAlignmentMode");
	rpcreq.set_word("mode", mode);

	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(rpcreq);
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (rpcrsp.get_key_exists("result"))
			mode = static_cast<UCT2016Layer1CTP7::AlignMode>(rpcrsp.get_word("result"));
		return true;
	}
	RPC_CATCH_RETURN;
}


bool UCT2016Layer1CTP7::alignInputLinks(bool negativeEta, uint32_t bx, uint32_t sub_bx, bool alignManual)
{
	GENERIC_CALL(RPCMsg(module_name + ".alignInputLinks")
	             .set_word("negativeEta", negativeEta)
	             .set_word("bx", bx)
	             .set_word("sub_bx", sub_bx)
	             .set_word("alignManual", alignManual)
	            );
}

bool UCT2016Layer1CTP7::getInputLinkAlignmentStatus(bool negativeEta, uint32_t &alignStatus)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getInputLinkStatus")
		                    .set_word("negativeEta", negativeEta)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("rawLinkStatus"))      return false;
		if (!rpcrsp.get_key_exists("bx0ErrorCount"))      return false;
		if (!rpcrsp.get_key_exists("alignmentMask"))      return false;

		std::vector<uint32_t> rawLinkStatus      = rpcrsp.get_word_array("rawLinkStatus");
		std::vector<uint32_t> bx0ErrorCount      = rpcrsp.get_word_array("bx0ErrorCount");
		std::vector<uint32_t> alignmentMask      = rpcrsp.get_word_array("alignmentMask");

		if (rawLinkStatus.size() != bx0ErrorCount.size())      return false;
		if (rawLinkStatus.size() != alignmentMask.size())      return false;

		alignStatus = 0; // default to no error

		for (int i = 0; i < rawLinkStatus.size(); i++)
		{

			uint32_t linkUp = (rawLinkStatus[i] >> 0) & 1;

			// check if we have any bx0 errors or if link is down with non-masked input links
			if (( alignmentMask[i] == 0 ) && ((bx0ErrorCount[i] != 0) || (linkUp == 0)) )
			{
				alignStatus = 1; // link alignment error
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
bool UCT2016Layer1CTP7::getInputLinkStatus(bool negativeEta, std::vector<LinkStatusSummary> &linkSummary)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getInputLinkStatus")
		                    .set_word("negativeEta", negativeEta)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (!rpcrsp.get_key_exists("rawLinkStatus"))      return false;
		if (!rpcrsp.get_key_exists("checkSumErrorCount")) return false;
		if (!rpcrsp.get_key_exists("bx0ErrorCount"))      return false;
		if (!rpcrsp.get_key_exists("bx0Latency"))         return false;
		if (!rpcrsp.get_key_exists("alignmentMask"))      return false;
		if (!rpcrsp.get_key_exists("towerMask"))          return false;

		std::vector<uint32_t> rawLinkStatus      = rpcrsp.get_word_array("rawLinkStatus");
		std::vector<uint32_t> checkSumErrorCount = rpcrsp.get_word_array("checkSumErrorCount");
		std::vector<uint32_t> bx0ErrorCount      = rpcrsp.get_word_array("bx0ErrorCount");
		std::vector<uint32_t> bx0Latency         = rpcrsp.get_word_array("bx0Latency");
		std::vector<uint32_t> alignmentMask      = rpcrsp.get_word_array("alignmentMask");
		std::vector<uint32_t> towerMask          = rpcrsp.get_word_array("towerMask");

		if (rawLinkStatus.size() != checkSumErrorCount.size()) return false;
		if (rawLinkStatus.size() != bx0ErrorCount.size())      return false;
		if (rawLinkStatus.size() != bx0Latency.size())         return false;
		if (rawLinkStatus.size() != alignmentMask.size())      return false;
		if (rawLinkStatus.size() != towerMask.size())          return false;

		linkSummary.clear();
		for (int i = 0; i < rawLinkStatus.size(); i++)
		{
			LinkStatusSummary summary;

			summary.rawLinkStatus      = rawLinkStatus[i];
			summary.linkUp             = (rawLinkStatus[i] >> 0) & 1;
			summary.linkGotAligned     = ((rawLinkStatus[i]) & 0x3) == 0x3;
			summary.linkFifoEmpty      = (rawLinkStatus[i] >> 2) & 1;
			summary.checkSumErrorCount = checkSumErrorCount[i];
			summary.bx0ErrorCount      = bx0ErrorCount[i];
			summary.bx0Latency         = bx0Latency[i];
			summary.alignmentMask      = alignmentMask[i];
			summary.towerMask          = towerMask[i];

			summary.linkLocked         = (summary.linkUp == 1 &&
			                              summary.bx0ErrorCount  < 1000 ); // allow certain number of sporadic BX0 errors before declaring link unlocked

			linkSummary.push_back(summary);
		}
		return true;
	}
	RPC_CATCH_RETURN;
}

/*
// No link Summary available in FW currently
bool UCT2016Layer1CTP7::getOutputLinkStatus(std::vector<LinkStatusSummary> &linkSummary)
{

}
*/

bool UCT2016Layer1CTP7::captureInputLinks(bool negativeEta, CaptureMode mode)
{
	GENERIC_CALL(RPCMsg(module_name + ".captureInputLinks")
	             .set_word("negativeEta", negativeEta)
	             .set_word("mode", mode)
	            );
}

/*
 * enum CaptureState {
 *  Idle = 0,
 *  Armed = 1,
 *  Capturing = 2,
 *  Done = 3
 * };
 *
 * typedef struct CaptureStatus {
 *  uint16_t localBXId;
 *  uint16_t linkBXId;
 *  CaptureState state;
 * } CaptureStatus;
 */

bool UCT2016Layer1CTP7::getInputLinkCaptureStatus(bool negativeEta, std::vector<CaptureStatus> &c)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getInputLinkCaptureStatus")
		                    .set_word("negativeEta", negativeEta)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (!rpcrsp.get_key_exists("status"))
			return false;

		std::vector<uint32_t> status = rpcrsp.get_word_array("status");

		c.clear();
		for (std::vector<uint32_t>::iterator it = status.begin(); it != status.end(); it++)
		{
			CaptureStatus status;
			status.localBXId = (*it >> 4) & 0x0fff;
			status.linkBXId  = (*it >> 16) & 0x0fff;
			status.state     = static_cast<enum CaptureState>(*it & 0x3);
			c.push_back(status);
		}
		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2016Layer1CTP7::setInputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> c)
{
	if (c.size() != 32)
	{
		return false;
	}
	std::vector<uint32_t> captureBx;

	captureBx.clear();
	for (std::vector<CaptureBX>::iterator it = c.begin(); it != c.end(); it++)
	{
		uint32_t bx, sub_bx, internal_bx;
		bx     =  (*it).bx;
		sub_bx =  (*it).sub_bx;

		if (bx > 3563 || sub_bx > 5)
		{
			return false;
		}

		internal_bx = (bx & 0xFFF) + ((sub_bx & 7 ) << 12 );
		captureBx.push_back(internal_bx);
	}

	GENERIC_CALL(RPCMsg(module_name + ".setInputLinkCaptureBX")
	             .set_word("negativeEta", negativeEta)
	             .set_word_array("captureBx", captureBx)
	            );
}

bool UCT2016Layer1CTP7::getInputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> &c)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getInputLinkCaptureBX")
		                    .set_word("negativeEta", negativeEta)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (!rpcrsp.get_key_exists("captureBx"))
			return false;

		std::vector<uint32_t> internalCaptureBx = rpcrsp.get_word_array("captureBx");

		c.clear();
		for (std::vector<uint32_t>::iterator it = internalCaptureBx.begin(); it != internalCaptureBx.end(); it++)
		{
			CaptureBX captureBx;

			captureBx.bx      = (*it) & 0x0fff;
			captureBx.sub_bx  = (*it >> 12) & 0x7;
			c.push_back(captureBx);
		}
		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2016Layer1CTP7::captureOutputLinks(bool negativeEta, CaptureMode mode)
{
	GENERIC_CALL(RPCMsg(module_name + ".captureOutputLinks")
	             .set_word("negativeEta", negativeEta)
	             .set_word("mode", mode)
	            );
}

bool UCT2016Layer1CTP7::getOutputLinkCaptureStatus(bool negativeEta, std::vector<CaptureStatus> &c)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getOutputLinkCaptureStatus")
		                    .set_word("negativeEta", negativeEta)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (!rpcrsp.get_key_exists("status"))
			return false;

		std::vector<uint32_t> status = rpcrsp.get_word_array("status");

		c.clear();
		for (std::vector<uint32_t>::iterator it = status.begin(); it != status.end(); it++)
		{
			CaptureStatus status;
			status.localBXId = (*it >> 4) & 0x0fff;
			status.linkBXId  = (*it >> 16) & 0x0fff;
			status.state     = static_cast<enum CaptureState> ( (*it >> 2) & 0x3);
			c.push_back(status);
		}
		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2016Layer1CTP7::setOutputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> c)
{
	if (c.size() != 24)
	{
		return false;
	}
	std::vector<uint32_t> captureBx;

	captureBx.clear();
	for (std::vector<CaptureBX>::iterator it = c.begin(); it != c.end(); it++)
	{
		uint32_t bx, sub_bx, internal_bx;
		bx     =  (*it).bx;
		sub_bx =  (*it).sub_bx;

		if (bx > 3563 || sub_bx > 5)
		{
			return false;
		}

		internal_bx = (bx & 0xFFF) + ((sub_bx & 7 ) << 12 );
		captureBx.push_back(internal_bx);
	}

	GENERIC_CALL(RPCMsg(module_name + ".setOutputLinkCaptureBX")
	             .set_word("negativeEta", negativeEta)
	             .set_word_array("captureBx", captureBx)
	            );
}

bool UCT2016Layer1CTP7::getOutputLinkCaptureBX(bool negativeEta, std::vector<CaptureBX> &c)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getOutputLinkCaptureBX")
		                    .set_word("negativeEta", negativeEta)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;
		if (!rpcrsp.get_key_exists("captureBx"))
			return false;

		std::vector<uint32_t> internalCaptureBx = rpcrsp.get_word_array("captureBx");

		c.clear();
		for (std::vector<uint32_t>::iterator it = internalCaptureBx.begin(); it != internalCaptureBx.end(); it++)
		{
			CaptureBX captureBx;

			captureBx.bx      = (*it) & 0x0fff;
			captureBx.sub_bx  = (*it >> 12) & 0x7;
			c.push_back(captureBx);
		}
		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2016Layer1CTP7::captureSyncdInputOutputLinks(bool negativeEta, CaptureSyncd mode)
{
	GENERIC_CALL(RPCMsg(module_name + ".captureSyncdInputOutputLinks")
	             .set_word("negativeEta", negativeEta)
	             .set_word("waitForSyncOrbitCnt", mode.waitForSyncOrbitCnt)
	             .set_word("syncOrbitCnt", mode.syncOrbitCnt)
	             .set_word("bx", mode.bx)
	            );
}

bool UCT2016Layer1CTP7::resetInputLinkDecoders(bool negativeEta)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetInputLinkDecoders")
	             .set_word("negativeEta", negativeEta)
	            );
}

bool UCT2016Layer1CTP7::resetInputLinkChecksumErrorCounters(bool negativeEta)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetInputLinkChecksumErrorCounters")
	             .set_word("negativeEta", negativeEta)
	            );
}

bool UCT2016Layer1CTP7::resetInputLinkBX0ErrorCounters(bool negativeEta)
{
	GENERIC_CALL(RPCMsg(module_name + ".resetInputLinkBX0ErrorCounters")
	             .set_word("negativeEta", negativeEta)
	            );
}

bool UCT2016Layer1CTP7::setInputLinkAlignmentMask(bool negativeEta, std::vector<uint32_t> mask)
{
	GENERIC_CALL(RPCMsg(module_name + ".setInputLinkAlignmentMask")
	             .set_word("negativeEta", negativeEta)
	             .set_word_array("mask", mask)
	            );
}

bool UCT2016Layer1CTP7::getInputLinkAlignmentMask(bool negativeEta, std::vector<uint32_t> & mask)
{
	TYPE_GENERIC_CALL(word_array, mask,
	                  RPCMsg(module_name + ".getInputLinkAlignmentMask")
	                  .set_word("negativeEta", negativeEta)
	                 );
}

bool UCT2016Layer1CTP7::setInputLinkTowerMask(bool negativeEta, std::vector<uint32_t> mask)
{
	GENERIC_CALL(RPCMsg(module_name + ".setInputLinkTowerMask")
	             .set_word("negativeEta", negativeEta)
	             .set_word_array("mask", mask)
	            );
}

bool UCT2016Layer1CTP7::getInputLinkTowerMask(bool negativeEta, std::vector<uint32_t> & mask)
{
	TYPE_GENERIC_CALL(word_array, mask,
	                  RPCMsg(module_name + ".getInputLinkTowerMask")
	                  .set_word("negativeEta", negativeEta)
	                 );
}

bool UCT2016Layer1CTP7::getInputLinkBuffer(bool negativeEta, InputLink link, std::vector<uint32_t> &selectedData)
{
	TYPE_GENERIC_CALL(word_array, selectedData,
	                  RPCMsg(module_name + ".getInputLinkBuffer")
	                  .set_word("negativeEta", negativeEta)
	                  .set_word("link", link)
	                 );
}

bool UCT2016Layer1CTP7::setInputLinkBuffer(bool negativeEta, InputLink link, const std::vector<uint32_t> &selectedData)
{
	GENERIC_CALL(RPCMsg(module_name + ".setInputLinkBuffer")
	             .set_word("negativeEta", negativeEta)
	             .set_word("link", link)
	             .set_word_array("selectedData", selectedData)
	            );
}


bool UCT2016Layer1CTP7::getOutputLinkBuffer(bool negativeEta, OutputLink link, std::vector<uint32_t> &data)
{
	TYPE_GENERIC_CALL(word_array, data,
	                  RPCMsg(module_name + ".getOutputLinkBuffer")
	                  .set_word("negativeEta", negativeEta)
	                  .set_word("link", link)
	                 );
}

bool UCT2016Layer1CTP7::setOutputLinkBuffer(bool negativeEta, OutputLink link, const std::vector<uint32_t> &data)
{
	GENERIC_CALL(RPCMsg(module_name + ".setOutputLinkBuffer")
	             .set_word("negativeEta", negativeEta)
	             .set_word("link", link)
	             .set_word_array("data", data)
	            );
}

bool UCT2016Layer1CTP7::getInputLinkLUT(bool negativeEta, LUTType type, LUTiEtaIndex iEta, std::vector<uint32_t> &lut)
{
	TYPE_GENERIC_CALL(word_array, lut,
	                  RPCMsg(module_name + ".getInputLinkLUT")
	                  .set_word("negativeEta", negativeEta)
	                  .set_word("type", type)
	                  .set_word("iEta", iEta)
	                 );
}

bool UCT2016Layer1CTP7::setInputLinkLUT(bool negativeEta, LUTType type, LUTiEtaIndex iEta, const std::vector<uint32_t> &lut)
{
	GENERIC_CALL(RPCMsg(module_name + ".setInputLinkLUT")
	             .set_word("negativeEta", negativeEta)
	             .set_word("type", type)
	             .set_word("iEta", iEta)
	             .set_word_array("lut", lut)
	            );
}
bool UCT2016Layer1CTP7::getInputLinkLUTHcalFb(bool negativeEta, LUTiEtaIndex iEta, std::vector<uint32_t> &lut)
{
	TYPE_GENERIC_CALL(word_array, lut,
	                  RPCMsg(module_name + ".getInputLinkLUTHcalFb")
	                  .set_word("negativeEta", negativeEta)
	                  .set_word("iEta", iEta)
	                 );
}

bool UCT2016Layer1CTP7::setInputLinkLUTHcalFb(bool negativeEta, LUTiEtaIndex iEta, const std::vector<uint32_t> &lut)
{
	GENERIC_CALL(RPCMsg(module_name + ".setInputLinkLUTHcalFb")
	             .set_word("negativeEta", negativeEta)
	             .set_word("iEta", iEta)
	             .set_word_array("lut", lut)
	            );
}

bool UCT2016Layer1CTP7::setOutputLinkPattern(bool negativeEta, const std::vector<uint32_t> value)
{
	GENERIC_CALL(RPCMsg(module_name + ".setOutputLinkPattern")
	             .set_word("negativeEta", negativeEta)
	             .set_word_array("value", value)
	            );
}

bool UCT2016Layer1CTP7::getOutputLinkPattern(bool negativeEta, std::vector<uint32_t> &value)
{
	TYPE_GENERIC_CALL(word_array, value,
	                  RPCMsg(module_name + ".getOutputLinkPattern")
	                  .set_word("negativeEta", negativeEta)
	                 );
}

bool UCT2016Layer1CTP7::setOutputLinkID(bool negativeEta, const std::vector<uint32_t> value)
{
	GENERIC_CALL(RPCMsg(module_name + ".setOutputLinkID")
	             .set_word("negativeEta", negativeEta)
	             .set_word_array("value", value)
	            );
}

bool UCT2016Layer1CTP7::getOutputLinkID(bool negativeEta, std::vector<uint32_t> &value)
{
	TYPE_GENERIC_CALL(word_array, value,
	                  RPCMsg(module_name + ".getOutputLinkID")
	                  .set_word("negativeEta", negativeEta)
	                 );
}

bool UCT2016Layer1CTP7::setTMTCycle(bool negativeEta, OutputTMTLinkPair olPair, uint32_t value)
{
	GENERIC_CALL(RPCMsg(module_name + ".setTMTCycle")
	             .set_word("negativeEta", negativeEta)
	             .set_word("olPair", olPair)
	             .set_word("value", value)
	            );
}

bool UCT2016Layer1CTP7::getTMTCycle(bool negativeEta, OutputTMTLinkPair olPair, uint32_t &value)
{
	WORD_GENERIC_CALL(value, RPCMsg(module_name + ".getTMTCycle")
	                  .set_word("negativeEta", negativeEta)
	                  .set_word("olPair", olPair)
	                 );
}

bool UCT2016Layer1CTP7::alignOutputLinks(bool negativeEta, uint32_t bx,  uint32_t sub_bx)
{
	GENERIC_CALL(RPCMsg(module_name + ".alignOutputLinks")
	             .set_word("negativeEta", negativeEta)
	             .set_word("bx", bx)
	             .set_word("sub_bx", sub_bx)
	            );
}


bool UCT2016Layer1CTP7::alignTTCDecoder(void)
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
bool UCT2016Layer1CTP7::getTTCStatus(TTCStatus &ttcStatus)
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

/* typedef struct TTCBGoCmdCnt {
 *      uint32_t L1A;
 *  uint32_t BX0;
 *  uint32_t EC0;
 *  uint32_t Resync;
 *  uint32_t OC0;
 *  uint32_t TestSync;
 *  uint32_t Start;
 *  uint32_t Stop;
 * } TTCBGoCmdCnt;
 */

bool UCT2016Layer1CTP7::getTTCBGoCmdCnt(TTCBGoCmdCnt &ttcBGoCmdCnt)
{
	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getTTCBGoCmdCnt")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("L1A"))         return false;
		if (!rpcrsp.get_key_exists("BX0"))         return false;
		if (!rpcrsp.get_key_exists("EC0"))         return false;
		if (!rpcrsp.get_key_exists("Resync"))      return false;
		if (!rpcrsp.get_key_exists("OC0"))         return false;
		if (!rpcrsp.get_key_exists("TestSync"))    return false;
		if (!rpcrsp.get_key_exists("Start"))       return false;
		if (!rpcrsp.get_key_exists("Stop"))        return false;
		if (!rpcrsp.get_key_exists("Orbit"))       return false;
		if (!rpcrsp.get_key_exists("L1ID"))        return false;

		ttcBGoCmdCnt.L1A         = rpcrsp.get_word("L1A");
		ttcBGoCmdCnt.BX0         = rpcrsp.get_word("BX0");
		ttcBGoCmdCnt.EC0         = rpcrsp.get_word("EC0");
		ttcBGoCmdCnt.Resync      = rpcrsp.get_word("Resync");
		ttcBGoCmdCnt.OC0         = rpcrsp.get_word("OC0");
		ttcBGoCmdCnt.TestSync    = rpcrsp.get_word("TestSync");
		ttcBGoCmdCnt.Start       = rpcrsp.get_word("Start");
		ttcBGoCmdCnt.Stop        = rpcrsp.get_word("Stop");
		ttcBGoCmdCnt.Orbit        = rpcrsp.get_word("Orbit");
		ttcBGoCmdCnt.L1ID        = rpcrsp.get_word("L1ID");

		return true;
	}
	RPC_CATCH_RETURN;
}


/*  typedef struct DAQConfig
 *  {
 *      uint32_t DAQDelayLineDepth;
 *  } DAQConfig;
 */

bool UCT2016Layer1CTP7::setDAQConfig(DAQConfig daqConfig)
{
	GENERIC_CALL(RPCMsg(module_name + ".setDAQConfig")
	             .set_word("DAQDelayLineDepth", daqConfig.DAQDelayLineDepth)
	            );
}


bool UCT2016Layer1CTP7::getDAQConfig(DAQConfig &daqConfig)
{

	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getDAQConfig")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("DAQDelayLineDepth"))    return false;

		daqConfig.DAQDelayLineDepth    = rpcrsp.get_word("DAQDelayLineDepth");

		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2016Layer1CTP7::setRunNumber(uint32_t runNumber)
{
	GENERIC_CALL(RPCMsg(module_name + ".setRunNumber")
	             .set_word("runNumber", runNumber)
	            );
}

bool UCT2016Layer1CTP7::getRunNumber(uint32_t &runNumber)
{

	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getRunNumber")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("runNumber"))    return false;

		runNumber = rpcrsp.get_word("runNumber");

		return true;
	}
	RPC_CATCH_RETURN;
}


/*
*   typedef struct DAQStatus
*   {
*       uint32_t fifoOcc;
*       uint32_t fifoOccMax;
*       uint32_t fifoEmpty;
*       uint32_t CTP77ToAMC13BP;
*       uint32_t AMC13ToCTP7BP;
*       uint32_t TTS;
*       uint32_t internalError;
*       uint32_t amcCoreReady;
*   } DAQStatus;
*/


//	virtual bool setDAQStatusReset(void);
bool UCT2016Layer1CTP7::setDAQStatusReset(void)
{
	GENERIC_CALL(RPCMsg(module_name + ".setDAQStatusReset")
	            );
}

//	virtual bool getDAQStatus(DAQStatus &daqStatus);
bool UCT2016Layer1CTP7::getDAQStatus(DAQStatus &daqStatus)
{

	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getDAQStatus")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("fifoOcc"))          return false;
		if (!rpcrsp.get_key_exists("fifoOccMax"))       return false;
		if (!rpcrsp.get_key_exists("fifoEmpty"))        return false;
		if (!rpcrsp.get_key_exists("CTP77ToAMC13BP"))   return false;
		if (!rpcrsp.get_key_exists("AMC13ToCTP7BP"))    return false;
		if (!rpcrsp.get_key_exists("TTS"))              return false;
		if (!rpcrsp.get_key_exists("internalError"))    return false;
		if (!rpcrsp.get_key_exists("amcCoreReady"))     return false;

		daqStatus.fifoOcc        = rpcrsp.get_word("fifoOcc");
		daqStatus.fifoOccMax     = rpcrsp.get_word("fifoOccMax");
		daqStatus.fifoEmpty      = rpcrsp.get_word("fifoEmpty");
		daqStatus.CTP77ToAMC13BP = rpcrsp.get_word("CTP77ToAMC13BP");
		daqStatus.AMC13ToCTP7BP  = rpcrsp.get_word("AMC13ToCTP7BP");
		daqStatus.TTS            = rpcrsp.get_word("TTS");
		daqStatus.internalError  = rpcrsp.get_word("internalError");
		daqStatus.amcCoreReady   = rpcrsp.get_word("amcCoreReady");

		return true;
	}
	RPC_CATCH_RETURN;
}

/*
 *  typedef struct FWInfo
 *  {
 *      uint32_t buildTimestamp;
 *      uint32_t gitHashCode;
 *      uint32_t gitHashDirty;
 *      uint32_t version;
 *      uint32_t projectCode;
 *      uint32_t uptime;
 *  } FWInfo;
*/

bool UCT2016Layer1CTP7::getFWInfo(FWInfo &fwInfo)
{

	if (!this->checkConnection())
		return false;
	try
	{
		RPCMsg rpcrsp = this->rpc.call_method(
		                    RPCMsg(module_name + ".getFWInfo")
		                    .set_word("dummy", 0)
		                );
		if (rpcrsp.get_key_exists("error"))
			return false;

		if (!rpcrsp.get_key_exists("buildTimestamp"))    return false;
		if (!rpcrsp.get_key_exists("gitHashCode"))       return false;
		if (!rpcrsp.get_key_exists("gitHashDirty"))      return false;
		if (!rpcrsp.get_key_exists("version"))           return false;
		if (!rpcrsp.get_key_exists("projectCode"))       return false;
		if (!rpcrsp.get_key_exists("uptime"))            return false;

		fwInfo.buildTimestamp    = rpcrsp.get_word("buildTimestamp");
		fwInfo.gitHashCode       = rpcrsp.get_word("gitHashCode");
		fwInfo.gitHashDirty      = rpcrsp.get_word("gitHashDirty");
		fwInfo.version           = rpcrsp.get_word("version");
		fwInfo.projectCode       = rpcrsp.get_word("projectCode");
		fwInfo.uptime            = rpcrsp.get_word("uptime");

		return true;
	}
	RPC_CATCH_RETURN;
}

bool UCT2016Layer1CTP7::getModuleBuildInfo(std::string &value)
{
	TYPE_GENERIC_CALL(string, value, RPCMsg(module_name + ".getModuleBuildInfo"));
}

bool UCT2016Layer1CTP7::getInputLinkLUT2S(bool negativeEta, LUTiEtaIndex iEta, std::vector<uint32_t> &lut)
{
  TYPE_GENERIC_CALL(word_array, lut,
                    RPCMsg(module_name + ".getInputLinkLUT2S")
                    .set_word("negativeEta", negativeEta)
                    .set_word("iEta", iEta)
                    );
}

bool UCT2016Layer1CTP7::setInputLinkLUT2S(bool negativeEta, LUTiEtaIndex iEta, const std::vector<uint32_t> &lut)
{
  GENERIC_CALL(RPCMsg(module_name + ".setInputLinkLUT2S")
               .set_word("negativeEta", negativeEta)
               .set_word("iEta", iEta)
               .set_word_array("lut", lut)
               );
}

#ifndef _calol1_ctp7_CTP7LinkInterface_h_
#define _calol1_ctp7_CTP7LinkInterface_h_

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// CTP7LinkInterface                                                       //
//                                                                         //
// Recieves metric values from CTP7 client and distributes them to         //
// link SWATCH objects, passes commands from the links to the CTP7         //
// when necessary.                                                         //
//                                                                         //
// Original author: Nate Woods, U. Wisconsin                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "swatch/processor/PortCollection.hpp"

#include "UCT2016Layer1CTP7.hh"

#include "CTP7Input.h"
#include "CTP7Output.h"

#include <vector>

namespace calol1
{
  typedef UCT2016Layer1CTP7::InputLink InputLink;
  typedef UCT2016Layer1CTP7::OutputLink OutputLink;
  typedef UCT2016Layer1CTP7::LinkStatusSummary LinkStatus;

  class CTP7InputLinkInterface : public swatch::processor::InputPortCollection
    {
    public:
      CTP7InputLinkInterface(UCT2016Layer1CTP7* const client);
      ~CTP7InputLinkInterface();

      // do the actual work of retrieveMetricValues(). Here so we can call it 
      // without mutexes and whatnot
      void updateLinkStatuses();

    protected:
      /// Does not have any metric values of its own; instead, this method
      /// retrieves link data from the CTP7 client and distributes it to the
      /// links, which can set the metrics appropriately
      void retrieveMetricValues();

    private:
      // no copy constructor
      CTP7InputLinkInterface(const CTP7InputLinkInterface&);
      // no assignment operator
      const CTP7InputLinkInterface& operator=(const CTP7InputLinkInterface&);

      // tell all links about their status
      void distributeLinkStatuses(const std::vector<LinkStatus>& inputStatusPos,
                                  const std::vector<LinkStatus>& inputStatusNeg);

      UCT2016Layer1CTP7* const client_;

    };


  class CTP7OutputLinkInterface : public swatch::processor::OutputPortCollection
    {
    public:
      CTP7OutputLinkInterface(UCT2016Layer1CTP7* const client);
      ~CTP7OutputLinkInterface();

      // do the actual work of retrieveMetricValues(). Here so we can call it 
      // without mutexes and whatnot
      void updateLinkStatuses();

    protected:
      /// Does not have any metric values of its own; instead, this method
      /// retrieves link data from the CTP7 client and distributes it to the
      /// links, which can set the metrics appropriately
      void retrieveMetricValues();

    private:
      // no copy constructor
      CTP7OutputLinkInterface(const CTP7OutputLinkInterface&);
      // no assignment operator
      const CTP7OutputLinkInterface& operator=(const CTP7OutputLinkInterface&);

      // tell all links about their status
      void distributeLinkStatuses(const std::vector<LinkStatus>& inputStatusPos,
                                  const std::vector<LinkStatus>& inputStatusNeg);

      UCT2016Layer1CTP7* const client_;

    };
}



#endif // header guard

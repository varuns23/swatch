#ifndef _calol1_ctp7_CicadaLinkInterface_h_
#define _calol1_ctp7_CicadaLinkInterface_h_


#include "swatch/processor/PortCollection.hpp"

#include "UCT2023SummaryCTP7.hh"

#include "CicadaInput.h"
#include "CicadaOutput.h"

#include <vector>

//namespace calol1
//{

namespace swatch {
namespace dummy {

  class CicadaInputLinkInterface : public swatch::processor::InputPortCollection
    {
    public:
      CicadaInputLinkInterface(UCT2023SummaryCTP7* const client);
      ~CicadaInputLinkInterface();


    protected:
      /// Does not have any metric values of its own; instead, this method
      /// retrieves link data from the CTP7 client and distributes it to the
      /// links, which can set the metrics appropriately
      void retrieveMetricValues();

    private:
      // no copy constructor
      CicadaInputLinkInterface(const CicadaInputLinkInterface&);
      // no assignment operator
      const CicadaInputLinkInterface& operator=(const CicadaInputLinkInterface&);

      UCT2023SummaryCTP7* const client_;

    };


  class CicadaOutputLinkInterface : public swatch::processor::OutputPortCollection
    {
    public:
      CicadaOutputLinkInterface(UCT2023SummaryCTP7* const client);
      ~CicadaOutputLinkInterface();


    protected:
      /// Does not have any metric values of its own; instead, this method
      /// retrieves link data from the Cicada client and distributes it to the
      /// links, which can set the metrics appropriately
      void retrieveMetricValues();

    private:
      // no copy constructor
      CicadaOutputLinkInterface(const CicadaOutputLinkInterface&);
      // no assignment operator
      const CicadaOutputLinkInterface& operator=(const CicadaOutputLinkInterface&);

      UCT2023SummaryCTP7* const client_;

    };
}}



#endif // header guard

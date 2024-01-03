#ifndef ASAPP_INCLUDE_ASAPP_STRUCTURES_DEDICATEDSTORAGE_H_
#define ASAPP_INCLUDE_ASAPP_STRUCTURES_DEDICATEDSTORAGE_H_

#include "container.h"
#include "../interfaces/baseinventory.h"

namespace asa::structures
{
    class DedicatedStorage final : public Container
    {
       public:
        DedicatedStorage() : Container("Dedicated Storage", 1800) {};
      
        bool can_deposit();
    };
}

#endif //ASAPP_INCLUDE_ASAPP_STRUCTURES_DEDICATEDSTORAGE_H_

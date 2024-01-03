
#include "asapp/structures/dedicatedstorage.h"

namespace asa::structures {
  bool DedicatedStorage::can_deposit() 
  {
    return window::match_template(window::screenshot(),
                                  resources::interfaces::deposit);
  }
}

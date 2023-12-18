#include "asapp/entities/baseentity.h"

asa::entities::BaseEntity::BaseEntity(std::string t_name,
                                      std::unique_ptr<interfaces::BaseInventory>
                                      t_inventory) : name(t_name),
                                                     inventory(t_inventory
                                                         ? std::move(t_inventory)
                                                         : std::make_unique<
                                                             interfaces::BaseInventory>(
                                                             true)) {};

#pragma once
#include "item.h"
#include "exceptions.h"

namespace asa
{
    void load_items();

    const item& get_item(const std::string& name);

    const std::map<std::string, std::unique_ptr<item> >& get_all_items();
}
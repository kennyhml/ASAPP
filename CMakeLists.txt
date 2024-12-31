cmake_minimum_required(VERSION 3.25)
project(asapp)

execute_process(COMMAND python embed.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_library(asapp STATIC
        src/core/exceptions.cpp
        src/core/init.cpp
        src/core/state.cpp
        src/entities/baseentity.cpp
        src/entities/exceptions.cpp
        src/entities/localplayer.cpp
        src/interfaces/info/baseentitiyinfo.cpp
        src/game/settings.cpp
        src/game/window.cpp
        src/interfaces/wheels/baseactionwheel.cpp
        src/interfaces/inventories/baseinventory.cpp
        src/interfaces/maps/basetravelmap.cpp
        src/interfaces/components/button.cpp
        src/interfaces/components/search_bar.cpp
        src/interfaces/components/slot.cpp
        src/interfaces/console.cpp
        src/interfaces/inventories/craftinginventory.cpp
        src/interfaces/info/dedicatedstorageinfo.cpp
        src/interfaces/exceptions.cpp
        src/interfaces/hud.cpp
        src/interfaces/inventories/localinventory.cpp
        src/interfaces/main_menu.cpp
        src/interfaces/mode_select.cpp
        src/interfaces/info/playerinfo.cpp
        src/interfaces/server_select.cpp
        src/interfaces/maps/spawnmap.cpp
        src/interfaces/info/structureinfo.cpp
        src/interfaces/maps/teleportmap.cpp
        src/interfaces/maps/travelmap.cpp
        src/interfaces/tribe_manager.cpp
        src/items/item.cpp
        src/items/itemdata.cpp
        src/structures/cavelootcreate.cpp
        src/structures/container.cpp
        src/structures/crafting_station.cpp
        src/structures/dedicated_storage.cpp
        src/structures/exceptions.cpp
        src/utility.cpp
        include/asa/core/exceptions.h
        include/asa/core/init.h
        include/asa/core/state.h
        include/asa/entities/baseentity.h
        include/asa/entities/dinoentity.h
        include/asa/entities/entities.h
        include/asa/entities/exceptions.h
        include/asa/entities/localplayer.h
        include/asa/game/settings.h
        include/asa/game/window.h
        include/asa/ui/wheels/baseactionwheel.h
        include/asa/ui/info/baseentityinfo.h
        include/asa/ui/info/baseinfo.h
        include/asa/ui/storage/baseinventory.h
        include/asa/ui/maps/basetravelmap.h
        include/asa/ui/components/button.h
        include/asa/ui/components/combo_box.h
        include/asa/ui/components/interface_component.h
        include/asa/ui/components/search_bar.h
        include/asa/ui/components/slot.h
        include/asa/ui/components/tooltip.h
        include/asa/ui/console.h
        include/asa/ui/info/containerinfo.h
        include/asa/ui/storage/craftinginventory.h
        include/asa/ui/info/dedicatedstorageinfo.h
        include/asa/ui/info/dinoinfo.h
        include/asa/ui/exceptions.h
        include/asa/ui/hud.h
        include/asa/ui/asainterface.h
        include/asa/ui/interfaces.h
        include/asa/ui/storage/localinventory.h
        include/asa/ui/main_menu.h
        include/asa/ui/menu.h
        include/asa/ui/mode_select.h
        include/asa/ui/info/playerinfo.h
        include/asa/ui/server_select.h
        include/asa/ui/maps/spawnmap.h
        include/asa/ui/maps/teleportmap.h
        include/asa/ui/maps/travelmap.h
        include/asa/ui/tribe_manager.h
        include/asa/items/item.h
        include/asa/items/itemdata.h
        include/asa/structures/basestructure.h
        include/asa/structures/cave_loot_crate.h
        include/asa/structures/container.h
        include/asa/structures/crafting_station.h
        include/asa/structures/dedicated_storage.h
        include/asa/structures/exceptions.h
        include/asa/structures/interactable.h
        include/asa/structures/simple_bed.h
        include/asa/structures/teleporter.h
        include/asa/utility.h
        include/asa/ui/components/tribelog_message.h
        src/interfaces/components/tribelog_message.cpp
        include/asa/network/server.h
        include/asa/network/queries.h
        src/network/server.cpp
        src/network/queries.cpp
        src/interfaces/components/tooltip.cpp
        src/interfaces/info/dinoinfo.cpp
        include/asa/ui/storage/dinoinventory.h
        src/interfaces/inventories/dinoinventory.cpp
        src/interfaces/menu.cpp
        include/asa/ui/storage/tribute_inventory.h
        src/interfaces/inventories/tributeinventory.cpp
        include/asa/structures/tributeable.h
        src/structures/tributable.cpp
        include/asa/game/game.h
        include/asa/ui/components/components.h
        include/asa/ui/maps/exceptions.h
        include/asa/ui/info/infos.h
        include/asa/ui/storage/inventories.h
        include/asa/ui/maps/maps.h
        include/asa/core/managedthread.h
        src/core/managedthread.cpp
        include/asa/structures/structures.h
        include/asa/game/embedded.h
        include/asa/items/exceptions.h
        include/asa/items/items.h
        src/items/items.cpp
        include/asa/core/logging.h
        src/core/logging.cpp
        include/asa/game/exceptions.h
)

set_target_properties(asapp PROPERTIES
        CXX_STANDARD 23
        CXX_EXTENSIONS OFF
)

target_include_directories(asapp PUBLIC include)
target_include_directories(asapp PRIVATE src)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(asapp PUBLIC spdlog::spdlog)

find_package(Tesseract CONFIG REQUIRED)
target_link_libraries(asapp PRIVATE Tesseract::libtesseract)

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(asapp PRIVATE nlohmann_json::nlohmann_json)

find_package(Boost QUIET REQUIRED COMPONENTS thread)
target_link_libraries(asapp PUBLIC Boost::thread)
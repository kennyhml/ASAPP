cmake_minimum_required(VERSION 3.25)
project(asapp)

add_library(asapp STATIC
        ASAPP/src/core/exceptions.cpp
        ASAPP/src/core/init.cpp
        ASAPP/src/core/state.cpp
        ASAPP/src/entities/baseentity.cpp
        ASAPP/src/entities/exceptions.cpp
        ASAPP/src/entities/localplayer.cpp
        ASAPP/src/interfaces/baseentitiyinfo.cpp
        ASAPP/src/game/controls.cpp
        ASAPP/src/game/resources.cpp
        ASAPP/src/game/settings.cpp
        ASAPP/src/game/window.cpp
        ASAPP/src/interfaces/actionwheel.cpp
        ASAPP/src/interfaces/baseinventory.cpp
        ASAPP/src/interfaces/base_travel_map.cpp
        ASAPP/src/interfaces/components/button.cpp
        ASAPP/src/interfaces/components/search_bar.cpp
        ASAPP/src/interfaces/components/slot.cpp
        ASAPP/src/interfaces/console.cpp
        ASAPP/src/interfaces/craftinginventory.cpp
        ASAPP/src/interfaces/dedicated_storage_info.cpp
        ASAPP/src/interfaces/exceptions.cpp
        ASAPP/src/interfaces/hud.cpp
        ASAPP/src/interfaces/interfacemanager.cpp
        ASAPP/src/interfaces/localinventory.cpp
        ASAPP/src/interfaces/main_menu.cpp
        ASAPP/src/interfaces/mode_select.cpp
        ASAPP/src/interfaces/player_info.cpp
        ASAPP/src/interfaces/server_select.cpp
        ASAPP/src/interfaces/spawnmap.cpp
        ASAPP/src/interfaces/structureinfo.cpp
        ASAPP/src/interfaces/teleportmap.cpp
        ASAPP/src/interfaces/travelmap.cpp
        ASAPP/src/interfaces/tribe_manager.cpp
        ASAPP/src/items/exceptions.cpp
        ASAPP/src/items/item.cpp
        ASAPP/src/items/itemdata.cpp
        ASAPP/src/items/items.cpp
        ASAPP/src/structures/cavelootcreate.cpp
        ASAPP/src/structures/container.cpp
        ASAPP/src/structures/crafting_station.cpp
        ASAPP/src/structures/dedicated_storage.cpp
        ASAPP/src/structures/exceptions.cpp
        ASAPP/src/util/util.cpp
        ASAPP/include/asa/core/exceptions.h
        ASAPP/include/asa/core/init.h
        ASAPP/include/asa/core/state.h
        ASAPP/include/asa/entities/baseentity.h
        ASAPP/include/asa/entities/dinoentity.h
        ASAPP/include/asa/entities/entities.h
        ASAPP/include/asa/entities/exceptions.h
        ASAPP/include/asa/entities/localplayer.h
        ASAPP/include/asa/game/controls.h
        ASAPP/include/asa/game/resources.h
        ASAPP/include/asa/game/settings.h
        ASAPP/include/asa/game/window.h
        ASAPP/include/asa/interfaces/wheels/baseactionwheel.h
        ASAPP/include/asa/interfaces/info/baseentityinfo.h
        ASAPP/include/asa/interfaces/info/baseinfo.h
        ASAPP/include/asa/interfaces/inventories/baseinventory.h
        ASAPP/include/asa/interfaces/maps/basetravelmap.h
        ASAPP/include/asa/interfaces/components/button.h
        ASAPP/include/asa/interfaces/components/combo_box.h
        ASAPP/include/asa/interfaces/components/interface_component.h
        ASAPP/include/asa/interfaces/components/search_bar.h
        ASAPP/include/asa/interfaces/components/slot.h
        ASAPP/include/asa/interfaces/components/tooltip.h
        ASAPP/include/asa/interfaces/console.h
        ASAPP/include/asa/interfaces/info/containerinfo.h
        ASAPP/include/asa/interfaces/inventories/crafting_inventory.h
        ASAPP/include/asa/interfaces/info/dedicated_storage_info.h
        ASAPP/include/asa/interfaces/info/dino_info.h
        ASAPP/include/asa/interfaces/exceptions.h
        ASAPP/include/asa/interfaces/hud.h
        ASAPP/include/asa/interfaces/interface.h
        ASAPP/include/asa/interfaces/interfaces.h
        ASAPP/include/asa/interfaces/inventories/localinventory.h
        ASAPP/include/asa/interfaces/main_menu.h
        ASAPP/include/asa/interfaces/menu.h
        ASAPP/include/asa/interfaces/mode_select.h
        ASAPP/include/asa/interfaces/info/player_info.h
        ASAPP/include/asa/interfaces/server_select.h
        ASAPP/include/asa/interfaces/maps/spawn_map.h
        ASAPP/include/asa/interfaces/maps/teleport_map.h
        ASAPP/include/asa/interfaces/maps/travel_map.h
        ASAPP/include/asa/interfaces/tribe_manager.h
        ASAPP/include/asa/items/exceptions.h
        ASAPP/include/asa/items/item.h
        ASAPP/include/asa/items/itemdata.h
        ASAPP/include/asa/items/items.h
        ASAPP/include/asa/structures/basestructure.h
        ASAPP/include/asa/structures/cave_loot_crate.h
        ASAPP/include/asa/structures/container.h
        ASAPP/include/asa/structures/crafting_station.h
        ASAPP/include/asa/structures/dedicated_storage.h
        ASAPP/include/asa/structures/exceptions.h
        ASAPP/include/asa/structures/interactable.h
        ASAPP/include/asa/structures/simple_bed.h
        ASAPP/include/asa/structures/teleporter.h
        ASAPP/include/asa/utility.h
        ASAPP/include/asa/interfaces/components/tribelog_message.h
        ASAPP/src/interfaces/components/tribelog_message.cpp
        ASAPP/include/asa/network/server.h
        ASAPP/include/asa/network/queries.h
        ASAPP/src/network/server.cpp
        ASAPP/src/network/queries.cpp
        ASAPP/src/interfaces/components/tooltip.cpp
        ASAPP/src/interfaces/dino_info.cpp
        ASAPP/include/asa/interfaces/inventories/dinoinventory.h
        ASAPP/src/interfaces/dinoinventory.cpp
        ASAPP/src/interfaces/menu.cpp
        ASAPP/include/asa/interfaces/inventories/tribute_inventory.h
        ASAPP/src/interfaces/tributeinventory.cpp
        ASAPP/include/asa/structures/tributeable.h
        ASAPP/src/structures/tributable.cpp
        ASAPP/include/asa/game/game.h
        ASAPP/include/asa/interfaces/components/components.h
        ASAPP/include/asa/interfaces/maps/exceptions.h
        ASAPP/include/asa/interfaces/info/infos.h
        ASAPP/include/asa/interfaces/inventories/inventories.h
        ASAPP/include/asa/interfaces/maps/maps.h
        ASAPP/include/asa/core/managedthread.h
        ASAPP/src/core/managedthread.cpp
        ASAPP/include/asa/structures/structures.h
)

set_target_properties(asapp PROPERTIES
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF
)

find_package(Tesseract CONFIG REQUIRED)
target_link_libraries(asapp PRIVATE Tesseract::libtesseract)

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

target_include_directories(asapp PUBLIC ASAPP/include)
target_include_directories(asapp PRIVATE ASAPP/src)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(asapp PRIVATE nlohmann_json::nlohmann_json)

find_package(Boost QUIET REQUIRED COMPONENTS thread)
target_link_libraries(asapp PUBLIC Boost::thread)

# Copy resources to build folder
add_custom_command(
        TARGET asapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/ASAPP/src/itemdata.json
        ${CMAKE_CURRENT_BINARY_DIR}/itemdata.json)

add_custom_command(
        TARGET asapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/ASAPP/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)
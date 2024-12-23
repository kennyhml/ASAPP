cmake_minimum_required(VERSION 3.25)
project(asapp)

add_library(asapp STATIC
        ASAPP/src/core/config.cpp
        ASAPP/src/core/exceptions.cpp
        ASAPP/src/core/init.cpp
        ASAPP/src/core/state.cpp
        ASAPP/src/entities/baseentity.cpp
        ASAPP/src/entities/dinoentity.cpp
        ASAPP/src/entities/exceptions.cpp
        ASAPP/src/entities/localplayer.cpp
        ASAPP/src/game/baseentitiyinfo.cpp
        ASAPP/src/game/controls.cpp
        ASAPP/src/game/resources.cpp
        ASAPP/src/game/settings.cpp
        ASAPP/src/game/window.cpp
        ASAPP/src/interfaces/actionwheel.cpp
        ASAPP/src/interfaces/baseinventory.cpp
        ASAPP/src/interfaces/basetravelmap.cpp
        ASAPP/src/interfaces/components/button.cpp
        ASAPP/src/interfaces/components/searchbar.cpp
        ASAPP/src/interfaces/components/slot.cpp
        ASAPP/src/interfaces/console.cpp
        ASAPP/src/interfaces/craftinginventory.cpp
        ASAPP/src/interfaces/dedicatedstorageinfo.cpp
        ASAPP/src/interfaces/exceptions.cpp
        ASAPP/src/interfaces/hud.cpp
        ASAPP/src/interfaces/interfacemanager.cpp
        ASAPP/src/interfaces/localinventory.cpp
        ASAPP/src/interfaces/mainmenu.cpp
        ASAPP/src/interfaces/modeselect.cpp
        ASAPP/src/interfaces/playerinfo.cpp
        ASAPP/src/interfaces/serverselect.cpp
        ASAPP/src/interfaces/spawnmap.cpp
        ASAPP/src/interfaces/structureinfo.cpp
        ASAPP/src/interfaces/teleportmap.cpp
        ASAPP/src/interfaces/travelmap.cpp
        ASAPP/src/interfaces/tribemanager.cpp
        ASAPP/src/items/exceptions.cpp
        ASAPP/src/items/item.cpp
        ASAPP/src/items/itemdata.cpp
        ASAPP/src/items/items.cpp
        ASAPP/src/structures/cavelootcreate.cpp
        ASAPP/src/structures/container.cpp
        ASAPP/src/structures/craftingstation.cpp
        ASAPP/src/structures/dedicatedstorage.cpp
        ASAPP/src/structures/exceptions.cpp
        ASAPP/src/util/util.cpp
        ASAPP/include/asa/core/config.h
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
        ASAPP/include/asa/interfaces/actionwheel.h
        ASAPP/include/asa/interfaces/baseentityinfo.h
        ASAPP/include/asa/interfaces/baseinfo.h
        ASAPP/include/asa/interfaces/baseinventory.h
        ASAPP/include/asa/interfaces/basetravelmap.h
        ASAPP/include/asa/interfaces/components/button.h
        ASAPP/include/asa/interfaces/components/combobox.h
        ASAPP/include/asa/interfaces/components/iinterfacecomponent.h
        ASAPP/include/asa/interfaces/components/searchbar.h
        ASAPP/include/asa/interfaces/components/slot.h
        ASAPP/include/asa/interfaces/components/tooltip.h
        ASAPP/include/asa/interfaces/console.h
        ASAPP/include/asa/interfaces/containerinfo.h
        ASAPP/include/asa/interfaces/craftinginventory.h
        ASAPP/include/asa/interfaces/dedicatedstorageinfo.h
        ASAPP/include/asa/interfaces/dinoinfo.h
        ASAPP/include/asa/interfaces/engrams.h
        ASAPP/include/asa/interfaces/exceptions.h
        ASAPP/include/asa/interfaces/explorernotes.h
        ASAPP/include/asa/interfaces/hud.h
        ASAPP/include/asa/interfaces/iinterface.h
        ASAPP/include/asa/interfaces/interfacemanager.h
        ASAPP/include/asa/interfaces/interfaces.h
        ASAPP/include/asa/interfaces/localinventory.h
        ASAPP/include/asa/interfaces/mainmenu.h
        ASAPP/include/asa/interfaces/map.h
        ASAPP/include/asa/interfaces/menu.h
        ASAPP/include/asa/interfaces/minimap.h
        ASAPP/include/asa/interfaces/modeselect.h
        ASAPP/include/asa/interfaces/playerinfo.h
        ASAPP/include/asa/interfaces/serverselect.h
        ASAPP/include/asa/interfaces/spawnmap.h
        ASAPP/include/asa/interfaces/teleportmap.h
        ASAPP/include/asa/interfaces/trackingtab.h
        ASAPP/include/asa/interfaces/travelmap.h
        ASAPP/include/asa/interfaces/tribemanager.h
        ASAPP/include/asa/items/exceptions.h
        ASAPP/include/asa/items/item.h
        ASAPP/include/asa/items/itemdata.h
        ASAPP/include/asa/items/items.h
        ASAPP/include/asa/structures/basestructure.h
        ASAPP/include/asa/structures/cavelootcrate.h
        ASAPP/include/asa/structures/container.h
        ASAPP/include/asa/structures/craftingstation.h
        ASAPP/include/asa/structures/dedicatedstorage.h
        ASAPP/include/asa/structures/exceptions.h
        ASAPP/include/asa/structures/interactable.h
        ASAPP/include/asa/structures/largecropplot.h
        ASAPP/include/asa/structures/mediumcropplot.h
        ASAPP/include/asa/structures/simplebed.h
        ASAPP/include/asa/structures/smallcropplot.h
        ASAPP/include/asa/structures/teleporter.h
        ASAPP/include/asa/util/util.h
        ASAPP/include/asa/interfaces/components/tribelogmessage.h
        ASAPP/src/interfaces/components/tribelogmessage.cpp
        ASAPP/include/asa/network/server.h
        ASAPP/include/asa/network/queries.h
        ASAPP/src/network/server.cpp
        ASAPP/src/network/queries.cpp
        ASAPP/src/interfaces/components/tooltip.cpp
        ASAPP/src/interfaces/dinoinfo.cpp
        ASAPP/include/asa/interfaces/dinoinventory.h
        ASAPP/src/interfaces/dinoinventory.cpp
        ASAPP/src/interfaces/menu.cpp
        ASAPP/include/asa/interfaces/tributeinventory.h
        ASAPP/src/interfaces/tributeinventory.cpp
        ASAPP/include/asa/structures/tributeable.h
        ASAPP/src/structures/tributable.cpp
        ASAPP/include/asa/game/actionmappings.h
        ASAPP/include/asa/game/usersettings.h
        ASAPP/src/game/actionmappings.cpp
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
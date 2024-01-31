cmake_minimum_required(VERSION 3.25)
project(asapp)

add_library(asapp STATIC
        ASAPP/source/core/config.cpp
        ASAPP/source/core/exceptions.cpp
        ASAPP/source/core/init.cpp
        ASAPP/source/core/state.cpp
        ASAPP/source/entities/baseentity.cpp
        ASAPP/source/entities/dinoent.cpp
        ASAPP/source/entities/exceptions.cpp
        ASAPP/source/entities/localplayer.cpp
        ASAPP/source/game/baseentitiyinfo.cpp
        ASAPP/source/game/controls.cpp
        ASAPP/source/game/resources.cpp
        ASAPP/source/game/settings.cpp
        ASAPP/source/game/window.cpp
        ASAPP/source/interfaces/actionwheel.cpp
        ASAPP/source/interfaces/baseinventory.cpp
        ASAPP/source/interfaces/basetravelmap.cpp
        ASAPP/source/interfaces/components/button.cpp
        ASAPP/source/interfaces/components/searchbar.cpp
        ASAPP/source/interfaces/components/slot.cpp
        ASAPP/source/interfaces/console.cpp
        ASAPP/source/interfaces/craftinginventory.cpp
        ASAPP/source/interfaces/dedicatedstorageinfo.cpp
        ASAPP/source/interfaces/exceptions.cpp
        ASAPP/source/interfaces/hud.cpp
        ASAPP/source/interfaces/interfacemanager.cpp
        ASAPP/source/interfaces/localinventory.cpp
        ASAPP/source/interfaces/mainmenu.cpp
        ASAPP/source/interfaces/modeselect.cpp
        ASAPP/source/interfaces/playerinfo.cpp
        ASAPP/source/interfaces/serverselect.cpp
        ASAPP/source/interfaces/spawnmap.cpp
        ASAPP/source/interfaces/structureinfo.cpp
        ASAPP/source/interfaces/teleportmap.cpp
        ASAPP/source/interfaces/travelmap.cpp
        ASAPP/source/interfaces/tribemanager.cpp
        ASAPP/source/items/exceptions.cpp
        ASAPP/source/items/item.cpp
        ASAPP/source/items/itemdata.cpp
        ASAPP/source/items/items.cpp
        ASAPP/source/structures/cavelootcreate.cpp
        ASAPP/source/structures/container.cpp
        ASAPP/source/structures/craftingstation.cpp
        ASAPP/source/structures/dedicatedstorage.cpp
        ASAPP/source/structures/exceptions.cpp
        ASAPP/source/util/util.cpp
        ASAPP/include/asapp/core/config.h
        ASAPP/include/asapp/core/exceptions.h
        ASAPP/include/asapp/core/init.h
        ASAPP/include/asapp/core/state.h
        ASAPP/include/asapp/entities/baseentity.h
        ASAPP/include/asapp/entities/dinoent.h
        ASAPP/include/asapp/entities/entities.h
        ASAPP/include/asapp/entities/exceptions.h
        ASAPP/include/asapp/entities/localplayer.h
        ASAPP/include/asapp/game/controls.h
        ASAPP/include/asapp/game/globals.h
        ASAPP/include/asapp/game/resources.h
        ASAPP/include/asapp/game/settings.h
        ASAPP/include/asapp/game/window.h
        ASAPP/include/asapp/interfaces/actionwheel.h
        ASAPP/include/asapp/interfaces/baseentityinfo.h
        ASAPP/include/asapp/interfaces/baseinfo.h
        ASAPP/include/asapp/interfaces/baseinventory.h
        ASAPP/include/asapp/interfaces/basetravelmap.h
        ASAPP/include/asapp/interfaces/components/button.h
        ASAPP/include/asapp/interfaces/components/combobox.h
        ASAPP/include/asapp/interfaces/components/iinterfacecomponent.h
        ASAPP/include/asapp/interfaces/components/searchbar.h
        ASAPP/include/asapp/interfaces/components/slot.h
        ASAPP/include/asapp/interfaces/components/tooltip.h
        ASAPP/include/asapp/interfaces/console.h
        ASAPP/include/asapp/interfaces/containerinfo.h
        ASAPP/include/asapp/interfaces/craftinginventory.h
        ASAPP/include/asapp/interfaces/dedicatedstorageinfo.h
        ASAPP/include/asapp/interfaces/dinoinfo.h
        ASAPP/include/asapp/interfaces/engrams.h
        ASAPP/include/asapp/interfaces/exceptions.h
        ASAPP/include/asapp/interfaces/explorernotes.h
        ASAPP/include/asapp/interfaces/hud.h
        ASAPP/include/asapp/interfaces/iinterface.h
        ASAPP/include/asapp/interfaces/interfacemanager.h
        ASAPP/include/asapp/interfaces/interfaces.h
        ASAPP/include/asapp/interfaces/localinventory.h
        ASAPP/include/asapp/interfaces/mainmenu.h
        ASAPP/include/asapp/interfaces/map.h
        ASAPP/include/asapp/interfaces/menu.h
        ASAPP/include/asapp/interfaces/minimap.h
        ASAPP/include/asapp/interfaces/modeselect.h
        ASAPP/include/asapp/interfaces/playerinfo.h
        ASAPP/include/asapp/interfaces/serverselect.h
        ASAPP/include/asapp/interfaces/spawnmap.h
        ASAPP/include/asapp/interfaces/teleportmap.h
        ASAPP/include/asapp/interfaces/trackingtab.h
        ASAPP/include/asapp/interfaces/travelmap.h
        ASAPP/include/asapp/interfaces/tribemanager.h
        ASAPP/include/asapp/items/exceptions.h
        ASAPP/include/asapp/items/item.h
        ASAPP/include/asapp/items/itemdata.h
        ASAPP/include/asapp/items/items.h
        ASAPP/include/asapp/structures/basestructure.h
        ASAPP/include/asapp/structures/cavelootcrate.h
        ASAPP/include/asapp/structures/container.h
        ASAPP/include/asapp/structures/craftingstation.h
        ASAPP/include/asapp/structures/dedicatedstorage.h
        ASAPP/include/asapp/structures/exceptions.h
        ASAPP/include/asapp/structures/interactablestructure.h
        ASAPP/include/asapp/structures/largecropplot.h
        ASAPP/include/asapp/structures/mediumcropplot.h
        ASAPP/include/asapp/structures/simplebed.h
        ASAPP/include/asapp/structures/smallcropplot.h
        ASAPP/include/asapp/structures/teleporter.h
        ASAPP/include/asapp/util/util.h
        ASAPP/include/asapp/interfaces/components/tribelogmessage.h
        ASAPP/source/interfaces/components/tribelogmessage.cpp)

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
target_include_directories(asapp PRIVATE ASAPP/source)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(asapp PRIVATE nlohmann_json::nlohmann_json)

# Copy resources to build folder
add_custom_command(
        TARGET asapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/ASAPP/source/itemdata.json
        ${CMAKE_CURRENT_BINARY_DIR}/itemdata.json)

add_custom_command(
        TARGET asapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/ASAPP/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)
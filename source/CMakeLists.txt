project(nodevmap_core)

set(CMAKE_CXX_STANDARD 11)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(nodevmap_SOURCES
    base/base_element.cpp
    base/elem_storage.cpp
    base/nvp_event.cpp
    base/nvp_layout.cpp
    draw/nvp_draw.cpp
    draw/draw_text.cpp
    draw/draw_rect.cpp
    draw/draw_line.cpp
    nvp-io/nvp_plist.cpp
    nvp-io/plist_io.cpp
    util/nvp_util.cpp

)

################################################################################

set(nodevmap_WIN_SOURCES
    port-win/win_sys.cpp
    port-win/win_main.cpp
    port-win/win_message.cpp
    port-win/direct2d/d2d_main.cpp
)

if(WIN32)
    add_library(nodevmap_core STATIC
        ${nodevmap_SOURCES}
        ${nodevmap_WIN_SOURCES}
    )

    target_link_libraries(nodevmap_core
        d2d1
        dwrite
    )
endif()

################################################################################

set(nodevmap_OSX_SOURCES
    port-osx/osx_sys.mm
    port-osx/osx_main.mm
    port-osx/osx_message.mm
    port-osx/quartz/qtz_main.mm
)

if(CMAKE_HOST_APPLE)
    #Xcode project
    #cd build-xcode
    #cmake .. -G "Xcode"
    #set(CMAKE_CXX_FLAGS "-std=c++11")
    find_library(COCOA Cocoa)

    add_library(nodevmap_core STATIC
        ${nodevmap_SOURCES}
        ${nodevmap_OSX_SOURCES}
    )

    target_link_libraries(nodevmap_core
        ${COCOA}
    )
endif()

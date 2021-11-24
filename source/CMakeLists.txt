project(nodevmap)

include_directories("./")

set(nodevmap_SOURCES
    base/base_element.cpp
    base/elem_storage.cpp
    base/nvp_event.cpp
    base/nvp_layout.cpp
    draw/nvp_draw.cpp
    draw/draw_text.cpp
    draw/draw_rect.cpp
    util/nvp_util.cpp
    TEST_0.cpp
    )

################################################################################

set(nodevmap_WIN_SOURCES
    port-win/win_main.cpp
    port-win/win_message.cpp
    port-win/direct2d/d2d_main.cpp
    )

if(WIN32)
    add_executable(nodevmap WIN32
        ${nodevmap_SOURCES}
        ${nodevmap_WIN_SOURCES}
    )

    target_link_libraries(nodevmap
        d2d1
        dwrite
    )
endif()

################################################################################

set(nodevmap_OSX_SOURCES
    port-osx/osx_main.mm
    port-osx/osx_message.mm
    port-osx/quartz/qtz_main.mm
    )

if(CMAKE_HOST_APPLE)
    #Xcode project
    #cd build-xcode
    #cmake .. -G "Xcode"
    set(CMAKE_CXX_FLAGS "-std=c++11")
    find_library(COCOA Cocoa)

    add_executable(nodevmap
        ${nodevmap_SOURCES}
        ${nodevmap_OSX_SOURCES}
    )

    target_link_libraries(nodevmap
        ${COCOA}
    )
endif()

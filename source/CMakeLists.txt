project(nodevmap)

include_directories("./")

set(nodevmap_SOURCES
    base/base_message.cpp
    base/base_element.cpp
    base/elem_storage.cpp
    base/nvp_layout.cpp
    draw/nvp_draw.cpp
    draw/draw_text.cpp
    draw/draw_rect.cpp
    util/nvp_util.cpp
    )

################################################################################

set(nodevmap_WIN_SOURCES
    port-win/win_main.cpp
    port-win/win_message.cpp
    port-win/direct2d/d2d_main.cpp
    )

if(WIN32)
    add_executable(TEST_d2d WIN32
        TEST_d2d.cpp
        ${nodevmap_SOURCES}
        ${nodevmap_WIN_SOURCES}
    )

    target_link_libraries(TEST_d2d
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

    add_executable(TEST_qtz
        TEST_qtz.cpp
        ${nodevmap_SOURCES}
        ${nodevmap_OSX_SOURCES}
    )

    target_link_libraries(TEST_qtz
        ${COCOA}
    )
endif()

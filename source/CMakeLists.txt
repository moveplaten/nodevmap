project(nodevmap)

include_directories("./")

set(nodevmap_SOURCES
    base/base_message.cpp
    base/base_element.cpp
    base/elem_storage.cpp
    draw/nvp_draw.cpp
    port-win/win_main.cpp
    port-win/win_message.cpp
    port-win/direct2d/d2d_main.cpp
    )

add_executable(TEST_01 WIN32
    TEST_01.cpp
    ${nodevmap_SOURCES})

target_link_libraries(TEST_01
    d2d1
)

add_executable(TEST_d2d WIN32
    TEST_d2d.cpp
    ${nodevmap_SOURCES})

target_link_libraries(TEST_d2d
    d2d1
)
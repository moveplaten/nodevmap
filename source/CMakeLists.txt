project(nodevmap)

include_directories("./")

set(nodevmap_SOURCES
    base/base_message.cpp
    base/base_element.cpp
    port-win/win_main.cpp
    port-win/win_message.cpp
    )

add_executable(TEST_01 WIN32
    TEST_01.cpp
    ${nodevmap_SOURCES})

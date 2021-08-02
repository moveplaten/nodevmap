project(nodevmap)

include_directories("./")

set(nodevmap_SOURCES	
    win_main.cpp
    base_message.cpp
    base_element.cpp
    win_message.cpp
    )

add_executable(nodevmap WIN32
    ${nodevmap_SOURCES})

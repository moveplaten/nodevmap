project(nodevmap)

set(CMAKE_CXX_STANDARD 11)

set(nodevmap_app_SOURCES
    app/entry_main.cpp
    app/nvp_app.cpp
    
)

################################################################################

if(WIN32)
    add_executable(nodevmap WIN32
        ${nodevmap_app_SOURCES}
    )
endif()

################################################################################

if(CMAKE_HOST_APPLE)
    #Xcode project
    #cd build-xcode
    #cmake .. -G "Xcode"

    add_executable(nodevmap
        ${nodevmap_app_SOURCES}
    )
endif()

################################################################################

target_include_directories(nodevmap PRIVATE
    "${CMAKE_SOURCE_DIR}/source"

)

target_link_libraries(nodevmap
    nodevmap_core
    libplist
)

add_subdirectory(source)

add_subdirectory(third-party/libplist)

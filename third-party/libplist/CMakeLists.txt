project(libplist)

set(CMAKE_CXX_STANDARD 11)

set(libplist_SOURCES
    libcnary/node.c
    libcnary/node_list.c
    src/base64.c
    src/bytearray.c
    src/hashtable.c
    src/ptrarray.c
    src/time64.c
    src/xplist.c
    #src/bplist.c
    src/plist.c
)

add_library(libplist STATIC
    ${libplist_SOURCES}
)

target_include_directories(libplist PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libcnary/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#
# sockets/chatd/CMakeLists.txt --
#

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVENT REQUIRED libevent)
include_directories(${LIBEVENT_INCLUDE_DIRS})
link_directories(${LIBEVENT_LIBRARY_DIRS})

add_executable(gwgd gwgd.c clnt-event.c clnt.c run.c chlng.c player.c)

target_link_libraries(gwgd LINK_PUBLIC libtcp ${LIBEVENT_LIBRARIES})
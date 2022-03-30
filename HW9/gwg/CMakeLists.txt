#
# sockets/chat/CMakeLists.txt --
#

add_executable(gwg gwg.c)

target_link_libraries(gwg LINK_PUBLIC libtcp)
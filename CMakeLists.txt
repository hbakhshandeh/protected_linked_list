cmake_minimum_required (VERSION 2.6)
project (protected_linked_list)

set(CPP_SOURCE src/main.cpp)

add_executable(${PROJECT_NAME} ${CPP_SOURCE})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC pthread)
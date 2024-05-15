cmake_minimum_required(VERSION 3.9.1)
project(myterminal)
include(CMakePrintHelpers)
cmake_print_variables(PROJECT_SOURCE_DIR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(THREADS_PREFER_PTHREAD_FLAG ON)

INCLUDE(FindPkgConfig)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

#pkg_check_modules(PJSIP "libpjproject")
pkg_check_modules(PJSIP libpjproject>=2.14 REQUIRED)

add_definitions(${PJSIP_CFLAGS})

include_directories(${PJSIP_INCLUDE_DIRS})
link_directories(${PJSIP_LIBRARY_DIRS})

add_executable(myterminal 
    src/main.cpp 
    src/GenAcc.cpp 
    src/GenCall.cpp 
    src/Bsubscriber.cpp
    src/CallTests.cpp
    src/C_acc.cpp
    src/CinCall.cpp
    src/BinCall.cpp
    src/B_acc.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PJSIP_STATIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)

cmake_print_variables(PJSIP_STATIC_LIBRARIES)

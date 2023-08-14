cmake_minimum_required(VERSION 3.20.0)
set(project_name imdb_app)
project(${project_name})
project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif ()

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/module/")

include_directories(bdb-lib)
include_directories(example-lib)
include_directories(imdb-lib)
include_directories(util-lib)

include(UseDoxygenDoc)
add_subdirectory(bdb-lib)
add_subdirectory(example-lib)
add_subdirectory(imdb-lib)
add_subdirectory(util-lib)

add_doxygen_doc(
        BUILD_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/_build
        DOXY_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
        TARGET_NAME
        docs
        COMMENT
        "HTML documentation"
)
## link libs

set(BUILD_SHARED_LIBS NO)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

find_package(oatpp 1.3.0 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
message("FOUND: ${Threads}")

# OpenSSL::SSL
# https://cmake.org/cmake/help/v3.6/module/FindOpenSSL.html
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# https://github.com/cloudshark/cshark/blob/master/build/modules/FindJSON-C.cmake
set(JSON-C_USE_STATIC_LIBS TRUE)
find_package(JSON-C REQUIRED)

# https://github.com/sum01/FindBerkeleyDB
set(BerkeleyDB_USE_STATIC_LIBS TRUE)
find_package(BerkeleyDB REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FindBerkeleyDB")

# imdb_app
add_executable(${project_name} apps/${project_name}.cpp)

target_link_libraries(${project_name} ${project_name}-lib)
add_dependencies(${project_name} ${project_name}-lib)

add_library(${project_name}-lib
        oatpp-lib/DTOs.hpp
        oatpp-lib/imdb_component.hpp
        oatpp-lib/imdb_controller.cpp
        oatpp-lib/imdb_controller.hpp
        oatpp-lib/imdb-dto.hpp
        )

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PRIVATE imdb-lib bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
        PUBLIC Threads::Threads
        )

target_include_directories(${project_name}-lib PUBLIC oatpp-lib)

# bdb-unix-lookup
add_executable(bdb_unix_lookup apps/bdb_unix_lookup.cpp)

target_link_libraries(bdb_unix_lookup
        PRIVATE imdb-lib bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
        PUBLIC Threads::Threads
        )

# bdb-inet-lookup
add_executable(bdb_inet_lookup apps/bdb_inet_lookup.cpp)

target_link_libraries(bdb_inet_lookup
        PRIVATE imdb-lib bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
        PUBLIC Threads::Threads
        )

# socket tests
add_executable(test_unix_server
        apps/test_unix_server.cpp)

target_link_libraries(test_unix_server
        PRIVATE bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        )

add_executable(test_unix_server_loop
        apps/test_unix_server_loop.cpp)

target_link_libraries(test_unix_server_loop
        PRIVATE bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        )

add_executable(test_unix_client
        apps/test_unix_client.cpp)

target_link_libraries(test_unix_client
        PRIVATE bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        )

add_executable(test_unix_client_request
        apps/test_unix_client_request.cpp)

target_link_libraries(test_unix_client_request
        PRIVATE bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        )

# examples
#
#add_executable(excxx_example_database_load apps/excxx_example_database_load.cpp)
#
#target_link_libraries(excxx_example_database_load
#        PRIVATE example-lib bdb-lib
#        PUBLIC Oracle::BerkeleyDB
#        PUBLIC ${JSON-C_LIBRARIES}
#        PUBLIC OpenSSL::SSL
#        PUBLIC OpenSSL::Crypto
#        PUBLIC Threads::Threads
#        )
#
#add_executable(excxx_example_database_read apps/excxx_example_database_read.cpp)
#
#target_link_libraries(excxx_example_database_read
#        PRIVATE example-lib bdb-lib
#        PUBLIC Oracle::BerkeleyDB
#        PUBLIC ${JSON-C_LIBRARIES}
#        PUBLIC OpenSSL::SSL
#        PUBLIC OpenSSL::Crypto
#        PUBLIC Threads::Threads
#        )

# ratings_top_num_votes

add_executable(ratings_top_num_votes apps/ratings_top_num_votes.cpp)

target_link_libraries(ratings_top_num_votes
        PRIVATE imdb-lib bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        )


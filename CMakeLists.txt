cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin" )
set( TEST_SUITE "0" CACHE TEST_SUITE "Include test suite in build" )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/shared/include" )
add_subdirectory( shared )

add_subdirectory( nodes/root )
add_subdirectory( nodes/trusted )
add_subdirectory( nodes/org )

add_subdirectory( clients/forge )
if( TEST_SUITE )
    add_subdirectory(testing)
endif( TEST_SUITE )

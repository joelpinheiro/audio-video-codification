cmake_minimum_required(VERSION 2.8)
project( CAV1516 )

MESSAGE ("\n==> Finding 3rd Party Packages")
find_package( OpenCV REQUIRED )

SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W" )
SET( CMAKE_C_FLAGS_RELEASE "-O3" )
SET( CMAKE_C_FLAGS_DEBUG "-g3" )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall" )
SET( CMAKE_CXX_FLAGS_RELEASE "-O3" )
SET( CMAKE_CXX_FLAGS_DEBUG "-g3" )

SET( BASE_DIR ${CMAKE_SOURCE_DIR} )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/bin )

# Make include directories available for all..
INCLUDE_DIRECTORIES(
		${OpenCV_INCLUDE_DIR}
)

add_executable( DisplayVideo DisplayVideo.cpp videoCmp.cpp Trabalho_3/videoCmp.h)
target_link_libraries( DisplayVideo ${OpenCV_LIBS} )

add_executable( ScanImages ScanImages.cpp)
target_link_libraries( ScanImages ${OpenCV_LIBS} )

add_executable( VideoFromFile VideoFromFile.cpp)
target_link_libraries( VideoFromFile ${OpenCV_LIBS} )
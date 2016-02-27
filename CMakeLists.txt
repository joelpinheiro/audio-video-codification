cmake_minimum_required(VERSION 2.8)
project( CAV1516 )

MESSAGE ("\n==> Finding 3rd Party Packages")
find_package( OpenCV REQUIRED )

SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -std=c++11" )
SET( CMAKE_C_FLAGS_RELEASE "-O3" )
SET( CMAKE_C_FLAGS_DEBUG "-g3" )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=c++11" )
SET( CMAKE_CXX_FLAGS_RELEASE "-O3" )
SET( CMAKE_CXX_FLAGS_DEBUG "-g3" )

SET( BASE_DIR ${CMAKE_SOURCE_DIR} )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/bin )

# Make include directories available for all..
INCLUDE_DIRECTORIES( 
	${OpenCV_INCLUDE_DIR}
)

add_executable( DisplayVideo Trabalho_3/DisplayVideo.cpp)
target_link_libraries( DisplayVideo ${OpenCV_LIBS} )

add_executable( ScanImages Trabalho_3/ScanImages.cpp)
target_link_libraries( ScanImages ${OpenCV_LIBS} )

add_executable( VideoFromFile Trabalho_3/VideoFromFile.cpp)
target_link_libraries( VideoFromFile ${OpenCV_LIBS} )

add_executable(VideoCmp Trabalho_3/VideoCmp.cpp)
target_link_libraries(VideoCmp ${OpenCV_LIBS})

add_executable( VideoResize Trabalho_3/VideoResize.cpp)
target_link_libraries( VideoResize ${OpenCV_LIBS} )

add_executable(IntraFrameLossless Trabalho_3/IntraFrameLossless.cpp
        Trabalho_3/RawIntraFrameCodecLossless.cpp Trabalho_3/RawIntraFrameCodecLossless.h
        Trabalho_3/Golomb/BitStream.cpp Trabalho_3/Golomb/BitStream.h Trabalho_3/Golomb/Golomb.cpp Trabalho_3/Golomb/Golomb.h)
target_link_libraries( IntraFrameLossless ${OpenCV_LIBS} )

add_executable(IntraFrameLossy Trabalho_3/IntraFrameLossy.cpp
        Trabalho_3/RawIntraFrameCodecLossy.cpp Trabalho_3/RawIntraFrameCodecLossy.h
        Trabalho_3/Golomb/BitStream.cpp Trabalho_3/Golomb/BitStream.h Trabalho_3/Golomb/Golomb.cpp Trabalho_3/Golomb/Golomb.h)
target_link_libraries( IntraFrameLossy ${OpenCV_LIBS} )
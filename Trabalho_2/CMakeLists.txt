cmake_minimum_required(VERSION 2.8)

project(TRABALHO2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
#set(SOURCE_FILES_TRABALHO2 ...)
add_executable( TRABALHO2 trabalho2.cpp Golomb/BitStream.cpp Golomb/BitStream.h
        Golomb/Golomb.cpp Golomb/Golomb.h LosslessAudioCodec.cpp LosslessAudioCodec.h
        LossyAudioCodec.cpp LossyAudioCodec.h)
target_link_libraries(TRABALHO2 sndfile)



cmake_minimum_required(VERSION 3.10)
project(Shazam)

set(CMAKE_CXX_STANDARD 17)

include_directories(third_party/kissfft)
include_directories(third_party/sndfile/include)

add_executable(shazam
    main.cpp
    audio_loader.cpp
    fft_processor.cpp
    peak_finder.cpp
    fingerprint.cpp
    matcher.cpp
    database.cpp
    third_party/kissfft/kiss_fft.c
)

find_library(SNDFILE_LIBRARY sndfile)
target_link_libraries(shazam ${SNDFILE_LIBRARY})
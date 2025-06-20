cmake_minimum_required(VERSION 3.10)
project(Shazam)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)


include_directories(third_party/kissfft)
include_directories(third_party/sndfile/include)
include_directories(${PQXX_INCLUDE_DIRS})

add_executable(shazam
    main.cpp
    audio_loader.cpp
    fft_processor.cpp
    peak_finder.cpp
    fingerprint.cpp
    matcher.cpp
    database.cpp
    mic_recorder.cpp

    third_party/kissfft/kiss_fft.c
)

find_library(SNDFILE_LIBRARY sndfile)
find_library(PORTAUDIO_LIB portaudio)
target_link_libraries(shazam ${SNDFILE_LIBRARY} ${PORTAUDIO_LIB} ${PQXX_LIBRARIES})
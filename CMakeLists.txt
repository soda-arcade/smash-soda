cmake_minimum_required(VERSION 3.18)

# >>> TEMP DEBUG (remove before merge): diagnose the Windows guard on the reporter's machine
message(STATUS "SS-DEBUG cmake=${CMAKE_VERSION}  gen=[${CMAKE_GENERATOR}]  plat=[${CMAKE_GENERATOR_PLATFORM}]")
message(STATUS "SS-DEBUG WIN32=[${WIN32}]  CMAKE_HOST_WIN32=[${CMAKE_HOST_WIN32}]  UNIX=[${UNIX}]")
message(STATUS "SS-DEBUG SYSTEM_NAME=[${CMAKE_SYSTEM_NAME}]  HOST_SYSTEM_NAME=[${CMAKE_HOST_SYSTEM_NAME}]")
message(STATUS "SS-DEBUG TOOLCHAIN=[${CMAKE_TOOLCHAIN_FILE}]  VS_PLATFORM=[${CMAKE_VS_PLATFORM_NAME}]  VOIDP=[${CMAKE_SIZEOF_VOID_P}]")
# <<< TEMP DEBUG

# Make sure we are on Windows
if(NOT CMAKE_HOST_WIN32)
    message(FATAL_ERROR "Smash Soda can only be built on Windows. [SS-DEBUG WIN32=${WIN32} HOST_WIN32=${CMAKE_HOST_WIN32} SYSTEM=${CMAKE_SYSTEM_NAME} TOOLCHAIN=${CMAKE_TOOLCHAIN_FILE}]")
endif()

project(SmashSoda LANGUAGES CXX)
message(STATUS "SS-DEBUG (after project) WIN32=[${WIN32}]  SYSTEM_NAME=[${CMAKE_SYSTEM_NAME}]  MSVC=[${MSVC}]  VS_PLATFORM=[${CMAKE_VS_PLATFORM_NAME}]")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Force C++17 (matching the subdirectory requirement)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect architecture for output directories
if(NOT CMAKE_VS_PLATFORM_NAME)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x64")
    else()
        set(ARCH "Win32")
    endif()
else()
    set(ARCH "${CMAKE_VS_PLATFORM_NAME}")
endif()

# Output folders (using standard CMake case: Release, Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${ARCH}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${ARCH}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/${ARCH}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/${ARCH}/Release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/${ARCH}/RelWithDebInfo")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/${ARCH}/MinSizeRel")

# Add dependencies if they have their own CMakeLists
if(EXISTS "${CMAKE_SOURCE_DIR}/Dependencies/CMakeLists.txt")
    add_subdirectory(Dependencies)
endif()

# Add main project
add_subdirectory(SmashSoda)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(SkypeRTC)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
include("${PROJECT_SOURCE_DIR}/cmake/utils.cmake")
set(PROJECT_VERSION 1.0.0 CACHE STRING "SkypeRTC version")
DISSECT_VERSION()
set(PROJECT_DESCRIPTION "Skype RTC")
set(PROJECT_VENDOR "ysuga_net")
set(PROJECT_AUTHOR "ysuga_net")
set(PROJECT_AUTHOR_SHORT "ysuga_net")

# Add an "uninstall" target
CONFIGURE_FILE ("${PROJECT_SOURCE_DIR}/cmake/uninstall_target.cmake.in"
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P
    "${PROJECT_BINARY_DIR}/uninstall_target.cmake")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(CMAKE_CXX_COMPILER "g++")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

#option(BUILD_EXAMPLES "Build and install examples" OFF)
option(BUILD_DOCUMENTATION "Build the documentation" ON)
#option(BUILD_TESTS "Build the tests" OFF)
#option(BUILD_TOOLS "Build the tools" OFF)
option(BUILD_IDL "Build and install idl" ON)
option(BUILD_SOURCES "Build and install sources" OFF)

option(STATIC_LIBS "Build static libraries" OFF)
if(STATIC_LIBS)
    set(LIB_TYPE STATIC)
else(STATIC_LIBS)
    set(LIB_TYPE SHARED)
endif(STATIC_LIBS)

set(SKYPEKIT_DIR $ENV{SKYPEKIT_ROOT})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(SKYPEKIT_BIN bin/mac-x86/mac-x86-skypekit)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(SKYPEKIT_BIN bin/linux-x86/linux-x86-skypekit)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (WIN32)
set(SKYPEKIT_BIN bin/windows-x86/windows-x86-skypekit.exe)
endif (WIN32)


configure_file(${PROJECT_SOURCE_DIR}/include/SkypeRTC/config.h.cmake
                ${PROJECT_SOURCE_DIR}/include/SkypeRTC/config.h)


set(SKYPEKIT_INC_DIR  ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/src/api 
    ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/src/ipc 
    ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/src/types 
    ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/src/platform/threading 
    ${SKYPEKIT_DIR}/ipc/cpp 
    ${SKYPEKIT_DIR}/ipc/cpp/platform/se 
    ${SKYPEKIT_DIR}/ipc/cpp/ssl/cyassl/include 
    ${SKYPEKIT_DIR}/ipc/cpp/ssl/cyassl/ctaocrypt/include 
    ${SKYPEKIT_DIR}/ipc/cpp/ssl/cyassl/include/openssl 
    ${SKYPEKIT_DIR}/ipc/cpp/VideoBuffers
)



# Set up installation directories
set(BIN_INSTALL_DIR "components/bin")
set(LIB_INSTALL_DIR "components/lib")
set(INC_INSTALL_DIR
    "components/include/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_MAJOR}")
set(SHARE_INSTALL_DIR
    "components/share/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_MAJOR}")

# Get necessary dependency information
if(NOT WIN32)
    list(APPEND SKYPEKIT_INC_DIR ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/src/platform/threading/pthread)

    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

#set(SKYPEKIT_LIBS  ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/build/libskypekit-cppwrapper_2_lib.a
#   ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/build/libskypekit-cyassl_lib.a)
set(SKYPEKIT_LIBS  ${SKYPEKIT_DIR}/lib/libskypekit-cppwrapper_2_lib.a
   ${SKYPEKIT_DIR}/lib/libskypekit-cyassl_lib.a
   ${SKYPEKIT_DIR}/lib/libskypekit-videobuffers.a)

endif(NOT WIN32)


if(WIN32)
list(APPEND SKYPEKIT_INC_DIR ${SKYPEKIT_DIR}/interfaces/skype/cpp_embedded/src/platform/threading/win)

set(SKYPEKIT_LIBS WS2_32.lib Advapi32.lib ${SKYPEKIT_DIR}/lib/skypekit-cyassl_lib.lib ${SKYPEKIT_DIR}/lib/skypekit-cppwrapper_2_lib.lib 
   ${SKYPEKIT_DIR}/lib/skypekit-videobuffers.lib)

endif(WIN32)







find_package(OpenRTM REQUIRED)

# Universal settings
#enable_testing()

# Subdirectories
add_subdirectory(cmake)
if(BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif(BUILD_DOCUMENTATION)
#if(BUILD_EXAMPLES)
#    add_subdirectory(examples)
#endif(BUILD_EXAMPLES)
if(BUILD_IDL)
    add_subdirectory(idl)
endif(BUILD_IDL)
add_subdirectory(include)
add_subdirectory(src)
#if(BUILD_TESTS)
#    add_subdirectory(test)
#endif(BUILD_TESTS)
#if(BUILD_TOOLS)
#    add_subdirectory(tools)
#endif(BUILD_TOOLS)
if(BUILD_SOURCES)
    add_subdirectory(src)
endif(BUILD_SOURCES)

# Package creation
include(InstallRequiredSystemLibraries)
set(PROJECT_EXECUTABLES ${PROJECT_NAME_LOWER}Comp
    "${PROJECT_NAME_LOWER}Comp")
set(cpack_options "${PROJECT_BINARY_DIR}/cpack_options.cmake")
configure_file("${PROJECT_SOURCE_DIR}/cmake/cpack_options.cmake.in"
    ${cpack_options} @ONLY)
set(CPACK_PROJECT_CONFIG_FILE ${cpack_options})
include(${CPACK_PROJECT_CONFIG_FILE})
include(CPack)



# Install script for directory: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/libgroup_sig.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/group_sig/algorithm/bbs04" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/bbs04/GroupSig_BBS.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/bbs04/GroupSig_BBS_Impl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/group_sig/algorithm/ring-sig" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/ring-sig/Common.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/ring-sig/LinkableRingSig_Impl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/group_sig/algorithm" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/GroupSig.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/GroupSigFactory.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/GroupSigInterface.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/KeyLoaderDumper.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/LinearPair.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/algorithm/RingSig.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/group_sig/devcore" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/Base64.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/CommonFunc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/CommonStruct.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/ConfigParser.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/SHA3.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/StatusCode.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/easylog.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/GroupSigLib/group_sig/devcore/vector_ref.h"
    )
endif()


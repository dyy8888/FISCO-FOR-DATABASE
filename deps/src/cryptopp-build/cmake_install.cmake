# Install script for directory: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp-build/libcryptopp.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cryptopp" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/3way.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/adler32.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/aes.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/algebra.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/algparam.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/arc4.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/argnames.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/asn.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/authenc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/base32.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/base64.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/basecode.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/bench.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/blake2.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/blowfish.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/blumshub.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/camellia.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/cast.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/cbcmac.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ccm.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/chacha.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/channels.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/cmac.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/config.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/cpu.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/crc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/cryptlib.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/default.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/des.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/dh.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/dh2.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/dll.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/dmac.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/drbg.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/dsa.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/eax.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ec2n.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/eccrypto.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ecp.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ecpoint.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/elgamal.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/emsa2.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/eprecomp.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/esign.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/factory.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/fhmqv.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/files.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/filters.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/fips140.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/fltrimpl.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gcm.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gf256.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gf2_32.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gf2n.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gfpcrypt.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gost.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/gzip.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/hex.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/hkdf.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/hmac.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/hmqv.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/hrtimer.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ida.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/idea.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/integer.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/iterhash.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/keccak.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/lubyrack.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/luc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/mars.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/md2.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/md4.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/md5.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/mdc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/mersenne.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/misc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/modarith.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/modes.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/modexppc.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/mqueue.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/mqv.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/nbtheory.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/network.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/nr.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/oaep.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/oids.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/osrng.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ossig.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/panama.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/pch.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/pkcspad.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/poly1305.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/polynomi.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/pssr.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/pubkey.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/pwdbased.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/queue.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rabin.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/randpool.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rc2.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rc5.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rc6.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rdrand.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/resource.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rijndael.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ripemd.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rng.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rsa.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/rw.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/safer.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/salsa.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/seal.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/secblock.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/seckey.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/seed.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/serpent.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/serpentp.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/sha.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/sha3.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/shacal2.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/shark.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/simple.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/siphash.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/skipjack.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/smartptr.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/socketft.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/sosemanuk.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/square.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/stdcpp.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/strciphr.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/tea.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/tiger.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/trap.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/trdlocal.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/trunhash.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/ttmac.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/twofish.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/validate.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/vmac.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/wait.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/wake.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/whrlpool.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/winpipes.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/words.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/xtr.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/xtrcrypt.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/zdeflate.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/zinflate.h"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/zlib.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp" TYPE FILE FILES
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp/cryptopp-config.cmake"
    "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp-build/cryptopp-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets.cmake"
         "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp-build/CMakeFiles/Export/lib/cmake/cryptopp/cryptopp-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp" TYPE FILE FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp-build/CMakeFiles/Export/lib/cmake/cryptopp/cryptopp-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp" TYPE FILE FILES "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp-build/CMakeFiles/Export/lib/cmake/cryptopp/cryptopp-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/cryptopp-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")

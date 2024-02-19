# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

function(check_file_hash has_hash hash_is_good)
  if("${has_hash}" STREQUAL "")
    message(FATAL_ERROR "has_hash Can't be empty")
  endif()

  if("${hash_is_good}" STREQUAL "")
    message(FATAL_ERROR "hash_is_good Can't be empty")
  endif()

  if("SHA256" STREQUAL "")
    # No check
    set("${has_hash}" FALSE PARENT_SCOPE)
    set("${hash_is_good}" FALSE PARENT_SCOPE)
    return()
  endif()

  set("${has_hash}" TRUE PARENT_SCOPE)

  message(STATUS "verifying file...
       file='/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2'")

  file("SHA256" "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2" actual_value)

  if(NOT "${actual_value}" STREQUAL "7f6130bc3cf65f56a618888ce9d5ea704fa10b462be126ad053e80e553d6d8b7")
    set("${hash_is_good}" FALSE PARENT_SCOPE)
    message(STATUS "SHA256 hash of
    /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2
  does not match expected value
    expected: '7f6130bc3cf65f56a618888ce9d5ea704fa10b462be126ad053e80e553d6d8b7'
      actual: '${actual_value}'")
  else()
    set("${hash_is_good}" TRUE PARENT_SCOPE)
  endif()
endfunction()

function(sleep_before_download attempt)
  if(attempt EQUAL 0)
    return()
  endif()

  if(attempt EQUAL 1)
    message(STATUS "Retrying...")
    return()
  endif()

  set(sleep_seconds 0)

  if(attempt EQUAL 2)
    set(sleep_seconds 5)
  elseif(attempt EQUAL 3)
    set(sleep_seconds 5)
  elseif(attempt EQUAL 4)
    set(sleep_seconds 15)
  elseif(attempt EQUAL 5)
    set(sleep_seconds 60)
  elseif(attempt EQUAL 6)
    set(sleep_seconds 90)
  elseif(attempt EQUAL 7)
    set(sleep_seconds 300)
  else()
    set(sleep_seconds 1200)
  endif()

  message(STATUS "Retry after ${sleep_seconds} seconds (attempt #${attempt}) ...")

  execute_process(COMMAND "${CMAKE_COMMAND}" -E sleep "${sleep_seconds}")
endfunction()

if("/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2" STREQUAL "")
  message(FATAL_ERROR "LOCAL can't be empty")
endif()

if("https://nchc.dl.sourceforge.net/project/boost/boost/1.68.0/boost_1_68_0.tar.bz2;http://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2;https://raw.githubusercontent.com/FISCO-BCOS/LargeFiles/master/libs/boost_1_68_0.tar.bz2;https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/FISCO-BCOS/deps/boost_1_68_0.tar.bz2" STREQUAL "")
  message(FATAL_ERROR "REMOTE can't be empty")
endif()

if(EXISTS "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2")
  check_file_hash(has_hash hash_is_good)
  if(has_hash)
    if(hash_is_good)
      message(STATUS "File already exists and hash match (skip download):
  file='/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2'
  SHA256='7f6130bc3cf65f56a618888ce9d5ea704fa10b462be126ad053e80e553d6d8b7'"
      )
      return()
    else()
      message(STATUS "File already exists but hash mismatch. Removing...")
      file(REMOVE "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2")
    endif()
  else()
    message(STATUS "File already exists but no hash specified (use URL_HASH):
  file='/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2'
Old file will be removed and new file downloaded from URL."
    )
    file(REMOVE "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2")
  endif()
endif()

set(retry_number 5)

message(STATUS "Downloading...
   dst='/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2'
   timeout='none'"
)

foreach(i RANGE ${retry_number})
  sleep_before_download(${i})

  foreach(url https://nchc.dl.sourceforge.net/project/boost/boost/1.68.0/boost_1_68_0.tar.bz2;http://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2;https://raw.githubusercontent.com/FISCO-BCOS/LargeFiles/master/libs/boost_1_68_0.tar.bz2;https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/FISCO-BCOS/deps/boost_1_68_0.tar.bz2)
    message(STATUS "Using src='${url}'")

    
    
    
    

    file(
        DOWNLOAD
        "${url}" "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2"
        
        # no TIMEOUT
        STATUS status
        LOG log
        
        
    )

    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(status_code EQUAL 0)
      check_file_hash(has_hash hash_is_good)
      if(has_hash AND NOT hash_is_good)
        message(STATUS "Hash mismatch, removing...")
        file(REMOVE "/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/boost_1_68_0.tar.bz2")
      else()
        message(STATUS "Downloading... done")
        return()
      endif()
    else()
      string(APPEND logFailedURLs "error: downloading '${url}' failed
       status_code: ${status_code}
       status_string: ${status_string}
       log:
       --- LOG BEGIN ---
       ${log}
       --- LOG END ---
       "
      )
    endif()
  endforeach()
endforeach()

message(FATAL_ERROR "Each download failed!
  ${logFailedURLs}
  "
)

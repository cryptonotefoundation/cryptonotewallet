
set(WALLET_NAME "KarbowanecWallet")
set(CN_CURRENCY_DISPLAY_NAME "Karbo")
set(CN_CURRENCY_TICKER "KRB")

# Get version number from git tag, parse it and split into separate components
find_package(Git)
message(STATUS "Found Git: ${GIT_FOUND}")
if(GIT_FOUND)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --tags --abbrev=0 --match "v*"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE WALLET_GIT_RESULT
    OUTPUT_VARIABLE WALLET_GIT_TAG
    ERROR_VARIABLE WALLET_GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE)
  if(NOT WALLET_GIT_RESULT EQUAL 0)
    set(WALLET_GIT_TAG "0.0.0")
	set(WALLET_VER_MAJOR 0)
	set(WALLET_VER_MINOR 0)
	set(WALLET_VER_PATCH 0)
    message(WARNING "Failed to execute Git: ${WALLET_GIT_ERROR}")
  else()
    string(REPLACE "v." "" WALLET_GIT_TAG ${WALLET_GIT_TAG})

    # Parse and split a version number into separate components
	string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)(.*)?" "\\1;\\2;\\3;\\4" VERSION_MATCHES ${WALLET_GIT_TAG})
    list(GET VERSION_MATCHES 0 version_major)
    set(WALLET_VER_MAJOR ${version_major})
    list(GET VERSION_MATCHES 1 version_minor)
    set(WALLET_VER_MINOR ${version_minor})
    list(GET VERSION_MATCHES 2 version_patch)
    set(WALLET_VER_PATCH ${version_patch})

    message(STATUS "Wallet git tag: ${WALLET_GIT_TAG}")
  endif()
endif()  

set(WALLET_VER_BUILD 1) # this one was always 1 anyways

set(WALLET_VENDOR_NAME "Karbo Developers")
set(WALLET_COPYRIGHT "Copyright 2016-2020 ${PROJECT_VENDOR_NAME}")
set(WALLET_VENDOR_URL "https://karbo.org")

set(WALLET_VERSION "${WALLET_VER_MAJOR}.${WALLET_VER_MINOR}.${WALLET_VER_PATCH}")

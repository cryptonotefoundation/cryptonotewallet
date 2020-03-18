cmake_minimum_required(VERSION 2.8)

include(CryptoNoteWallet.cmake)
include(external/ext.cmake)

if (NOT ARCH OR ARCH STREQUAL "" OR ARCH STREQUAL "native" OR ARCH STREQUAL "default")
  set(ARCH_ID "${CMAKE_SYSTEM_PROCESSOR}")
else()
  set(ARCH_ID "${ARCH}")
endif()
string(TOLOWER "${ARCH_ID}" ARM_ID)
if (ARM_ID)
  string(SUBSTRING "${ARM_ID}" 0 3 ARM_TEST)
endif()
if (ARM_TEST STREQUAL "arm")
  set(ARM 1)
  string(SUBSTRING "${ARM_ID}" 0 5 ARM_TEST)
  if (ARM_TEST STREQUAL "armv6")
    set(ARM6 1)
  endif()
  if (ARM_TEST STREQUAL "armv7")
    set(ARM7 1)
  endif()
endif()

if (ARM_ID STREQUAL "aarch64" OR ARM_ID STREQUAL "arm64" OR ARM_ID STREQUAL "armv8-a")
  set(ARM 1)
  set(ARM8 1)
  set(ARCH "armv8-a")
endif()

if(ARCH_ID STREQUAL "ppc64le")
  set(PPC64LE 1)
endif()

project(${CN_PROJECT_NAME})

execute_process(COMMAND git log -1 --pretty=format:%h
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE GIT_REVISION)

set(CRYPTONOTE_LIB cryptonote)

include_directories(${CMAKE_BINARY_DIR}
    src
    cryptonote/external
    cryptonote/include
    cryptonote/src)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(Boost_NO_BOOST_CMAKE ON)
set(Boost_USE_STATIC_LIBS ON)
if(WIN32)
  set(Boost_USE_STATIC_RUNTIME OFF)
else(WIN32)
  set(Boost_USE_STATIC_RUNTIME ON)
endif(WIN32)

find_package(Boost REQUIRED COMPONENTS date_time filesystem program_options regex serialization system thread chrono)
if ((${Boost_MAJOR_VERSION} EQUAL 1) AND (${Boost_MINOR_VERSION} EQUAL 54))
	message(SEND_ERROR "Boost version 1.54 is unsupported, more details are available here http://goo.gl/RrCFmA")
endif ()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(VERSION "")
configure_file("cryptonote/src/version.h.in" "version.h")
configure_file("src/CryptoNoteWalletConfig.h.in" "CryptoNoteWalletConfig.h")

add_definitions(-DVERSION=\"${CN_VERSION}\")
add_definitions(-DGIT_REVISION=\"${GIT_REVISION}\")
add_definitions(-DUPNP_STATIC -DMINIUPNP_STATICLIB -DUPNPC_BUILD_STATIC)

set(CMAKE_AUTOMOC ON)

set(CRYPTONOTE_SOURCES
  cryptonote/external/miniupnpc/connecthostport.c
  cryptonote/external/miniupnpc/igd_desc_parse.c
  cryptonote/external/miniupnpc/minisoap.c
  cryptonote/external/miniupnpc/minissdpc.c
  cryptonote/external/miniupnpc/miniupnpc.c
  cryptonote/external/miniupnpc/miniwget.c
  cryptonote/external/miniupnpc/minixml.c
  cryptonote/external/miniupnpc/portlistingparse.c
  cryptonote/external/miniupnpc/receivedata.c
  cryptonote/external/miniupnpc/upnpcommands.c
  cryptonote/external/miniupnpc/upnpdev.c
  cryptonote/external/miniupnpc/upnperrors.c
  cryptonote/external/miniupnpc/upnpreplyparse.c
  cryptonote/src/Common/Base58.cpp
  cryptonote/src/Common/Base64.cpp
  cryptonote/src/Common/CommandLine.cpp
  cryptonote/src/Common/FormatTools.cpp
  cryptonote/src/Common/Util.cpp
  cryptonote/src/Common/StringTools.cpp
  cryptonote/src/Common/JsonValue.cpp
  cryptonote/src/Common/ConsoleTools.cpp
  cryptonote/src/Common/MemoryInputStream.cpp
  cryptonote/src/Common/PathTools.cpp
  cryptonote/src/Common/PasswordContainer.cpp
  cryptonote/src/Common/DnsTools.cpp
  cryptonote/src/Common/StdInputStream.cpp
  cryptonote/src/Common/StdOutputStream.cpp
  cryptonote/src/Common/StreamTools.cpp
  cryptonote/src/Common/StringOutputStream.cpp
  cryptonote/src/Common/StringView.cpp
  cryptonote/src/Common/VectorOutputStream.cpp
  cryptonote/src/crypto/blake256.c
  cryptonote/src/crypto/chacha8.cpp
  cryptonote/src/crypto/crypto-ops-data.c
  cryptonote/src/crypto/crypto-ops.c
  cryptonote/src/crypto/crypto-util.c
  cryptonote/src/crypto/crypto.cpp
  cryptonote/src/crypto/groestl.c
  cryptonote/src/crypto/hash-extra-blake.c
  cryptonote/src/crypto/hash-extra-groestl.c
  cryptonote/src/crypto/hash-extra-jh.c
  cryptonote/src/crypto/hash-extra-skein.c
  cryptonote/src/crypto/hash.c
  cryptonote/src/crypto/jh.c
  cryptonote/src/crypto/keccak.c
  cryptonote/src/crypto/oaes_lib.c
  cryptonote/src/crypto/skein.c
  cryptonote/src/crypto/slow-hash.c
  cryptonote/src/crypto/slow-hash.cpp
  cryptonote/src/crypto/tree-hash.c
  cryptonote/src/CryptoNoteCore/BlockchainIndices.cpp
  cryptonote/src/CryptoNoteCore/BlockchainMessages.cpp
  cryptonote/src/CryptoNoteCore/BlockIndex.cpp
  cryptonote/src/CryptoNoteCore/CoreConfig.cpp
  cryptonote/src/CryptoNoteCore/CryptoNoteBasic.cpp
  cryptonote/src/CryptoNoteCore/CryptoNoteTools.cpp
  cryptonote/src/CryptoNoteCore/Currency.cpp
  cryptonote/src/CryptoNoteCore/MinerConfig.cpp
  cryptonote/src/CryptoNoteCore/Transaction.cpp
  cryptonote/src/CryptoNoteCore/Account.cpp
  cryptonote/src/CryptoNoteCore/Blockchain.cpp
  cryptonote/src/CryptoNoteCore/Checkpoints.cpp
  cryptonote/src/CryptoNoteCore/CryptoNoteBasicImpl.cpp
  cryptonote/src/CryptoNoteCore/Core.cpp
  cryptonote/src/CryptoNoteCore/CryptoNoteFormatUtils.cpp
  cryptonote/src/CryptoNoteCore/CryptoNoteSerialization.cpp
  cryptonote/src/CryptoNoteCore/Difficulty.cpp
  cryptonote/src/CryptoNoteCore/IBlock.cpp
  cryptonote/src/CryptoNoteCore/Miner.cpp
  cryptonote/src/CryptoNoteCore/TransactionExtra.cpp
  cryptonote/src/CryptoNoteCore/TransactionPool.cpp
  cryptonote/src/CryptoNoteCore/TransactionPrefixImpl.cpp
  cryptonote/src/CryptoNoteCore/TransactionUtils.cpp
  cryptonote/src/InProcessNode/InProcessNode.cpp
  cryptonote/src/InProcessNode/InProcessNodeErrors.cpp
  cryptonote/src/NodeRpcProxy/NodeErrors.cpp
  cryptonote/src/NodeRpcProxy/NodeRpcProxy.cpp
  cryptonote/src/P2p/NetNodeConfig.cpp
  cryptonote/src/Serialization/BinaryInputStreamSerializer.cpp
  cryptonote/src/Serialization/BinaryOutputStreamSerializer.cpp
  cryptonote/src/Serialization/JsonInputValueSerializer.cpp
  cryptonote/src/Serialization/JsonOutputStreamSerializer.cpp
  cryptonote/src/Serialization/KVBinaryInputStreamSerializer.cpp
  cryptonote/src/Serialization/KVBinaryOutputStreamSerializer.cpp
  cryptonote/src/Serialization/BlockchainExplorerDataSerialization.cpp
  cryptonote/src/Serialization/SerializationOverloads.cpp
  cryptonote/src/Transfers/BlockchainSynchronizer.cpp
  cryptonote/src/Transfers/SynchronizationState.cpp
  cryptonote/src/Transfers/TransfersConsumer.cpp
  cryptonote/src/Transfers/TransfersContainer.cpp
  cryptonote/src/Transfers/TransfersSubscription.cpp
  cryptonote/src/Transfers/TransfersSynchronizer.cpp
  cryptonote/src/Wallet/LegacyKeysImporter.cpp
  cryptonote/src/Wallet/WalletAsyncContextCounter.cpp
  cryptonote/src/Wallet/WalletErrors.cpp
  cryptonote/src/Wallet/WalletSerializationV1.cpp
  cryptonote/src/Wallet/WalletSerializationV2.cpp
  cryptonote/src/Wallet/WalletUtils.cpp
  cryptonote/src/WalletLegacy/KeysStorage.cpp
  cryptonote/src/WalletLegacy/WalletLegacy.cpp
  cryptonote/src/WalletLegacy/WalletHelper.cpp
  cryptonote/src/WalletLegacy/WalletLegacySerializer.cpp
  cryptonote/src/WalletLegacy/WalletLegacySerialization.cpp
  cryptonote/src/WalletLegacy/WalletTransactionSender.cpp
  cryptonote/src/WalletLegacy/WalletUnconfirmedTransactions.cpp
  cryptonote/src/WalletLegacy/WalletUserTransactionsCache.cpp
  cryptonote/src/System/ContextGroup.cpp
  cryptonote/src/System/Event.cpp
  cryptonote/src/System/EventLock.cpp
  cryptonote/src/System/InterruptedException.cpp
  cryptonote/src/System/Ipv4Address.cpp
  cryptonote/src/System/TcpStream.cpp
  cryptonote/src/HTTP/HttpRequest.cpp
  cryptonote/src/HTTP/HttpParser.cpp
  cryptonote/src/HTTP/HttpParserErrorCodes.cpp
  cryptonote/src/HTTP/HttpResponse.cpp
  cryptonote/src/Rpc/HttpClient.cpp
  cryptonote/src/Rpc/JsonRpc.cpp
  cryptonote/src/P2p/NetNode.cpp
  cryptonote/src/P2p/LevinProtocol.cpp
  cryptonote/src/P2p/NetNodeConfig.cpp
  cryptonote/src/P2p/PeerListManager.cpp
  cryptonote/src/CryptoNoteProtocol/CryptoNoteProtocolHandler.cpp
  cryptonote/src/Logging/ILogger.cpp
  cryptonote/src/Logging/LoggerGroup.cpp
  cryptonote/src/Logging/CommonLogger.cpp
  cryptonote/src/Logging/LoggerManager.cpp
  cryptonote/src/Logging/FileLogger.cpp
  cryptonote/src/Logging/StreamLogger.cpp
  cryptonote/src/Logging/ConsoleLogger.cpp
  cryptonote/src/Logging/LoggerMessage.cpp
  cryptonote/src/Logging/LoggerRef.cpp
  cryptonote/src/BlockchainExplorer/BlockchainExplorer.cpp
  cryptonote/src/BlockchainExplorer/BlockchainExplorerDataBuilder.cpp
  cryptonote/src/BlockchainExplorer/BlockchainExplorerErrors.cpp
)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE FORMS src/gui/ui/*.ui)
file(GLOB_RECURSE Mnemonics cryptonote/src/Mnemonics/*)
file(GLOB TRANSLATION_FILES src/languages/*.ts)

set(QRC
	src/resources.qrc
	src/qdarkstyle/style.qrc)
set_source_files_properties(${TRANSLATION_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/languages")

qt5_wrap_ui(UIS ${FORMS})
qt5_add_resources(RCC ${QRC})
#qt5_create_translation(TRANSLATION_MESSAGES ${FORMS} ${TRANSLATION_FILES})
qt5_add_translation(TRANSLATION_QM ${TRANSLATION_FILES})

add_custom_target(translations DEPENDS ${TRANSLATION_QM})
#add_custom_target(translations_update DEPENDS ${TRANSLATION_MESSAGES})

if (WIN32)
  file(GLOB _TRANSLATION_FILES_DEFAULT ${_qt5Core_install_prefix}/translations/*.qm)
  list(FILTER _TRANSLATION_FILES_DEFAULT INCLUDE REGEX "\\/[a-z][a-z]_[a-z][a-z].qm$")
  add_custom_target(translations_delault)
  foreach(_qm_file_src ${_TRANSLATION_FILES_DEFAULT})
    get_filename_component(_qm_file_name ${_qm_file_src} NAME)
    set(_qm_file_dst "${CMAKE_BINARY_DIR}/languages/${_qm_file_name}")
    add_custom_command(TARGET translations_delault
                       COMMAND ${CMAKE_COMMAND} -E copy ${_qm_file_src} ${_qm_file_dst})
  endforeach()
endif()

if (WIN32)
  if (NOT MSVC)
	message(FATAL_ERROR "Only MSVC is supported on this platform")
  endif ()
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x0600 /DSTATICLIB)
  include_directories(cryptonote/src/platform/msc)
  set( CMAKE_CXX_FLAGS "-bigobj -EHsc")
  set(PLATFORM_DIR Windows)
  set(BUILD_PLATFORM WIN32)
  set(BUILD_RESOURCES src/cryptonotewallet.rc)
  set(QTMAIN Qt5::WinMain)

elseif (UNIX)
  set(CRYPTONOTE_SOURCES ${CRYPTONOTE_SOURCES} cryptonote/external/miniupnpc/minissdpc.c)
  if (APPLE)
    enable_language(ASM)
    file(GLOB_RECURSE OBJC_SOURCES src/*.mm)
    set(SOURCES ${SOURCES} ${OBJC_SOURCES})
    set(PLATFORM_DIR OSX)
    set(MACOSX_BUNDLE_INFO_STRING "Karbowanec GUI Wallet")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")
    set(MACOSX_BUNDLE_BUNDLE_NAME "Karbo Wallet")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "$${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")

    find_package(Qt5PrintSupport REQUIRED)

    include_directories(/usr/include/malloc)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -std=c++11 -stdlib=libc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -maes -D_DARWIN_C_SOURCE")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework CoreFoundation -framework Carbon -framework IOKit -L/usr/lib")

    set(MACOSX_BUNDLE_ICON_FILE karbowanec.icns)
    set(APPLICATION_ICON src/images/karbowanec.icns)
    set_source_files_properties(${APPLICATION_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set(BUILD_PLATFORM MACOSX_BUNDLE)
    set(BUILD_RESOURCES ${APPLICATION_ICON})
	
    GET_TARGET_PROPERTY(QT_LIB_DIR "${Qt5Widgets_LIBRARIES}" LOCATION)
    GET_FILENAME_COMPONENT(QT_LIB_DIR "${QT_LIB_DIR}" PATH)
  else()
    include(TestCXXAcceptsFlag)
    if (CMAKE_SYSTEM_NAME MATCHES "kFreeBSD.*|FreeBSD")
      set(PLATFORM_DIR "FreeBSD")
      enable_language(ASM)
    else()
      set(PLATFORM_DIR "Linux")
    endif()
    list(REMOVE_ITEM HEADERS ${CMAKE_SOURCE_DIR}/src/gui/macdockiconhandler.h)

    set(ARCH native CACHE STRING "CPU to build for: -march value or default to not pass -march at all")
    message(STATUS "Building on ${CMAKE_SYSTEM_PROCESSOR} for ${ARCH}")
    if ("${ARCH}" STREQUAL "default")
      set(ARCH_FLAG "")
    elseif(PPC64LE)
      set(ARCH_FLAG "-mcpu=${ARCH}")
    else()
      set(ARCH_FLAG "-march=${ARCH}")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GNU_SOURCE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_GNU_SOURCE")

    if (NOT ARM AND NOT PPC64LE)
      message(STATUS "AES support enabled")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes")
    elseif (PPC64LE)
      message(STATUS "AES support not available on ppc64le")
    elseif (ARM6)
      message(STATUS "AES support not available on ARMv6")
    elseif (ARM7)
      message(STATUS "AES support not available on ARMv7")
    elseif (ARM8)
      CHECK_CXX_ACCEPTS_FLAG("-march=${ARCH}+crypto" ARCH_PLUS_CRYPTO)
      if (ARCH_PLUS_CRYPTO)
        message(STATUS "Crypto extensions enabled for ARMv8")
        set(ARCH_FLAG "-march=${ARCH}+crypto")
      else()
        message(STATUS "Crypto extensions unavailable on your ARMv8 device")
      endif()
    else()
      message(STATUS "AES support disabled")
    endif()

    # Compiled binary malfunctions due to aliasing (GCC 6.1+)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCH_FLAG}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_FLAG}")

    if (ARM)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    endif()

    if (ARM)
      message(STATUS "Setting FPU Flags for ARM Processors")
      # NB NEON hardware does not fully implement the IEEE 754 standard for floating-point arithmetic
      # Need custom assembly code to take full advantage of NEON SIMD
      #
      # Cortex-A5/9  -mfpu=neon-fp16
      # Cortex-A7/15 -mfpu=neon-vfpv4
      # Cortex-A8    -mfpu=neon
      # ARMv8        -FP and SIMD on by default for all ARM8v-A series, NO -mfpu setting needed
      #
      # For custom -mtune, processor IDs for ARMv8-A series:
      # 0xd04 - Cortex-A35
      # 0xd07 - Cortex-A57
      # 0xd08 - Cortex-A72
      # 0xd03 - Cortex-A73
      if (NOT ARM8)
        CHECK_CXX_ACCEPTS_FLAG(-mfpu=vfp3-d16 CXX_ACCEPTS_VFP3_D16)
        CHECK_CXX_ACCEPTS_FLAG(-mfpu=vfp4 CXX_ACCEPTS_VFP4)
        CHECK_CXX_ACCEPTS_FLAG(-mfloat-abi=hard CXX_ACCEPTS_MFLOAT_HARD)
        CHECK_CXX_ACCEPTS_FLAG(-mfloat-abi=softfp CXX_ACCEPTS_MFLOAT_SOFTFP)
      endif()
      if (ARM8)
        CHECK_CXX_ACCEPTS_FLAG(-mfix-cortex-a53-835769 CXX_ACCEPTS_MFIX_CORTEX_A53_835769)
        CHECK_CXX_ACCEPTS_FLAG(-mfix-cortex-a53-843419 CXX_ACCEPTS_MFIX_CORTEX_A53_843419)
      endif()
      if (ARM6)
        message(STATUS "Selecting VFP for ARMv6")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfp")
      endif()
      if (ARM7)
        if (CXX_ACCEPTS_VFP3_D16 AND NOT CXX_ACCEPTS_VFP4)
          message(STATUS "Selecting VFP3 for ARMv7")
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp3-d16")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfp3-d16")
        endif()
        if (CXX_ACCEPTS_VFP4)
          message(STATUS "Selecting VFP4 for ARMv7")
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp4")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfp4")
        endif()
        if (CXX_ACCEPTS_MFLOAT_HARD)
          message(STATUS "Setting Hardware ABI for Floating Point")
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
        endif()
        if (CXX_ACCEPTS_MFLOAT_SOFTFP AND NOT CXX_ACCEPTS_MFLOAT_HARD)
          message(STATUS "Setting Software ABI for Floating Point")
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=softfp")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=softfp")
        endif()
      endif()
      if (ARM8)
        if (CXX_ACCEPTS_MFIX_CORTEX_A53_835769)
          message(STATUS "Enabling Cortex-A53 workaround 835769")
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfix-cortex-a53-835769")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfix-cortex-a53-835769")
        endif()
        if (CXX_ACCEPTS_MFIX_CORTEX_A53_843419)
          message(STATUS "Enabling Cortex-A53 workaround 843419")
          set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfix-cortex-a53-843419")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfix-cortex-a53-843419")
        endif()
      endif()
    endif()

  endif()
endif ()

include_directories(cryptonote/src/Platform/${PLATFORM_DIR})
include_directories(cryptonote/src/Platform/Posix)
file(GLOB PLATFORM_SOURCES cryptonote/src/Platform/${PLATFORM_DIR}/System/*)
set(CRYPTONOTE_SOURCES ${CRYPTONOTE_SOURCES} ${PLATFORM_SOURCES})

add_library(${CRYPTONOTE_LIB} STATIC ${CRYPTONOTE_SOURCES})
add_library(Mnemonics ${Mnemonics})
set_target_properties(${CRYPTONOTE_LIB} PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)

if (APPLE OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
target_link_libraries(${CRYPTONOTE_LIB} ${Boost_LIBRARIES} -lresolv)
else ()
target_link_libraries(${CRYPTONOTE_LIB} ${Boost_LIBRARIES})
endif ()
set_target_properties(Mnemonics PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)
target_link_libraries(Mnemonics ${Boost_LIBRARIES})

add_executable(${PROJECT_NAME} ${BUILD_PLATFORM} ${BUILD_RESOURCES} ${SOURCES} ${HEADERS} ${UIS} ${RCC})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${QTMAIN} ${CRYPTONOTE_LIB} ${QRENCODE_LIB} Mnemonics)
add_dependencies(${PROJECT_NAME} translations)
if (WIN32)
  add_dependencies(${PROJECT_NAME} translations_delault)
endif ()

if (APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
  
  target_link_libraries(${PROJECT_NAME} Qt5::PrintSupport) 
elseif (UNIX)
  target_link_libraries(${PROJECT_NAME} -lpthread)
elseif (WIN32)
  target_link_libraries(${PROJECT_NAME} Imm32 Iphlpapi Winmm)
endif (APPLE)

qt5_use_modules(${PROJECT_NAME} Widgets Gui Network)

# Installation

set(CPACK_PACKAGE_NAME ${CN_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CN_VERSION})
set(CPACK_PACKAGE_VENDOR "Karbowanec-project")
set(CPACK_PACKAGE_CONTACT "https://karbo.org")
set(CPACK_STRIP_FILES ON)
if (APPLE)
    set(CPACK_GENERATOR DragNDrop)
	set(CPACK_BUNDLE_PLIST ${CMAKE_SOURCE_DIR}/Info.plist)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
	install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
elseif (UNIX)
    find_program(RPMBUILD rpmbuild)

    install(PROGRAMS ${CMAKE_BINARY_DIR}/${PROJECT_NAME} DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(FILES src/karbowanecwallet.desktop DESTINATION share/applications)
    install(FILES src/images/karbowanez.png DESTINATION share/pixmaps RENAME karbowanec.png)
    install(FILES copyright DESTINATION share/doc/karbowanecwallet)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/languages" DESTINATION /opt/karbo/ FILES_MATCHING PATTERN "*.qm")

    if (RPMBUILD)
        set(CPACK_GENERATOR "RPM;DEB")
        set(CPACK_SYSTEM_NAME x86_64)
        set(CPACK_RPM_PACKAGE_RELEASE ${CN_VERSION})
        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
        set(CPACK_RPM_PACKAGE_GROUP Office)
        set(CPACK_RPM_PACKAGE_REQUIRES "qt5-qtbase >= 5.3.2, qt5-qtbase-gui >= 5.3.2")
        set(CPACK_RPM_PACKAGE_SUMMARY "Karbowanec KRB wallet")
        set(CPACK_RPM_PACKAGE_DESCRIPTION "Karbowanec KRB wallet
 Karbowanec is Ukrainian decentrilized, privacy oriented peer-to-peer
 cryptocurrency. It is open-source, nobody owns or controls Karbowanec
 and everyone can take part.")
    else ()
		set(CPACK_GENERATOR DEB)
	endif ()
	
	set(CPACK_SYSTEM_NAME 64-bit)
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR} <krbcoin@ukr.net>")
	set(CPACK_DEBIAN_PACKAGE_SECTION Office)
	set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Karbowanec KRB wallet
 Karbowanec is Ukrainian decentralized, privacy oriented peer-to-peer
 cryptocurrency. It is open-source, nobody owns or controls Karbowanec
 and everyone can take part.")

	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
elseif (WIN32)
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)

	set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Karbowanec KRB wallet
 Karbowanec is Ukrainian decentralized, privacy oriented peer-to-peer
 cryptocurrency. It is open-source, nobody owns or controls Karbowanec
 and everyone can take part.")
    set (CPACK_PACKAGE_VERSION_MAJOR "1")
    set (CPACK_PACKAGE_VERSION_MINOR "4")
    set (CPACK_PACKAGE_VERSION_PATCH "1")
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "Karbowanec")

    # Define components and their display names
    set (CPACK_COMPONENTS_ALL applications)
    set (CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Karbo wallet")

    # Human readable component descriptions
    set (CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
      "Ukrainian decentralized, privacy oriented peer-to-peer cryptocurrency")

    # Define groups
    set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")

    set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION
       "Ukrainian decentralized, privacy oriented peer-to-peer cryptocurrency")

    # Define NSIS installation types
    set(CPACK_ALL_INSTALL_TYPES Full)
    set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)

    #install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
    set(CPACK_GENERATOR NSIS)
endif (APPLE)

include(CPack)

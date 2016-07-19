cmake_minimum_required(VERSION 2.8)

include(CryptoNoteWallet.cmake)

project(${CN_PROJECT_NAME})

execute_process(COMMAND git log -1 --pretty=format:%h
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE GIT_REVISION)

set(CRYPTONOTE_LIB cryptonote)

include_directories(${CMAKE_BINARY_DIR}
    src
    Lyte-Cryptocurency/external
    Lyte-Cryptocurency/include
    Lyte-Cryptocurency/src)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
if(WIN32)
  set(Boost_USE_STATIC_RUNTIME OFF)
else(WIN32)
  set(Boost_USE_STATIC_RUNTIME ON)
endif(WIN32)

find_package(Boost 1.55 REQUIRED COMPONENTS date_time filesystem program_options regex serialization system thread chrono)
if ((${Boost_MAJOR_VERSION} EQUAL 1) AND (${Boost_MINOR_VERSION} EQUAL 54))
	message(SEND_ERROR "Boost version 1.54 is unsupported, more details are available here http://goo.gl/RrCFmA")
endif ()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(VERSION "")
configure_file("Lyte-Cryptocurency/src/version.h.in" "version.h")
configure_file("src/CryptoNoteWalletConfig.h.in" "CryptoNoteWalletConfig.h")

add_definitions(-DGIT_REVISION=\"${GIT_REVISION}\")

set(CMAKE_AUTOMOC ON)

set(CRYPTONOTE_SOURCES
  Lyte-Cryptocurency/external/miniupnpc/connecthostport.c
  Lyte-Cryptocurency/external/miniupnpc/igd_desc_parse.c
  Lyte-Cryptocurency/external/miniupnpc/minisoap.c
  Lyte-Cryptocurency/external/miniupnpc/miniupnpc.c
  Lyte-Cryptocurency/external/miniupnpc/miniwget.c
  Lyte-Cryptocurency/external/miniupnpc/minixml.c
  Lyte-Cryptocurency/external/miniupnpc/portlistingparse.c
  Lyte-Cryptocurency/external/miniupnpc/receivedata.c
  Lyte-Cryptocurency/external/miniupnpc/upnpcommands.c
  Lyte-Cryptocurency/external/miniupnpc/upnpreplyparse.c
  Lyte-Cryptocurency/src/Common/Base58.cpp
  Lyte-Cryptocurency/src/Common/CommandLine.cpp
  Lyte-Cryptocurency/src/Common/Util.cpp
  Lyte-Cryptocurency/src/Common/StringTools.cpp
  Lyte-Cryptocurency/src/Common/JsonValue.cpp
  Lyte-Cryptocurency/src/Common/ConsoleTools.cpp
  Lyte-Cryptocurency/src/Common/MemoryInputStream.cpp
  Lyte-Cryptocurency/src/Common/PathTools.cpp
  Lyte-Cryptocurency/src/Common/StdInputStream.cpp
  Lyte-Cryptocurency/src/Common/StdOutputStream.cpp
  Lyte-Cryptocurency/src/Common/StreamTools.cpp
  Lyte-Cryptocurency/src/Common/StringOutputStream.cpp
  Lyte-Cryptocurency/src/Common/StringView.cpp
  Lyte-Cryptocurency/src/Common/VectorOutputStream.cpp
  Lyte-Cryptocurency/src/crypto/blake256.c
  Lyte-Cryptocurency/src/crypto/chacha8.c
  Lyte-Cryptocurency/src/crypto/crypto-ops-data.c
  Lyte-Cryptocurency/src/crypto/crypto-ops.c
  Lyte-Cryptocurency/src/crypto/crypto.cpp
  Lyte-Cryptocurency/src/crypto/groestl.c
  Lyte-Cryptocurency/src/crypto/hash-extra-blake.c
  Lyte-Cryptocurency/src/crypto/hash-extra-groestl.c
  Lyte-Cryptocurency/src/crypto/hash-extra-jh.c
  Lyte-Cryptocurency/src/crypto/hash-extra-skein.c
  Lyte-Cryptocurency/src/crypto/hash.c
  Lyte-Cryptocurency/src/crypto/jh.c
  Lyte-Cryptocurency/src/crypto/keccak.c
  Lyte-Cryptocurency/src/crypto/oaes_lib.c
  Lyte-Cryptocurency/src/crypto/random.c
  Lyte-Cryptocurency/src/crypto/skein.c
  Lyte-Cryptocurency/src/crypto/slow-hash.c
  Lyte-Cryptocurency/src/crypto/slow-hash.cpp
  Lyte-Cryptocurency/src/crypto/tree-hash.c
  Lyte-Cryptocurency/src/CryptoNoteCore/BlockchainIndices.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/BlockchainMessages.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/BlockIndex.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/CoreConfig.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/CryptoNoteBasic.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/CryptoNoteTools.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Currency.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/MinerConfig.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Transaction.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Account.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Blockchain.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Checkpoints.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/CryptoNoteBasicImpl.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Core.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/CryptoNoteFormatUtils.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/CryptoNoteSerialization.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Difficulty.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/IBlock.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/Miner.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/TransactionExtra.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/TransactionPool.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/TransactionPrefixImpl.cpp
  Lyte-Cryptocurency/src/CryptoNoteCore/TransactionUtils.cpp
  Lyte-Cryptocurency/src/InProcessNode/InProcessNode.cpp
  Lyte-Cryptocurency/src/InProcessNode/InProcessNodeErrors.cpp
  Lyte-Cryptocurency/src/NodeRpcProxy/NodeErrors.cpp
  Lyte-Cryptocurency/src/NodeRpcProxy/NodeRpcProxy.cpp
  Lyte-Cryptocurency/src/P2p/NetNodeConfig.cpp
  Lyte-Cryptocurency/src/Serialization/BinaryInputStreamSerializer.cpp
  Lyte-Cryptocurency/src/Serialization/BinaryOutputStreamSerializer.cpp
  Lyte-Cryptocurency/src/Serialization/JsonInputValueSerializer.cpp
  Lyte-Cryptocurency/src/Serialization/JsonOutputStreamSerializer.cpp
  Lyte-Cryptocurency/src/Serialization/KVBinaryInputStreamSerializer.cpp
  Lyte-Cryptocurency/src/Serialization/KVBinaryOutputStreamSerializer.cpp
  Lyte-Cryptocurency/src/Serialization/SerializationOverloads.cpp
  Lyte-Cryptocurency/src/Transfers/BlockchainSynchronizer.cpp
  Lyte-Cryptocurency/src/Transfers/SynchronizationState.cpp
  Lyte-Cryptocurency/src/Transfers/TransfersConsumer.cpp
  Lyte-Cryptocurency/src/Transfers/TransfersContainer.cpp
  Lyte-Cryptocurency/src/Transfers/TransfersSubscription.cpp
  Lyte-Cryptocurency/src/Transfers/TransfersSynchronizer.cpp
  Lyte-Cryptocurency/src/Wallet/LegacyKeysImporter.cpp
  Lyte-Cryptocurency/src/Wallet/WalletAsyncContextCounter.cpp
  Lyte-Cryptocurency/src/Wallet/WalletErrors.cpp
  Lyte-Cryptocurency/src/WalletLegacy/KeysStorage.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletLegacy.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletHelper.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletLegacySerializer.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletLegacySerialization.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletTransactionSender.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletUnconfirmedTransactions.cpp
  Lyte-Cryptocurency/src/WalletLegacy/WalletUserTransactionsCache.cpp
  Lyte-Cryptocurency/src/System/ContextGroup.cpp
  Lyte-Cryptocurency/src/System/Event.cpp
  Lyte-Cryptocurency/src/System/EventLock.cpp
  Lyte-Cryptocurency/src/System/InterruptedException.cpp
  Lyte-Cryptocurency/src/System/Ipv4Address.cpp
  Lyte-Cryptocurency/src/System/TcpStream.cpp
  Lyte-Cryptocurency/src/HTTP/HttpRequest.cpp
  Lyte-Cryptocurency/src/HTTP/HttpParser.cpp
  Lyte-Cryptocurency/src/HTTP/HttpParserErrorCodes.cpp
  Lyte-Cryptocurency/src/HTTP/HttpResponse.cpp
  Lyte-Cryptocurency/src/Rpc/HttpClient.cpp
  Lyte-Cryptocurency/src/Rpc/JsonRpc.cpp
  Lyte-Cryptocurency/src/P2p/NetNode.cpp
  Lyte-Cryptocurency/src/P2p/LevinProtocol.cpp
  Lyte-Cryptocurency/src/P2p/NetNodeConfig.cpp
  Lyte-Cryptocurency/src/P2p/PeerListManager.cpp
  Lyte-Cryptocurency/src/CryptoNoteProtocol/CryptoNoteProtocolHandler.cpp
  Lyte-Cryptocurency/src/Logging/ILogger.cpp
  Lyte-Cryptocurency/src/Logging/LoggerGroup.cpp
  Lyte-Cryptocurency/src/Logging/CommonLogger.cpp
  Lyte-Cryptocurency/src/Logging/LoggerManager.cpp
  Lyte-Cryptocurency/src/Logging/FileLogger.cpp
  Lyte-Cryptocurency/src/Logging/StreamLogger.cpp
  Lyte-Cryptocurency/src/Logging/ConsoleLogger.cpp
  Lyte-Cryptocurency/src/Logging/LoggerMessage.cpp
  Lyte-Cryptocurency/src/Logging/LoggerRef.cpp
  Lyte-Cryptocurency/src/BlockchainExplorer/BlockchainExplorer.cpp
  Lyte-Cryptocurency/src/BlockchainExplorer/BlockchainExplorerDataBuilder.cpp
  Lyte-Cryptocurency/src/BlockchainExplorer/BlockchainExplorerErrors.cpp
)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE FORMS src/gui/ui/*.ui)

set(QRC src/resources.qrc)

qt5_wrap_ui(UIS ${FORMS})
qt5_add_resources(RCC ${QRC})


if (WIN32)
  if (NOT MSVC)
	message(FATAL_ERROR "Only MSVC is supported on this platform")
  endif ()
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x0600 /DSTATICLIB)
  include_directories(Lyte-Cryptocurency/src/platform/msc)

  set(PLATFORM_DIR Windows)
  set(BUILD_PLATFORM WIN32)
  set(BUILD_RESOURCES src/cryptonotewallet.rc)
  set(QTMAIN Qt5::WinMain)

elseif (UNIX)
  set(CRYPTONOTE_SOURCES ${CRYPTONOTE_SOURCES} Lyte-Cryptocurency/external/miniupnpc/minissdpc.c)
  if (APPLE)
    enable_language(ASM)
    file(GLOB_RECURSE OBJC_SOURCES src/*.mm)
    set(SOURCES ${SOURCES} ${OBJC_SOURCES})
    set(PLATFORM_DIR OSX)
    set(MACOSX_BUNDLE_INFO_STRING "Cryptonote GUI wallet")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")
    set(MACOSX_BUNDLE_BUNDLE_NAME CryptonoteWallet)
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "$${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")

    find_package(Qt5PrintSupport REQUIRED)

    include_directories(/usr/include/malloc)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -std=c++11 -stdlib=libc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -maes -D_DARWIN_C_SOURCE")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework CoreFoundation -framework Carbon -framework IOKit -L/usr/lib")

    set(MACOSX_BUNDLE_ICON_FILE cryptonote.icns)
    set(APPLICATION_ICON src/images/cryptonote.icns)
    set_source_files_properties(${APPLICATION_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set(BUILD_PLATFORM MACOSX_BUNDLE)
    set(BUILD_RESOURCES ${APPLICATION_ICON})
	
    GET_TARGET_PROPERTY(QT_LIB_DIR "${Qt5Widgets_LIBRARIES}" LOCATION)
    GET_FILENAME_COMPONENT(QT_LIB_DIR "${QT_LIB_DIR}" PATH)
  else(APPLE)
    set(PLATFORM_DIR Linux)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes -std=c11")
  endif (APPLE)
endif ()

include_directories(Lyte-Cryptocurency/src/Platform/${PLATFORM_DIR})
file(GLOB PLATFORM_SOURCES Lyte-Cryptocurency/src/Platform/${PLATFORM_DIR}/System/*)
set(CRYPTONOTE_SOURCES ${CRYPTONOTE_SOURCES} ${PLATFORM_SOURCES})

add_library(${CRYPTONOTE_LIB} STATIC ${CRYPTONOTE_SOURCES})
set_target_properties(${CRYPTONOTE_LIB} PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)
target_link_libraries(${CRYPTONOTE_LIB} ${Boost_LIBRARIES})

add_executable(${PROJECT_NAME} ${BUILD_PLATFORM} ${BUILD_RESOURCES} ${SOURCES} ${HEADERS} ${UIS} ${RCC})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${QTMAIN} ${CRYPTONOTE_LIB})

if (APPLE)
  qt5_use_modules(${PROJECT_NAME} PrintSupport)
elseif (UNIX)
  target_link_libraries(${PROJECT_NAME} -lpthread)
elseif (WIN32)
  target_link_libraries(${PROJECT_NAME} Imm32 Iphlpapi Winmm)
endif (APPLE)

qt5_use_modules(${PROJECT_NAME} Widgets Gui)

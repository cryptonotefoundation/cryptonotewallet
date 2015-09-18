cmake_minimum_required(VERSION 2.8)

include(CryptoNoteWallet.cmake)

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
configure_file("cryptonote/src/version.h.in" "version.h")
configure_file("src/CryptoNoteWalletConfig.h.in" "CryptoNoteWalletConfig.h")

add_definitions(-DGIT_REVISION=\"${GIT_REVISION}\")

set(CMAKE_AUTOMOC ON)

set(CRYPTONOTE_SOURCES
  cryptonote/external/miniupnpc/connecthostport.c
  cryptonote/external/miniupnpc/igd_desc_parse.c
  cryptonote/external/miniupnpc/minisoap.c
  cryptonote/external/miniupnpc/miniupnpc.c
  cryptonote/external/miniupnpc/miniwget.c
  cryptonote/external/miniupnpc/minixml.c
  cryptonote/external/miniupnpc/portlistingparse.c
  cryptonote/external/miniupnpc/receivedata.c
  cryptonote/external/miniupnpc/upnpcommands.c
  cryptonote/external/miniupnpc/upnpreplyparse.c
  cryptonote/src/Common/Base58.cpp
  cryptonote/src/Common/CommandLine.cpp
  cryptonote/src/Common/Util.cpp
  cryptonote/src/Common/StringTools.cpp
  cryptonote/src/Common/JsonValue.cpp
  cryptonote/src/Common/ConsoleTools.cpp
  cryptonote/src/Common/MemoryInputStream.cpp
  cryptonote/src/Common/PathTools.cpp
  cryptonote/src/Common/StdInputStream.cpp
  cryptonote/src/Common/StdOutputStream.cpp
  cryptonote/src/Common/StreamTools.cpp
  cryptonote/src/Common/StringOutputStream.cpp
  cryptonote/src/Common/StringView.cpp
  cryptonote/src/Common/VectorOutputStream.cpp
  cryptonote/src/crypto/blake256.c
  cryptonote/src/crypto/chacha8.c
  cryptonote/src/crypto/crypto-ops-data.c
  cryptonote/src/crypto/crypto-ops.c
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
  cryptonote/src/crypto/random.c
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

set(QRC src/resources.qrc)

qt5_wrap_ui(UIS ${FORMS})
qt5_add_resources(RCC ${QRC})


if (WIN32)
  if (NOT MSVC)
	message(FATAL_ERROR "Only MSVC is supported on this platform")
  endif ()
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x0600 /DSTATICLIB)
  include_directories(cryptonote/src/platform/msc)

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

include_directories(cryptonote/src/Platform/${PLATFORM_DIR})
file(GLOB PLATFORM_SOURCES cryptonote/src/Platform/${PLATFORM_DIR}/System/*)
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

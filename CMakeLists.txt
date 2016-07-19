cmake_minimum_required(VERSION 2.8)

include(CryptoNoteWallet.cmake)

project(${CN_PROJECT_NAME})

execute_process(COMMAND git log -1 --pretty=format:%h
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE GIT_REVISION)

set(CRYPTONOTE_LIB cryptonote)

include_directories(${CMAKE_BINARY_DIR}
    src
    Lyte-Cryptocurrency/external
    Lyte-Cryptocurrency/include
    Lyte-Cryptocurrency/src)

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
configure_file("Lyte-Cryptocurrency/src/version.h.in" "version.h")
configure_file("src/CryptoNoteWalletConfig.h.in" "CryptoNoteWalletConfig.h")

add_definitions(-DGIT_REVISION=\"${GIT_REVISION}\")

set(CMAKE_AUTOMOC ON)

set(CRYPTONOTE_SOURCES
  Lyte-Cryptocurrency/external/miniupnpc/connecthostport.c
  Lyte-Cryptocurrency/external/miniupnpc/igd_desc_parse.c
  Lyte-Cryptocurrency/external/miniupnpc/minisoap.c
  Lyte-Cryptocurrency/external/miniupnpc/miniupnpc.c
  Lyte-Cryptocurrency/external/miniupnpc/miniwget.c
  Lyte-Cryptocurrency/external/miniupnpc/minixml.c
  Lyte-Cryptocurrency/external/miniupnpc/portlistingparse.c
  Lyte-Cryptocurrency/external/miniupnpc/receivedata.c
  Lyte-Cryptocurrency/external/miniupnpc/upnpcommands.c
  Lyte-Cryptocurrency/external/miniupnpc/upnpreplyparse.c
  Lyte-Cryptocurrency/src/Common/Base58.cpp
  Lyte-Cryptocurrency/src/Common/CommandLine.cpp
  Lyte-Cryptocurrency/src/Common/Util.cpp
  Lyte-Cryptocurrency/src/Common/StringTools.cpp
  Lyte-Cryptocurrency/src/Common/JsonValue.cpp
  Lyte-Cryptocurrency/src/Common/ConsoleTools.cpp
  Lyte-Cryptocurrency/src/Common/MemoryInputStream.cpp
  Lyte-Cryptocurrency/src/Common/PathTools.cpp
  Lyte-Cryptocurrency/src/Common/StdInputStream.cpp
  Lyte-Cryptocurrency/src/Common/StdOutputStream.cpp
  Lyte-Cryptocurrency/src/Common/StreamTools.cpp
  Lyte-Cryptocurrency/src/Common/StringOutputStream.cpp
  Lyte-Cryptocurrency/src/Common/StringView.cpp
  Lyte-Cryptocurrency/src/Common/VectorOutputStream.cpp
  Lyte-Cryptocurrency/src/crypto/blake256.c
  Lyte-Cryptocurrency/src/crypto/chacha8.c
  Lyte-Cryptocurrency/src/crypto/crypto-ops-data.c
  Lyte-Cryptocurrency/src/crypto/crypto-ops.c
  Lyte-Cryptocurrency/src/crypto/crypto.cpp
  Lyte-Cryptocurrency/src/crypto/groestl.c
  Lyte-Cryptocurrency/src/crypto/hash-extra-blake.c
  Lyte-Cryptocurrency/src/crypto/hash-extra-groestl.c
  Lyte-Cryptocurrency/src/crypto/hash-extra-jh.c
  Lyte-Cryptocurrency/src/crypto/hash-extra-skein.c
  Lyte-Cryptocurrency/src/crypto/hash.c
  Lyte-Cryptocurrency/src/crypto/jh.c
  Lyte-Cryptocurrency/src/crypto/keccak.c
  Lyte-Cryptocurrency/src/crypto/oaes_lib.c
  Lyte-Cryptocurrency/src/crypto/random.c
  Lyte-Cryptocurrency/src/crypto/skein.c
  Lyte-Cryptocurrency/src/crypto/slow-hash.c
  Lyte-Cryptocurrency/src/crypto/slow-hash.cpp
  Lyte-Cryptocurrency/src/crypto/tree-hash.c
  Lyte-Cryptocurrency/src/CryptoNoteCore/BlockchainIndices.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/BlockchainMessages.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/BlockIndex.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/CoreConfig.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/CryptoNoteBasic.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/CryptoNoteTools.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Currency.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/MinerConfig.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Transaction.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Account.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Blockchain.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Checkpoints.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/CryptoNoteBasicImpl.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Core.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/CryptoNoteFormatUtils.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/CryptoNoteSerialization.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Difficulty.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/IBlock.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/Miner.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/TransactionExtra.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/TransactionPool.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/TransactionPrefixImpl.cpp
  Lyte-Cryptocurrency/src/CryptoNoteCore/TransactionUtils.cpp
  Lyte-Cryptocurrency/src/InProcessNode/InProcessNode.cpp
  Lyte-Cryptocurrency/src/InProcessNode/InProcessNodeErrors.cpp
  Lyte-Cryptocurrency/src/NodeRpcProxy/NodeErrors.cpp
  Lyte-Cryptocurrency/src/NodeRpcProxy/NodeRpcProxy.cpp
  Lyte-Cryptocurrency/src/P2p/NetNodeConfig.cpp
  Lyte-Cryptocurrency/src/Serialization/BinaryInputStreamSerializer.cpp
  Lyte-Cryptocurrency/src/Serialization/BinaryOutputStreamSerializer.cpp
  Lyte-Cryptocurrency/src/Serialization/JsonInputValueSerializer.cpp
  Lyte-Cryptocurrency/src/Serialization/JsonOutputStreamSerializer.cpp
  Lyte-Cryptocurrency/src/Serialization/KVBinaryInputStreamSerializer.cpp
  Lyte-Cryptocurrency/src/Serialization/KVBinaryOutputStreamSerializer.cpp
  Lyte-Cryptocurrency/src/Serialization/SerializationOverloads.cpp
  Lyte-Cryptocurrency/src/Transfers/BlockchainSynchronizer.cpp
  Lyte-Cryptocurrency/src/Transfers/SynchronizationState.cpp
  Lyte-Cryptocurrency/src/Transfers/TransfersConsumer.cpp
  Lyte-Cryptocurrency/src/Transfers/TransfersContainer.cpp
  Lyte-Cryptocurrency/src/Transfers/TransfersSubscription.cpp
  Lyte-Cryptocurrency/src/Transfers/TransfersSynchronizer.cpp
  Lyte-Cryptocurrency/src/Wallet/LegacyKeysImporter.cpp
  Lyte-Cryptocurrency/src/Wallet/WalletAsyncContextCounter.cpp
  Lyte-Cryptocurrency/src/Wallet/WalletErrors.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/KeysStorage.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletLegacy.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletHelper.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletLegacySerializer.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletLegacySerialization.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletTransactionSender.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletUnconfirmedTransactions.cpp
  Lyte-Cryptocurrency/src/WalletLegacy/WalletUserTransactionsCache.cpp
  Lyte-Cryptocurrency/src/System/ContextGroup.cpp
  Lyte-Cryptocurrency/src/System/Event.cpp
  Lyte-Cryptocurrency/src/System/EventLock.cpp
  Lyte-Cryptocurrency/src/System/InterruptedException.cpp
  Lyte-Cryptocurrency/src/System/Ipv4Address.cpp
  Lyte-Cryptocurrency/src/System/TcpStream.cpp
  Lyte-Cryptocurrency/src/HTTP/HttpRequest.cpp
  Lyte-Cryptocurrency/src/HTTP/HttpParser.cpp
  Lyte-Cryptocurrency/src/HTTP/HttpParserErrorCodes.cpp
  Lyte-Cryptocurrency/src/HTTP/HttpResponse.cpp
  Lyte-Cryptocurrency/src/Rpc/HttpClient.cpp
  Lyte-Cryptocurrency/src/Rpc/JsonRpc.cpp
  Lyte-Cryptocurrency/src/P2p/NetNode.cpp
  Lyte-Cryptocurrency/src/P2p/LevinProtocol.cpp
  Lyte-Cryptocurrency/src/P2p/NetNodeConfig.cpp
  Lyte-Cryptocurrency/src/P2p/PeerListManager.cpp
  Lyte-Cryptocurrency/src/CryptoNoteProtocol/CryptoNoteProtocolHandler.cpp
  Lyte-Cryptocurrency/src/Logging/ILogger.cpp
  Lyte-Cryptocurrency/src/Logging/LoggerGroup.cpp
  Lyte-Cryptocurrency/src/Logging/CommonLogger.cpp
  Lyte-Cryptocurrency/src/Logging/LoggerManager.cpp
  Lyte-Cryptocurrency/src/Logging/FileLogger.cpp
  Lyte-Cryptocurrency/src/Logging/StreamLogger.cpp
  Lyte-Cryptocurrency/src/Logging/ConsoleLogger.cpp
  Lyte-Cryptocurrency/src/Logging/LoggerMessage.cpp
  Lyte-Cryptocurrency/src/Logging/LoggerRef.cpp
  Lyte-Cryptocurrency/src/BlockchainExplorer/BlockchainExplorer.cpp
  Lyte-Cryptocurrency/src/BlockchainExplorer/BlockchainExplorerDataBuilder.cpp
  Lyte-Cryptocurrency/src/BlockchainExplorer/BlockchainExplorerErrors.cpp
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
  include_directories(Lyte-Cryptocurrency/src/platform/msc)

  set(PLATFORM_DIR Windows)
  set(BUILD_PLATFORM WIN32)
  set(BUILD_RESOURCES src/cryptonotewallet.rc)
  set(QTMAIN Qt5::WinMain)

elseif (UNIX)
  set(CRYPTONOTE_SOURCES ${CRYPTONOTE_SOURCES} Lyte-Cryptocurrency/external/miniupnpc/minissdpc.c)
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

include_directories(Lyte-Cryptocurrency/src/Platform/${PLATFORM_DIR})
file(GLOB PLATFORM_SOURCES Lyte-Cryptocurrency/src/Platform/${PLATFORM_DIR}/System/*)
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

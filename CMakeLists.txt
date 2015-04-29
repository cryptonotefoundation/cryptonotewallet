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
    cryptonote/src
    cryptonote/contrib/epee/include)

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
    cryptonote/contrib/epee/include/misc_log_ex.cpp
    cryptonote/contrib/epee/include/misc_os_dependent.cpp
    cryptonote/contrib/epee/include/string_tools.cpp
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
    cryptonote/src/common/base58.cpp
    cryptonote/src/common/command_line.cpp
    cryptonote/src/common/util.cpp
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
    cryptonote/src/cryptonote_core/BlockIndex.cpp
    cryptonote/src/cryptonote_core/CoreConfig.cpp
    cryptonote/src/cryptonote_core/Currency.cpp
    cryptonote/src/cryptonote_core/MinerConfig.cpp
    cryptonote/src/cryptonote_core/Transaction.cpp
    cryptonote/src/cryptonote_core/account.cpp
    cryptonote/src/cryptonote_core/blockchain_storage.cpp
    cryptonote/src/cryptonote_core/checkpoints.cpp
    cryptonote/src/cryptonote_core/cryptonote_basic_impl.cpp
    cryptonote/src/cryptonote_core/cryptonote_core.cpp
    cryptonote/src/cryptonote_core/cryptonote_format_utils.cpp
    cryptonote/src/cryptonote_core/cryptonote_serialization.cpp
    cryptonote/src/cryptonote_core/difficulty.cpp
    cryptonote/src/cryptonote_core/miner.cpp
    cryptonote/src/cryptonote_core/tx_pool.cpp
    cryptonote/src/inprocess_node/InProcessNode.cpp
    cryptonote/src/inprocess_node/InProcessNodeErrors.cpp
    cryptonote/src/node_rpc_proxy/NodeErrors.cpp
    cryptonote/src/node_rpc_proxy/NodeRpcProxy.cpp
    cryptonote/src/p2p/NetNodeConfig.cpp
    cryptonote/src/serialization/BinaryInputStreamSerializer.cpp
    cryptonote/src/serialization/BinaryOutputStreamSerializer.cpp
    cryptonote/src/transfers/BlockchainSynchronizer.cpp
    cryptonote/src/transfers/SynchronizationState.cpp
    cryptonote/src/transfers/TransfersConsumer.cpp
    cryptonote/src/transfers/TransfersContainer.cpp
    cryptonote/src/transfers/TransfersSubscription.cpp
    cryptonote/src/transfers/TransfersSynchronizer.cpp
    cryptonote/src/wallet/KeysStorage.cpp
    cryptonote/src/wallet/Wallet.cpp
    cryptonote/src/wallet/WalletAsyncContextCounter.cpp
    cryptonote/src/wallet/WalletErrors.cpp
    cryptonote/src/wallet/WalletSerializer.cpp
    cryptonote/src/wallet/WalletSerialization.cpp
    cryptonote/src/wallet/WalletTransactionSender.cpp
    cryptonote/src/wallet/WalletUnconfirmedTransactions.cpp
    cryptonote/src/wallet/WalletUserTransactionsCache.cpp
    cryptonote/src/wallet/LegacyKeysImporter.cpp
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

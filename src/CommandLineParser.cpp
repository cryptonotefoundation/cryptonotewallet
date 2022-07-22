// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2022, The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <Common/Util.h>
#include <CryptoNoteConfig.h>

#include "CommandLineParser.h"

namespace WalletGui {

CommandLineParser::CommandLineParser(QObject* _parent) : QObject(_parent), m_parser(), m_helpOption(m_parser.addHelpOption()),
  m_versionOption(m_parser.addVersionOption()),
  m_testnetOption("testnet", tr("Used to deploy test nets. Checkpoints and hardcoded seeds are ignored, network id is changed. "
    "Use it with –data-dir flag. The wallet must be launched with –testnet flag")),
  m_withoutCheckpointsOption("without-checkpoints", tr("Do not load checkpoints for full blocks validation")),
  m_p2pBindIpOption("p2p-bind-ip", tr("Interface for p2p network protocol"), tr("ip"), "0.0.0.0"),
  m_p2pBindPortOption("p2p-bind-port", tr("Port for p2p network protocol"), tr("port"), QString::number(CryptoNote::P2P_DEFAULT_PORT)),
  m_rpcBindIpOption("rpc-bind-ip", tr("Interface for RPC server"), tr("ip"), "127.0.0.1"),
  m_rpcBindPortOption("rpc-bind-port", tr("Port for RPC server"), tr("port"), QString::number(CryptoNote::RPC_DEFAULT_PORT)),
  m_rpcOption("rpc-server", tr("Run RPC server")),
  m_restrictedRpcOption("restricted-rpc", tr("Disallow some RPC server queries")),
  m_p2pExternalOption("p2p-external-port", tr("External port for p2p network protocol (if port forwarding used with NAT)"),
    tr("port"), 0),
  m_allowLocalIpOption("allow-local-ip", tr("Allow local ip add to peer list, mostly in debug purposes")),
  m_addPeerOption("add-peer", tr("Manually add peer to local peerlist"), tr("peer")),
  m_addPriorityNodeOption("add-priority-node", tr("Specify list of peers to connect to and attempt to keep the connection open"),
    tr("node")),
  m_addExclusiveNodeOption("add-exclusive-node", tr("Specify list of peers to connect to only. If this option is given the options "
    "add-priority-node and seed-node are ignored"), tr("node")),
  m_seedNodeOption("seed-node", tr("Connect to a node to retrieve peer addresses, and disconnect"), tr("node")),
  m_hideMyPortOption("hide-my-port", tr("Do not announce yourself as peerlist candidate")),
  m_portableOption("portable", tr("Keep files in the same directory as wallet executable")),
  m_dataDirOption("data-dir", tr("Specify data directory"), tr("directory"), QString::fromLocal8Bit(Tools::getDefaultDataDirectory().c_str())),
  m_rollBackOption("rollback", tr("Rollback to height"), tr("height"), QString::number(std::numeric_limits<uint32_t>::max())),
  m_allowReorgOption("allow-reorg", tr("Allow deep reorganization to make it possible to self-heal chain split")),
  m_minimized("minimized", tr("Run application in minimized mode")) {
  m_parser.setApplicationDescription(tr("Karbowanec wallet"));
  m_parser.addHelpOption();
  m_parser.addVersionOption();
  m_parser.addOption(m_testnetOption);
  m_parser.addOption(m_withoutCheckpointsOption);
  m_parser.addOption(m_p2pBindIpOption);
  m_parser.addOption(m_p2pBindPortOption);
  m_parser.addOption(m_rpcBindIpOption);
  m_parser.addOption(m_rpcBindPortOption);
  m_parser.addOption(m_rpcOption);
  m_parser.addOption(m_restrictedRpcOption);
  m_parser.addOption(m_p2pExternalOption);
  m_parser.addOption(m_allowLocalIpOption);
  m_parser.addOption(m_addPeerOption);
  m_parser.addOption(m_addPriorityNodeOption);
  m_parser.addOption(m_addExclusiveNodeOption);
  m_parser.addOption(m_seedNodeOption);
  m_parser.addOption(m_hideMyPortOption);
  m_parser.addOption(m_portableOption);
  m_parser.addOption(m_dataDirOption);
  m_parser.addOption(m_rollBackOption);
  m_parser.addOption(m_allowReorgOption);
  m_parser.addOption(m_minimized);
}

CommandLineParser::~CommandLineParser() {
}

bool CommandLineParser::process(const QStringList& _argv) {
#ifdef Q_OS_WIN
  return m_parser.parse(_argv);
#else
  m_parser.process(_argv);
  return true;
#endif
}

bool CommandLineParser::hasHelpOption() const {
  return m_parser.isSet(m_helpOption);
}

bool CommandLineParser::hasMinimizedOption() const {
  return m_parser.isSet(m_minimized);
}

bool CommandLineParser::hasVersionOption() const {
  return m_parser.isSet(m_versionOption);
}

bool CommandLineParser::hasTestnetOption() const {
  return m_parser.isSet(m_testnetOption);
}

bool CommandLineParser::hasWithoutCheckpointsOption() const {
  return m_parser.isSet(m_withoutCheckpointsOption);
}

bool CommandLineParser::hasAllowLocalIpOption() const {
  return m_parser.isSet(m_allowLocalIpOption);
}

bool CommandLineParser::hasHideMyPortOption() const {
  return m_parser.isSet(m_hideMyPortOption);
}

bool CommandLineParser::hasPortableOption() const {
  return m_parser.isSet(m_portableOption);
}

bool CommandLineParser::hasAllowReorgOption() const {
  return m_parser.isSet(m_allowReorgOption);
}

QString CommandLineParser::getErrorText() const {
  return m_parser.errorText();
}

QString CommandLineParser::getHelpText() const {
  return m_parser.helpText();
}

QString CommandLineParser::getP2pBindIp() const {
  return m_parser.value(m_p2pBindIpOption);
}

quint16 CommandLineParser::getP2pBindPort() const {
  return m_parser.value(m_p2pBindPortOption).toUShort();
}

QString CommandLineParser::getRpcBindIp() const {
  return m_parser.value(m_rpcBindIpOption);
}

quint16 CommandLineParser::getRpcBindPort() const {
  return m_parser.value(m_rpcBindPortOption).toUShort();
}

bool CommandLineParser::hasRpcOption() const {
  return m_parser.isSet(m_rpcOption);
}

bool CommandLineParser::hasRestrictedRpcOption() const {
  return m_parser.isSet(m_restrictedRpcOption);
}

quint16 CommandLineParser::getP2pExternalPort() const {
  return m_parser.value(m_p2pExternalOption).toUShort();
}

QStringList CommandLineParser::getPeers() const {
  return m_parser.values(m_addPeerOption);
}

QStringList CommandLineParser::getPiorityNodes() const {
  return m_parser.values(m_addPriorityNodeOption);
}

QStringList CommandLineParser::getExclusiveNodes() const {
  return m_parser.values(m_addExclusiveNodeOption);
}

QStringList CommandLineParser::getSeedNodes() const {
  return m_parser.values(m_seedNodeOption);
}

QString CommandLineParser::getDataDir() const {
  if (hasPortableOption()) {
    return "./";
  }
  return m_parser.value(m_dataDirOption);
}

quint32 CommandLineParser::rollBack() const {
  return m_parser.value(m_rollBackOption).toULong();
}

}

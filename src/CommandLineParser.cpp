// Copyright (c) 2011-2015 The Cryptonote developers
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
  m_p2pBindIpOption("p2p-bind-ip", tr("Interface for p2p network protocol"), tr("ip"), "0.0.0.0"),
  m_p2pBindPortOption("p2p-bind-port", tr("Port for p2p network protocol"), tr("port"), QString::number(CryptoNote::P2P_DEFAULT_PORT)),
  m_p2pExternalOption("p2p-external-port", tr("xternal port for p2p network protocol (if port forwarding used with NAT)"),
    tr("port"), 0),
  m_allowLocalIpOption("allow-local-ip", tr("Allow local ip add to peer list, mostly in debug purposes")),
  m_addPeerOption("add-peer", tr("Manually add peer to local peerlist"), tr("peer")),
  m_addPriorityNodeOption("add-priority-node", tr("Specify list of peers to connect to and attempt to keep the connection open"),
    tr("node")),
  m_addExclusiveNodeOption("add-exclusive-node", tr("Specify list of peers to connect to only. If this option is given the options "
    "add-priority-node and seed-node are ignored"), tr("node")),
  m_seedNodeOption("seed-node", tr("Connect to a node to retrieve peer addresses, and disconnect"), tr("node")),
  m_hideMyPortOption("hide-my-port", tr("Do not announce yourself as peerlist candidate")),
  m_dataDirOption("data-dir", tr("Specify data directory"), tr("directory"), QString::fromStdString(Tools::getDefaultDataDirectory())),
  m_minimized("minimized", tr("Run application in minimized mode")) {
  m_parser.setApplicationDescription(tr("Bytecoin wallet"));
  m_parser.addHelpOption();
  m_parser.addVersionOption();
  m_parser.addOption(m_testnetOption);
  m_parser.addOption(m_p2pBindIpOption);
  m_parser.addOption(m_p2pBindPortOption);
  m_parser.addOption(m_p2pExternalOption);
  m_parser.addOption(m_allowLocalIpOption);
  m_parser.addOption(m_addPeerOption);
  m_parser.addOption(m_addPriorityNodeOption);
  m_parser.addOption(m_addExclusiveNodeOption);
  m_parser.addOption(m_seedNodeOption);
  m_parser.addOption(m_hideMyPortOption);
  m_parser.addOption(m_dataDirOption);
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

bool CommandLineParser::hasAllowLocalIpOption() const {
  return m_parser.isSet(m_allowLocalIpOption);
}

bool CommandLineParser::hasHideMyPortOption() const {
  return m_parser.isSet(m_hideMyPortOption);
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
  return m_parser.value(m_dataDirOption);
}

}

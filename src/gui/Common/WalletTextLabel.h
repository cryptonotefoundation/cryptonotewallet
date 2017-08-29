// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QLabel>

namespace WalletGui {

class WalletTextLabel : public QLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletTextLabel)

public:
  explicit WalletTextLabel(QWidget* _parent);
  ~WalletTextLabel();
};

class WalletTinyGrayTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletTinyGrayTextLabel)

public:
  explicit WalletTinyGrayTextLabel(QWidget* _parent);
  ~WalletTinyGrayTextLabel();
};

class WalletSmallGrayTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletSmallGrayTextLabel)

public:
  explicit WalletSmallGrayTextLabel(QWidget* _parent);
  ~WalletSmallGrayTextLabel();
};

class WalletSmallGreenBoldTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletSmallGreenBoldTextLabel)

public:
  explicit WalletSmallGreenBoldTextLabel(QWidget* _parent);
  ~WalletSmallGreenBoldTextLabel();
};

class WalletSmallBlackTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletSmallBlackTextLabel)

public:
  explicit WalletSmallBlackTextLabel(QWidget* _parent);
  ~WalletSmallBlackTextLabel();
};

class WalletNormalGrayTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletNormalGrayTextLabel)

public:
  explicit WalletNormalGrayTextLabel(QWidget* _parent);
  ~WalletNormalGrayTextLabel();
};

class WalletNormalGreenBoldTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletNormalGreenBoldTextLabel)

public:
  explicit WalletNormalGreenBoldTextLabel(QWidget* _parent);
  ~WalletNormalGreenBoldTextLabel();
};

class WalletExtraNormalGrayTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletExtraNormalGrayTextLabel)

public:
  explicit WalletExtraNormalGrayTextLabel(QWidget* _parent);
  ~WalletExtraNormalGrayTextLabel();
};

class WalletNormalBlackTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletNormalBlackTextLabel)

public:
  explicit WalletNormalBlackTextLabel(QWidget* _parent);
  ~WalletNormalBlackTextLabel();
};

class WalletExtraNormalBlackTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletExtraNormalBlackTextLabel)

public:
  explicit WalletExtraNormalBlackTextLabel(QWidget* _parent);
  ~WalletExtraNormalBlackTextLabel();
};

class WalletLargeBlackTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletLargeBlackTextLabel)

public:
  explicit WalletLargeBlackTextLabel(QWidget* _parent);
  ~WalletLargeBlackTextLabel();
};

class WalletExtraLargeBlackTextLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletExtraLargeBlackTextLabel)

public:
  explicit WalletExtraLargeBlackTextLabel(QWidget* _parent);
  ~WalletExtraLargeBlackTextLabel();
};

class WalletHeaderLabel : public WalletTextLabel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletHeaderLabel)

public:
  explicit WalletHeaderLabel(QWidget* _parent);
  ~WalletHeaderLabel();
};

}

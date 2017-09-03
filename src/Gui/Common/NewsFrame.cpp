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

#include <QDesktopServices>
#include <QTextDocument>
#include <QTextOption>
#include <QUrl>

#include "NewsFrame.h"
#include "Settings/Settings.h"
#include "Style/Style.h"
#include "ui_NewsFrame.h"

namespace WalletGui {

namespace {

const char NEWS_FRAME_STYLE_SHEEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--NewsFrame #m_linkLabel {"
    "color: %fontColorBlueNormal%;"
    "font-size:%fontSizeLarge%;"
  "}"

  "WalletGui--NewsFrame #m_linkLabel:hover {"
    "color: %fontColorBlueHover%;"
  "}"

  "WalletGui--NewsFrame #m_scrollFrame {"
    "min-width: 45px;"
    "max-width: 45px;"
    "border: none;"
    "border-left: 1px solid %borderColor%;"
    "background-color: %headerBackgroundColor%;"
  "}"

  "WalletGui--NewsFrame #m_scrollFrame QPushButton {"
    "border: none;"
  "}"

  "WalletGui--NewsFrame #m_scrollFrame #m_scrollUpButton {"
    "image: url(:icons/scroll-news-up);"
  "}"

  "WalletGui--NewsFrame #m_scrollFrame #m_scrollUpButton:hover {"
    "image: url(:icons/scroll-news-up-hover);"
  "}"

  "WalletGui--NewsFrame #m_scrollFrame #m_scrollDownButton {"
    "image: url(:icons/scroll-news-down);"
  "}"

  "WalletGui--NewsFrame #m_scrollFrame #m_scrollDownButton:hover {"
    "image: url(:icons/scroll-news-down-hover);"
  "}"
;

const char TEXT_HTML_TEMPLATE[] =
  "<html><body style=\"font-family:%fontFamily%;font-size:%fontSizeNormal%;\"><p><font color=\"%fontColorGray%\">%1</font>"
  " <font color=\"black\">%2</font></p></body></html>";

}

NewsFrame::NewsFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::NewsFrame), m_reader(nullptr) {
  m_ui->setupUi(this);
  m_ui->m_linkLabel->installEventFilter(this);
  for (quintptr i = 0; i < Settings::instance().getStyleCount(); ++i) {
    if (!Settings::instance().getStyle(i).getStyleName().compare("light", Qt::CaseInsensitive)) {
      setStyleSheet(Settings::instance().getStyle(i).makeStyleSheet(NEWS_FRAME_STYLE_SHEEET_TEMPLATE));
    }
  }

  m_ui->m_scrollFrame->setVisible(false);
}

NewsFrame::~NewsFrame() {
}

void NewsFrame::setNewsReader(INewsReader* _reader) {
  m_reader = _reader;
  m_reader->addObserver(this);
  m_ui->m_iconLabel->setPixmap(m_reader->getIcon());
  m_ui->m_nameLabel->setText(m_reader->getName());
  m_currentMessageId = m_reader->getLastMessageId();
  updateText();
  updateScrollButtons();
}

bool NewsFrame::eventFilter(QObject* _object, QEvent* _event) {
  if (_object == m_ui->m_linkLabel && _event->type() == QEvent::MouseButtonRelease) {
    if (!m_currentMessageId.isEmpty()) {
      QDesktopServices::openUrl(QUrl::fromUserInput(m_reader->getMessageSourceUrl(m_currentMessageId)));
      Q_EMIT linkActivatedSignal(m_reader->getMessageHeader(m_currentMessageId));
    }
  }

  return QFrame::eventFilter(_object, _event);
}

void NewsFrame::newMessage() {
  m_currentMessageId = m_reader->getLastMessageId();
  updateText();
  updateScrollButtons();
}

void NewsFrame::resizeEvent(QResizeEvent* _event) {
  QFrame::resizeEvent(_event);
  QMetaObject::invokeMethod(this, "updateText", Qt::QueuedConnection);
}

void NewsFrame::updateText() {
  if (m_currentMessageId.isEmpty()) {
    return;
  }

  updateSourceText();
  QString html = getElidedText(m_reader->getMessageText(m_currentMessageId),
    QLocale::c().toString(m_reader->getMessageTime(m_currentMessageId), "dd MMM yyyy"), m_ui->m_textLabel->size());
  m_ui->m_textLabel->setText(html);
}

void NewsFrame::updateSourceText() {
  QString header = m_reader->getMessageHeader(m_currentMessageId);
  QFont font;
  font.setPixelSize(Style::FONT_LARGE);
  QFontMetrics fm(font);
  header = fm.elidedText(header, Qt::ElideRight, m_ui->m_linkLabel->width());
  m_ui->m_linkLabel->setText(header);
}

void NewsFrame::updateScrollButtons() {
  if (m_currentMessageId.isEmpty()) {
    m_ui->m_scrollUpButton->setVisible(false);
    m_ui->m_scrollDownButton->setVisible(false);
    return;
  }

  QString nextMessageId = m_reader->getNextMessageId(m_currentMessageId);
  QString prevMessageId = m_reader->getPrevMessageId(m_currentMessageId);
  if (nextMessageId.isEmpty() && prevMessageId.isEmpty()) {
    m_ui->m_scrollFrame->setVisible(false);
    return;
  }

  m_ui->m_scrollFrame->setVisible(true);
  if (nextMessageId.isEmpty()) {
    m_ui->m_scrollUpButton->setVisible(false);
  } else {
    m_ui->m_scrollUpButton->setVisible(true);
  }

  if (prevMessageId.isEmpty()) {
    m_ui->m_scrollDownButton->setVisible(false);
  } else {
    m_ui->m_scrollDownButton->setVisible(true);
  }
}

void NewsFrame::upClicked() {
  m_currentMessageId = m_reader->getNextMessageId(m_currentMessageId);
  updateText();
  updateScrollButtons();
}

void NewsFrame::downClicked() {
  m_currentMessageId = m_reader->getPrevMessageId(m_currentMessageId);
  updateText();
  updateScrollButtons();
}

QString NewsFrame::getElidedText(const QString& _text, const QString& _timeString, const QSize& _size) {
  QString htmlTemplate = Settings::instance().getCurrentStyle().makeStyleSheet(TEXT_HTML_TEMPLATE);
  QString html = htmlTemplate.arg(_timeString).arg(_text);
  QTextDocument doc;
  doc.setHtml(html);
  doc.setTextWidth(_size.width());
  doc.setDocumentMargin(0);
  QTextOption option = doc.defaultTextOption();
  option.setAlignment(Qt::AlignLeft | Qt::AlignTop);
  doc.setDefaultTextOption(option);
  if (doc.size().height() <= _size.height() + 10) {
    return html;
  }

  for (int i = _text.indexOf(QRegExp("\\s+")); i != -1; i = _text.indexOf(QRegExp("\\s+"), i + 1)) {
    if (i >= 0) {
      html = htmlTemplate.arg(_timeString).arg(_text.left(i));
      doc.setHtml(html);
      if (_size.height() + 10 < doc.size().height() ) {
        i = _text.lastIndexOf(QRegExp("\\s+"), std::max(0, i - 1));
        i = _text.lastIndexOf(QRegExp("\\s+"), std::max(0, i - 1));
        html = htmlTemplate.arg(_timeString).arg(QString("%1%2").arg(_text.left(i)).arg("..."));
        return html;
      }
    }
  }

  return "";
}

}

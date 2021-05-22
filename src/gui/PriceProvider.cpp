// Copyright (c) 2011-2017 The Cryptonote developers
// Copyright (c) 2014-2017 XDN developers 
// Copyright (c) 2018 The Circle Foundation
//
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QStringList>
#include <QUrl>

#include "PriceProvider.h"

namespace WalletGui {

PriceProvider::PriceProvider(QObject *parent) : QObject(parent), m_networkManager() {
}

PriceProvider::~PriceProvider() {
}

void PriceProvider::getPrice() {
  QUrl url = QUrl::fromUserInput("http://explorer.fandom.gold/q/wallet/data.php");

  QNetworkRequest request(url);
  QNetworkReply* reply = m_networkManager.get(request);
  connect(reply, &QNetworkReply::readyRead, this, &PriceProvider::readyRead);
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}

void PriceProvider::readyRead() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
  QString data = (QString)reply->readAll();
  QStringList pairs = data.split(" ");
  QString usdxfg = pairs[1];
  QString usdmarketcap = pairs[2];
  QString usdvolume = pairs[3];

  Q_EMIT priceFoundSignal(usdxfg,usdmarketcap,usdvolume);

}

}
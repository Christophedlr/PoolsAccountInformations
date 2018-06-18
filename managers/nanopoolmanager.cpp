/*
 * Copyright 2018, Christophe Daloz - De Los Rios
 * This file is part of PoolsAccountInformations.
 *
 *  PoolsAccountInformations is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PoolsAccountInformations is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PoolsAccountInformations.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "nanopoolmanager.h"
#include <QJsonDocument>
#include <QJsonObject>

NanopoolManager::NanopoolManager()
{
}

/**
 * @brief Save nanopool configuration
 * @param settings
 */
void NanopoolManager::save(QStringList data)
{
    QSettings settings("christophedlr", "PoolsAccountInformations");

    settings.setValue(data[0]+"/pool", "Nanopool");
    settings.setValue(data[0]+"/wallet", data[1]);
    settings.setValue(data[0]+"/currency", data[2]);
    settings.setValue(data[0]+"/addr", data[3]);
    settings.setValue(data[0]+"/api", data[4]);
}

/**
 * @brief Read nanopool configuration
 * @param list
 * @param widget
 */
void NanopoolManager::read(QStringList list, QListWidget *widget)
{
    widget->clear();

    for (int i = 0; i < list.count(); ++i) {
        int pos = list[i].indexOf('/');
        QString element = list[i];
        widget->addItem(element.left(pos));
        i = i+4;
    }
}

void NanopoolManager::downloadSignal(QString pool, QString wallet, QString api, QString addr, QString currency, QNetworkAccessManager* manager)
{
    m_pool = pool;
    m_wallet = wallet;
    m_api = api;
    m_addr = addr;
    m_currency = currency;

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadResult(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(m_pool+"/user/"+m_wallet)));
    manager->get(QNetworkRequest(QUrl(m_pool+"/payments/"+m_wallet)));
}

void NanopoolManager::downloadResult(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject json = doc.object();
    QJsonObject forUi;

    if (json["status"] == true) {
        if (json["data"].toObject().contains("balance")) {
            forUi.insert("wallet", "Wallet: "+json["data"].toObject()["account"].toString());
            forUi.insert("hashrate", json["data"].toObject()["hashrate"].toString()+" H/s");
            forUi.insert("balance", json["data"].toObject()["balance"].toString()+" "+m_currency);
            forUi.insert("unconfimed", json["data"].toObject()["unconfirmed_balance"].toString()+" "+m_currency);

            forUi.insert("workers", json["data"].toObject()["workers"].toArray());
            /*ui->tbl_worker->setRowCount(workers.count());

            for (int i = 0; i < workers.count(); ++i) {
                QJsonObject worker = workers[i].toObject();

                QDateTime time;
                time.setTime_t(static_cast<int>(worker["lastshare"].toDouble()));

                ui->tbl_worker->setItem(i, 0, new QTableWidgetItem(worker["id"].toString()));
                ui->tbl_worker->setItem(i, 1, new QTableWidgetItem(time.toString(Qt::SystemLocaleShortDate)));
                ui->tbl_worker->setItem(i, 2, new QTableWidgetItem(QString::number(worker["rating"].toDouble())));
                ui->tbl_worker->setItem(i, 3, new QTableWidgetItem(worker["hashrate"].toString()+" H/s"));
            }

            ui->tbl_worker->resize(500, 400);
            ui->tbl_worker->show();*/
        } else {
            forUi.insert("payments", json["data"].toArray());
            //QJsonArray payments = ;
            //ui->tbl_payments->setRowCount(payments.count());

            /*for (int i = 0; i < payments.count(); ++i) {
                QJsonObject payment = payments[i].toObject();

                QDateTime time;
                time.setTime_t(static_cast<int>(payment["date"].toDouble()));
                QString confirmedMsg;

                ui->tbl_payments->setItem(i, 0, new QTableWidgetItem(time.toString(Qt::SystemLocaleShortDate)));
                ui->tbl_payments->setItem(i, 1, new QTableWidgetItem(QString::number(payment["amount"].toDouble())));
                ui->tbl_payments->setItem(i, 2, new QTableWidgetItem(payment["txHash"].toString()));

                if (payment["confirmed"].toBool()) {
                    ui->tbl_payments->setItem(i, 3, new QTableWidgetItem("Confirmed"));
                } else {
                    ui->tbl_payments->setItem(i, 3, new QTableWidgetItem("Unconfirmed"));
                }
            }

            ui->tbl_payments->resize(500, 400);
            ui->tbl_payments->show();*/
        }
    }

    //return forUi;
}

NanopoolManager::~NanopoolManager()
{
}

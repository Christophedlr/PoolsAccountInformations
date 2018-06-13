#include "displaydata.h"
#include "ui_displaydata.h"
#include <QDebug>

DisplayData::DisplayData(QString name, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayData)
{
    QStringList list;
    list << "Worker" << "Last share" << "Rating" << "Hashrate";

    ui->setupUi(this);
    ui->tbl_worker->setHorizontalHeaderLabels(list);

    QSettings settings("christophedlr", "PoolsAccountInformations");
    m_wallet = settings.value(name+"/wallet").toString();
    m_poolAddr = settings.value(name+"/addr").toString();
    m_poolApi = settings.value(name+"/api").toString();

    manager = new QNetworkAccessManager;
    this->downloadPoolData();
}

void DisplayData::downloadPoolData()
{
    manager->get(QNetworkRequest(QUrl(m_poolApi+"/user/"+m_wallet)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadResult(QNetworkReply*)));
}

void DisplayData::downloadResult(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject json = doc.object();

    if (json["status"] == true) {
        ui->lb_wallet->setText(json["data"].toObject()["account"].toString());
        ui->lb_current_hashrate->setText(json["data"].toObject()["hashrate"].toString()+" H/s");
        ui->lb_balance->setText(json["data"].toObject()["balance"].toString());
        ui->lb_unconfirmed_balance->setText(json["data"].toObject()["unconfirmed_balance"].toString());

        QJsonArray workers = json["data"].toObject()["workers"].toArray();
        ui->tbl_worker->setRowCount(workers.count());

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
        ui->tbl_worker->show();
    }
}

DisplayData::~DisplayData()
{
    delete manager;
    delete ui;
}

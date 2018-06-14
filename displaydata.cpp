#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QString name, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayData)
{
    QStringList list;
    list << "Worker" << "Last share" << "Rating" << "Hashrate";

    QStringList list2;
    list2 << "Date" << "Amount" << "txHash" << "State";

    ui->setupUi(this);
    ui->tbl_worker->setHorizontalHeaderLabels(list);
    ui->tbl_payments->setHorizontalHeaderLabels(list2);

    QSettings settings("christophedlr", "PoolsAccountInformations");
    m_wallet = settings.value(name+"/wallet").toString();
    m_poolAddr = settings.value(name+"/addr").toString();
    m_poolApi = settings.value(name+"/api").toString();
    m_currency = settings.value(name+"/currency").toString();

    manager = new QNetworkAccessManager;
    this->downloadPoolData();
}

void DisplayData::downloadPoolData()
{
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadResult(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(m_poolApi+"/user/"+m_wallet)));
    manager->get(QNetworkRequest(QUrl(m_poolApi+"/payments/"+m_wallet)));
}

void DisplayData::downloadResult(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject json = doc.object();

    if (json["status"] == true) {
        if (json["data"].toObject().contains("balance")) {
            ui->lb_wallet->setText("Wallet: "+json["data"].toObject()["account"].toString());
            ui->lb_current_hashrate->setText(json["data"].toObject()["hashrate"].toString()+" H/s");
            ui->lb_balance->setText(json["data"].toObject()["balance"].toString()+" "+m_currency);
            ui->lb_unconfirmed_balance->setText(json["data"].toObject()["unconfirmed_balance"].toString()+" "+m_currency);

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
        } else {
            QJsonArray payments = json["data"].toArray();
            ui->tbl_payments->setRowCount(payments.count());

            for (int i = 0; i < payments.count(); ++i) {
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
            ui->tbl_payments->show();
        }
    }
}

DisplayData::~DisplayData()
{
    delete manager;
    delete ui;
}

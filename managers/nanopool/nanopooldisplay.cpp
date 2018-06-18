#include "nanopooldisplay.h"
#include "ui_nanopooldisplay.h"

NanopoolDisplay::NanopoolDisplay(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NanopoolDisplay)
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
    m_addr = settings.value(name+"/addr").toString();
    m_api = settings.value(name+"/api").toString();
    m_currency = settings.value(name+"/currency").toString();
    m_pool = settings.value(name+"/pool").toString();

    manager = new QNetworkAccessManager;

    if (m_pool == "Nanopool") {
        nanopool = new NanopoolManager();

        this->downloadPoolData();
    }
}

void NanopoolDisplay::downloadPoolData()
{
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadResult(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(m_api+"/user/"+m_wallet)));
    manager->get(QNetworkRequest(QUrl(m_api+"/payments/"+m_wallet)));
}

void NanopoolDisplay::downloadResult(QNetworkReply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject json = doc.object();

    if (json["status"] == true) {
        if (json["data"].toObject().contains("balance")) {
            this->generalInfo(json["data"].toObject());
            this->workerInfo(json["data"].toObject());

        } else {
            this->paymentsInfo(json["data"].toArray());
        }
    }
}

/**
 * @brief Display balance, account, hashrate and wallet informations
 * @param json
 */
void NanopoolDisplay::generalInfo(QJsonObject json)
{
    ui->lb_wallet->setText("Wallet: "+json["account"].toString());
    ui->lb_current_hashrate->setText(json["hashrate"].toString()+" H/s");
    ui->lb_balance->setText(json["balance"].toString()+" "+m_currency);
    ui->lb_unconfirmed_balance->setText(json["unconfirmed_balance"].toString()+" "+m_currency);
}

/**
 * @brief Display workers informations
 * @param json
 */
void NanopoolDisplay::workerInfo(QJsonObject json)
{
    QJsonArray workers = json["workers"].toArray();
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

/**
 * @brief Display payments informations
 * @param json
 */
void NanopoolDisplay::paymentsInfo(QJsonArray payments)
{
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

NanopoolDisplay::~NanopoolDisplay()
{
    delete ui;
}

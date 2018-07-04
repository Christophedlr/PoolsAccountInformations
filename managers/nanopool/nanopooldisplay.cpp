#include "nanopooldisplay.h"
#include "ui_nanopooldisplay.h"

NanopoolDisplay::NanopoolDisplay(QString name, QWidget *parent) :
    QMainWindow(parent),
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
    status = new QLabel(this);
    ui->statusbar->addWidget(status);

    cyclicTime = new QTimer(this);
    connect(cyclicTime, SIGNAL(timeout()), this, SLOT(downloadPoolData()));

    m_replyUser = manager->get(QNetworkRequest(QUrl(m_api+"/user/"+m_wallet)));
    connect(m_replyUser, SIGNAL(finished()), this, SLOT(finishedDownloadUserData()));

    QDateTime time = QDateTime::currentDateTime();
    status->setText("Last update: "+time.toString(Qt::SystemLocaleShortDate));
    cyclicTime->start(600000);
}

/**
 * @brief Update hashrate, balance, unconfirmed balance, and workers information
 */
void NanopoolDisplay::finishedDownloadUserData()
{
    QJsonDocument jsonUser = QJsonDocument::fromJson(m_replyUser->readAll());

    if (jsonUser.object()["status"] == true) {
        QJsonObject json = jsonUser.object()["data"].toObject();

        //General informations
        m_balance = json["balance"].toString().toDouble();
        m_unconfirmedBalande = json["unconfirmed_balance"].toDouble();

        ui->lb_wallet->setText("Wallet: "+json["account"].toString());
        ui->lb_current_hashrate->setText(json["hashrate"].toString()+" H/s");
        ui->lb_balance->setText(json["balance"].toString()+" "+m_currency);
        ui->lb_unconfirmed_balance->setText(json["unconfirmed_balance"].toString()+" "+m_currency);

        //Workers informations
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

    m_replyPayments = manager->get(QNetworkRequest(QUrl(m_api+"/payments/"+m_wallet)));
    connect(m_replyPayments, SIGNAL(finished()), this, SLOT(finishedDownloadPaymentsData()));
}

/**
 * @brief Update payments information
 */
void NanopoolDisplay::finishedDownloadPaymentsData()
{
    QJsonDocument jsonPayments = QJsonDocument::fromJson(m_replyPayments->readAll());

    if (jsonPayments["status"] == true) {
        QJsonArray payments = jsonPayments.object()["data"].toArray();
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

    m_replyUsersettings = manager->get(QNetworkRequest(QUrl(m_api+"/usersettings/"+m_wallet)));
    connect(m_replyUsersettings, SIGNAL(finished()), this, SLOT(finishedDownloadSettingsData()));
}

/**
 * @brief Update payout information
 */
void NanopoolDisplay::finishedDownloadSettingsData()
{
    QJsonDocument jsonUsersettings = QJsonDocument::fromJson(m_replyUsersettings->readAll());
    m_payout = jsonUsersettings.object()["data"].toObject()["payout"].toDouble();

    if (jsonUsersettings["status"] == true) {
        ui->lb_payout->setText(QString::number(jsonUsersettings.object()["data"].toObject()["payout"].toDouble())+" "+m_currency);
    }

    m_replyPrice = manager->get(QNetworkRequest(QUrl("https://min-api.cryptocompare.com/data/price?fsym=ZEC&tsyms=EUR,USD&=Kraken")));
    connect(m_replyPrice, SIGNAL(finished()), this, SLOT(finishedDownloadPricesData()));
}

/**
 * @brief Update EUR and USD information
 */
void NanopoolDisplay::finishedDownloadPricesData()
{
    QJsonDocument jsonPrice = QJsonDocument::fromJson(m_replyPrice->readAll());

    double eurBalance = jsonPrice.object()["EUR"].toDouble()*m_balance;
    double usdBalance = jsonPrice.object()["USD"].toDouble()*m_balance;

    double eurUnconfirmedBalance = jsonPrice.object()["EUR"].toDouble()*m_unconfirmedBalande;
    double usdUnconfirmedBalance = jsonPrice.object()["USD"].toDouble()*m_unconfirmedBalande;

    double eurPayout = jsonPrice.object()["EUR"].toDouble()*m_payout;
    double usdPayout = jsonPrice.object()["USD"].toDouble()*m_payout;

    ui->lb_balance->setToolTip("<span style='color:black;font-weight: normal'>EUR: "+
                               QString::asprintf("%.2f", eurBalance) +", USD: "+
                               QString::asprintf("%.2f", usdBalance)+"</span>"
                               );

    ui->lb_unconfirmed_balance->setToolTip("<span style='color:black;font-weight: normal'>EUR: "+
                                           QString::asprintf("%.2f", eurUnconfirmedBalance)+", USD: "+
                                           QString::asprintf("%.2f", usdUnconfirmedBalance)+"</span>"
                                           );

    ui->lb_payout->setToolTip("<span style='color:black;font-weight: normal'>EUR: "+
                              QString::asprintf("%.2f", eurPayout)+", USD: "+
                              QString::asprintf("%.2f", usdPayout)+"</span"
                              );
}

NanopoolDisplay::~NanopoolDisplay()
{
    delete status;
    delete cyclicTime;
    delete ui;
}

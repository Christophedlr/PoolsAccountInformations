#ifndef NANOPOOLDISPLAY_H
#define NANOPOOLDISPLAY_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
#include "../nanopoolmanager.h"

namespace Ui {
class NanopoolDisplay;
}

class NanopoolDisplay : public QMainWindow
{
    Q_OBJECT

public:
    explicit NanopoolDisplay(QString name, QWidget *parent = 0);
    ~NanopoolDisplay();

public slots:
    void finishedDownloadUserData();
    void finishedDownloadPaymentsData();
    void finishedDownloadSettingsData();
    void finishedDownloadPricesData();
    void downloadPoolData();

private:
    Ui::NanopoolDisplay *ui;
    QNetworkAccessManager *manager;
    NanopoolManager* nanopool;
    QLabel* status;
    QTimer* cyclicTime;

    QString m_wallet;
    QString m_api;
    QString m_addr;
    QString m_currency;
    QString m_pool;

    double m_balance;
    double m_unconfirmedBalande;
    double m_payout;

    QNetworkReply* m_replyUser;
    QNetworkReply* m_replyPayments;
    QNetworkReply* m_replyUsersettings;
    QNetworkReply* m_replyPrice;
};

#endif // NANOPOOLDISPLAY_H

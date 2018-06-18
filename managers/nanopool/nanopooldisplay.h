#ifndef NANOPOOLDISPLAY_H
#define NANOPOOLDISPLAY_H

#include <QWidget>
#include <QtNetwork>
#include "../nanopoolmanager.h"

namespace Ui {
class NanopoolDisplay;
}

class NanopoolDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit NanopoolDisplay(QString name, QWidget *parent = 0);
    void downloadPoolData();
    void generalInfo(QJsonObject json);
    void workerInfo(QJsonObject json);
    void paymentsInfo(QJsonArray payments);
    ~NanopoolDisplay();

public slots:
    void downloadResult(QNetworkReply* reply);

private:
    Ui::NanopoolDisplay *ui;
    QNetworkAccessManager *manager;
    QString m_wallet;
    QString m_api;
    QString m_addr;
    QString m_currency;
    QString m_pool;
    NanopoolManager* nanopool;
};

#endif // NANOPOOLDISPLAY_H

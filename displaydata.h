#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <QMainWindow>
#include <QtNetwork>

namespace Ui {
class DisplayData;
}

class DisplayData : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayData(QString name, QWidget *parent = 0);
    void downloadPoolData();
    ~DisplayData();

public slots:
    void downloadResult(QNetworkReply* reply);

private:
    Ui::DisplayData *ui;
    QNetworkAccessManager *manager;
    QString m_wallet;
    QString m_poolApi;
    QString m_poolAddr;
};

#endif // DISPLAYDATA_H

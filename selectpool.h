#ifndef SELECTPOOL_H
#define SELECTPOOL_H

#include <QWidget>

namespace Ui {
class SelectPool;
}

class SelectPool : public QWidget
{
    Q_OBJECT

public:
    explicit SelectPool(QWidget *parent = 0);
    void readPools();
    ~SelectPool();

public slots:
    void openDisplayData();

private:
    Ui::SelectPool *ui;
};

#endif // SELECTPOOL_H

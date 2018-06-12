#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>

namespace Ui {
class MainUi;
}

class MainUi : public QWidget
{
    Q_OBJECT

public:
    explicit MainUi(QWidget *parent = 0);
    ~MainUi();

private:
    Ui::MainUi *ui;
};

#endif // MAINUI_H

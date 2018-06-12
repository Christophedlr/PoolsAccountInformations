#include "mainui.h"
#include "ui_mainui.h"
#include "about.h"

MainUi::MainUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUi)
{
    ui->setupUi(this);

    connect(ui->btn_about, SIGNAL(clicked(bool)), this, SLOT(openAboutWindow()));
}

void MainUi::openAboutWindow()
{
    About* aboutWindow = new About();
    aboutWindow->show();
}

MainUi::~MainUi()
{
    delete ui;
}

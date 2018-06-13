#include "selectpool.h"
#include "ui_selectpool.h"
#include "displaydata.h"

SelectPool::SelectPool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectPool)
{
    ui->setupUi(this);
    connect(ui->list_pools, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openDisplayData()));

    this->readPools();
}

/**
 * @brief Read the pools and add in list
 */
void SelectPool::readPools()
{
    ui->list_pools->clear();
    QSettings settings("christophedlr", "PoolsAccountInformations");

    QStringList list = settings.allKeys();

    for (int i = 0; i < list.count(); ++i) {
        int pos = list[i].indexOf('/');
        QString element = list[i];
        ui->list_pools->addItem(element.left(pos));
        i = i+2;
    }
}

void SelectPool::openDisplayData()
{
    DisplayData* displayData = new DisplayData(ui->list_pools->currentItem()->text());
    displayData->show();
}

SelectPool::~SelectPool()
{
    delete ui;
}

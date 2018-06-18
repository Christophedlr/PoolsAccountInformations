#include "selectpool.h"
#include "ui_selectpool.h"
#include "managers/nanopool/nanopoolDisplay.h"

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
        m_pool = settings.value(element.left(pos)+"/pool").toString();

        ui->list_pools->addItem(element.left(pos));
        i = i+4;
    }
}

void SelectPool::openDisplayData()
{
    /*DisplayData* displayData = new DisplayData(ui->list_pools->currentItem()->text());
    displayData->show();*/

    if (m_pool == "Nanopool") {
        QSettings settings("christophedlr", "PoolsAccountInformations");
        NanopoolDisplay* nanopoolDisplay = new NanopoolDisplay(ui->list_pools->currentItem()->text());
        nanopoolDisplay->show();
    }
}

SelectPool::~SelectPool()
{
    delete ui;
}

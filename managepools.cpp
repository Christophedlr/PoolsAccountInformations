/*
 * Copyright 2018, Christophe Daloz - De Los Rios
 * This file is part of PoolsAccountInformations.
 *
 *  PoolsAccountInformations is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PoolsAccountInformations is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PoolsAccountInformations.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "managepools.h"
#include "ui_managepools.h"

ManagePools::ManagePools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagePools)
{
    ui->setupUi(this);

    this->readPools();
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(savePool()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(deletePool()));
    //connect(ui->list_pools, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activateDeleteBtn(bool)));
}

/**
 * @brief Save the pool informations
 */
void ManagePools::savePool()
{
    QSettings settings("christophedlr", "PoolsAccountInformations");

    settings.setValue(ui->edit_name->text()+"/addr", ui->edit_pool_addr->text());
    settings.setValue(ui->edit_name->text()+"/api", ui->edit_pool_api_addr->text());
    settings.setValue(ui->edit_name->text()+"/wallet", ui->edit_wallet_addr->text());

    this->readPools();
}

/**
 * @brief Read the pools and add in list
 */
void ManagePools::readPools()
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

/**
 * @brief Delete selected pool
 */
void ManagePools::deletePool()
{
    if (ui->list_pools->currentRow() > -1) {
        QSettings settings("christophedlr", "PoolsAccountInformations");
        QString name = ui->list_pools->currentItem()->text();

        settings.remove(name+"/addr");
        settings.remove(name+"/api");
        settings.remove(name+"/wallet");

        this->readPools();
    }
}

ManagePools::~ManagePools()
{
    delete ui;
}

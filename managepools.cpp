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
    connect(ui->btn_nanopool_save, SIGNAL(clicked(bool)), this, SLOT(savePool()));
    connect(ui->btn_nanopool_delete, SIGNAL(clicked(bool)), this, SLOT(deletePool()));
}

/**
 * @brief Save the pool informations
 */
void ManagePools::savePool()
{
    QStringList data;

    if (ui->tabWidget->currentIndex() == 0) {
        data
                << ui->edit_nanopool_name->text()
                << ui->edit_nanopool_wallet->text()
                << ui->edit_nanopool_currency->text()
                << ui->edit_nanopool_addr->text()
                << ui->edit_nanopool_api->text();
    }

    nanopool.save(data);
    this->readPools();
}

/**
 * @brief Read the pools and add in list
 */
void ManagePools::readPools()
{
    if (ui->tabWidget->currentIndex() == 0) {
        QSettings settings("christophedlr", "PoolsAccountInformations");
        nanopool.read(settings.allKeys(), ui->list_nanopool);
    }
}

/**
 * @brief Delete selected pool
 */
void ManagePools::deletePool()
{
    QSettings settings("christophedlr", "PoolsAccountInformations");

    if (ui->tabWidget->currentIndex() == 0) {
        if (ui->list_nanopool->currentRow() > -1) {
            QString name = ui->list_nanopool->currentItem()->text();
        }
    }

    settings.remove(name);
    this->readPools();
}

ManagePools::~ManagePools()
{
    delete ui;
}

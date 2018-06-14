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


#include "nanopoolmanager.h"

NanopoolManager::NanopoolManager()
{
}

/**
 * @brief Save nanopool configuration
 * @param settings
 */
void NanopoolManager::save(QStringList data)
{
    QSettings settings("christophedlr", "PoolsAccountInformations");

    settings.setValue(data[0]+"/pool", "Nanopool");
    settings.setValue(data[0]+"/wallet", data[1]);
    settings.setValue(data[0]+"/currency", data[2]);
    settings.setValue(data[0]+"/addr", data[3]);
    settings.setValue(data[0]+"/api", data[4]);
}

/**
 * @brief Read nanopool configuration
 * @param list
 * @param widget
 */
void NanopoolManager::read(QStringList list, QListWidget *widget)
{
    widget->clear();

    for (int i = 0; i < list.count(); ++i) {
        int pos = list[i].indexOf('/');
        QString element = list[i];
        widget->addItem(element.left(pos));
        i = i+4;
    }
}

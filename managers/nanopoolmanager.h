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

#ifndef NANOPOOLMANAGER_H
#define NANOPOOLMANAGER_H

#include <QListWidget>
#include <QSettings>


class NanopoolManager
{
public:
    NanopoolManager();
    void save(QStringList data);
    void read(QStringList list, QListWidget *widget);
};

#endif // NANOPOOLMANAGER_H

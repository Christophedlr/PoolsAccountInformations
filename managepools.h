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

#ifndef MANAGEPOOLS_H
#define MANAGEPOOLS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class ManagePools;
}

class ManagePools : public QWidget
{
    Q_OBJECT

public:
    explicit ManagePools(QWidget *parent = 0);
    void readPools();
    ~ManagePools();

public slots:
    void savePool();
    void deletePool();

private:
    Ui::ManagePools *ui;
};

#endif // MANAGEPOOLS_H

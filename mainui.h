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

public slots:
    void openAboutWindow();
    void openManagePools();
    void openSelectPool();

private:
    Ui::MainUi *ui;
};

#endif // MAINUI_H

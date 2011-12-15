/*
 *  This file is part of choqok-sina
 *  Copyright (C) 2011 Ni Hui <shuizhuyuanluo@126.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SINAPOSTWIDGET_H
#define SINAPOSTWIDGET_H

#include <choqok/postwidget.h>

class SinaAccount;
class SinaPostWidget : public Choqok::UI::PostWidget
{
    Q_OBJECT
    public:
        explicit SinaPostWidget( SinaAccount* account, const Choqok::Post& post, QWidget* parent = 0 );
        virtual ~SinaPostWidget();
        virtual void initUi();
    protected Q_SLOTS:
        virtual void slotResendPost();
    private Q_SLOTS:
        void slotReply();
        void slotWrite();
        void slotReplyAll();
        void slotFavorite();
        void slotFavoriteCreated( Choqok::Account* account, Choqok::Post* post );
        void slotFavoriteRemoved( Choqok::Account* account, Choqok::Post* post );
    private:
        KPushButton* favoriteButton;
};

#endif // SINAPOSTWIDGET_H

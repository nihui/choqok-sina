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

#include "sinatimelinewidget.h"

#include "sinamicroblog.h"

#include <choqok/postwidget.h>

SinaTimelineWidget::SinaTimelineWidget( Choqok::Account* account, const QString& timelineName, QWidget* parent )
: Choqok::UI::TimelineWidget(account,timelineName,parent)
{
    if ( timelineName == "favorite" ) {
        /// set up proxy
        SinaMicroBlog* microblog = dynamic_cast<SinaMicroBlog*>(account->microblog());
        connect( microblog, SIGNAL(favoriteCreated(Choqok::Account*,Choqok::Post*)),
                 this, SLOT(slotFavoriteCreated(Choqok::Account*,Choqok::Post*)) );
        connect( microblog, SIGNAL(favoriteRemoved(Choqok::Account*,Choqok::Post*)),
                 this, SLOT(slotFavoriteRemoved(Choqok::Account*,Choqok::Post*)) );
    }
}

SinaTimelineWidget::~SinaTimelineWidget()
{
}

void SinaTimelineWidget::slotFavoriteCreated( Choqok::Account* account, Choqok::Post* post )
{
    if ( account == currentAccount() ) {
        if ( !posts().contains( post->postId ) )
            addNewPosts( QList<Choqok::Post*>() << post );
    }
}

void SinaTimelineWidget::slotFavoriteRemoved( Choqok::Account* account, Choqok::Post* post )
{
    if ( account == currentAccount() ) {
        if ( posts().contains( post->postId ) )
            posts().value( post->postId )->close();
    }
}

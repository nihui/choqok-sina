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

#include "sinaaccount.h"

#include "sinamicroblog.h"

#include <choqok/passwordmanager.h>
#include <KIO/AccessManager>
#include <QtOAuth/QtOAuth>

static const char sinaConsumerKey[] = "3958149448";
static const char sinaConsumerSecret[] = "75c7d73f76c90edbe668650d97b07eee";

SinaAccount::SinaAccount( SinaMicroBlog* parent, const QString& alias )
: Choqok::Account(parent,alias)
{
    m_oauthToken = configGroup()->readEntry( QString( "%1_OAuthToken" ).arg( alias ), QByteArray() );
    m_oauthTokenSecret = Choqok::PasswordManager::self()->readPassword( QString( "%1_OAuthTokenSecret" ).arg( alias ) ).toUtf8();
    m_timelineNames = configGroup()->readEntry( QString( "%1_Timelines" ).arg( alias ), QStringList() );

    qoauth = new QOAuth::Interface( new KIO::AccessManager( this ), this );
    qoauth->setConsumerKey( SinaAccount::oauthConsumerKey() );
    qoauth->setConsumerSecret( SinaAccount::oauthConsumerSecret() );
    qoauth->setRequestTimeout( 10000 );
    qoauth->setIgnoreSslErrors( true );
}

SinaAccount::~SinaAccount()
{
    delete qoauth;
}

void SinaAccount::writeConfig()
{
    configGroup()->writeEntry( QString( "%1_OAuthToken" ).arg( alias() ), m_oauthToken );
    Choqok::PasswordManager::self()->writePassword( QString( "%1_OAuthTokenSecret" ).arg( alias() ),
                                                    QString::fromUtf8( m_oauthTokenSecret ) );
    configGroup()->writeEntry( QString( "%1_Timelines" ).arg( alias() ), m_timelineNames );
    Choqok::Account::writeConfig();
}

const QByteArray SinaAccount::oauthConsumerKey()
{
    return sinaConsumerKey;
}

const QByteArray SinaAccount::oauthConsumerSecret()
{
    return sinaConsumerSecret;
}

QOAuth::Interface* SinaAccount::qoauthInterface() const
{
    return qoauth;
}

void SinaAccount::setOauthToken( const QByteArray& token )
{
    m_oauthToken = token;
}

const QByteArray SinaAccount::oauthToken() const
{
    return m_oauthToken;
}

void SinaAccount::setOauthTokenSecret( const QByteArray& tokenSecret )
{
    m_oauthTokenSecret = tokenSecret;
}

const QByteArray SinaAccount::oauthTokenSecret() const
{
    return m_oauthTokenSecret;
}

QStringList SinaAccount::timelineNames() const
{
    return m_timelineNames;
}

void SinaAccount::setTimelineNames( const QStringList& list )
{
    m_timelineNames.clear();
    foreach ( const QString& name, list ) {
        if ( microblog()->timelineNames().contains( name ) )
            m_timelineNames << name;
    }
}

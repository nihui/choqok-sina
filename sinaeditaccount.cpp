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

#include "sinaeditaccount.h"
#include "sinaaccount.h"
#include "sinamicroblog.h"

#include <choqok/accountmanager.h>
#include <choqok/choqoktools.h>

#include <KDebug>
#include <KMessageBox>
#include <KInputDialog>
#include <kio/netaccess.h>
#include <kio/accessmanager.h>

#include <QtOAuth/QtOAuth>

#include <QCheckBox>

SinaEditAccountWidget::SinaEditAccountWidget( SinaMicroBlog* microblog, SinaAccount* account, QWidget* parent )
: ChoqokEditAccountWidget(account,parent)
{
    setupUi( this );
    connect( kcfg_authorize, SIGNAL(clicked(bool)),
             this, SLOT(authorizeUser()) );
    if ( account ) {
        if ( account->oauthToken().isEmpty() || account->oauthTokenSecret().isEmpty() )
            setAuthenticated( false );
        else {
            setAuthenticated( true );
            token = account->oauthToken();
            tokenSecret = account->oauthTokenSecret();
            username = account->username();
        }
        kcfg_alias->setText( account->alias() );
    }
    else {
        setAuthenticated( false );
        QString newAccountAlias = microblog->serviceName();
        /// find a default unique alias name for new account
        QString serviceName = newAccountAlias;
        int counter = 1;
        while ( Choqok::AccountManager::self()->findAccount( newAccountAlias ) ) {
            newAccountAlias = QString( "%1%2" ).arg( serviceName ).arg( counter );
            counter++;
        }
        setAccount( new SinaAccount( microblog, newAccountAlias ) );
        kcfg_alias->setText( newAccountAlias );
    }
    loadTimelinesTableState();
    kcfg_alias->setFocus( Qt::OtherFocusReason );

}

SinaEditAccountWidget::~SinaEditAccountWidget()
{
}

bool SinaEditAccountWidget::validateData()
{
    return ( !kcfg_alias->text().isEmpty() && isAuthenticated );
}

Choqok::Account* SinaEditAccountWidget::apply()
{
    SinaAccount* acc = static_cast<SinaAccount*>(account());
    acc->setAlias( kcfg_alias->text() );
    acc->setUsername( username );
    acc->setOauthToken( token );
    acc->setOauthTokenSecret( tokenSecret );
    saveTimelinesTableState();
    acc->writeConfig();
    return acc;
}

void SinaEditAccountWidget::authorizeUser()
{
    SinaAccount* acc = static_cast<SinaAccount*>(account());
    QOAuth::Interface* qoauth = acc->qoauthInterface();
    QOAuth::ParamMap reply = qoauth->requestToken( "http://api.t.sina.com.cn/oauth/request_token",
                                                   QOAuth::GET, QOAuth::HMAC_SHA1 );
    if ( qoauth->error() == QOAuth::NoError ) {
        token = reply.value( QOAuth::tokenParameterName() );
        tokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
        qWarning() << token;
        qWarning() << tokenSecret;
        QUrl url( "http://api.t.sina.com.cn/oauth/authorize" );
        url.addQueryItem( "oauth_token", token );
        Choqok::openUrl( url );
        getPinCode();
    }
    else {
        qWarning() << "ERROR: " << qoauth->error();
        KMessageBox::detailedError( this, i18n( "Authorization Error" ), Choqok::qoauthErrorText( qoauth->error() ) );
    }
}

void SinaEditAccountWidget::getPinCode()
{
    SinaAccount* acc = static_cast<SinaAccount*>(account());
    QOAuth::Interface* qoauth = acc->qoauthInterface();
    isAuthenticated = false;
    while ( !isAuthenticated ) {
        QString verifier = KInputDialog::getText( i18n( "PIN number" ),
                                                  i18n( "Enter PIN number received from Sina:" ) );
        if ( verifier.isEmpty() )
            return;

        QOAuth::ParamMap otherArgs;
        otherArgs.insert( "oauth_verifier", verifier.toUtf8() );
        QOAuth::ParamMap reply = qoauth->accessToken( "http://api.t.sina.com.cn/oauth/access_token",
                                                      QOAuth::POST, token, tokenSecret, QOAuth::HMAC_SHA1, otherArgs );

        if ( qoauth->error() == QOAuth::NoError ) {
            username = QString::fromUtf8( reply.value( "screen_name" ) );
            token = reply.value( QOAuth::tokenParameterName() );
            tokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
            setAuthenticated( true );
            KMessageBox::information( this, i18n( "Choqok is authorized successfully." ),
                                     i18n( "Authorized" ) );
        }
        else {
            qWarning() << "ERROR: " << qoauth->error();
            KMessageBox::detailedError( this, i18n( "Authorization Error" ), Choqok::qoauthErrorText( qoauth->error() ) );
        }
    }
}

void SinaEditAccountWidget::setAuthenticated( bool authenticated )
{
    isAuthenticated = authenticated;
    if ( authenticated ) {
        kcfg_authorize->setIcon( KIcon( "object-unlocked" ) );
        kcfg_authenticateLed->on();
        kcfg_authenticateStatus->setText( i18n( "Authenticated" ) );
    }
    else {
        kcfg_authorize->setIcon( KIcon( "object-locked" ) );
        kcfg_authenticateLed->off();
        kcfg_authenticateStatus->setText( i18n( "Not Authenticated" ) );
    }
}

void SinaEditAccountWidget::loadTimelinesTableState()
{
    SinaAccount* acc = static_cast<SinaAccount*>(account());
    foreach ( const QString& timeline, acc->microblog()->timelineNames() ){
        int newRow = timelinesTable->rowCount();
        timelinesTable->insertRow( newRow );
        timelinesTable->setItem( newRow, 0, new QTableWidgetItem( timeline ) );

        QCheckBox *enable = new QCheckBox( timelinesTable );
        enable->setChecked( acc->timelineNames().contains( timeline ) );
        timelinesTable->setCellWidget( newRow, 1, enable );
    }
}

void SinaEditAccountWidget::saveTimelinesTableState()
{
    SinaAccount* acc = static_cast<SinaAccount*>(account());
    QStringList timelines;
    int rowCount = timelinesTable->rowCount();
    for ( int i = 0; i < rowCount; ++i ) {
        QCheckBox* enable = qobject_cast<QCheckBox*>(timelinesTable->cellWidget( i, 1 ));
        if ( enable && enable->isChecked() )
            timelines << timelinesTable->item( i, 0 )->text();
    }
    timelines.removeDuplicates();
    acc->setTimelineNames( timelines );
}

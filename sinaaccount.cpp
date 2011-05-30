#include "sinaaccount.h"

#include "sinamicroblog.h"

#include <choqok/passwordmanager.h>
#include <QtOAuth/QtOAuth>

static const char sinaConsumerKey[] = "3958149448";
static const char sinaConsumerSecret[] = "75c7d73f76c90edbe668650d97b07eee";

SinaAccount::SinaAccount( SinaMicroBlog* parent, const QString& alias )
: Choqok::Account(parent,alias)
{
    m_oauthToken = configGroup()->readEntry( QString( "%1_OAuthToken" ).arg( alias ), QByteArray() );
    m_oauthTokenSecret = Choqok::PasswordManager::self()->readPassword( QString( "%1_OAuthTokenSecret" ).arg( alias ) ).toUtf8();
    m_timelineNames = configGroup()->readEntry( QString( "%1_Timelines" ).arg( alias ), QStringList() );

    /// TODO KDE 4.5 Change to use new class
    qoauth = new QOAuth::Interface;//( new KIO::AccessManager( this ), this );
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

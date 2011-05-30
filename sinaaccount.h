#ifndef SINAACCOUNT_H
#define SINAACCOUNT_H

#include <choqok/account.h>

namespace QOAuth { class Interface; }
class SinaMicroBlog;

class SinaAccount : public Choqok::Account
{
    Q_OBJECT
    public:
        explicit SinaAccount( SinaMicroBlog* parent, const QString& alias );
        virtual ~SinaAccount();
        virtual void writeConfig();

        static const QByteArray oauthConsumerKey();
        static const QByteArray oauthConsumerSecret();

        QOAuth::Interface* qoauthInterface() const;
        void setOauthToken( const QByteArray& token );
        const QByteArray oauthToken() const;
        void setOauthTokenSecret( const QByteArray& tokenSecret );
        const QByteArray oauthTokenSecret() const;

        virtual QStringList timelineNames() const;
        void setTimelineNames( const QStringList& list );
    private:
        QOAuth::Interface* qoauth;
        QByteArray m_oauthToken;
        QByteArray m_oauthTokenSecret;
        QStringList m_timelineNames;
};

#endif // SINAACCOUNT_H

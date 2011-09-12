#ifndef SINAMICROBLOG_H
#define SINAMICROBLOG_H

#include <choqok/microblog.h>
#include <qjson/parser.h>
#include <QHash>

class KJob;
class SinaAccount;

class SinaMicroBlog : public Choqok::MicroBlog
{
    Q_OBJECT
    public:
        explicit SinaMicroBlog( QObject* parent, const QVariantList& args );
        virtual ~SinaMicroBlog();
        virtual void aboutToUnload();
        virtual ChoqokEditAccountWidget* createEditAccountWidget( Choqok::Account* account, QWidget* parent );
        virtual Choqok::UI::PostWidget* createPostWidget( Choqok::Account* account, const Choqok::Post& post, QWidget* parent );
        virtual Choqok::UI::TimelineWidget* createTimelineWidget( Choqok::Account* account, const QString& timelineName, QWidget* parent );
        virtual void createPost( Choqok::Account* theAccount, Choqok::Post* post );
        virtual void abortCreatePost( Choqok::Account* theAccount, Choqok::Post* post = 0 );
        virtual void fetchPost( Choqok::Account* theAccount, Choqok::Post* post );
        virtual void removePost( Choqok::Account* theAccount, Choqok::Post* post );
        virtual void saveTimeline( Choqok::Account* account, const QString& timelineName,
                                   const QList<Choqok::UI::PostWidget*>& timeline );
        virtual QList<Choqok::Post*> loadTimeline( Choqok::Account* theAccount, const QString& timelineName );
        virtual Choqok::Account* createNewAccount( const QString& alias );
        virtual void updateTimelines( Choqok::Account* theAccount );
        virtual Choqok::TimelineInfo* timelineInfo( const QString& timelineName );
        virtual QString postUrl( Choqok::Account* account, const QString& username, const QString& postId ) const;
        virtual QString profileUrl( Choqok::Account* account, const QString& username ) const;
        void retweetPost( Choqok::Account* theAccount, Choqok::Post* post );
        void createFavorite( Choqok::Account* theAccount, Choqok::Post* post );
        void removeFavorite( Choqok::Account* theAccount, Choqok::Post* post );
        void createFriendship( Choqok::Account* theAccount, Choqok::User* user );
        void removeFriendship( Choqok::Account* theAccount, Choqok::User* user );
    Q_SIGNALS:
        void favoriteCreated( Choqok::Account* account, Choqok::Post* post );
        void favoriteRemoved( Choqok::Account* account, Choqok::Post* post );
        void friendshipCreated( Choqok::Account* theAccount, Choqok::User* user );
        void friendshipRemoved( Choqok::Account* theAccount, Choqok::User* user );
    private Q_SLOTS:
        void slotCreatePost( KJob* job );
        void slotFetchPost( KJob* job );
        void slotRemovePost( KJob* job );
        void slotRequestTimeline( KJob* job );
        void slotCreateFavorite( KJob* job );
        void slotRemoveFavorite( KJob* job );
        void slotCreateFriendship( KJob* job );
        void slotRemoveFriendship( KJob* job );
    private:
        int m_countOfTimelinesToSave;
        QHash<QString, QString> m_timelineApiPath;
        QHash<QString, Choqok::TimelineInfo*> m_timelineInfo;
        QHash<SinaAccount*, QHash<QString, QString> > m_timelineLatestId;
        QHash<KJob*, Choqok::Post*> m_createPost;
        QHash<KJob*, Choqok::Post*> m_fetchPost;
        QHash<KJob*, Choqok::Post*> m_removePost;
        QHash<KJob*, QString> m_jobTimeline;
        QHash<KJob*, Choqok::Post*> m_createFavorite;
        QHash<KJob*, Choqok::Post*> m_removeFavorite;
        QHash<KJob*, Choqok::User*> m_createFriendship;
        QHash<KJob*, Choqok::User*> m_removeFriendship;
        QHash<KJob*, SinaAccount*> m_jobAccount;
        QJson::Parser parser;

        QDateTime dateFromString( const QString &date );
        QHash<QString, int> monthes;

        /// json data
        void readPostFromJsonMap( const QVariantMap& varmap, Choqok::Post* post );
        void readDMessageFromJsonMap( Choqok::Account* account, const QVariantMap& varmap, Choqok::Post* post );
        void readUserFromJsonMap( const QVariantMap& varmap, Choqok::User* user );
};

#endif // SINAMICROBLOG_H

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

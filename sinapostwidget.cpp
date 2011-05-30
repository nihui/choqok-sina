#include "sinapostwidget.h"
#include "sinaaccount.h"
#include "sinamicroblog.h"

#include <choqok/mediamanager.h>

#include <KAction>
#include <KLocale>
#include <KMenu>
#include <KPushButton>

static const KIcon unFavIcon( Choqok::MediaManager::convertToGrayScale( KIcon( "rating" ).pixmap( 16 ) ) );

SinaPostWidget::SinaPostWidget( SinaAccount* account, const Choqok::Post& post, QWidget* parent )
: Choqok::UI::PostWidget(account,post,parent)
{
    SinaMicroBlog* microblog = dynamic_cast<SinaMicroBlog*>(account->microblog());
    connect( microblog, SIGNAL(favoriteRemoved(Choqok::Account*,Choqok::Post*)),
             this, SLOT(slotFavoriteRemoved(Choqok::Account*,Choqok::Post*)) );
    connect( microblog, SIGNAL(favoriteCreated(Choqok::Account*,Choqok::Post*)),
             this, SLOT(slotFavoriteCreated(Choqok::Account*,Choqok::Post*)) );

}

SinaPostWidget::~SinaPostWidget()
{
}

void SinaPostWidget::initUi()
{
    Choqok::UI::PostWidget::initUi();

    KPushButton* replyButton = addButton( "replyButton", i18n( "Reply" ), "edit-undo" );
    connect( replyButton, SIGNAL(clicked(bool)), this, SLOT(slotReply()) );

    KMenu* menu = new KMenu;
    KAction* replyAction = new KAction( KIcon( "edit-undo" ), i18n( "Reply to %1", currentPost().author.userName ), menu );
    connect( replyAction, SIGNAL(triggered(bool)), this, SLOT(slotReply()) );
    menu->addAction( replyAction );
    KAction* writeAction = new KAction( KIcon( "document-edit" ), i18n( "Write to %1", currentPost().author.userName ), menu );
    connect( writeAction, SIGNAL(triggered(bool)), this, SLOT(slotWrite()) );
    menu->addAction( writeAction );
    if( !currentPost().isPrivate ) {
        KAction* replyAllAction = new KAction( i18n( "Reply to all" ), menu );
        connect( replyAllAction, SIGNAL(triggered(bool)), this, SLOT(slotReplyAll()) );
        menu->addAction(replyAllAction);
    }

    menu->setDefaultAction( replyAction );
    replyButton->setDelayedMenu( menu );

    if( !currentPost().isPrivate ) {
        favoriteButton = addButton( "favoriteButton",i18n( "Favorite" ), "rating" );
        favoriteButton->setCheckable( true );
        connect( favoriteButton, SIGNAL(clicked(bool)), this, SLOT(slotFavorite()) );
        if ( currentPost().isFavorited ) {
            favoriteButton->setChecked( true );
            favoriteButton->setIcon( KIcon( "rating" ) );
        }
        else {
            favoriteButton->setChecked( false );
            favoriteButton->setIcon( unFavIcon );
        }
    }
}

void SinaPostWidget::slotResendPost()
{
    setReadWithSignal();
    SinaMicroBlog* microblog = dynamic_cast<SinaMicroBlog*>(currentAccount()->microblog());
    Choqok::Post* post = new Choqok::Post;
    post->postId = currentPost().postId;
    microblog->retweetPost( currentAccount(), post );
}

void SinaPostWidget::slotReply()
{
}

void SinaPostWidget::slotWrite()
{
    emit reply( QString("@%1").arg( currentPost().author.userName ), QString() );
}

void SinaPostWidget::slotReplyAll()
{
    QString txt = QString("@%1").arg( currentPost().author.userName );
    emit reply( txt, currentPost().postId );
}

void SinaPostWidget::slotFavorite()
{
    setReadWithSignal();
    SinaMicroBlog* microblog = dynamic_cast<SinaMicroBlog*>(currentAccount()->microblog());
    Choqok::Post* post = new Choqok::Post;
    post->postId = currentPost().postId;
    if ( currentPost().isFavorited )
        microblog->removeFavorite( currentAccount(), post );
    else
        microblog->createFavorite( currentAccount(), post );
}

void SinaPostWidget::slotFavoriteCreated( Choqok::Account* account, Choqok::Post* post )
{
    if ( currentAccount() != account || post->postId != currentPost().postId )
        return;

    delete post;
    Choqok::Post tmp = currentPost();
    tmp.isFavorited = true;
    setCurrentPost( tmp );
    favoriteButton->setChecked( true );
    favoriteButton->setIcon( KIcon( "rating" ) );
}

void SinaPostWidget::slotFavoriteRemoved( Choqok::Account* account, Choqok::Post* post )
{
    if ( currentAccount() != account || post->postId != currentPost().postId )
        return;

    delete post;
    Choqok::Post tmp = currentPost();
    tmp.isFavorited = false;
    setCurrentPost( tmp );
    favoriteButton->setChecked( false );
    favoriteButton->setIcon( unFavIcon );
}

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

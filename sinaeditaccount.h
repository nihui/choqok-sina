#ifndef SINAEDITACCOUNT_H
#define SINAEDITACCOUNT_H

#include <choqok/editaccountwidget.h>
#include "ui_sinaeditaccount_base.h"

class SinaAccount;
class SinaMicroBlog;

class SinaEditAccountWidget : public ChoqokEditAccountWidget, public Ui::SinaEditAccountBase
{
    Q_OBJECT
    public:
        explicit SinaEditAccountWidget( SinaMicroBlog* microblog, SinaAccount* account, QWidget* parent );
        virtual ~SinaEditAccountWidget();
        virtual bool validateData();
        virtual Choqok::Account* apply();
    protected Q_SLOTS:
        virtual void authorizeUser();
    private:
        void getPinCode();
        void setAuthenticated( bool authenticated );
        void loadTimelinesTableState();
        void saveTimelinesTableState();
    private:
        QByteArray token;
        QByteArray tokenSecret;
        QString username;
        bool isAuthenticated;
};

#endif // SINAEDITACCOUNT_H

#ifndef SINATIMELINEWIDGET_H
#define SINATIMELINEWIDGET_H

#include <choqok/timelinewidget.h>

class SinaTimelineWidget : public Choqok::UI::TimelineWidget
{
    Q_OBJECT
    public:
        explicit SinaTimelineWidget( Choqok::Account* account, const QString& timelineName, QWidget* parent = 0 );
        virtual ~SinaTimelineWidget();
    private Q_SLOTS:
        void slotFavoriteCreated( Choqok::Account* account, Choqok::Post* post );
        void slotFavoriteRemoved( Choqok::Account* account, Choqok::Post* post );
};

#endif // SINATIMELINEWIDGET_H

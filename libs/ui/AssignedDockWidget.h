#ifndef ASSIGNEDDOCKWIDGET_H
#define ASSIGNEDDOCKWIDGET_H

#include <QtGui/QDockWidget>


/**
@brief
Represents an assigned dock widget.
*/
class AssignedDockWidget : public QObject
{
public:
    explicit AssignedDockWidget(QObject * parent = 0);
    virtual ~AssignedDockWidget();

    QWidget * contentWidget() const;
    void setContentWidget(QWidget *w);

    Qt::DockWidgetArea area() const;
    void setArea(Qt::DockWidgetArea area);

    QDockWidget * dockWidget() const;

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    QDockWidget *           _privateDW;

    QString                 _widgetKey; //!< Class name of the contents widget (CASE SENSITIVE!).
    Qt::DockWidgetArea      _area;
};

#endif // ASSIGNEDDOCKWIDGET_H

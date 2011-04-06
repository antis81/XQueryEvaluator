#ifndef ASSIGNEDDOCKWIDGET_H
#define ASSIGNEDDOCKWIDGET_H

#include <QtCore/QFlags>
#include <QtGui/QDockWidget>


/**
@brief
Represents an assigned dock widget.
*/
class AssignedDockWidget
        Q_FLAGS( Options )
{
public:
    enum Option
    {
        NoFixedDockWidgetOptions    = 0x00 ,
        CloseOnHide                 = 0x01 ,
        Resizable                   = 0x02
    };
    Q_DECLARE_FLAGS(Options, Option)

    explicit AssignedDockWidget();
    virtual ~AssignedDockWidget();

    const QString& widgetKey() const;
    void setWidgetKey(const QString &widgetClassName);

    Qt::DockWidgetArea area() const;
    void setArea(Qt::DockWidgetArea area);

    Options options() const;
    void setOptions( Options options);

    QDockWidget * dockWidget() const;

private:
    QDockWidget *           _privateDW;

    QString                 _widgetKey; //!< Class name of the contents widget (CASE SENSITIVE!).
    Qt::DockWidgetArea      _area;
    Options                 _options;
};

#endif // ASSIGNEDDOCKWIDGET_H

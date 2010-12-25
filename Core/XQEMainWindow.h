#ifndef XQEMAINWINDOW_H
#define XQEMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtXmlPatterns/QXmlQuery>

class XQEditor;

namespace Ui {
    class XQEMainWindow;
}

class QComboBox;

class XQEMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit XQEMainWindow(QWidget *parent = 0);
    ~XQEMainWindow();

private slots:
    void on_btnQuery_clicked();
    void on_btnOpenSource_clicked();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();


    void queryLanguageSelected(int comboIndex);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::XQEMainWindow *ui;

    QComboBox *                 _combo; //!< @todo Outsource this and connect to an action manager or something.

    XQEditor *                  _textQuery;
    QXmlQuery::QueryLanguage    _queryLanguage;

    QString selectSourceFile();
    QString loadSourceFile(const QString &path) const;
};

#endif // XQEMAINWINDOW_H

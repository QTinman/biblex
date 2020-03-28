#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tools.h"
#include <QDialog>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewDialog>

extern QString greek_lexicon,hebrew_lexicon;
extern QString hmem[10];
extern int hmempos;
extern std::string source;
#ifdef WINNT
    extern char dir[256];
    extern std::string pwd;

#else
    extern char * PWD;
    extern std::string pwd;
 #endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_returnPressed();

    void on_actionSelect_Greek_lexicon_triggered();

    void on_actionSelect_Hebrew_lexicon_triggered();
    void keymem(QString memstr);

    void on_action_Print_triggered();
    void doPrint(QPrinter * printer);
    void savelog(QString line, QString filename);

    void on_action_Save_output_triggered();



    void on_action_Clear_output_triggered();

private:
    Ui::MainWindow *ui;
    QPushButton *backButton;
    QPushButton *clearButton;

protected:
    //void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
};
#endif // MAINWINDOW_H

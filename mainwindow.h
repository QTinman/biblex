#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tools.h"

extern QString greek_lexicon,hebrew_lexicon;

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

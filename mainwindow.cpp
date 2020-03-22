#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"
#include <QFileDialog>
#include <QDebug>


QString greek_lexicon,hebrew_lexicon;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    char csettings[13]="settings.txt";
    ui->setupUi(this);
    setCentralWidget(ui->groupBox_3);
    ui->lineEdit->focusWidget();
    ui->textBrowser->setStyleSheet("background-color: #1f1414; color: white");
    ui->lineEdit->setStyleSheet("background-color: #1f1414; color: white");
    ui->textBrowser->setOpenExternalLinks(true);
    //ui->textBrowser->setStyleSheet("text-color: #1f1414;");

    if (!existSettings("settings.txt")) {
        createSettings("settings.txt");
        greek_lexicon = QFileDialog::getExistingDirectory(this,"Select Greek lexicon's directory",".");
        hebrew_lexicon = QFileDialog::getExistingDirectory(this,"Select Hebrew lexicon's directory",".");
        writeSettings(csettings,"greek",greek_lexicon.toUtf8().constData());
        writeSettings(csettings,"hebrew",hebrew_lexicon.toUtf8().constData());
    } else {
        greek_lexicon = readSettings("settings.txt","greek");
        hebrew_lexicon = readSettings("settings.txt","hebrew");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineEdit_returnPressed()
{
    QString html="";
    std::string line = ui->lineEdit->text().toUtf8().constData();
    int ns1 = getwordnumericvalue(line,0,0,0);
    int ns2 = getwordnumericvalue(line,0,0,4);
    int ns3 = getwordnumericvalue(line,0,0,5);
    QString tphrase = ui->lineEdit->text();
    html = "<left><h2>Phrase: "+tphrase+"</h2></left>";
    html += readbib(ns1,"EO",hebrew_lexicon,greek_lexicon);
    //ui->textBrowser->append(html);
    html += readbib(ns2,"Jew",hebrew_lexicon,greek_lexicon);
    //ui->textBrowser->append(html);
    html += readbib(ns3,"Sum",hebrew_lexicon,greek_lexicon);
    ui->textBrowser->append("<html>"+html+"</html>");
    /*html = readbib(ns2,hebrew_lexicon,greek_lexicon);
    ui->textBrowser->append(html);
    html = readbib(ns3,hebrew_lexicon,greek_lexicon);
    ui->textBrowser->append(html);*/
    //ui->textBrowser->append(QString::fromStdString(line));

    //ui->textBrowser->append(QString::number(ns1));
    //ui->textBrowser->append(QString::number(ns2));
    //ui->textBrowser->append(QString::number(ns3));
    ui->lineEdit->clear();
}

void MainWindow::on_actionSelect_Greek_lexicon_triggered()
{
    greek_lexicon = QFileDialog::getExistingDirectory(this,"Select Greek lexicon's directory",".");
    writeSettings("settings.txt","greek",greek_lexicon.toUtf8().constData());
}

void MainWindow::on_actionSelect_Hebrew_lexicon_triggered()
{
    hebrew_lexicon = QFileDialog::getExistingDirectory(this,"Select Hebrew lexicon's directory",".");
    writeSettings("settings.txt","hebrew",hebrew_lexicon.toUtf8().constData());
}

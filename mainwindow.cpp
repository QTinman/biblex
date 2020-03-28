#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include <QDialog>
#include <QPushButton>
#include <stdio.h>
#include <string.h>

//#include <iostream>
#include <fstream>
//#include <sstream>

#ifdef WINNT
    #include <direct.h>
    #include <Windows.h>
    #define GetCurrentDir GetCurrentDirectoryA(256, dir)
    #define windud pwd=dir;
    char dir[256];
    std::string pwd("");
#else
    char * PWD;
    //#include <unistd.h>
    #define GetCurrentDir PWD = getenv ("PWD")
    std::string pwd("");
    #define windud pwd.append(PWD)
 #endif

/*
  QString::fromStdString(string)  <- from string to Qstring
  QString::number(int) <- from int to QString
  string = QString.toUtf8().constData() from QString to string
  QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,1))
  std::to_string(42) int to string
 */


using namespace std;
QString greek_lexicon,hebrew_lexicon;
QString hmem[10];
int hmempos = -1;
string source;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    char csettings[13]="settings.txt";
    GetCurrentDir;
    windud;
    while (replacestring(pwd,"\\","/"));
    pwd += "/tmp.htm";
    createSettings(pwd);
    source = "file:///"+pwd;

    //qDebug() << "Working directory : " << QString::fromStdString(source) << flush;
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    setCentralWidget(ui->frame_3);
    ui->lineEdit->focusWidget();
    ui->textBrowser->setStyleSheet("background-color: #1f1414; color: white");
    ui->lineEdit->setStyleSheet("background-color: #1f1414; color: white");
    ui->textBrowser->setOpenExternalLinks(true);
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
    //ui->textBrowser->;
    //ui->textBrowser->show();
    QString srt = QString::fromStdString(source);
    ui->textBrowser->setSource(srt);
    //ui->textBrowser->home();
}

MainWindow::~MainWindow()
{


    char * writable = new char[pwd.size() + 1];
    std::copy(pwd.begin(), pwd.end(), writable);
    writable[pwd.size()] = '\0'; // don't forget the terminating 0
    remove(writable);
    // don't forget to free the string after finished using it
    delete[] writable;


    delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent *event)
{
    if (obj == ui->lineEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up)
            {

                 //ui->lineEdit->setText("Up Key");
                 if (hmempos > 0) {
                     hmempos --;
                     ui->lineEdit->setText(hmem[hmempos]);
                     //qDebug() << hmem[hmempos] << hmempos;

                 }
                 return true;
            }
            else if(keyEvent->key() == Qt::Key_Down)
            {

                //ui->lineEdit->setText("Down Key");
                if (hmempos > -1) {
                if (hmempos < 10 && hmem[hmempos] != "") {
                    if (hmem[hmempos] != "") hmempos ++;
                    //qDebug() << hmem[hmempos] << hmempos;
                    if(hmempos <= 9 ) ui->lineEdit->setText(hmem[hmempos]);
                    else ui->lineEdit->setText("");

                    //if (hmem[hmempos+2] == "") ui->lineEdit->setText("");
                }
            }
                //if (hmempos == 10) ui->lineEdit->setText("");
                return true;
            }

        }
        return false;
    }
    return false;
}

void MainWindow::keymem(QString memstr)
{
    if (hmem[9] != "") {
        for (int i=0;i<9;i++){
            hmem[i] = hmem[i+1];
        }
    }
    hmem[9] = "";
    for (hmempos=1;hmempos<10;hmempos++){
        if (hmem[hmempos-1] == "") break;
    }
    hmem[hmempos-1] = memstr;
}

void MainWindow::on_lineEdit_returnPressed()
{
    QString backbutton = "<a href=\"javascript:history.back()\">Go Back</a>";
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
    savelog(html,QString::fromStdString(pwd));
    //qDebug() << html+" qstring";
    keymem(tphrase);
    ui->lineEdit->clear();
}

void MainWindow::on_actionSelect_Greek_lexicon_triggered()
{
    char csettings[13]="settings.txt";
    greek_lexicon = QFileDialog::getExistingDirectory(this,"Select Greek lexicon's directory",".");
    writeSettings(csettings,"greek",greek_lexicon.toUtf8().constData());
}

void MainWindow::on_actionSelect_Hebrew_lexicon_triggered()
{
    char csettings[13]="settings.txt";
    hebrew_lexicon = QFileDialog::getExistingDirectory(this,"Select Hebrew lexicon's directory",".");
    writeSettings(csettings,"hebrew",hebrew_lexicon.toUtf8().constData());
}

void MainWindow::doPrint(QPrinter * printer)
{
    QTime ct = QTime::currentTime();
    //ui->textBrowser->print(printer);
    printer->newPage();
    printer->setDocName("Gematria Analyzer - "+ct.currentTime().toString());
    ui->textBrowser->print(printer);
}

void MainWindow::on_action_Print_triggered()
{
    QPrintPreviewDialog * printPreview = new QPrintPreviewDialog(this);
    connect(printPreview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(doPrint(QPrinter *)));
    printPreview->exec();
}

void MainWindow::on_action_Save_output_triggered()
{
    QFileDialog saveAsdialog(this);
    QString filename = saveAsdialog.getSaveFileName(this, tr("Save file"), ".", tr("Files (*.htm)"));
    //if (saveAsdialog.exec() == QDialog::Accepted) {
     savelog(ui->textBrowser->toHtml(),filename);
    //}
}

void MainWindow::savelog(QString line, QString filename)
{
    std::ofstream fout;  // Create Object of Ofstream
    std::ifstream fin;
    fout.open (filename.toUtf8().constData(),ios::app); // Append mode
    fin.open(filename.toUtf8().constData());
    if(fin.is_open())
        fout<< line.toUtf8().constData(); // Writing data to file
    fin.close();
    fout.close(); // Closing the file
    line = "";
}




void MainWindow::on_action_Clear_output_triggered()
{
    ui->textBrowser->clear();
}

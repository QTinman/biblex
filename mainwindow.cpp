#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include <QDialog>
#include <QtWidgets>
#include <QPushButton>
#include <stdio.h>
#include <string.h>

//#include <iostream>
#include <fstream>
//#include <sstream>


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
QString source,pwd;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    char csettings[13]="settings.txt";
    pwd = QDir::currentPath();
    pwd += "/tmp.htm";
    //qDebug() << pwd;
    createSettings(pwd.toUtf8().constData());
    source = "file:///"+pwd;
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->textBrowser->installEventFilter(this);
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

    ui->textBrowser->setSource(source);

}

MainWindow::~MainWindow()
{

    QString location = pwd;
    QFile *rmFile = new QFile(location);
    rmFile->remove();
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
    } else if (obj == ui->textBrowser)
    {
        //qDebug() << event->type();

        if (event->type() == QEvent::InputMethodQuery)
        {

           // QString html = ui->textBrowser->toHtml();
           // while (replacestring(pwd,"../greek/",""));
           // while (replacestring(pwd,"/greek/",""));
           // ui->textBrowser->setHtml(html);
            //qDebug() << html;
        }
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
    savelog(html,pwd);
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
    printer->setDocName("Bible Lexicon - "+ct.currentTime().toString());
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
    //QFileDialog saveAsdialog(this);
    //QString filename = saveAsdialog.getSaveFileName(this, tr("Save file"), ".", tr("Files (*.htm)"));


    const QString format = "htm";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(this, tr("Save Output As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fileName = fileDialog.selectedFiles().first();

     savelog(ui->textBrowser->toHtml(),fileName);
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

void MainWindow::saveScreenshot()
{

    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;


    originalPixmap = screen->grabWindow(QWidget::hasFocus(),MainWindow::x(),MainWindow::y(),MainWindow::width(),MainWindow::height());

    const QString format = "png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);
    QStringList mimeTypes;
    const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
    for (const QByteArray &bf : baMimeTypes)
        mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fileName = fileDialog.selectedFiles().first();
    if (!originalPixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}
void MainWindow::on_actionScreenShot_triggered()
{
    saveScreenshot();
}

void MainWindow::on_actionSelect_Font_triggered()
{
    //const QFontDialog::FontDialogOptions options = QFlag(fontDialogOptionsWidget->value());
    bool ok;
    QFont font = QFontDialog::getFont(&ok,QFont(ui->textBrowser->font()),this,"Select Font");
    if (ok) {
        ui->textBrowser->setFont(font);
    }
}

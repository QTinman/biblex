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
#include <QStandardPaths>
#include <QMessageBox>
#include <QInputDialog>
#include <stdio.h>
#include <string.h>

//#include <iostream>
#include <fstream>
//#include <sstream>

#ifdef ANDROID_BUILD
#include <QJniObject>
#include <QJniEnvironment>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtAndroid>
#include <QAndroidJniObject>
#endif
#include <QCoreApplication>
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
QString source,pwd;
bool nightmode=true;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
#ifdef ANDROID_BUILD
    // Request storage permissions on Android
    requestAndroidPermissions();

    // Use Android-appropriate storage paths
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDir(appDataPath);
    if (!appDir.exists()) {
        appDir.mkpath(".");
    }
    QString settingsPath = appDataPath + "/settings.txt";
    pwd = appDataPath + "/tmp.htm";
#else
    QString settingsPath = "settings.txt";
    pwd = QDir::currentPath();
    pwd += "/tmp.htm";
#endif

    char csettings[256];
    strcpy(csettings, settingsPath.toUtf8().constData());

    //qDebug() << pwd;
    createSettings(pwd.toUtf8().constData(),"");
    source = "file:///"+pwd;
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->textBrowser->installEventFilter(this);
    setCentralWidget(ui->frame_3);
    ui->lineEdit->focusWidget();
    ui->textBrowser->setOpenExternalLinks(true);
    QString font = readSettings(csettings,"font");
    greek_lexicon = readSettings(csettings,"greek");
    hebrew_lexicon = readSettings(csettings,"hebrew");
    QString nightm = readSettings(csettings,"nightmode");
    //qDebug() << font;
    if (font != "none") {
        QFont f1;
        f1.fromString(font);
        ui->textBrowser->setFont(f1);
    }
    if (greek_lexicon == "none") {
#ifdef ANDROID_BUILD
        // On Android, provide guidance for setting up lexicons
        QMessageBox::information(this, "Greek Lexicon Setup",
                               "Please place Greek lexicon files in:\n" +
                               QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BibleLexicon/greek/\n\n" +
                               "Or use Menu > Select Greek Lexicon to choose a different location.");
        greek_lexicon = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BibleLexicon/greek";
        QDir().mkpath(greek_lexicon);
#else
        greek_lexicon = QFileDialog::getExistingDirectory(this,"Select Greek lexicon's directory",".");
#endif
        writeSettings(csettings,"greek",greek_lexicon.toUtf8().constData());
    }
    if (hebrew_lexicon == "none") {
#ifdef ANDROID_BUILD
        // On Android, provide guidance for setting up lexicons
        QMessageBox::information(this, "Hebrew Lexicon Setup",
                               "Please place Hebrew lexicon files in:\n" +
                               QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BibleLexicon/hebrew/\n\n" +
                               "Or use Menu > Select Hebrew Lexicon to choose a different location.");
        hebrew_lexicon = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BibleLexicon/hebrew";
        QDir().mkpath(hebrew_lexicon);
#else
        hebrew_lexicon = QFileDialog::getExistingDirectory(this,"Select Hebrew lexicon's directory",".");
#endif
        writeSettings(csettings,"hebrew",hebrew_lexicon.toUtf8().constData());
    }
    if (nightm == "true") {
        nightmode = true;
        ui->action_Nightmode->setChecked(true);
    } else {
        nightmode = false;
        ui->action_Nightmode->setChecked(false);
    }
    if (nightmode) ui->textBrowser->setStyleSheet("background-color: #1f1414; color: white");
    if (nightmode) ui->lineEdit->setStyleSheet("background-color: #1f1414; color: white");
    ui->textBrowser->setSource(source);

#ifdef ANDROID_BUILD
    // Mobile-friendly UI adjustments
    // Make input field larger for touch
    ui->lineEdit->setMinimumHeight(50);

    // Increase default font size for better readability on mobile
    QFont mobileFont = ui->textBrowser->font();
    mobileFont.setPointSize(mobileFont.pointSize() + 2);
    ui->textBrowser->setFont(mobileFont);
    ui->lineEdit->setFont(mobileFont);

    // Set minimum window size for mobile
    this->setMinimumSize(360, 600);

    // Enable kinetic scrolling for touch
    ui->textBrowser->viewport()->setProperty("FingerScrollable", true);

    // Make the window fullscreen on Android
    this->showMaximized();
#endif

}

MainWindow::~MainWindow()
{

    QString location = pwd;
    QFile *File = new QFile(location);
    File->remove();
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
    QString anumber=QString::fromStdString(line);
    int nr=anumber.toInt();

    int ns1 = getwordnumericvalue(line,0,0,0);
    int ns2 = getwordnumericvalue(line,0,0,4);
    int ns3 = getwordnumericvalue(line,0,0,5);
    QString tphrase = ui->lineEdit->text();
    html = "<left><h2>Phrase: "+tphrase+"</h2></left>";
    if (nr==0) {
        html += readbib(ns1,"EO",hebrew_lexicon,greek_lexicon);
        html += readbib(ns2,"Jew",hebrew_lexicon,greek_lexicon);
        html += readbib(ns3,"Sum",hebrew_lexicon,greek_lexicon);
    } else {
        html += readbib(nr,"EO",hebrew_lexicon,greek_lexicon);
        html += readbib(nr,"Jew",hebrew_lexicon,greek_lexicon);
        html += readbib(nr,"Sum",hebrew_lexicon,greek_lexicon);
    }
    ui->textBrowser->append("<html>"+html+"</html>");
    savelog(html,pwd);
    //qDebug() << html+" qstring";
    keymem(tphrase);
    ui->lineEdit->clear();
}

void MainWindow::on_actionSelect_Greek_lexicon_triggered()
{
#ifdef ANDROID_BUILD
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString settingsPath = appDataPath + "/settings.txt";
    char csettings[256];
    strcpy(csettings, settingsPath.toUtf8().constData());

    bool ok;
    QString path = QInputDialog::getText(this, "Greek Lexicon Path",
                                        "Enter path to Greek lexicon directory:",
                                        QLineEdit::Normal,
                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BibleLexicon/greek",
                                        &ok);
    if (ok && !path.isEmpty()) {
        greek_lexicon = path;
        QDir().mkpath(greek_lexicon);
        writeSettings(csettings,"greek",greek_lexicon.toUtf8().constData());
    }
#else
    char csettings[13]="settings.txt";
    greek_lexicon = QFileDialog::getExistingDirectory(this,"Select Greek lexicon's directory",".");
    writeSettings(csettings,"greek",greek_lexicon.toUtf8().constData());
#endif
}

void MainWindow::on_actionSelect_Hebrew_lexicon_triggered()
{
#ifdef ANDROID_BUILD
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString settingsPath = appDataPath + "/settings.txt";
    char csettings[256];
    strcpy(csettings, settingsPath.toUtf8().constData());

    bool ok;
    QString path = QInputDialog::getText(this, "Hebrew Lexicon Path",
                                        "Enter path to Hebrew lexicon directory:",
                                        QLineEdit::Normal,
                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BibleLexicon/hebrew",
                                        &ok);
    if (ok && !path.isEmpty()) {
        hebrew_lexicon = path;
        QDir().mkpath(hebrew_lexicon);
        writeSettings(csettings,"hebrew",hebrew_lexicon.toUtf8().constData());
    }
#else
    char csettings[13]="settings.txt";
    hebrew_lexicon = QFileDialog::getExistingDirectory(this,"Select Hebrew lexicon's directory",".");
    writeSettings(csettings,"hebrew",hebrew_lexicon.toUtf8().constData());
#endif
}

#ifndef ANDROID_BUILD
void MainWindow::doPrint(QPrinter * printer)
{
    QTime ct = QTime::currentTime();
    //ui->textBrowser->print(printer);
    printer->newPage();
    printer->setDocName("Bible Lexicon - "+ct.currentTime().toString());
    ui->textBrowser->print(printer);
}
#endif

void MainWindow::on_action_Print_triggered()
{
#ifdef ANDROID_BUILD
    // On Android, export to HTML and share instead of printing
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDateTime now = QDateTime::currentDateTime();
    QString fileName = appDataPath + "/BibleLexicon_" +
                      now.toString("yyyyMMdd_hhmmss") + ".html";

    // Save the HTML content
    savelog(ui->textBrowser->toHtml(), fileName);

    // Share the file
    QMessageBox::information(this, "Export Complete",
                           "Document saved to:\n" + fileName + "\n\nOpening share dialog...");
    shareFile(fileName, "text/html");
#else
    QPrintPreviewDialog * printPreview = new QPrintPreviewDialog(this);
    connect(printPreview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(doPrint(QPrinter *)));
    printPreview->exec();
#endif
}

void MainWindow::on_action_Save_output_triggered()
{
#ifdef ANDROID_BUILD
    // On Android, save to app documents and optionally share
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir().mkpath(documentsPath + "/BibleLexicon");

    QDateTime now = QDateTime::currentDateTime();
    QString fileName = documentsPath + "/BibleLexicon/output_" +
                      now.toString("yyyyMMdd_hhmmss") + ".html";

    savelog(ui->textBrowser->toHtml(), fileName);

    QMessageBox msgBox;
    msgBox.setText("Output saved to:\n" + fileName);
    msgBox.setInformativeText("Would you like to share this file?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        shareFile(fileName, "text/html");
    }
#else
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
#endif
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
#ifdef ANDROID_BUILD
    // On Android, use grab() instead of grabWindow()
    originalPixmap = this->grab();

    const QString format = "png";
    QString picturesPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QDir().mkpath(picturesPath + "/BibleLexicon");

    QDateTime now = QDateTime::currentDateTime();
    QString fileName = picturesPath + "/BibleLexicon/screenshot_" +
                      now.toString("yyyyMMdd_hhmmss") + "." + format;

    if (originalPixmap.save(fileName)) {
        QMessageBox msgBox;
        msgBox.setText("Screenshot saved to:\n" + fileName);
        msgBox.setInformativeText("Would you like to share this image?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes) {
            shareFile(fileName, "image/png");
        }
    } else {
        QMessageBox::warning(this, tr("Save Error"),
                           tr("The screenshot could not be saved to \"%1\".")
                           .arg(fileName));
    }
#else
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
#endif
}
void MainWindow::on_actionScreenShot_triggered()
{
    saveScreenshot();
}

void MainWindow::on_actionSelect_Font_triggered()
{
#ifdef ANDROID_BUILD
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString settingsPath = appDataPath + "/settings.txt";
    char csettings[256];
    strcpy(csettings, settingsPath.toUtf8().constData());
#else
    char csettings[13] = "settings.txt";
#endif

    //const QFontDialog::FontDialogOptions options = QFlag(fontDialogOptionsWidget->value());
    bool ok;
    QFont font = QFontDialog::getFont(&ok,QFont(ui->textBrowser->font()),this,"Select Font");
    if (ok) {
        writeSettings(csettings,"font",font.toString().toUtf8().constData());
        ui->textBrowser->setFont(font);
    }
}



void MainWindow::on_action_Nightmode_toggled(bool arg1)
{
    if (arg1) nightmode = true;
    else nightmode = false;

#ifdef ANDROID_BUILD
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString settingsPath = appDataPath + "/settings.txt";
    char csettings[256];
    strcpy(csettings, settingsPath.toUtf8().constData());
#else
    char csettings[13] = "settings.txt";
#endif

    if (nightmode) {
        writeSettings(csettings,"nightmode","true");
        ui->textBrowser->setStyleSheet("background-color: #1f1414; color: white");
        ui->lineEdit->setStyleSheet("background-color: #1f1414; color: white");
    } else  {
        writeSettings(csettings,"nightmode","false");
        ui->textBrowser->setStyleSheet("");
        ui->lineEdit->setStyleSheet("");
    }
}

#ifdef ANDROID_BUILD
void MainWindow::requestAndroidPermissions()
{
    // Request storage permissions for Android
    QStringList permissions;
    permissions << "android.permission.READ_EXTERNAL_STORAGE";
    permissions << "android.permission.WRITE_EXTERNAL_STORAGE";

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt 6: Use QCoreApplication permission API
    for (const QString &permission : permissions) {
        // In Qt 6.2+, use the new permission API
        // For now, we'll use JNI directly for compatibility
        QJniObject activity = QJniObject::callStaticObjectMethod(
            "org/qtproject/qt/android/QtNative",
            "activity",
            "()Landroid/app/Activity;");

        if (activity.isValid()) {
            QJniObject permissionString = QJniObject::fromString(permission);
            jint result = activity.callMethod<jint>(
                "checkSelfPermission",
                "(Ljava/lang/String;)I",
                permissionString.object<jstring>());

            if (result != 0) { // PackageManager.PERMISSION_GRANTED = 0
                // Request permission using activity
                QJniObject javaPermissions = QJniObject::fromString(permission);
                QJniEnvironment env;
                jobjectArray permissionsArray = env->NewObjectArray(
                    1,
                    env->FindClass("java/lang/String"),
                    javaPermissions.object<jstring>());

                activity.callMethod<void>(
                    "requestPermissions",
                    "([Ljava/lang/String;I)V",
                    permissionsArray,
                    1);

                env->DeleteLocalRef(permissionsArray);
            }
        }
    }
#else
    // Qt 5: Use QtAndroid
    for (const QString &permission : permissions) {
        auto result = QtAndroid::checkPermission(permission);
        if (result == QtAndroid::PermissionResult::Denied) {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList() << permission);
            if (resultHash[permission] == QtAndroid::PermissionResult::Denied) {
                qDebug() << "Permission denied:" << permission;
            }
        }
    }
#endif
}

void MainWindow::shareFile(const QString &filePath, const QString &mimeType)
{
    // Share file using Android Intent
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt 6: Use QJniObject
    QJniObject javaPath = QJniObject::fromString(filePath);
    QJniObject javaMimeType = QJniObject::fromString(mimeType);

    QJniObject intent("android/content/Intent");
    if (intent.isValid()) {
        QJniObject action = QJniObject::getStaticObjectField(
            "android/content/Intent", "ACTION_SEND", "Ljava/lang/String;");
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;",
                              action.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;",
                              javaMimeType.object<jstring>());

        // Get Android context
        QJniObject context = QJniObject::callStaticObjectMethod(
            "org/qtproject/qt/android/QtNative",
            "getContext",
            "()Landroid/content/Context;");

        // Use FileProvider for file sharing
        QJniObject uri = QJniObject::callStaticObjectMethod(
            "androidx/core/content/FileProvider",
            "getUriForFile",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/io/File;)Landroid/net/Uri;",
            context.object(),
            QJniObject::fromString("com.biblex.app.fileprovider").object<jstring>(),
            QJniObject("java/io/File", "(Ljava/lang/String;)V",
                            javaPath.object<jstring>()).object());

        if (uri.isValid()) {
            intent.callObjectMethod("putExtra",
                                  "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;",
                                  QJniObject::getStaticObjectField(
                                      "android/content/Intent", "EXTRA_STREAM",
                                      "Ljava/lang/String;").object<jstring>(),
                                  uri.object<jobject>());

            intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;",
                                  QJniObject::getStaticField<jint>(
                                      "android/content/Intent",
                                      "FLAG_GRANT_READ_URI_PERMISSION"));

            QJniObject chooser = QJniObject::callStaticObjectMethod(
                "android/content/Intent",
                "createChooser",
                "(Landroid/content/Intent;Ljava/lang/CharSequence;)Landroid/content/Intent;",
                intent.object<jobject>(),
                QJniObject::fromString("Share File").object<jstring>());

            // Start activity
            QJniObject activity = QJniObject::callStaticObjectMethod(
                "org/qtproject/qt/android/QtNative",
                "activity",
                "()Landroid/app/Activity;");

            if (activity.isValid()) {
                activity.callMethod<void>("startActivity",
                                         "(Landroid/content/Intent;)V",
                                         chooser.object<jobject>());
            }
        }
    }
#else
    // Qt 5: Use QAndroidJniObject
    QAndroidJniObject javaPath = QAndroidJniObject::fromString(filePath);
    QAndroidJniObject javaMimeType = QAndroidJniObject::fromString(mimeType);

    QAndroidJniObject intent("android/content/Intent");
    if (intent.isValid()) {
        QAndroidJniObject action = QAndroidJniObject::getStaticObjectField(
            "android/content/Intent", "ACTION_SEND", "Ljava/lang/String;");
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;",
                              action.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;",
                              javaMimeType.object<jstring>());

        // Use FileProvider for file sharing
        QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod(
            "androidx/core/content/FileProvider",
            "getUriForFile",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/io/File;)Landroid/net/Uri;",
            QtAndroid::androidContext().object(),
            QAndroidJniObject::fromString("com.biblex.app.fileprovider").object<jstring>(),
            QAndroidJniObject("java/io/File", "(Ljava/lang/String;)V",
                            javaPath.object<jstring>()).object());

        if (uri.isValid()) {
            intent.callObjectMethod("putExtra",
                                  "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;",
                                  QAndroidJniObject::getStaticObjectField(
                                      "android/content/Intent", "EXTRA_STREAM",
                                      "Ljava/lang/String;").object<jstring>(),
                                  uri.object<jobject>());

            intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;",
                                  QAndroidJniObject::getStaticField<jint>(
                                      "android/content/Intent",
                                      "FLAG_GRANT_READ_URI_PERMISSION"));

            QAndroidJniObject chooser = QAndroidJniObject::callStaticObjectMethod(
                "android/content/Intent",
                "createChooser",
                "(Landroid/content/Intent;Ljava/lang/CharSequence;)Landroid/content/Intent;",
                intent.object<jobject>(),
                QAndroidJniObject::fromString("Share File").object<jstring>());

            QtAndroid::startActivity(chooser, 0);
        }
    }
#endif
}
#endif

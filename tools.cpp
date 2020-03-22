#include "tools.h"
#include <QString>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QDebug>
#include <cstdio>
#include<stdio.h>
#include <QFile>
//#include <unistd.h>
//#include <vector>

#define BUFFERSIZE 256
using namespace std;

int reduce(int sum){
            int s1, s2;
           if (sum > 18)
              {
                s2 = sum-20;
                s1 = 2;
           } else if (sum > 9)
              {
                s2 = sum-10;
                s1 = 1;
           } else
              {
                s2 = sum;
                s1 = 0;
              }
    return (s1 + s2);

}
int reverse(int sum){
    sum = 27-sum;
    return(sum);
}

std::string tolowerCase(std::string &str)
{
    const int length = str.length();
    for(int i=0; i < length; ++i)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

int getwordnumericvalue(std::string word, int reduced, int reversed, int type) // type 0 english ordenal, 1 Single Reduction, 2 Francis Bacon, 3 Satanic, 4 Jewish, 5 Sumerian
{
        int s1 = 0, sum = 0;// for summing the letter values.
        if (type != 2) tolowerCase(word);
        for(size_t i=0; i<word.size(); ++i){	    	// loop through the string 1 char at a time
            //qDebug() << int(word[i]) << "\n";
            if (int(word[i]) <= 122 && int(word[i]) >= 97) {
               sum = (int(word[i]-96));// using the ascii value for each letter
               if (type == 3) sum += 35; //Satanic
               if (type == 5) sum = sum*6; // Sumerian
            }
            if (int(word[i]) <= 90 && int(word[i]) >= 65 && type == 2) sum = (int(tolower(word[i])-96)) + 26; //Francis
            if ((int((word[i]) <= 122 && int(word[i]) >= 97) || int(word[i]) == 38) && type == 4) { // Jewish
                if (int(word[i]) == 116) sum = 100; //t
                if (int(word[i]) == 117) sum = 200; //u
                if (int(word[i]) == 120) sum = 300; //x
                if (int(word[i]) == 121) sum = 400; //y
                if (int(word[i]) == 122) sum = 500; //z
                if (int(word[i]) == 106) sum = 600; //j
                if (int(word[i]) == 118) sum = 700; //v
                if (int(word[i]) == 38) sum = 800; //&
                if (int(word[i]) == 119) sum = 900; //w
                if (int(word[i]) <= 115 && int(word[i]) >= 107) { // 10-90
                    sum = ((int(word[i])-96) - 10)*10;
                }

            }
            if (reversed == 1 && int(word[i]) <= 122 && int(word[i]) >= 97) sum = reverse(sum);
            if (reduced == 1 && int(word[i]) <= 122 && int(word[i]) >= 97) sum = reduce(sum);
            if (type == 1 && int(word[i]) <= 122 && int(word[i]) >= 97 && tolower(word[i]) != 's') { //Single reduction
                sum = reduce(sum);
               } else if (tolower(word[i]) == 's' && type == 1) sum = 10;
            if (int(word[i]) == 38 && type == 4) s1 += sum;
            if (int(word[i]) <= 122 && int(word[i]) >= 97) s1 += sum;
            if (int(word[i]) <= 90 && int(word[i]) >= 65) s1 += sum;
         if (int(word[i]) <= 57 && int(word[i]) >= 49) s1 += int(word[i]-48);
        }
        //qDebug() << word[i] << " = " << s1;
        return(s1);
}

bool existSettings(string file)
{
        //std::string stext;
        //eraseAllSubStr(line,"");
        //eraseAllSubStr(line,"");
        //eraseAllSubStr(line,"");
        //std::ofstream fout;  // Create Object of Ofstream
        std::ifstream fin;
        fin.open(file);
       //fout.open (file,ios::app); // Append mode
        if(fin.is_open()) {
        fin.close();
        return true;
        }
       // fout.close(); // Closing the file
    return false;
}

void createSettings(string file)
{
        //std::string stext;
        //eraseAllSubStr(line,"");
        //eraseAllSubStr(line,"");
        //eraseAllSubStr(line,"");
        std::ofstream fout;  // Create Object of Ofstream
       // std::ifstream fin;
       // fin.open(file);
       fout.open (file,ios::app); // Append mode
       fout << "greek=\n";
       fout << "hebrew=\n";
       // if(fin.is_open()) {
       // fin.close();
       // return true;
       // }
        fout.close(); // Closing the file
   // return false;
}


QString readSettings(string file, string entry)
{
    string strReplace = entry+"=";
    ifstream filein(file); //File to read from
    if(!filein)
    {
        cout << "Error opening files!" << endl;
        //return 1;
    }
    string strTemp;
    //bool found = false;
    while(filein >> strTemp)
    {

        if(strTemp.substr(0,strReplace.length()) == strReplace){

            return QString::fromStdString(strTemp.substr(strReplace.length(),strTemp.length()-strReplace.length()));


            //found = true;
        }
        strTemp += "\n";

        //if(found) break;
    }
}

void writeSettings(char file[], string entry,string settings)
{
    string strReplace = entry+"=";
    string strNew = entry+"="+settings;
    //qDebug() << QString::fromStdString(strNew);
    ifstream filein(file); //File to read from
    ofstream fileout("settings.txt.tmp"); //Temporary file
    if(!filein || !fileout)
    {
        cout << "Error opening files!" << endl;
        //return 1;
    }

    string strTemp;
    //bool found = false;
    while(filein >> strTemp)
    {

        if(strTemp.substr(0,strReplace.length()) == strReplace){

            strTemp = strNew;
            qDebug() << QString::fromStdString(strTemp);
            //found = true;
        }
        strTemp += "\n";
        fileout << strTemp;
        //if(found) break;
    }


    filein.close();
    fileout.close();
    const int result = remove(file);
    if (rename("settings.txt.tmp","settings.txt") ==0)
        cout<<"file renamed successfully.";
     else
        cout<<"error remaining file.";
}


void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;

    // Search for the substd::string in std::string in a loop untill nothing is found
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        // If found then erase it from std::string
        mainStr.erase(pos, toErase.length());
    }
}

void eraseToSubstr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    if ((pos  = mainStr.find(toErase) )!= std::string::npos)
        mainStr.erase(0,pos);

}

void eraseFromSubstr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos,lastpos=0;
    while ((pos  = mainStr.find(toErase, pos+1) )!= std::string::npos){

        lastpos = mainStr.find(toErase, pos+1);
        if (lastpos > mainStr.length()) break;
    }
    pos = mainStr.find("<br>",pos+1);
    pos = mainStr.find("<br>",pos+1);
    mainStr.erase(pos,mainStr.length()-pos);

}

void deleteAttributes(std::string & attr)
{

    //eraseAllSubStr(attr,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"//www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
    //eraseAllSubStr(attr,"<html xmlns=\"//www.w3.org/1999/xhtml\">");

}

bool replacestring(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void readinfile(string file, QString &h_def1, QString &h_def2, QString &h_def3, QString &g_def1, QString &g_def2, QString &g_def3)
{
    stringstream strTemp;
    string attr,tstring;
    size_t pos = std::string::npos,nextpos;
    QString html,def1,def2,def3;
    ifstream filein(file); //File to read from
    while(strTemp << filein.rdbuf())
    {
        attr = strTemp.str();
        eraseToSubstr(attr,"Short Definition:");
        eraseFromSubstr(attr,"Definition");



        eraseAllSubStr(attr,"</span>");
        eraseAllSubStr(attr,"<br>");
        pos = attr.find("<");
        //qDebug() << QString::fromStdString(attr);
        def1 = QString::fromStdString(attr.substr(0,pos));

        pos = attr.find("Definition",pos+1);
        nextpos = attr.find("<",pos+1);
        if (pos < attr.length()) def2 = QString::fromStdString(attr.substr(pos,nextpos-pos));
        if (nextpos < pos) pos = nextpos;
        pos = attr.find("Definition",pos+1);
        if (pos < attr.length()) def3 = QString::fromStdString(attr.substr(pos,attr.length()-pos));

        tstring = def1.toUtf8().constData();
        eraseAllSubStr(tstring,"<br>");
        if (tstring.find("Definition:") > tstring.length()) replacestring(tstring,"Definition","Definition: ");
        eraseAllSubStr(tstring,"Short Definition: ");
        def1 = QString::fromStdString(tstring);

        tstring = def2.toUtf8().constData();
        eraseAllSubStr(tstring,"<br>");
        if (tstring.find("Definition:") > tstring.length()) replacestring(tstring,"Definition","Definition: ");
        eraseAllSubStr(tstring,"Definition: ");
        def2 = QString::fromStdString(tstring);

        tstring = def3.toUtf8().constData();
        eraseAllSubStr(tstring,"<br>");
        if (tstring.find("Definition:") > tstring.length()) replacestring(tstring,"Definition","Definition: ");
        eraseAllSubStr(tstring,"Definition: ");
        def3 = QString::fromStdString(tstring);
    }
    filein.close();
    if (file.find("greek") > file.length()) {
        g_def1 = def1;
        g_def2 = def2;
        g_def3 = def3;
    } else {
        h_def1 = def1;
        h_def2 = def2;
        h_def3 = def3;
    }
}

QString readbib(int ns, QString nstype, QString hebrew_lexicon, QString greek_lexicon)
{
    QString html="",h_def1,h_def2,h_def3,g_def1,g_def2,g_def3,g_link,h_link;
    string settingsFilehebrew=hebrew_lexicon.toUtf8().constData();
    string settingsFilegreek=greek_lexicon.toUtf8().constData();
    settingsFilehebrew += "/"+std::to_string(ns)+".htm";
    settingsFilegreek += "/"+std::to_string(ns)+".htm";
    readinfile(settingsFilegreek,h_def1,h_def2,h_def3,g_def1,g_def2,g_def3);
    readinfile(settingsFilehebrew,h_def1,h_def2,h_def3,g_def1,g_def2,g_def3);
    g_link = " <a href=\"https://biblehub.com/str/greek/"+QString::number(ns)+".htm\">Source</a> ";
    h_link = " <a href=\"https://biblehub.com/str/hebrew/"+QString::number(ns)+".htm\">Source</a> ";
    //g_link = "<a href=\"#\" onClick=\"MyWindow=window.open('"+QString::fromStdString(settingsFilegreek)+"','MyWindow','width=600,height=300'); return false;\">Source</a>";
    //h_link = "<a href=\"#\" onClick=\"MyWindow=window.open('"+QString::fromStdString(settingsFilehebrew)+"','MyWindow','width=600,height=300'); return false;\">Source</a>";
    if (nstype == "EO") html = "<center><h2>"+formattext("English Ordinal",5,0)+" = "+formattext(QString::number(ns),1,0)+"</h2></center>";
    if (nstype == "Jew") html = "<center><h2>"+formattext("Jewish",3,0)+" = "+formattext(QString::number(ns),1,0)+"</h2></center>";
    if (nstype == "Sum") html = "<center><h2>"+formattext("Sumerian",4,0)+" = "+formattext(QString::number(ns),1,0)+"</h2></center>";
    html +=Qtotable("",1,0,0,0);
    if (nstype == "EO") html +=Qtotable("",0,1,0,0)+Qtotable("<h2>Greek</h3>",0,0,1,400)+Qtotable("<h2>Hebrew</h3>",0,0,1,500)+Qtotable("",0,2,0,0);
    html +=Qtotable("",0,1,0,0)+Qtotable("Short Definition: "+formattext(h_def1,2,0),0,0,1,400)+Qtotable("Short Definition: "+formattext(g_def1,2,0),0,0,1,400)+Qtotable("",0,2,0,0);
    html +=Qtotable("",0,1,0,0)+Qtotable("Definition: "+formattext(h_def2,2,0),0,0,1,400)+Qtotable("Definition: "+formattext(g_def2,2,0),0,0,1,400)+Qtotable("",0,2,0,0);
    html +=Qtotable("",0,1,0,0)+Qtotable("Definition: "+formattext(h_def3,2,0),0,0,1,400)+Qtotable("Definition: "+formattext(g_def3,2,0),0,0,1,400)+Qtotable("",0,2,0,0);
    html +=Qtotable("",0,1,0,0)+Qtotable(g_link,0,0,1,400)+Qtotable(h_link,0,0,1,500)+Qtotable("",0,2,0,0);
    html +=Qtotable("",2,0,0,0);
    //qDebug() << html;
    return html;

}

string totable(string str, int start, int endrow, int colum, int width)
{
    //QString str;
    if (start == 1) str = "<table>";
    if (endrow == 1) str = "<tr>";
    if (colum == 1) str = "<td width=\"" + to_string(width) + "\">" + str + "</td>";
    if (endrow == 2) str = "</tr>";
    if (start == 2) str = "</table>";
    return str;
}

QString Qtotable(QString str, int start, int endrow, int colum, int width)
{
    //QString str;
    if (start == 1) str = "<table>";
    if (endrow == 1) str = "<tr>";
    if (colum == 1) str = "<td width=\"" + QString::number(width) + "\">" + str + "</td>";
    if (endrow == 2) str = "</tr>";
    if (start == 2) str = "</table>";
    return str;
}

QString formattext(QString line, int color, int bold)
{

    QString formatedcolor;
    QString formatedtag;
    int i;
    if (color != 10 && color != 20) {
    switch (color) {
        case 0: //none
        formatedcolor = line;
        break;
        case 1: // red
        formatedcolor = "<font color=\"red\">" +line+ "</font>";
        break;
        case 2: // blue
        formatedcolor = "<font color=\"#00ccff\">" +line+ "</font>";
        break;
        case 3: // purple
        formatedcolor = "<font color=\"#6600cc\">" +line+ "</font>";
        break;
        case 4: // cyan
        formatedcolor = "<font color=\"#999999\">" +line+ "</font>";
        break;
        case 5: // Green
        formatedcolor = "<font color=\"#336600\">" +line+ "</font>";
        break;
    }
    switch (bold) {
        case 0: //none
        formatedtag = formatedcolor;
        break;
        case 1: //bold
        formatedtag = "<b>" +formatedcolor+"</b>";
        break;
        case 2: //italic
        formatedtag = "<i>" +formatedcolor+"</i>";
        break;
        case 3: //bold & italic
        formatedtag = "<i><b>" +formatedcolor+"</b></i>";
        break;

    }
    }
    else formatedtag = line;
    //qDebug() <<  QString::fromStdString(formatedtag);
    if (color == 10) for (i=1;i<bold;i++) formatedtag=formatedtag+"&emsp;"; //tab after
    if (color == 20) for (i=1;i<bold;i++) formatedtag="&emsp;"+formatedtag; //tab before
    // html == "#include &lt;QtCore&gt;"
    return formatedtag;
}

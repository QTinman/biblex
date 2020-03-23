#ifndef TOOLS_H
#define TOOLS_H
#include <QString>



    int reduce(int sum);
    int reverse(int sum);
    std::string tolowerCase(std::string &str);
    int getwordnumericvalue(std::string word, int reduced, int reversed, int type);
    bool existSettings(std::string file);
    void createSettings(std::string file);
    QString readSettings(std::string file, std::string entry);
    void writeSettings(char file[], std::string entry,std::string settings);
    void eraseAllSubStr(std::string & mainStr, const std::string & toErase);
    void eraseToSubstr(std::string & mainStr, const std::string & toErase);
    void eraseFromSubstr(std::string & mainStr, const std::string & toErase);
    QString readbib(int ns, QString nstype, QString hebrew_lexicon, QString greek_lexicon);
    std::string totable(std::string str, int start, int endrow, int colum, int width);
    QString Qtotable(QString str, int start, int endrow, int colum, int width);
    QString formattext(QString line, int color, int bold);
    void readinfile(std::string file, QString &h_def1, QString &h_def2, QString &h_def3, QString &g_def1, QString &g_def2, QString &g_def3);
    bool replacestring(std::string& str, const std::string& from, const std::string& to);
#endif // TOOLS_H

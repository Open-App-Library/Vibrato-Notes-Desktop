/*
 * helper-io.h
 * -----------------------------------------------------------------------
 * This file is to provide quick little helper functions for this project.
 * It is essentially a header-only library.
 */

#pragma once
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>

class HelperIO {
public:

  // File/directory functions
  static QDir dataDir()
  {
    QDir data_dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if ( !data_dir.exists() )
      data_dir.mkpath(".");
    return data_dir;
  }

  // Warning: Returns a char* that must be deleted.
  static char *QString2CString(QString str)
  {
    int len = str.length()+1;
    char *new_string = static_cast<char*>(malloc(len));

    std::string std_str = str.toStdString();

    for (int i=0; i<len-1; i++)
      new_string[i] = std_str[i];

    new_string[len-1] = '\0';
    return new_string;
  }

  static QString fileToQString(QString filename)
  {
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    return val;
  }

  static QJsonDocument fileToQJsonDocument(QString filename)
  {
    return QJsonDocument::fromJson(fileToQString(filename).toUtf8());
  }

  static QString numberToString(int number, bool capitalize=false)
  {
    QString str;
    switch (number) {
    case 0:
      str = "zero";
      break;
    case 1:
      str = "one";
      break;
    case 2:
      str = "two";
      break;
    case 3:
      str = "three";
      break;
    case 4:
      str = "four";
      break;
    case 5:
      str = "five";
      break;
    case 6:
      str = "six";
      break;
    case 7:
      str = "seven";
      break;
    case 8:
      str = "eight";
      break;
    case 9:
      str = "nine";
      break;
    case 10:
      str = "ten";
      break;
    default:
      return QString("%1").arg(number);
    }
    if ( capitalize )
      str[0] = str[0].toUpper();
    return str;
  }

  static bool promptUserYesNo(QString title, QString question, QWidget *parent=nullptr)
  {
    QMessageBox::StandardButton deleteChildrenPrompt;
    deleteChildrenPrompt = QMessageBox::question(parent, title, question,
                                                 QMessageBox::Yes|QMessageBox::No);
    if ( deleteChildrenPrompt == QMessageBox::Yes )
      return true;
    return false;
  }
};

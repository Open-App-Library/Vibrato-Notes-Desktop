/*
 * helper-io.h
 * -----------------------------------------------------------------------
 * This file is to provide quick little helper functions for this project.
 * It is essentially a header-only library.
 */

#ifndef HELPERIO_H
#define HELPERIO_H
#include <QString>
#include <QFile>
#include <QJsonDocument>

QString fileToQString(QString filename)
{
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    return val;
}

QJsonDocument fileToQJsonDocument(QString filename)
{
    return QJsonDocument::fromJson(fileToQString(filename).toUtf8());
}

#endif // HELPERIO_H

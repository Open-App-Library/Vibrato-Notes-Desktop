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

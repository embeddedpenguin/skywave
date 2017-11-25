#ifndef GSTERMINAL_H
#define GSTERMINAL_H

#pragma once
#include <QPlainTextEdit>
#include "gs_headers.h"

class GSTerminal : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit GSTerminal(QWidget* parent = nullptr);
    ~GSTerminal();

     void putData(const QByteArray &data);

private:
     QFile *logPath;
};

#endif // GSTERMINAL_H

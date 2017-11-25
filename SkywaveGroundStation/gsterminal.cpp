#include "gsterminal.h"
#include <QTextStream>
#include <QDir>


GSTerminal::GSTerminal(QWidget *parent) : QPlainTextEdit(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setMinimumHeight(50);
    setMaximumHeight(200);

    QPalette p =  palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
    QFont newFont("Consolas", 10, QFont::Normal, true);
    newFont.setItalic(false);
    setFont(newFont);
    setReadOnly(true);
    int x = 0;
    //QDir::rootPath for open logger <-----------PROBABLY. not tested
    while(QFile::exists(QDir::currentPath() + QString(tr("/log_%1.csv").arg(x))))
    {
        x++;
    }
    logPath = new QFile(QString(QDir::currentPath() + QString(tr("/log_%1.csv").arg(x))), this);


}

GSTerminal::~GSTerminal()
{

}

void GSTerminal::putData(const QByteArray &data)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
    insertPlainText(QString(data));
    QScrollBar* bar = verticalScrollBar();
    //bar->setValue(bar->maximumHeight());
    bar->setSliderDown(true);
    if(logPath->open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream stream(logPath);
        stream << toPlainText();
        logPath->close();
    }
}

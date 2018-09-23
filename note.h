#ifndef NOTE_H
#define NOTE_H
#include <QString>

class Note
{
public:
    Note();
    QString title();
    QString text();
    void    setTitle(QString title);
    void    setText(QString text);
private:
    QString m_title;
    QString m_text;
};

#endif // NOTE_H

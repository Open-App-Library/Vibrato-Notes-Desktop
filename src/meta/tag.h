#ifndef TAG_H
#define TAG_H
#include <QString>

class Tag
{
public:
    Tag();
    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString &value);

private:
    int     m_id;
    QString m_title;
    // TODO: Implement user
};

#endif // TAG_H

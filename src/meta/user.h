#ifndef USER_H
#define USER_H
#include <QObject>
#include <QString>

class User : public QObject
{
  Q_OBJECT

 private:
  QString m_username;
  QString m_email;
  QString m_public_key;
  QString m_private_key;

};

#endif

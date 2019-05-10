#ifndef VCRYPTO_H
#define VCRYPTO_H

#include <QObject>

class VCrypto : public QObject
{
  Q_OBJECT
public:
  explicit VCrypto(QObject *parent = nullptr);

  typedef struct {
    QString
  } keypair;

  bool login(QString email, QString password);
  QString encrypt(QString message);
  QString decrypt(QString enc_message);

signals:

public slots:

private:
  QString m_private_key;
  QString m_public_key;
};

#endif // VCRYPTO_H

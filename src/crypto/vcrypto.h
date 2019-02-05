#ifndef VCRYPTO_H
#define VCRYPTO_H

#include <QObject>

class VCrypto : public QObject
{
  Q_OBJECT
public:
  explicit VCrypto(QObject *parent = nullptr);

signals:

public slots:
};

#endif // VCRYPTO_H
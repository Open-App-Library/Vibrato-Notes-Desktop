#include "vcrypto.h"
#include "vibrato-crypto-utils/vibrato-crypto.h"
#include <sodium.h>

VCrypto::VCrypto(QObject *parent) : QObject(parent)
{
  if (vcrypto_init() < 0) {
    puts("vcrypto cannot init!");
  }
}

bool VCrypto::login(QString email, QString password) {

  unsigned char key[crypto_secretbox_KEYBYTES];
  unsigned char pubkey[crypto_box_PUBLICKEYBYTES];

  if (vcrypto_get_privatekey(key, (unsigned char*) email.toLatin1().data(), password.toLatin1().data()) != 0) {
    puts("FAILED! privkey");
    free(key);
    return false;
  }
  if (vcrypto_get_publickey(pubkey, key) != 0) {
    puts("FAILED! pubkey");
    free(key);
    free(pubkey);
    return false;
  }

  m_private_key = "";
  m_public_key  = "";

  for (int i=0; i < (int) crypto_secretbox_KEYBYTES; i++)
    m_private_key.append(key[i]);

  for (int i=0; i < (int) crypto_box_PUBLICKEYBYTES; i++)
    m_public_key.append(pubkey[i]);

  return true;
}

QString VCrypto::encrypt(QString message) {
  QString b;

  return b;
}

QString VCrypto::decrypt(QString enc_message) {
  QString b;

  return b;
}

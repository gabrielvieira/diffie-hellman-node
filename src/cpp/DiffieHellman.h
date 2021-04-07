#include <openssl/evp.h>
#include <openssl/dh.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/pem.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

char* generate_key_pair(EVP_PKEY **ppkey);
char* key_agreement(EVP_PKEY *ppkey, const char* public_key_hex);

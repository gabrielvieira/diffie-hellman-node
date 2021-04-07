#include "DiffieHellman.h"

void handleErrors() {
    printf("trigger callback error or something like that");
}

static int hex_to_int(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

const unsigned char* hex_to_byte_array(const char* src, long byte_array_length) {

    unsigned char* target = (unsigned char*)malloc(sizeof(unsigned char) * byte_array_length);
    long i = 0;
    while(*src && src[1]) {
        *(target + i) = hex_to_int(*src)*16 + hex_to_int(src[1]);
        src += 2;
        i++;
    }
    return target;
}

void byte_array_to_hex(unsigned char byte_array[], size_t length, char* hex_string) {

    for(int i=0; i < length; i++) {
      sprintf(&hex_string[i*2], "%02X", byte_array[i]);
    }
}

X509_PUBKEY* import_public_key(const char *public_key_hex) {

    long byte_array_length = (strlen(public_key_hex) / 2 );
    const unsigned char* result = hex_to_byte_array(public_key_hex, byte_array_length);
    X509_PUBKEY *imported_server_key = d2i_X509_PUBKEY(NULL, &result, byte_array_length);
    return imported_server_key;
}

char* export_public_key(EVP_PKEY *pkey) {

    unsigned char *buf = NULL;
    X509_PUBKEY *x509 = NULL;

    if (1 != X509_PUBKEY_set(&x509, pkey)) {
        X509_PUBKEY_free(x509);
        return "";
    }

    int length = 0;
    if (0 >= (length = i2d_X509_PUBKEY(x509, &buf))) {
        if(buf != NULL) {
            free(buf);
        }
        X509_PUBKEY_free(x509);
        return "";
    }

    char* hex_string = (char*)malloc(sizeof(char) * (length*2 + 1));
    byte_array_to_hex(buf, length, hex_string);
    X509_PUBKEY_free(x509);
    free(buf);
    return hex_string;
}

// generate DH keypair, return public key in hex string and save keypair in EVP_PKEY pointer
char* generate_key_pair(EVP_PKEY **ppkey) {

    EVP_PKEY *params = NULL;
    EVP_PKEY_CTX *kctx = NULL;
    /* Use built-in parameters */
    if(NULL == (params = EVP_PKEY_new())) handleErrors();
    if(1 != EVP_PKEY_set1_DH(params,DH_get_1024_160())) {
        handleErrors();
        EVP_PKEY_free(params);
    }

    /* Create context for the key generation */
    if(!(kctx = EVP_PKEY_CTX_new(params, NULL))){
        handleErrors();
        EVP_PKEY_free(params);
        EVP_PKEY_CTX_free(kctx);
    }
    /* Generate a new key */
    if(1 != EVP_PKEY_keygen_init(kctx)) {
        handleErrors();
        EVP_PKEY_free(params);
        EVP_PKEY_CTX_free(kctx);
    }

    if(1 != EVP_PKEY_keygen(kctx, ppkey)) {
        handleErrors();
        EVP_PKEY_free(params);
        EVP_PKEY_CTX_free(kctx);
    }

    EVP_PKEY_free(params);
    EVP_PKEY_CTX_free(kctx);

    return export_public_key(*ppkey);
}

char* key_agreement(EVP_PKEY *ppkey, const char* public_key_hex) {

    X509_PUBKEY *imported_server_key = import_public_key(public_key_hex);
    EVP_PKEY *peerkey = NULL;
    EVP_PKEY_CTX *ctx = NULL;
    unsigned char *skey = NULL;
    size_t skeylen;

    if (NULL == (peerkey = X509_PUBKEY_get(imported_server_key))) {
        printf("Error importing x509 pub key");
        handleErrors();
        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
    }

    /* NB: assumes pkey, peerkey have been already set up */
    ctx = EVP_PKEY_CTX_new(ppkey, NULL);
    if (!ctx) {

        handleErrors();
        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
        EVP_PKEY_CTX_free(ctx);
    }
           /* Error occurred */
    if (EVP_PKEY_derive_init(ctx) <= 0) {

        handleErrors();
        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
        EVP_PKEY_CTX_free(ctx);
    }
           /* Error */
    if (EVP_PKEY_derive_set_peer(ctx, peerkey) <= 0) {

        handleErrors();
        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
        EVP_PKEY_CTX_free(ctx);
    }
    /* Error */
    /* Determine buffer length */
    if (EVP_PKEY_derive(ctx, NULL, &skeylen) <= 0) {
        handleErrors();

        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
        EVP_PKEY_CTX_free(ctx);
    }
           /* Error */
    skey = OPENSSL_malloc(skeylen);

    if (!skey) {

        printf("Error alloc open ssl key");
        OPENSSL_free(skey);
        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
        EVP_PKEY_CTX_free(ctx);
    }

    if (EVP_PKEY_derive(ctx, skey, &skeylen) <= 0) {

        OPENSSL_free(skey);
        X509_PUBKEY_free(imported_server_key);
        EVP_PKEY_free(peerkey);
        EVP_PKEY_CTX_free(ctx);
        printf("derivate error");
    }
           /* Error */
    char* hex_shared_secret = malloc(sizeof(char) * (skeylen*2 + 1));
    byte_array_to_hex(skey, skeylen, hex_shared_secret);

    //oopen ssl clean
    OPENSSL_free(skey);
    X509_PUBKEY_free(imported_server_key);
    EVP_PKEY_free(peerkey);
    EVP_PKEY_CTX_free(ctx);

    return hex_shared_secret;
}

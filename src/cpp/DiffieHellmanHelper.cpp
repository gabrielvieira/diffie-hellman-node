#include "DiffieHellmanHelper.h"

std::string DiffieHellmanHelper::generateKeyPair() {
    if (client_dhkey != NULL) {
        EVP_PKEY_free(client_dhkey);
        client_dhkey = NULL;
    }
    char *result = generate_key_pair(&this->client_dhkey);
    std::string resultString(result);
    free(result);
    return resultString;
}

std::string DiffieHellmanHelper::keyAgreement(std::string publicKey) {
    const char *cstr = publicKey.c_str();
    char *result = key_agreement(this->client_dhkey, cstr);
    std::string resultString(result);
    free(result);
    return resultString;
}
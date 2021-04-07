#include <iostream>

extern "C" {
    #include "DiffieHellman.h"
}

class DiffieHellmanHelper {
    public:
        std::string generateKeyPair();
        std::string keyAgreement(std::string publicKey);
    private:
        EVP_PKEY *client_dhkey = NULL;
};
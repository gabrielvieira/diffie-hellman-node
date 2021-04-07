const diffieHellmanAddon = require('../../build/Release/DiffieHellmanAddon');

class DiffieHellman {
   
    constructor() {
        this.dh = new diffieHellmanAddon.DiffieHellmanWrapper();
    }

    generateKeyPair() {
        return this.dh.generateKeyPair();
    }

    keyAgreement(peerPublicKey) {
        return this.dh.keyAgreement(peerPublicKey);
    }
}

module.exports = DiffieHellman;
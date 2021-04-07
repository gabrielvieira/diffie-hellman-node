const hkdf = require('futoin-hkdf');

class Hkdf {

    config = {
        salt: Buffer.from("dd0a655b69231c8a42daf1ae0e7d02953f26447442542baca35db30174fece1a", 'hex'),
        info: Buffer.from("32383944334235453838324334", 'hex'),
        hash: 'SHA-256'
    }

    derivateSharedSecret(secret) {
        // initial keying material
        const hexSecret = Buffer.from(secret,'hex');
        const length = 32;
        return hkdf(hexSecret, length, this.config).toString('hex');
    }
}

module.exports = Hkdf;
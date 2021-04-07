const crypto = require('crypto');

class AESCipher {

    constructor(aesKey, iv) {
        this.aesKey = Buffer.from(aesKey, 'hex');
        this.iv = Buffer.from(iv, 'hex');
    }

    encrypt(toEncrypt) {
        const cipher = crypto.createCipheriv('aes-256-cbc', this.aesKey, this.iv);
        let encrypted = cipher.update(toEncrypt, 'utf8', 'hex');
        encrypted += cipher.final('hex');
        return encrypted;
    }

    decrypt(encrypted) {
        const decipher = crypto.createDecipheriv('aes-256-cbc', this.aesKey, this.iv);
        let decrypted = decipher.update(encrypted, 'hex', 'utf8');
        return (decrypted + decipher.final('utf8'));
    }
}

module.exports = AESCipher;
const crypto = require("../index")
const assert = require('assert');


describe('Crypto tests', () => {

    it('should be same shared secret across fixed and multiple instances', () => {
    
        const diffieHellmanA = new crypto.DiffieHellman();
        
        for (i = 0; i < 500; i++) {
            const diffieHellmanB = new crypto.DiffieHellman();
            const publicKeyA = diffieHellmanA.generateKeyPair();
            const publicKeyB = diffieHellmanB.generateKeyPair();
            const sharedSecretA = diffieHellmanA.keyAgreement(publicKeyB);
            const sharedSecretB = diffieHellmanB.keyAgreement(publicKeyA);
            assert.equal(sharedSecretA, sharedSecretB);
        }
    });

    it('test external key agreement', () => { 
        const peerPublicKey = "308201B73082012C06072A8648CE3E02013082011F02818100B10B8F96A080E01DDE92DE5EAE5D54EC52C99FBCFB06A3C69A6A9DCA52D23B616073E28675A23D189838EF1E2EE652C013ECB4AEA906112324975C3CD49B83BFACCBDD7D90C4BD7098488E9C219A73724EFFD6FAE5644738FAA31A4FF55BCCC0A151AF5F0DC8B4BD45BF37DF365C1A65E68CFDA76D4DA708DF1FB2BC2E4A437102818100A4D1CBD5C3FD34126765A442EFB99905F8104DD258AC507FD6406CFF14266D31266FEA1E5C41564B777E690F5504F213160217B4B01B886A5E91547F9E2749F4D7FBD7D3B9A92EE1909D0D2263F80A76A6A24C087A091F531DBF0A0169B6A28AD662A4D18E73AFA32D779D5918D08BC8858F4DCEF97C2A24855E6EEB22B3B2E5021500F518AA8781A8DF278ABA4E7D64B7CB9D494623530381840002818064311FFB11513FE421996B8F5ADD083564363F4F4492DB97DB4C0AB54A623FA4B5968F247ABF007D3F58A7E2F38BBE9E227C52A0518BCC00A8494801673DD195A5CA93FCB65BD65917517DD74E956BA041E4A377E0D21940E2CDEAB4FBDD0FFCEE581CBF40F603C9C4B4C2B521628C176DF27350C3641DDE9DADD4D84DC012AF"
        const diffieHellman = new crypto.DiffieHellman();
        diffieHellman.generateKeyPair();
        const sharedSecret = diffieHellman.keyAgreement(peerPublicKey);
        assert.ok(sharedSecret);
    });

    it('derived key should be same as expected', () => { 
        const sharedSecret = "2F00BFC076D21CBFB93219E531BDC4D2DFC6C97B72B59C9FE20408F17B6775CF0726BD3EEEA38AD6B65611D8515B2CF583F794E7DC40A7CBCCF9DE291A1A4EA280523C5904310A900F77AF8F5B1C3A47809F100E1D9742645A989F3CE5339D1391942CEA7B868BF8371DB9B2D16C826DA3A6ED179A8CD7A3F4952A64BA1E46A5";
        const hkdf = new crypto.Hkdf();
        let expectedAesKey = "34c2bbdc869384c94156f41c04655b544d3c70ea01ea1259b0426d6d01a5efcd";
        const derivedAesKey = hkdf.derivateSharedSecret(sharedSecret);
        assert.equal(expectedAesKey, derivedAesKey);
    });

    it('encrypted text should be same as expected', () => {  
        const plainText = "test string to encrypt";
        const expectedEncrypt = "1cb098964cd17763056bb6a3baf0dfa9b7df4a3d6fff5b5c64cc10f6fa73b4ef";
        const aesKey = "34c2bbdc869384c94156f41c04655b544d3c70ea01ea1259b0426d6d01a5efcd";
        const iv = "45bd286474ec0a09705b882381a79404";
        let cipher = new crypto.AESCipher(aesKey, iv);
        let encrypted = cipher.encrypt(plainText);
        assert.equal(encrypted, expectedEncrypt);
    });

    it('decrpted text should be same as expected', () => {  
        const expectedPlainText = "test string to encrypt"
        const encryptedText = "1cb098964cd17763056bb6a3baf0dfa9b7df4a3d6fff5b5c64cc10f6fa73b4ef"
        const aesKey = "34c2bbdc869384c94156f41c04655b544d3c70ea01ea1259b0426d6d01a5efcd";
        const iv = "45bd286474ec0a09705b882381a79404";
        let cipher = new crypto.AESCipher(aesKey, iv);
        let decrypted = cipher.decrypt(encryptedText);
        assert.equal(expectedPlainText, decrypted);
    });
});

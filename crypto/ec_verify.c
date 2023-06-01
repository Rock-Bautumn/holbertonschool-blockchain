#include "hblk_crypto.h"

/**
 * ec_verify - Verify the signature of a given set of bytes,
 * using a given EC_KEY public key
 * @key: The address of the EC_KEY containing the private key
 * @msg: The digest hash value (given bytes) to verify
 * @msglen: The length of the given bytes/digest hash value
 * @sig: The signature to verify
 * Return: The result of whether it is verified or not, 1 = verified, 0 = not
*/
int ec_verify(
	EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t const *sig)
{
	int result;

	if (!key || !msg || !sig || sig->len == 0)
		return (0);

	result = ECDSA_verify(0, msg, (int)msglen,
		(unsigned char *)sig->sig, (int)sig->len, (EC_KEY *)key);

	return (result);
}

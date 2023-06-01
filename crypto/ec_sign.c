#include "hblk_crypto.h"

/**
 * ec_sign - Sign a given set of bytes, using a given EC_KEY private key,
 * into the given sig_t structure
 * @key: The EC_KEY object containing the private key
 * @msg: The value of the hash value (the given bytes) to sign
 * @msglen: The length of the digest hash value (the given bytes)
 * @sig: The signature structure that stores the signature and its length
 * Return: The signature that is contained in the given sig_t signature struct
*/

uint8_t *ec_sign(
	EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t *sig)
{
	uint32_t sig_len = 0;

	if (!key || !msg || !sig)
		return (NULL);

	memset(sig->sig, '\0', sizeof(sig->sig));

	if (!ECDSA_sign(0, msg, msglen, sig->sig, &sig_len, (EC_KEY *)key))
	{
		sig->len = 0;
		return (NULL);
	}

	sig->len = (uint8_t)sig_len;

	return (sig->sig);
}

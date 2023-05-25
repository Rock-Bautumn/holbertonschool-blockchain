#include "hblk_crypto.h"

/**
 * ec_create - Create a public and private key using EC_CURVE
 * Return: The created EC_KEY object containing the private and public keys
*/

EC_KEY *ec_create(void)
{
	EC_KEY *ec_key;

	ec_key = EC_KEY_new_by_curve_name(EC_CURVE);

	if (!EC_KEY_generate_key(ec_key))
	{
		EC_KEY_free(ec_key);
		return (NULL);
	}

	return (ec_key);
}

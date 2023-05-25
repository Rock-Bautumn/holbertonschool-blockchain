#include "hblk_crypto.h"

/**
 * ec_to_pub - Extract public from a EC_KEY object
 * @key: The EC_KEY object to extract the public key from
 * @pub: The buffer to save the extracted public key
 * Return: The address of the saved extracted public key
*/

uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *ec_point;
	const EC_GROUP *ec_group;
	size_t len = 0;

	if (!key)
		return (NULL);

	ec_point = EC_KEY_get0_public_key(key);
	ec_group = EC_KEY_get0_group(key);

	if (!ec_point || !ec_group)
		return (NULL);

	len = EC_POINT_point2oct(ec_group, ec_point, POINT_CONVERSION_UNCOMPRESSED,
		pub, EC_PUB_LEN, NULL);

	if (len != EC_PUB_LEN)
		return (NULL);

	return (pub);
}

#include "hblk_crypto.h"

/**
 * ec_from_pub - Create an EC_KEY object from a given public key string
 * @pub: The public key string
 * Return: The created EC_KEY object
*/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	const EC_GROUP *ec_group;
	EC_POINT *ec_point;
	EC_KEY *ec_key;

	if (!pub)
		return (NULL);

	ec_key = EC_KEY_new_by_curve_name(EC_CURVE);
	ec_group = EC_KEY_get0_group(ec_key);
	ec_point = EC_POINT_new(ec_group);

	if (!ec_point)
	{
		EC_KEY_free(ec_key);
		return (NULL);
	}

	if (!EC_POINT_oct2point(ec_group, ec_point, pub, EC_PUB_LEN, NULL))
	{
		EC_KEY_free(ec_key);
		EC_POINT_free(ec_point);
		return (NULL);
	}

	if (!EC_KEY_set_public_key(ec_key, ec_point))
	{
		EC_KEY_free(ec_key);
		EC_POINT_free(ec_point);
		return (NULL);
	}

	EC_POINT_free(ec_point);

	return (ec_key);
}

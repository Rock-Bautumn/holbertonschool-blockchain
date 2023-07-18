#include "../blockchain.h"

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_change;

	tx_change = calloc(1, sizeof(tx_out_t));
	if (!tx_change)
		return (NULL);

	tx_change->amount = amount;
	memcpy(tx_change->pub, pub, EC_PUB_LEN);
	SHA256((const unsigned char *)tx_change,
		4 + EC_PUB_LEN,
		tx_change->hash);

	return (tx_change);
}

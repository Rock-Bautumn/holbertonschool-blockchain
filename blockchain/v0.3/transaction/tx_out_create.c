#include "../blockchain.h"

/**
 * tx_out_create - Create a tx_out data structure
 * @amount: The amount of the transaction
 * @pub: The public key of the transaction recipient
 * Return: The address of the created transaction data structure, NULL if fail
*/

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_change = NULL;

	tx_change = calloc(1, sizeof(tx_out_t));
	if (!tx_change)
		return (NULL);

	tx_change->amount = amount;
	memcpy(tx_change->pub, pub, EC_PUB_LEN);
	SHA256((const unsigned char *)tx_change,
		sizeof(tx_change->amount) + EC_PUB_LEN,
		tx_change->hash);

	return (tx_change);
}

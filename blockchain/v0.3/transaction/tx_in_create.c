#include "blockchain.h"

/**
 * tx_in_create - Create a transaction input data structure
 * @unspent: The unspent transaction to be converted to transaction input
 * Return: The created transaction input, NULL if failure
*/

tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *input;

	input = calloc(1, sizeof(tx_in_t));

	if (!unspent || !input)
		return (NULL);

	memcpy(input->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(input->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(input->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);

	return (input);
}

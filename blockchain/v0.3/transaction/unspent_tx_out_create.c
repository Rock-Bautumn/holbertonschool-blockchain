#include "blockchain.h"

/**
 * unspent_tx_out_create - Create an unspent output transaction data structure
 * @block_hash: The output transaction block hash
 * @tx_id: The transaction hash
 * @out: The transaction output data structure
 * Return: The address of the created unspent transaction output, or NULL if
 * failure
*/

unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out)
{
	unspent_tx_out_t *unspent;

	unspent = calloc(1, sizeof(unspent_tx_out_t));
	if (!unspent)
		return (NULL);

	memcpy(unspent->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&unspent->out, out, sizeof(tx_out_t));

	return (unspent);
}

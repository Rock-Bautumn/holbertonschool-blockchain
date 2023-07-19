#include "blockchain.h"


/**
 * tx_in_sign - Sign a transaction input from a given transaction ID with the
 * @in: The transaction in
 * @tx_id: The transaction in ID to match
 * @sender: The private key of the sender
 * @all_unspent: The list of all unspent transaction outputs to date
 * Return: The address of the resulting signature structure or NULL if failure
*/

sig_t *tx_in_sign(
	tx_in_t *in,
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender,
	llist_t *all_unspent)
{
	uint8_t sender_pub_key[EC_PUB_LEN];
	unspent_tx_out_t *uxto;
	uint32_t index, unspent_size;

	if (in == NULL || tx_id == NULL ||
		sender == NULL || all_unspent == NULL)
		return (NULL);

	unspent_size = llist_size(all_unspent);
	for (index = 0; index < unspent_size; index++)
	{
		uxto = llist_get_node_at(all_unspent, index);
		if (memcmp(in->tx_out_hash, uxto->out.hash,
				SHA256_DIGEST_LENGTH) == 0)
			break;
	}

	if (index == unspent_size ||
		!ec_to_pub(sender, sender_pub_key) ||
		memcmp(sender_pub_key, uxto->out.pub, EC_PUB_LEN) != 0 ||
		!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))

		return (NULL);

	return (&in->sig);
}

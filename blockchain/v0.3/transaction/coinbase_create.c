#include "blockchain.h"

/**
 * coinbase_create - Create a coinbase transaction
 * @receiver: Contains the public key of the miner, who will receive coinbase
 * coins
 * @block_index: The index of the block the coinbase transaction will belong to
 * Return: The address of the created coinbase transaction, NULL if failure
*/

transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *transaction;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	uint8_t pub_key[EC_PUB_LEN];

	transaction = calloc(1, sizeof(transaction_t));
	if (!transaction)
		return (NULL);
	tx_in = calloc(1, sizeof(tx_in_t));
	if (!tx_in)
		goto FAIL1;
	if (!memcpy(tx_in->tx_out_hash, &block_index, 4) ||
		!ec_to_pub(receiver, pub_key))
		goto FAIL2;
	tx_out = tx_out_create(COINBASE_AMOUNT, pub_key);
	if (!tx_out)
		goto FAIL2;
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs)
		goto FAIL3;
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->outputs)
		goto FAIL4;
	if (llist_add_node(transaction->inputs, tx_in, ADD_NODE_REAR) ||
		llist_add_node(transaction->outputs, tx_out, ADD_NODE_REAR) ||
		!transaction_hash(transaction, transaction->id))
		goto FAIL5;
	return (transaction);
FAIL5:
	llist_destroy(transaction->outputs, 0, NULL);
FAIL4:
	llist_destroy(transaction->inputs, 0, NULL);
FAIL3:
	free(tx_out);
FAIL2:
	free(tx_in);
FAIL1:
	free(transaction);
	return (NULL);
}

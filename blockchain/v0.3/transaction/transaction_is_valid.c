#include "blockchain.h"

#define SDL SHA256_DIGEST_LENGTH

/**
 * check_unspent - Check the unspent transactions for a match and
 * then check if that match is valid as an input in the transaction
 * @all_unspent: All unspent transaction outputs
 * @unspent_qty: The number of items in the all_unspent linked list
 * @transaction: The transaction to check is valid
 * @tx_in: The transaction in to compare against
 * Return: The amount of the unspent transaction, -1 if failure
*/

ssize_t check_unspent(
	llist_t *all_unspent,
	uint32_t unspent_qty,
	const transaction_t *transaction,
	tx_in_t *tx_in
)
{
	uint32_t index = 0;

	unspent_tx_out_t *t_u_t; /* t_u_t = this_unsigned_tx */
	EC_KEY *pub_key;

	for (; index < unspent_qty; index++)
	{
		t_u_t = llist_get_node_at(all_unspent, index);
		if (!t_u_t)
			return (-1);

		if (!memcmp(t_u_t->tx_id, tx_in->tx_id, SDL) &&
			!memcmp(t_u_t->block_hash, tx_in->block_hash, SDL) &&
			!memcmp(t_u_t->out.hash, tx_in->tx_out_hash, SDL))
			break;
	}

	if (index == unspent_qty)
		return (-1);

	pub_key = ec_from_pub(t_u_t->out.pub);
	if (!ec_verify(pub_key, transaction->id, SDL, &tx_in->sig))
	{
		EC_KEY_free(pub_key);
		return (-1);
	}
	EC_KEY_free(pub_key);

	return (t_u_t->out.amount);
}

/**
 * transaction_is_valid - Check if a transaction is valid or not
 * @transaction: The transaction to check validity
 * @all_unspent: All unspent transaction outputs
 * Return: 0/false if not valid, 1/true if valid
*/
int transaction_is_valid(
	transaction_t const *transaction,
	llist_t *all_unspent)
{
	uint8_t tx_hash[SDL];
	uint32_t unspent_qty, inputs_qty, outputs_qty, index = 0;
	ssize_t this_in_amount = 0, all_in = 0, all_out = 0;
	tx_in_t *this_tx_in;

	if (!transaction || !all_unspent)
		return (false);
	transaction_hash(transaction, tx_hash);
	if (memcmp(transaction->id, tx_hash, SDL))
		return (false);
	unspent_qty = llist_size(all_unspent);
	inputs_qty = llist_size(transaction->inputs);

	for (; index < inputs_qty; index++)
	{
		this_tx_in = llist_get_node_at(transaction->inputs, index);
		this_in_amount = check_unspent(
			all_unspent, unspent_qty, transaction, this_tx_in);
		if (this_in_amount == -1)
			return (false);
		all_in += this_in_amount;
	}

	outputs_qty = llist_size(transaction->outputs);
	for (index = 0; index < outputs_qty; index++)
		all_out += ((tx_out_t *)llist_get_node_at(
			transaction->outputs, index))->amount;

	if (all_in != all_out)
		return (false);

	return (true);
}

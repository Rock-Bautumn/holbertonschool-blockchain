#include "blockchain.h"

/**
 * transaction_init - Initialize a new transaction data structure
 * Return: The address of the new transaction, or NULL if failure
*/
transaction_t *transaction_init(void)
{
	transaction_t *new_tx;

	new_tx = malloc(sizeof(transaction_t));
	if (new_tx == NULL)
		return (NULL);
	new_tx->inputs = llist_create(MT_SUPPORT_FALSE);
	new_tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!(new_tx->inputs))
	{
		free(new_tx);
		return (NULL);
	}
	else if (!(new_tx->outputs))
	{
		llist_destroy(new_tx->inputs, 0, NULL);
		return (NULL);
	}
	return (new_tx);
}

/**
 * process_balance - Handle the balance by selecting the inputs and creating
 * the outputs
 * @unspents: All unspent
 * @inputs: The transaction inputs
 * @outputs: The transaction outputs
 * @sender_pub_key: The public key of the sender
 * @receiver_pub_key: The public key of the receiver
 * @sending_amount: The amount being sent by the sender in the transaction
 * Return: The value of the sum of the inputs being used, -1 if failure
*/
ssize_t process_balance(
	llist_t *unspents,
	llist_t *inputs,
	llist_t *outputs,
	uint8_t sender_pub_key[EC_PUB_LEN],
	uint8_t receiver_pub_key[EC_PUB_LEN],
	uint32_t sending_amount)
{
	uint32_t index = 0, unspent_qty;
	ssize_t inputs_amount_sum = 0;
	unspent_tx_out_t *unspent_tok;
	tx_in_t *new_tx_in;
	tx_out_t *new_tx_out;

	if (!unspents || !inputs || !outputs || !sender_pub_key)
		return (-1);
	for (unspent_qty = llist_size(unspents); index < unspent_qty; index++)
	{
		unspent_tok = llist_get_node_at(unspents, index);
		if (!memcmp(unspent_tok->out.pub, sender_pub_key, EC_PUB_LEN))
			{
			new_tx_in = tx_in_create(unspent_tok);
			if (!new_tx_in || llist_add_node(inputs, new_tx_in, ADD_NODE_REAR))
				goto FAIL;

			inputs_amount_sum += unspent_tok->out.amount;
			if (inputs_amount_sum >= sending_amount)
				break;
			}
	}
	if (inputs_amount_sum < sending_amount)
		goto FAIL;
	new_tx_out = tx_out_create(sending_amount, receiver_pub_key);
	if (!new_tx_out || llist_add_node(outputs, new_tx_out, ADD_NODE_REAR))
		goto FAIL;
	if (inputs_amount_sum > sending_amount)
	{
		new_tx_out = tx_out_create(
			inputs_amount_sum - sending_amount, sender_pub_key);
		if (!new_tx_out ||
			llist_add_node(outputs, new_tx_out, ADD_NODE_REAR))
			goto FAIL;
	}
	return (inputs_amount_sum);
FAIL:
	llist_destroy(inputs, 0, NULL);
	llist_destroy(outputs, 0, NULL);
	return (-1);
}

transaction_t *transaction_create(
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount,
	llist_t *all_unspent)
{
	transaction_t *new_tx;
	uint8_t sender_pub_key[EC_PUB_LEN], receiver_pub_key[EC_PUB_LEN];
	uint32_t index = 0, ins_qty;
	tx_in_t *tx_in;

	if (!sender || !receiver || !all_unspent)
		return (NULL);

	new_tx = transaction_init();
	if (new_tx == NULL)
		return (NULL);
	if (!ec_to_pub(sender, sender_pub_key) ||
	!ec_to_pub(receiver, receiver_pub_key))
		goto FFAIL;

	if (process_balance(
		all_unspent, new_tx->inputs, new_tx->outputs,
		sender_pub_key, receiver_pub_key, amount) == -1)
		goto FFAIL;
	if (!transaction_hash(new_tx, new_tx->id))
		goto FAIL;

	for (ins_qty = llist_size(new_tx->inputs); index < ins_qty; index++)
	{
		tx_in = llist_get_node_at(new_tx->inputs, index);
		if (tx_in_sign(tx_in, new_tx->id, sender, all_unspent) == 0)
			goto FAIL;
	}
	return (new_tx);
FAIL:
	llist_destroy(new_tx->inputs, 0, NULL);
	llist_destroy(new_tx->outputs, 0, NULL);
FFAIL:
	free(new_tx);
	return (NULL);
}

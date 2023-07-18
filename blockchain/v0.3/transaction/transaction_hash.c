#include "blockchain.h"

/**
 * transaction_hash - Generate a hash for a transaction
 * @transaction: The transaction to generate the hash from
 * @hash_buf: The location to save the generated hash
 * Return: The generated hash, NULL if failure
*/

uint8_t *transaction_hash(
	transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buf_to_hash;
	uint32_t buf_to_hash_len, buffer_offset = 0, input_qty, output_qty;
	uint32_t inputs_len, outputs_len, index;
	tx_in_t *this_input;
	tx_out_t *this_output;

	if (!transaction || !hash_buf)
		return (NULL);

	input_qty = llist_size(transaction->inputs);
	output_qty = llist_size(transaction->outputs);
	inputs_len = input_qty * TX_IN_HASH_QTY * SHA256_DIGEST_LENGTH;
	outputs_len = output_qty * SHA256_DIGEST_LENGTH;
	buf_to_hash_len = inputs_len + outputs_len;

	buf_to_hash = malloc(buf_to_hash_len);
	if (!buf_to_hash)
		return (NULL);

	for (index = 0; index < input_qty; index++)
	{
		this_input = llist_get_node_at(transaction->inputs, index);
		memcpy(buf_to_hash + buffer_offset,
			this_input,
			TX_IN_HASH_QTY * SHA256_DIGEST_LENGTH);
		buffer_offset += TX_IN_HASH_QTY * SHA256_DIGEST_LENGTH;
	}
	for (index = 0; index < output_qty; index++)
	{
		this_output = llist_get_node_at(transaction->outputs, index);
		memcpy(buf_to_hash + buffer_offset,
			this_output->hash,
			SHA256_DIGEST_LENGTH);
		buffer_offset += SHA256_DIGEST_LENGTH;
	}

	SHA256(buf_to_hash, buf_to_hash_len, hash_buf);
	free(buf_to_hash);

	return (hash_buf);
}

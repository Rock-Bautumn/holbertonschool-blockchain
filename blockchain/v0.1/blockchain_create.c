#include "blockchain.h"

/**
 * blockchain_create - Create a new blockchain
 * Return: The address of the new blockchain
*/

blockchain_t *blockchain_create(void)
{
	block_t *new_block;
	blockchain_t *new_blockchain;

	new_block = malloc(sizeof(block_t));
	if (!new_block)
		return (NULL);
	new_blockchain = malloc(sizeof(blockchain_t));
	if (!new_blockchain)
	{
		free(new_block);
		return (NULL);
	}
	memset(new_block, 0, sizeof(block_t));
	memset(new_blockchain, 0, sizeof(blockchain_t));

	new_blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	new_block->info.index = 0;
	new_block->info.difficulty = 0;
	new_block->info.timestamp = 1537578000;
	new_block->info.nonce = 0;
	memset(new_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
	memcpy(new_block->data.buffer, "Holberton School", 16);
	new_block->data.len = 16;
	memcpy(new_block->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);

	llist_add_node(new_blockchain->chain, new_block, ADD_NODE_FRONT);

	return (new_blockchain);
}

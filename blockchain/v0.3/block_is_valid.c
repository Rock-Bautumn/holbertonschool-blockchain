#include "blockchain.h"

block_t const genesis_block = {
	{
		0,
		0,
		1537578000,
		0,
		{0}
	},
	{
		"Holberton School",
		16
	},
	GENESIS_HASH
};

/**
 * block_is_valid - Check if a block in the blockchain is valid or not
 * @block: The block to check for validity
 * @prev_block: The previous block in the blockchain chain
 * Return: 1 if invalid, 0 if valid
*/

int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buffer[SHA256_DIGEST_LENGTH] = {0};
	block_t const gen_block = genesis_block;

	if (!block || (!prev_block && block->info.index != 0))
		return (1);

	if (block->info.index == 0)
		return (memcmp(block, &gen_block, sizeof(genesis_block)));

	if (block->info.index != prev_block->info.index + 1)
		return (1);

	if (hash_matches_difficulty(block->hash, block->info.difficulty) == 0)
		return (1);

	if (!block_hash(prev_block, hash_buffer) ||
		memcmp(hash_buffer, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (1);

	if (memcmp(prev_block->hash,
		block->info.prev_hash, SHA256_DIGEST_LENGTH))
		return (1);

	if (!block_hash(block, hash_buffer) ||
		memcmp(hash_buffer, block->hash, SHA256_DIGEST_LENGTH))
		return (1);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);

	return (0);
}

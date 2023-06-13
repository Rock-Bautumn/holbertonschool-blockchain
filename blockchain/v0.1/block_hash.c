#include "blockchain.h"

/**
 * block_hash - Compute the hash of a block
 * @block: The address of the block to hash
 * @hash_buf: The place to store the resulting hash
 * Return: The address of the saved resulting hash
*/

uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t size_to_hash;

	if (!block)
		return (NULL);
	
	size_to_hash = sizeof(block->info) + block->data.len;
	return (sha256((int8_t const *) block, size_to_hash, hash_buf));
}

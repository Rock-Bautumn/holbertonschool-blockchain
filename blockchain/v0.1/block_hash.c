#include "blockchain.h"

/**
 * block_hash - Compute the hash of a block
 * @block: The address of the block to hash
 * @hash_buf: The place to store the resulting hash
 * Return: The address of the saved resulting hash
*/

uint8_t *block_hash(
	block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	if (!block)
		return (NULL);

	SHA256(
		(unsigned char *) block,
		sizeof(block_info_t) + block->data.len,
		hash_buf
	);

	return (hash_buf);
}

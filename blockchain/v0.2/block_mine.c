#include "blockchain.h"

/**
 * all_array_bytes_are_zero - Check if the entire byte array consists of zeros
 * @array: The array to check to see if it is comprised of only zeros
 * @size: The size of the array to check
 * Return: true if only zeros, false if not only zeros
*/

bool all_array_bytes_are_zero(uint8_t array[], uint32_t size)
{
	uint32_t index = 0;

	for (; array[index] < size; index++)
		if (array[index] != 0)
			return (false);

	return (true);
}

/**
 * block_mine - Mine the block by brute forcing a hash that matches the
 * block's difficulty by incrementing the nonce of the block until a valid
 * hash is produced that matches the block's difficulty
 * @block: The block to mine
 * Return: void
*/
void block_mine(block_t *block)
{
	if (!block)
		return;

	if (block->info.difficulty == 0)
		return;

	block_hash(block, block->hash);
	while (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		block->info.nonce += 1;
		block_hash(block, block->hash);
	}
}

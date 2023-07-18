#include "blockchain.h"

/**
 * blockchain_difficulty - Compute the difficulty of the next blocks based
 * on the amount of time taken to mine the previous blocks when considering
 * the DIFFICULTY_ADJUSTMENT_INTERVAL and the BLOCK_GENERATION_INTERVAL
 * @blockchain: The blockchain to adjust the mining difficulty of
 * Return: The newly computer difficulty
*/


uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	uint64_t time_difference, expected_difference;
	block_t *current_block, *prev_block;

	if (!blockchain || !blockchain->chain)
		return (0);

	current_block = llist_get_tail(blockchain->chain);

	if (current_block->info.index == 0)
		return (current_block->info.difficulty);

	if (current_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
		return (current_block->info.difficulty);

	prev_block = llist_get_node_at(blockchain->chain,
		current_block->info.index -
		DIFFICULTY_ADJUSTMENT_INTERVAL + 1);

	time_difference =
		current_block->info.timestamp - prev_block->info.timestamp;
	expected_difference =
		BLOCK_GENERATION_INTERVAL * DIFFICULTY_ADJUSTMENT_INTERVAL;

	if (time_difference < expected_difference / 2)
		return (current_block->info.difficulty + 1);
	else if (time_difference > expected_difference * 2)
		return (current_block->info.difficulty - 1);
	else
		return (current_block->info.difficulty);
}

#include "blockchain.h"

/**
 * block_destroy - Delete and free a block in memory
 * @block: The address of the block to free
 * Return: void
*/

void block_destroy(block_t *block)
{
	if (block)
		free(block);
}

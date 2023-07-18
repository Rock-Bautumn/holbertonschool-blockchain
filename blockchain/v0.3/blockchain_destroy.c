#include "blockchain.h"

/**
 * blockchain_destroy - Destroy and free a blockchain from memory
 * @blockchain: The address of the blockchain to free and destroy
 * Return: void
*/

void blockchain_destroy(blockchain_t *blockchain)
{
	if (blockchain)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
	}
}

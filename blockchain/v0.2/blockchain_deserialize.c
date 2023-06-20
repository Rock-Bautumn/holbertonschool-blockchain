#include "blockchain.h"

/**
 * blockchain_deserialize_setup - Does some preliminary error checking and
 * opens the file
 * @path: The path to the file to open
 * Return: The opened file pointer when successful, NULL when failure
*/

FILE *blockchain_deserialize_setup(char const *path)
{
	FILE *file;
	char *magic, *version;

	if (!path)
		return (NULL);
	file = fopen(path, "rb");
	if (!file)
	{
		perror("unable to open file");
		return (NULL);
	}
	magic = calloc(1, sizeof(HBLK_MAGIC));
	if (!magic)
		return (NULL);
	version = calloc(1, sizeof(HBLK_VERSION));
	if (!version)
	{
		free(magic);
		return (NULL);
	}
	fseek(file, 0, SEEK_SET);
	if (fread(magic, sizeof(HBLK_MAGIC) - 1, 1, file) == 0)
		goto FAILURE;
	if (fread(version, sizeof(HBLK_VERSION) - 1, 1, file) == 0)
		goto FAILURE;
	if (strcmp(HBLK_MAGIC, magic) != 0 ||
		strcmp(HBLK_VERSION, version) != 0)
		goto FAILURE;
	free(magic);
	free(version);
	return (file);
FAILURE:
	free(magic);
	free(version);
	fclose(file);
	return (NULL);
}

/**
 * read_and_swap - Read from the blockchain file and swap the bytes if
 * necessary due to big endianness
 * @dest: The place to store the data
 * @size: The number of bytes to read and store into dest
 * @endianness: The endianness of the running machine
 * @file: The open file pointer of the blockchain file
 * Return: The number of bytes read, 0 if failure or reached EOF
*/

uint8_t read_and_swap(
	void *dest, uint32_t size, uint8_t endianness, FILE *file)
{
	uint32_t read_bytes = fread(dest, size, 1, file);

	if (read_bytes == 0)
		return (0);
	if (endianness == HBLK_BIG_ENDIAN)
		_swap_endian(dest, size);
	return (read_bytes);
}
/**
 * fill_block - Fill the block with data from disk
 * @this_block: The address of the block to fill
 * @file: The file pointer for the opened blockchain file
 * @endianness: The endianness of the running system
 * Return: 0 if failure, 1 if success
*/
uint8_t fill_block(block_t *this_block, FILE *file, uint8_t endianness)
{
	if (read_and_swap(&this_block->info.index,
		sizeof(this_block->info.index), endianness, file) == 0)
		return (0);
	if (read_and_swap(&this_block->info.difficulty,
		sizeof(this_block->info.difficulty), endianness, file) == 0)
		return (0);
	if (read_and_swap(&this_block->info.timestamp,
		sizeof(this_block->info.timestamp), endianness, file) == 0)
		return (0);
	if (read_and_swap(&this_block->info.nonce,
		sizeof(this_block->info.nonce), endianness, file) == 0)
		return (0);
	if (read_and_swap(this_block->info.prev_hash,
		sizeof(this_block->info.prev_hash), endianness, file) == 0)
		return (0);
	if (read_and_swap(&this_block->data.len,
		sizeof(this_block->data.len), endianness, file) == 0)
		return (0);
	if (read_and_swap(this_block->data.buffer,
		this_block->data.len, endianness, file) == 0)
		return (0);
	if (read_and_swap(this_block->hash,
		sizeof(this_block->hash), endianness, file) == 0)
		return (0);
	return (1);
}

/**
 * create_blockchain_in_mem - Create the blockchain data structure in
 * heap memory
 * @file: The file pointer to the blockchain file
 * @endianness: The endianness of the running system
 * @block_qty: The number of blocks contained in the blockchain file
 * Return: The created blockchain data structure
*/
blockchain_t *create_blockchain_in_mem(
	FILE *file, uint8_t endianness, uint8_t block_qty)
{
	blockchain_t *blockchain;
	block_t *this_block;
	uint32_t i = 0;

	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}
	for (; i < block_qty; i++)
	{
		this_block = calloc(1, sizeof(block_t));
		if (!this_block)
			goto FAILURE;
		if (fill_block(this_block, file, endianness) == 0)
		{
			free(this_block);
			goto FAILURE;
		}
		llist_add_node(blockchain->chain, this_block, ADD_NODE_REAR);
	}
	return (blockchain);
FAILURE:
	llist_destroy(blockchain->chain, i, NULL);
	fclose(file);
	return (NULL);
}

/**
 * blockchain_deserialize - The entry point, creates a blockchain
 * data structure from a file on the disk
 * @path: The path of the blockchain file to create the blockchain data
 * structure from
 * Return: The created blockchain data structure
*/

blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *file;
	blockchain_t *blockchain;
	uint8_t endianness, fatal_error = false;
	uint32_t block_qty;

	file = blockchain_deserialize_setup(path);
	if (!file)
		return (NULL);
	if (fread(&endianness, sizeof(uint8_t), 1, file) == 0)
		goto CLEANUP;
	if (read_and_swap(&block_qty, sizeof(uint32_t), endianness, file) == 0)
		goto CLEANUP;
	blockchain = create_blockchain_in_mem(file, endianness, block_qty);

CLEANUP:
	if (file)
		fclose(file);
	return ((fatal_error == true) ? NULL : blockchain);
}

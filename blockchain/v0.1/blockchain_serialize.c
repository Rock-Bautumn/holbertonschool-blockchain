#include "blockchain.h"

/**
 * swap_and_write - Swap the endianness if in big endian, and write the content
 * @content: The content to possibly swap, and then write
 * @size: The size of the content
 * @file: The file pointer to write to
 * @endianness: The endianness of the system
 * Return: void
*/

void swap_and_write(void *content, size_t size, FILE *file, int endianness)
{
	if (endianness == HBLK_BIG_ENDIAN)
		_swap_endian(content, size);
	fwrite(content, 1, size, file);
}

/**
 * write_blocks - Write the blockchain blocks to disk
 * @blockchain: The blockchain to write to disk
 * @block_qty: The number of blocks inside of the blockchain
 * @file: The file pointer to write the data to
 * @endianness: The endianness of the system
 * Return: void
*/

void write_blocks(
	blockchain_t const *blockchain,
	uint32_t block_qty,
	FILE *file,
	uint8_t endianness
)
{
	uint32_t i = 0;
	block_t *this_block;

	fwrite(HBLK_MAGIC, 1, strlen(HBLK_MAGIC), file);
	fwrite(HBLK_VERSION, 1, strlen(HBLK_VERSION), file);
	fwrite(&endianness, 1, sizeof(uint8_t), file);
	swap_and_write(&block_qty, sizeof(block_qty), file, endianness);

	for (; i < block_qty; i++)
	{
		this_block = llist_get_node_at(blockchain->chain, i);
		swap_and_write(&this_block->info.index,
			sizeof(this_block->info.index), file, endianness);
		swap_and_write(&this_block->info.difficulty,
			sizeof(this_block->info.difficulty), file, endianness);
		swap_and_write(&this_block->info.timestamp,
			sizeof(this_block->info.timestamp), file, endianness);
		swap_and_write(&this_block->info.nonce,
			sizeof(this_block->info.nonce), file, endianness);
		swap_and_write(this_block->info.prev_hash,
			sizeof(this_block->info.prev_hash), file, endianness);
		swap_and_write(&this_block->data.len,
			sizeof(this_block->data.len), file, endianness);
		swap_and_write(this_block->data.buffer,
			this_block->data.len, file, endianness);
		swap_and_write(this_block->hash,
			sizeof(this_block->hash), file, endianness);
	}
}

/**
 * blockchain_serialize - Write to disk a blockchain to a desired filename
 * @blockchain: The blockchain to write to the disk
 * @path: The path and filename to write the blockchain data to
 * Return: 0 if success, -1 if failure
*/

int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *file;
	uint8_t endianness;
	uint32_t block_qty;

	if (!blockchain || !path)
		return (-1);

	endianness = _get_endianness();

	file = fopen(path, "w+");
	if (!file || endianness == HBLK_INVALID_ENDIAN)
		return (-1);

	block_qty = llist_size(blockchain->chain);

	write_blocks(blockchain, block_qty, file, endianness);

	fclose(file);
	return (0);
}

#include "blockchain.h"

/**
 * countLeadingZerosBits - Count the number of leading zeros in an
 * array of bytes
 * @array: The byte array to count the leading zeros of
 * @size: The size of the byte array
 * Return: The number of leading zeros in the array
*/
uint32_t countLeadingZerosBits(uint8_t const *array, uint32_t size)
{
	uint32_t count = 0, i = 0;
	int32_t j, value;

	for (; i < size; i++)
	{
		value = array[i];

		for (j = 7; j >= 0; j--)
		{
			if ((value >> j) & 1)
				break;

			else
				count++;
		}

		if ((value >> j) & 1)
			break;

	}

	return (count);
}

/**
 * hash_matches_difficulty - Check if a hash meets a minimum degree of
 * difficulty, where the difficulty number is the number of leading zeros
 * in the hash.
 * @hash: The hash to check
 * @difficulty: The minimum number of leading zeros in the hash to match with
 * Return: 1 if the hash has at least difficulty number of leading zeros,
 * 0 if not.
*/

int hash_matches_difficulty(
	uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty)
{
	uint32_t leading_zeros;

	leading_zeros = countLeadingZerosBits(hash, SHA256_DIGEST_LENGTH);

	if (leading_zeros >= difficulty)
		return (1);

	return (0);
}

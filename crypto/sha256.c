#include "hblk_crypto.h"

/**
 * sha256 - Generate a hash from some data
 * @s: The data string to generate a hash from
 * @len: The size or amount of the data to generate the hash from using the
 * data string s.
 * @digest: The array to save the generated hash, aka "message digest"
 * Return: The address of the digest/generated hash
*/

uint8_t *sha256(int8_t const *s, size_t len,
	uint8_t digest[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX context;

	SHA256_Init(&context);
	SHA256_Update(&context, s, len);
	SHA256_Final(digest, &context);
	return (digest);
}


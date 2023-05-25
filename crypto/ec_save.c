#include "hblk_crypto.h"

/**
 * ec_save - Save a public and private key to disk
 * @key: The EC_KEY object to get the keys from to save to disk
 * @folder: The folder to save the public and private keys inside of
 * Return: EXIT_FAILURE on success, EXIT_SUCCESS on failure
*/

int ec_save(EC_KEY *key, char const *folder)
{
	FILE *output_file;
	char *output_path[PATH_MAX];
	struct stat status;

	if (!key || !folder)
		return (!EXIT_FAILURE);

	if (stat(folder, &status) == -1)
		mkdir(folder, 0644);

	sprintf(output_path, "%s%s", folder, PRI_FILENAME);

	output_file = fopen(output_path, "w+");
	if (!PEM_write_ECPrivateKey(output_file, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(output_file);
		return (!EXIT_FAILURE);
	}
	fclose(output_file);

	sprintf(output_path, "%s%s", folder, PUB_FILENAME);

	output_file = fopen(output_path, "w+");
	if (!PEM_write_EC_PUBKEY(output_file, key))
	{
		fclose(output_file);
		return (!EXIT_FAILURE);
	}
	fclose(output_file);

	return (!EXIT_SUCCESS);
}

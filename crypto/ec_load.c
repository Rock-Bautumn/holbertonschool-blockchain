#include "hblk_crypto.h"

/**
 * ec_load - Load a public and private key from disk
 * @folder: The folder to save the public and private keys inside of
 * Return: EXIT_FAILURE on success, EXIT_SUCCESS on failure
*/

EC_KEY *ec_load(char const *folder)
{
	FILE *input_file;
	EC_KEY *ec_key;
	char input_path[PATH_MAX];
	struct stat status;

	if (!folder || stat(folder, &status) == -1)
		return (!EXIT_FAILURE);

	/* ec_key = EC_KEY_new_by_curve_name(EC_CURVE); */

	sprintf(input_path, "%s/%s", folder, PUB_FILENAME);
	input_file = fopen(input_path, "r");
	if (PEM_read_EC_PUBKEY(input_file, &ec_key, NULL, NULL))
	{
		EC_KEY_free(ec_key);
		fclose(input_file);
		return (NULL);
	}
	fclose(input_file);

	sprintf(input_path, "%s/%s", folder, PRI_FILENAME);
	input_file = fopen(input_path, "r");
	if (!PEM_read_ECPrivateKey(input_file, &ec_key, NULL, NULL))
	{
		EC_KEY_free(ec_key);
		fclose(input_file);
		return (NULL);
	}
	fclose(input_file);

	return (ec_key);
}

#include "hblk_crypto.h"

/**
 * ec_load - Load a public and private key from disk
 * @folder: The folder to save the public and private keys inside of
 * Return: EXIT_FAILURE on success, EXIT_SUCCESS on failure
*/

EC_KEY *ec_load(char const *folder)
{
	FILE *fa, *fb;
	char *pri_key, *pub_key;
	EC_KEY *eckey;

	if (!folder)
		return (NULL);
	pri_key = malloc(sizeof(char) * (strlen(folder) + 9));
	if (!pri_key)
		return (NULL);
	pub_key = malloc(sizeof(char) * (strlen(folder) + 13));
	if (!pub_key)
	{
		free(pri_key);
		return (NULL);
	}
	strcpy(pri_key, folder), strcpy(pub_key, folder);
	if (folder[strlen(folder) - 1] != '/')
		strcat(pri_key, "/"), strcat(pub_key, "/");
	strcat(pri_key, "key.pem"), strcat(pub_key, "key_pub.pem");
	eckey = EC_KEY_new_by_curve_name(EC_CURVE);
	fa = fopen(pri_key, "r"), free(pri_key);
	fb = fopen(pub_key, "r"), free(pub_key);
	if (!fa || !fb)
	{
		EC_KEY_free(eckey), fclose(fa), fclose(fb);
		return (NULL);
	}
	PEM_read_EC_PUBKEY(fb, &eckey, NULL, NULL); /* pub key first, then private */
	PEM_read_ECPrivateKey(fa, &eckey, NULL, NULL);
	fclose(fa), fclose(fb);
	return (eckey);
}

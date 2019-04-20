#ifndef MESSAGEDIGEST_H
#define MESSAGEDIGEST_H

#include <openssl/evp.h>
#include <openssl/err.h>

class MessageDigest {

public:
	MessageDigest() {

	}

	void handleErrors(void)
	{
	  ERR_print_errors_fp(stderr);
	  abort();
	}

	void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len)
	{
		EVP_MD_CTX *mdctx;

		if((mdctx = EVP_MD_CTX_create()) == NULL)
			handleErrors();

		if(1 != EVP_DigestInit_ex(mdctx, EVP_md5(), NULL))
			handleErrors();

		if(1 != EVP_DigestUpdate(mdctx, message, message_len))
			handleErrors();

		if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_md5()))) == NULL)
			handleErrors();

		if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
			handleErrors();

		EVP_MD_CTX_destroy(mdctx);
	}
};
#endif

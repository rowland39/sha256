#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

char *
binaryToHex(const unsigned char *d)
{
    static const char *hex = "0123456789abcdef";
    char *ascii;

    if ((ascii = malloc(strlen((const char*)d) * 2 + 1)) == NULL) {
        return NULL;
    }

    for (int i = 0; i < strlen((const char *)d); i++) {
        ascii[2 * i] = hex[((d[i] & 0xf0) >> 4)];
        ascii[2 * i + 1] = hex[(d[i] & 0x0f)];
    }

    ascii[strlen(ascii)] = '\0';
    return ascii;
}

char *
sha256(char *path)
{
    FILE* file;
    size_t n;
    EVP_MD_CTX *ctx;

    if ((file = fopen(path, "rb")) == NULL) {
        return NULL;
    }

    ctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    char* buffer[65536];

    while ((n = fread(buffer, 1, sizeof(buffer), file)) != 0) {
        if (ferror(file) != 0) {
            return NULL;
        }

        EVP_DigestUpdate(ctx, buffer, n);
    }

    unsigned int digest_size = EVP_MD_size(EVP_sha256());
    unsigned char digest[digest_size];
    EVP_DigestFinal_ex(ctx, digest, &digest_size);
    fclose(file);
    EVP_MD_CTX_destroy(ctx);
    return binaryToHex(digest);
}

int
main(int argc, char **argv)
{
    char *checksum;

    if (argc != 2) {
        fprintf(stderr, "\nUsage: %s <file>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((checksum = sha256(argv[1])) == NULL) {
        perror("sha256 error");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", checksum);
    free(checksum);
    exit(EXIT_SUCCESS);
}

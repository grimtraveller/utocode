extern "C" {
#include "CRC32.h"
#include "sha256.h"
}
#include "md5.h"
#include "SHA1.h"

int
main(int argc, char* argv[])
{
	static char s[] = "zuohaitao";
	const int len = sizeof(s) - 1;
	printf("%s(%d)\n", s, len);

	unsigned long crc32;
	crc32Init(&crc32);
	crc32Update(&crc32, (unsigned char*)s, len);
	crc32Finish(&crc32);
	printf("crc\t- %04X\n", (unsigned int)crc32);

	MD5_CTX mdContext; // MD5 context
	MD5Init(&mdContext, 0);
	MD5Update(&mdContext, (unsigned char *)s, len);
	MD5Final(&mdContext);
	printf("md5\t- %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
								mdContext.digest[0],
								mdContext.digest[1],
								mdContext.digest[2],
								mdContext.digest[3],
								mdContext.digest[4],
								mdContext.digest[5],
								mdContext.digest[6],
								mdContext.digest[7],
								mdContext.digest[8],
								mdContext.digest[9],
								mdContext.digest[10],
								mdContext.digest[11],
								mdContext.digest[12],
								mdContext.digest[13],
								mdContext.digest[14],
								mdContext.digest[15]);

	SHA256_CTX sha256Context;
	sha256_init(&sha256Context);
	sha256_update(&sha256Context, (const unsigned char*)s, len);
	sha256_final(&sha256Context);
	unsigned char sha256digest[32];
	sha256_digest(&sha256Context, sha256digest);
	printf("sha256\t- %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
								sha256digest[0],
								sha256digest[1],
								sha256digest[2],
								sha256digest[3],
								sha256digest[4],
								sha256digest[5],
								sha256digest[6],
								sha256digest[7],
								sha256digest[8],
								sha256digest[9],
								sha256digest[10],
								sha256digest[11],
								sha256digest[12],
								sha256digest[13],
								sha256digest[14],
								sha256digest[15],
								sha256digest[16],
								sha256digest[17],
								sha256digest[18],
								sha256digest[19],
								sha256digest[20],
								sha256digest[21],
								sha256digest[22],
								sha256digest[23],
								sha256digest[24],
								sha256digest[25],
								sha256digest[26],
								sha256digest[27],
								sha256digest[28],
								sha256digest[29],
								sha256digest[30],
								sha256digest[31]);


	CSHA1 sha1;
	char szSHA1[256];
	szSHA1[255] = 0;
	sha1.Reset();
	sha1.Update((unsigned char*)s, len);
	sha1.Final();
	sha1.ReportHash(szSHA1, CSHA1::REPORT_HEX);
	printf("sha1\t- %s", szSHA1);

	return 0;
}

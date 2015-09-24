#include <Windows.h>
#include <stdlib.h>
#include "CompressionTools.h"
#include "miniz.c"

/*
  TODO: Error testing
  BONUS: Another algorithm (if more effective)
*/

BYTE*		CompressData(BYTE *source, LONG souceLength, LONG *destLength)
{
	BYTE	*compressed;
	uLong	cmp_len;

	compressed = (BYTE	*)malloc(compressBound(souceLength));
	compress((unsigned char *)compressed, &cmp_len, (const unsigned char *)source, (mz_ulong)souceLength);
	*destLength = cmp_len;
	return (compressed);
}

void		UncompressData(BYTE *source, LONG souceLength, BYTE	*buf, LONG *destLength)
{
	uLong	cmp_len;

	uncompress((unsigned char *)buf, &cmp_len, (const unsigned char *)source, (mz_ulong)souceLength);
	*destLength = cmp_len;
}

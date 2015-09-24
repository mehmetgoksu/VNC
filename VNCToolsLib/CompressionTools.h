#ifndef __COMPRESSIONTOOLS_H__
# define __COMPRESSIONTOOLS_H__

BYTE*		CompressData(BYTE *data, LONG souceLength, LONG *destLength);
void		UncompressData(BYTE *source, LONG souceLength, BYTE	*buf, LONG *destLength);

#endif

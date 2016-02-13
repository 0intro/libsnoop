#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#include "snoop.h"
#include "bele.h"

int
packglobalhdr(GlobalHdr *hdr, uint8_t *buf, unsigned int size)
{
	uint8_t *p;

	if (size < GlobalHdrSize)
		return -1;

	p = buf;

	p += be32put(p, hdr->magic);
	p += be16put(p, hdr->version_major);
	p += be16put(p, hdr->version_minor);
	p += be32put(p, hdr->thiszone);
	p += be32put(p, hdr->snaplen);
	p += be32put(p, hdr->sigfigs);
	p += be32put(p, hdr->linktype);

	return p-buf;
}

int
packrecordhdr(RecordHdr *hdr, uint8_t *buf, unsigned int size)
{
	uint8_t *p;

	if (size < RecordHdrSize)
		return -1;

	p = buf;

	p += be32put(p, hdr->ts_sec);
	p += be32put(p, hdr->ts_usec);
	p += be32put(p, hdr->caplen);
	p += be32put(p, hdr->len);

	return p-buf;
}

int
writeglobalhdr(FILE *f)
{
	GlobalHdr hdr;
	uint8_t buf[GlobalHdrSize];

	hdr.magic = PcapMagic;
	hdr.version_major = PcapVersionMajor;
	hdr.version_minor = PcapVersionMinor;
	hdr.thiszone = 0;
	hdr.snaplen = MaxSnapLen;
	hdr.sigfigs = 0;
	hdr.linktype = LINKTYPE_ETHERNET;

	if (packglobalhdr(&hdr, buf, sizeof(buf)) < 0)
		return -1;

	if (fwrite(buf, sizeof(buf), 1, f) != 1)
		return -1;

	return 0;
}

int
writerecordhdr(FILE *f, int len)
{
	RecordHdr hdr;
	struct timeval tv;
	uint8_t buf[RecordHdrSize];

	if (gettimeofday(&tv, NULL) < 0)
		return -1;

	hdr.ts_sec = tv.tv_sec;
	hdr.ts_usec = tv.tv_usec;
	hdr.caplen = len;
	hdr.len = len;

	if (packrecordhdr(&hdr, buf, sizeof(buf)) < 0)
		return -1;

	if (fwrite(buf, sizeof(buf), 1, f) != 1)
		return -1;

	return 0;
}

int
writerecord(FILE *f, uint8_t *buf, int len)
{
	if (len > MaxSnapLen)
		return -1;

	if (writerecordhdr(f, len) < 0)
		return -1;

	if (fwrite(buf, len, 1, f) != 1)
		return -1;

	return 0;
}

int
writerecordhdrts(FILE *f, int len, uint32_t sec, uint32_t usec)
{
	RecordHdr hdr;
	uint8_t buf[RecordHdrSize];

	hdr.ts_sec = sec;
	hdr.ts_usec = usec;
	hdr.caplen = len;
	hdr.len = len;

	if (packrecordhdr(&hdr, buf, sizeof(buf)) < 0)
		return -1;

	if (fwrite(buf, sizeof(buf), 1, f) != 1)
		return -1;

	return 0;
}

int
writerecordts(FILE *f, uint8_t *buf, int len, uint32_t sec, uint32_t usec)
{
	if (len > MaxSnapLen)
		return -1;

	if (writerecordhdrts(f, len, sec, usec) < 0)
		return -1;

	if (fwrite(buf, len, 1, f) != 1)
		return -1;

	return 0;
}

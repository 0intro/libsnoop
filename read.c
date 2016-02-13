#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "snoop.h"
#include "bele.h"

int
unpackglobalhdr(GlobalHdr *hdr, uint8_t *buf, unsigned int size)
{
	uint8_t *p;

	if (size < GlobalHdrSize)
		return -1;

	p = buf;

	p += be32get(p, &hdr->magic);
	p += be16get(p, &hdr->version_major);
	p += be16get(p, &hdr->version_minor);
	p += be32get(p, (uint32_t*)&hdr->thiszone);
	p += be32get(p, &hdr->snaplen);
	p += be32get(p, &hdr->sigfigs);
	p += be32get(p, &hdr->linktype);

	return p-buf;
}

int
unpackrecordhdr(RecordHdr *hdr, uint8_t *buf, unsigned int size)
{
	uint8_t *p;

	if (size < RecordHdrSize)
		return -1;

	p = buf;

	p += be32get(p, &hdr->ts_sec);
	p += be32get(p, &hdr->ts_usec);
	p += be32get(p, &hdr->caplen);
	p += be32get(p, &hdr->len);

	return p-buf;
}

int
readglobalhdr(FILE *f, GlobalHdr *hdr)
{
	uint8_t buf[GlobalHdrSize];

	if (fread(buf, sizeof(buf), 1, f) != 1) {
		if (feof(f))
			return 0;
		return -1;
	}

	if (unpackglobalhdr(hdr, buf, sizeof(buf)) < 0)
		return -1;

	return 0;
}

int
readrecordhdr(FILE *f, RecordHdr *hdr)
{
	uint8_t buf[RecordHdrSize];
	int n;

	if (fread(buf, sizeof(buf), 1, f) != 1) {
		if (feof(f))
			return 0;
		return -1;
	}

	if ((n = unpackrecordhdr(hdr, buf, sizeof(buf))) < 0)
		return -1;

	return n;
}

int
readrecord(FILE *f, RecordHdr *hdr, uint8_t *buf)
{
	int n;

	if ((n = readrecordhdr(f, hdr)) <= 0)
		return n;

	if (fread(buf, hdr->caplen, 1, f) != 1) {
		if (feof(f))
			return 0;
		return -1;
	}

	return hdr->caplen;
}

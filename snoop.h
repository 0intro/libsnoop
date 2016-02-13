#ifndef _PCAP_H_
#define _PCAP_H_ 1
#if defined(__cplusplus)
extern "C" {
#endif

enum {
	PcapMagic = 0xa1b2c3d4,
	PcapVersionMajor = 2,
	PcapVersionMinor = 4,
	MaxSnapLen = 65535,
	GlobalHdrSize = 24,
	RecordHdrSize = 16,
};

typedef struct GlobalHdr GlobalHdr;
typedef struct RecordHdr RecordHdr;

/*
 * Global Header
 */
struct GlobalHdr {
	uint32_t magic;			/* magic number */
	uint16_t version_major;	/* major version number */
	uint16_t version_minor;	/* minor version number */
	int32_t thiszone;		/* GMT to local correction */
	uint32_t sigfigs;		/* accuracy of timestamps */
	uint32_t snaplen;		/* max length of captured packets */
	uint32_t linktype;		/* data link type (LINKTYPE_*) */
};

/*
 * Record (Packet) Header
 */
struct RecordHdr {
	uint32_t ts_sec;		/* timestamp seconds */
	uint32_t ts_usec;		/* timestamp microseconds */
	uint32_t caplen;		/* length of packet saved in file */
	uint32_t len;			/* actual length of packet */
};

#define LINKTYPE_NULL		0
#define LINKTYPE_ETHERNET	1

/*
 * read.c
 */

int readglobalhdr(FILE*, GlobalHdr*);
int readrecordhdr(FILE*, RecordHdr*);
int readrecord(FILE*, RecordHdr*, uint8_t*);

/*
 * write.c
 */

int writeglobalhdr(FILE*);
int writerecordhdr(FILE*, int);
int writerecord(FILE*, uint8_t*, int);
int writerecordhdrts(FILE*, int, uint32_t, uint32_t);
int writerecordts(FILE*, uint8_t*, int, uint32_t, uint32_t);

#if defined(__cplusplus)
}
#endif
#endif

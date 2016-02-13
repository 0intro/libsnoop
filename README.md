[![Build Status](https://drone.io/github.com/0intro/libsnoop/status.png)](https://drone.io/github.com/0intro/libsnoop/latest)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/7835/badge.svg)](https://scan.coverity.com/projects/7835)

libsnoop
========

Libsnoop is a simple library which provides functions to read
and write pcap files.

Headers
-------

```
#include <stdint.h>
#include <snoop.h>
```

Structures
----------

```
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
```

Read
----

```
int readglobalhdr(FILE *f, GlobalHdr *hdr);
int readrecordhdr(FILE *f, RecordHdr *hdr);
int readrecord(FILE *f, RecordHdr *hdr, uint8_t *buf);
```

Write
-----

```
int writeglobalhdr(FILE *f);
int writerecordhdr(FILE *f, int len)
int writerecord(FILE *f, uint8_t *buf, int len);
int writerecordhdrts(FILE *f, int len, uint32_t sec, uint32_t usec);
int writerecordts(FILE *f, uint8_t *buf, int len, uint32_t sec, uint32_t usec);
```

// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void free_header_fields(gz_header *head) {
    if (head == NULL) return;
    if (head->extra != NULL) free(head->extra);
    if (head->name != NULL) free(head->name);
    if (head->comment != NULL) free(head->comment);
    head->extra = NULL;
    head->name = NULL;
    head->comment = NULL;
}

static int fuzz_deflate_set_header(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    gz_header head;
    int ret;

    memset(&head, 0, sizeof(head));
    
    /* Initialize deflate stream with GZIP wrap (2) */
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        return 0;
    }

    /* Prepare header data from fuzz input if available */
    if (size > 0) {
        head.text = data[0] % 2;
        head.time = (size > 1) ? (data[1] << 8) | data[2] : 0;
        head.xflags = (size > 3) ? data[3] : 0;
        head.os = (size > 4) ? data[4] : 0;
        head.hcrc = (size > 5) ? (data[5] % 2) : 0;
        
        if (size > 10) {
            head.extra_len = (data[6] % 20);
            head.extra_max = head.extra_len + 1;
            head.extra = (Bytef *)malloc(head.extra_max);
            if (head.extra) {
                memcpy(head.extra, &data[7], head.extra_len);
            }
        }
        if (size > 30) {
            head.name_max = (data[8] % 20) + 1;
            head.name = (Bytef *)malloc(head.name_max);
            if (head.name) {
                size_t name_len = (data[9] % 20);
                if (name_len >= head.name_max) name_len = head.name_max - 1;
                memcpy(head.name, &data[10], name_len);
                head.name[name_len] = '\0';
            }
        }
        if (size > 50) {
            head.comm_max = (data[10] % 20) + 1;
            head.comment = (Bytef *)malloc(head.comm_max);
            if (head.comment) {
                size_t comm_len = (data[11] % 20);
                if (comm_len >= head.comm_max) comm_len = head.comm_max - 1;
                memcpy(head.comment, &data[12], comm_len);
                head.comment[comm_len] = '\0';
            }
        }
    }

    ret = deflateSetHeader(&strm, &head);
    
    free_header_fields(&head);
    deflateEnd(&strm);
    
    return ret;
}

static int fuzz_inflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;

    /* Initialize inflate stream */
    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    /* Perform some dummy inflate to set up state */
    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = size;
        Byte out_buf[1024];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateReset(&strm);
    inflateEnd(&strm);

    return ret;
}

static int fuzz_inflate(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;
    Byte out_buf[4096];

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    strm.next_in = (const Bytef *)data;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Try different flush modes */
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FINISH, Z_BLOCK};
    int mode = (size > 0) ? (data[0] % 4) : 0;
    int flush = flush_modes[mode];

    ret = inflate(&strm, flush);

    inflateEnd(&strm);
    return ret;
}

static int fuzz_deflate_reset(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    /* Perform some compression to set up state */
    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = size;
        Byte out_buf[1024];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        deflate(&strm, Z_NO_FLUSH);
    }

    ret = deflateReset(&strm);
    deflateEnd(&strm);

    return ret;
}

static int fuzz_inflate_get_header(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    gz_header head;
    int ret;

    memset(&head, 0, sizeof(head));
    head.extra_max = 100;
    head.name_max = 100;
    head.comm_max = 100;

    /* Must be called before inflateInit or inflateReset */
    ret = inflateGetHeader(&strm, &head);
    
    /* If successful, try to init and process some data */
    if (ret == Z_OK) {
        if (inflateInit(&strm) == Z_OK) {
            if (size > 0) {
                strm.next_in = (const Bytef *)data;
                strm.avail_in = size;
                Byte out_buf[1024];
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                inflate(&strm, Z_NO_FLUSH);
            }
            inflateEnd(&strm);
        }
    }

    return ret;
}

static int fuzz_inflate_copy(const uint8_t *data, size_t size) {
    z_stream source = {0};
    z_stream dest = {0};
    int ret;

    if (inflateInit(&source) != Z_OK) {
        return 0;
    }

    /* Process some data in source */
    if (size > 0) {
        source.next_in = (const Bytef *)data;
        source.avail_in = size;
        Byte out_buf[1024];
        source.next_out = out_buf;
        source.avail_out = sizeof(out_buf);
        inflate(&source, Z_NO_FLUSH);
    }

    ret = inflateCopy(&dest, &source);

    inflateEnd(&source);
    if (ret == Z_OK) {
        inflateEnd(&dest);
    }

    return ret;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Fuzz deflateSetHeader */
    fuzz_deflate_set_header(Data, Size);

    /* Fuzz inflateReset */
    fuzz_inflate_reset(Data, Size);

    /* Fuzz inflate */
    fuzz_inflate(Data, Size);

    /* Fuzz deflateReset */
    fuzz_deflate_reset(Data, Size);

    /* Fuzz inflateGetHeader */
    fuzz_inflate_get_header(Data, Size);

    /* Fuzz inflateCopy */
    fuzz_inflate_copy(Data, Size);

    return 0;
}
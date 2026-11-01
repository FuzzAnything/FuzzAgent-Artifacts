// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateMark at inflate.c:1397:14 in zlib.h
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

static int fuzz_inflate_undermine(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t out_buf[1024];
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    // Try both 0 and 1 for subvert
    ret = inflateUndermine(&strm, 0);
    (void)ret;
    ret = inflateUndermine(&strm, 1);
    (void)ret;

    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_sync(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t out_buf[1024];
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateSync(&strm);
    (void)ret;

    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_sync_point(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t out_buf[1024];
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateSyncPoint(&strm);
    (void)ret;

    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_mark(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t out_buf[1024];
    long ret = 0;

    if (inflateInit(&strm) != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateMark(&strm);
    (void)ret;

    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_prime(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t out_buf[1024];
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    // Try various bit values and values
    int bits = (Size > 0) ? (Data[0] % 17) : 10;
    int value = (Size > 1) ? (Data[1] & 0xFF) : 0;
    ret = inflatePrime(&strm, bits, value);
    (void)ret;

    // Try negative bits to clear
    ret = inflatePrime(&strm, -1, 0);
    (void)ret;

    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_copy(const uint8_t *Data, size_t Size) {
    z_stream source = {0};
    z_stream dest = {0};
    uint8_t out_buf[1024];
    int ret = Z_OK;

    if (inflateInit(&source) != Z_OK) return 0;

    if (Size > 0) {
        source.next_in = Data;
        source.avail_in = Size;
        source.next_out = out_buf;
        source.avail_out = sizeof(out_buf);
        inflate(&source, Z_NO_FLUSH);
    }

    ret = inflateCopy(&dest, &source);
    if (ret == Z_OK) {
        // Try to use the copied stream
        dest.next_out = out_buf;
        dest.avail_out = sizeof(out_buf);
        inflate(&dest, Z_NO_FLUSH);
        inflateEnd(&dest);
    }

    inflateEnd(&source);
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz each function individually
    fuzz_inflate_undermine(Data, Size);
    fuzz_inflate_sync(Data, Size);
    fuzz_inflate_sync_point(Data, Size);
    fuzz_inflate_mark(Data, Size);
    fuzz_inflate_prime(Data, Size);
    fuzz_inflate_copy(Data, Size);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
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
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        // Attempt to decompress a bit to initialize state
        Byte out[256];
        strm.next_out = out;
        strm.avail_out = sizeof(out);
        inflate(&strm, Z_NO_FLUSH);
    }

    // Fuzz subvert flag (0 or 1)
    int subvert = (Size > 0) ? (Data[0] & 1) : 0;
    ret = inflateUndermine(&strm, subvert);
    
    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_sync(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        Byte out[256];
        strm.next_out = out;
        strm.avail_out = sizeof(out);
        // Try to trigger an error or just run
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateSync(&strm);
    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_copy(const uint8_t *Data, size_t Size) {
    z_stream source = {0};
    z_stream dest = {0};
    int ret = inflateInit(&source);
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        source.next_in = Data;
        source.avail_in = (uInt)Size;
        Byte out[256];
        source.next_out = out;
        source.avail_out = sizeof(out);
        inflate(&source, Z_NO_FLUSH);
    }

    ret = inflateCopy(&dest, &source);
    if (ret == Z_OK) {
        // Verify dest is independent by trying to use it
        Byte out2[256];
        dest.next_out = out2;
        dest.avail_out = sizeof(out2);
        inflate(&dest, Z_NO_FLUSH);
        inflateEnd(&dest);
    }
    inflateEnd(&source);
    return 0;
}

static int fuzz_inflate_codes_used(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        Byte out[256];
        strm.next_out = out;
        strm.avail_out = sizeof(out);
        inflate(&strm, Z_NO_FLUSH);
    }

    unsigned long codes = inflateCodesUsed(&strm);
    (void)codes; // Use value to avoid unused warning
    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_prime(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        Byte out[256];
        strm.next_out = out;
        strm.avail_out = sizeof(out);
        inflate(&strm, Z_NO_FLUSH);
    }

    // Fuzz bits (0-16) and value
    int bits = (Size > 0) ? (Data[0] % 17) : 0;
    int value = (Size > 1) ? (Data[1] << 8 | Data[2]) : 0;
    
    // Also test negative bits to clear buffer
    if (Size > 3 && (Data[3] & 1)) {
        bits = -1;
        value = 0;
    }

    ret = inflatePrime(&strm, bits, value);
    inflateEnd(&strm);
    return 0;
}

static int fuzz_inflate_sync_point(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (Size > 0) {
        strm.next_in = Data;
        strm.avail_in = (uInt)Size;
        Byte out[256];
        strm.next_out = out;
        strm.avail_out = sizeof(out);
        inflate(&strm, Z_NO_FLUSH);
    }

    int is_sync = inflateSyncPoint(&strm);
    (void)is_sync;
    inflateEnd(&strm);
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run all target functions to maximize state exploration
    fuzz_inflate_undermine(Data, Size);
    fuzz_inflate_sync(Data, Size);
    fuzz_inflate_copy(Data, Size);
    fuzz_inflate_codes_used(Data, Size);
    fuzz_inflate_prime(Data, Size);
    fuzz_inflate_sync_point(Data, Size);

    return 0;
}
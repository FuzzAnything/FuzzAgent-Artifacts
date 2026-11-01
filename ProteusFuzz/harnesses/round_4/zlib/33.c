// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream dest = {0};
    uint8_t *out_buf;
    int ret;
    unsigned long codes_used;
    int bits, value;
    int subvert;
    int sync_point;
    int sync_ret;
    int copy_ret;
    int undermine_ret;

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(1024);
    if (!out_buf) return 0;

    // Initialize source stream
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = 1024;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Fuzz inflateCodesUsed
    codes_used = inflateCodesUsed(&strm);
    (void)codes_used;

    // Fuzz inflatePrime with various bit values
    if (Size >= 2) {
        bits = Data[0] % 17; // 0 to 16
        value = Data[1];
        ret = inflatePrime(&strm, bits, value);
        (void)ret;
    }
    // Test negative bits (clear buffer)
    ret = inflatePrime(&strm, -1, 0);
    (void)ret;

    // Fuzz inflateCopy
    dest.next_in = (const Bytef *)Data;
    dest.avail_in = Size;
    dest.next_out = out_buf;
    dest.avail_out = 1024;
    dest.zalloc = Z_NULL;
    dest.zfree = Z_NULL;
    dest.opaque = Z_NULL;
    copy_ret = inflateCopy(&dest, &strm);
    (void)copy_ret;

    // Fuzz inflateSync
    sync_ret = inflateSync(&strm);
    (void)sync_ret;

    // Fuzz inflateUndermine
    if (Size >= 3) {
        subvert = (Data[2] % 2) - 1; // -1 or 0
        undermine_ret = inflateUndermine(&strm, subvert);
        (void)undermine_ret;
    }

    // Fuzz inflateSyncPoint
    sync_point = inflateSyncPoint(&strm);
    (void)sync_point;

    // Perform a few inflate steps to exercise state changes
    if (strm.avail_in > 0) {
        ret = inflate(&strm, Z_NO_FLUSH);
        (void)ret;
    }

    // Cleanup
    if (copy_ret == Z_OK) {
        inflateEnd(&dest);
    }
    inflateEnd(&strm);
    free(out_buf);

    return 0;
}
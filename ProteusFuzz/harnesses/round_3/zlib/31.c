// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static int fuzz_inflate_reset_keep(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)(size > 65535 ? 65535 : size);
        Bytef out_buf[256];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    ret = inflateResetKeep(&strm);
    inflateEnd(&strm);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_reset2(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)(size > 65535 ? 65535 : size);
        Bytef out_buf[256];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    int windowBits = (int)(data[0] % 32) - 16; // Range -16 to 15
    ret = inflateReset2(&strm, windowBits);
    inflateEnd(&strm);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_undermine(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)(size > 65535 ? 65535 : size);
        Bytef out_buf[256];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    int subvert = (int)(data[0] % 2);
    ret = inflateUndermine(&strm, subvert);
    inflateEnd(&strm);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_copy(const uint8_t *data, size_t size) {
    z_stream source = {0};
    z_stream dest = {0};
    int ret = inflateInit(&source);
    if (ret != Z_OK) return 0;

    if (size > 0) {
        source.next_in = (const Bytef *)data;
        source.avail_in = (uInt)(size > 65535 ? 65535 : size);
        Bytef out_buf[256];
        source.next_out = out_buf;
        source.avail_out = sizeof(out_buf);
        inflate(&source, Z_NO_FLUSH);
    }

    ret = inflateCopy(&dest, &source);
    if (ret == Z_OK) {
        inflateEnd(&dest);
    }
    inflateEnd(&source);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_prime(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)(size > 65535 ? 65535 : size);
        Bytef out_buf[256];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    int bits = (int)(data[0] % 17); // 0 to 16
    int value = 0;
    if (size > 1) {
        value = (int)(data[1] % 65536);
    }
    if (bits == 0 && size > 2) bits = -1; // Test negative bits case

    ret = inflatePrime(&strm, bits, value);
    inflateEnd(&strm);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_validate(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;

    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)(size > 65535 ? 65535 : size);
        Bytef out_buf[256];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        inflate(&strm, Z_NO_FLUSH);
    }

    int check = (int)(data[0] % 2);
    ret = inflateValidate(&strm, check);
    inflateEnd(&strm);
    return ret == Z_OK ? 1 : 0;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int mode = Data[0] % 6;

    switch (mode) {
        case 0:
            fuzz_inflate_reset_keep(Data, Size);
            break;
        case 1:
            fuzz_inflate_reset2(Data, Size);
            break;
        case 2:
            fuzz_inflate_undermine(Data, Size);
            break;
        case 3:
            fuzz_inflate_copy(Data, Size);
            break;
        case 4:
            fuzz_inflate_prime(Data, Size);
            break;
        case 5:
            fuzz_inflate_validate(Data, Size);
            break;
        default:
            break;
    }

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
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

static int fuzz_inflate_reset_keep(z_streamp strm, const uint8_t *data, size_t size) {
    if (strm == NULL) return 0;
    
    // Initialize stream if not already initialized
    if (strm->state == NULL) {
        if (inflateInit(strm) != Z_OK) return 0;
    }
    
    // Provide some input data to potentially trigger state changes
    if (size > 0) {
        strm->next_in = data;
        strm->avail_in = (uInt)size;
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            strm->next_out = out_buf;
            strm->avail_out = out_size;
            inflate(strm, Z_NO_FLUSH);
            free(out_buf);
        }
    }
    
    // Call the target function
    int ret = inflateResetKeep(strm);
    
    // Cleanup
    inflateEnd(strm);
    return ret;
}

static int fuzz_inflate_copy(const uint8_t *data, size_t size) {
    z_stream source, dest;
    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));
    
    // Initialize source stream
    if (inflateInit(&source) != Z_OK) return 0;
    
    // Provide some input data
    if (size > 0) {
        source.next_in = data;
        source.avail_in = (uInt)size;
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            source.next_out = out_buf;
            source.avail_out = out_size;
            inflate(&source, Z_NO_FLUSH);
            free(out_buf);
        }
    }
    
    // Copy the stream
    int ret = inflateCopy(&dest, &source);
    
    // Cleanup both streams
    inflateEnd(&source);
    if (ret == Z_OK) {
        inflateEnd(&dest);
    }
    
    return ret;
}

static int fuzz_inflate_sync_point(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (inflateInit(&strm) != Z_OK) return 0;
    
    // Provide input data
    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            strm.next_out = out_buf;
            strm.avail_out = out_size;
            inflate(&strm, Z_NO_FLUSH);
            free(out_buf);
        }
    }
    
    // Call the target function
    int ret = inflateSyncPoint(&strm);
    
    inflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_prime(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (inflateInit(&strm) != Z_OK) return 0;
    
    // Provide input data
    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            strm.next_out = out_buf;
            strm.avail_out = out_size;
            inflate(&strm, Z_NO_FLUSH);
            free(out_buf);
        }
    }
    
    // Call the target function with various parameters
    int bits = (size > 0) ? (data[0] % 17) : 0;
    int value = (size > 1) ? (data[1] << 8) | (size > 2 ? data[2] : 0) : 0;
    
    int ret = inflatePrime(&strm, bits, value);
    
    // Also test negative bits
    ret = inflatePrime(&strm, -1, 0);
    
    inflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_codes_used(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (inflateInit(&strm) != Z_OK) return 0;
    
    // Provide input data
    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            strm.next_out = out_buf;
            strm.avail_out = out_size;
            inflate(&strm, Z_NO_FLUSH);
            free(out_buf);
        }
    }
    
    // Call the target function
    unsigned long ret = inflateCodesUsed(&strm);
    
    inflateEnd(&strm);
    return (int)ret;
}

static int fuzz_inflate_undermine(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (inflateInit(&strm) != Z_OK) return 0;
    
    // Provide input data
    if (size > 0) {
        strm.next_in = data;
        strm.avail_in = (uInt)size;
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            strm.next_out = out_buf;
            strm.avail_out = out_size;
            inflate(&strm, Z_NO_FLUSH);
            free(out_buf);
        }
    }
    
    // Call the target function with both true and false
    int ret1 = inflateUndermine(&strm, 1);
    int ret2 = inflateUndermine(&strm, 0);
    
    inflateEnd(&strm);
    return ret1 | ret2;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz inflateResetKeep
    fuzz_inflate_reset_keep(NULL, Data, Size);
    
    // Fuzz inflateCopy
    fuzz_inflate_copy(Data, Size);
    
    // Fuzz inflateSyncPoint
    fuzz_inflate_sync_point(Data, Size);
    
    // Fuzz inflatePrime
    fuzz_inflate_prime(Data, Size);
    
    // Fuzz inflateCodesUsed
    fuzz_inflate_codes_used(Data, Size);
    
    // Fuzz inflateUndermine
    fuzz_inflate_undermine(Data, Size);
    
    return 0;
}
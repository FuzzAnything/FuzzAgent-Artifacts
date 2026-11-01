// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static int test_compress(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uLong sourceLen = (uLong)Size;
    uLong destLen = compressBound(sourceLen);
    
    if (destLen == (uLong)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    int ret = compress(dest, &destLen, (const Bytef *)Data, sourceLen);
    
    free(dest);
    return ret;
}

static int test_compress_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    z_size_t sourceLen = (z_size_t)Size;
    z_size_t destLen = compressBound_z(sourceLen);
    
    if (destLen == (z_size_t)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    int ret = compress_z(dest, &destLen, (const Bytef *)Data, sourceLen);
    
    free(dest);
    return ret;
}

static int test_deflateBound(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }
    
    uLong sourceLen = (uLong)Size;
    uLong bound = deflateBound(&strm, sourceLen);
    
    deflateEnd(&strm);
    
    if (bound == (uLong)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) return 0;
    
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = dest;
    strm.avail_out = (uInt)bound;
    
    int ret = deflate(&strm, Z_FINISH);
    
    free(dest);
    return ret;
}

static int test_deflateBound_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }
    
    z_size_t sourceLen = (z_size_t)Size;
    z_size_t bound = deflateBound_z(&strm, sourceLen);
    
    deflateEnd(&strm);
    
    if (bound == (z_size_t)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) return 0;
    
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = dest;
    strm.avail_out = (uInt)bound;
    
    int ret = deflate(&strm, Z_FINISH);
    
    free(dest);
    return ret;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_compress(Data, Size);
    test_compress_z(Data, Size);
    test_deflateBound(Data, Size);
    test_deflateBound_z(Data, Size);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate source buffer */
    Bytef *source = (Bytef *)malloc(Size);
    if (!source) {
        return 0;
    }
    memcpy(source, Data, Size);

    /* Allocate destination buffers */
    size_t max_dest_size = Size * 2 + 1024;
    Bytef *dest = (Bytef *)malloc(max_dest_size);
    if (!dest) {
        free(source);
        return 0;
    }

    /* Test 1: compressBound */
    uLong sourceLen_ul = (Size <= ULONG_MAX) ? (uLong)Size : ULONG_MAX;
    uLong bound_ul = compressBound(sourceLen_ul);
    if (bound_ul == (uLong)-1) {
        /* Error case, continue */
    }

    /* Test 2: compressBound_z */
    z_size_t sourceLen_z = (z_size_t)Size;
    z_size_t bound_z = compressBound_z(sourceLen_z);
    if (bound_z == (z_size_t)-1) {
        /* Error case, continue */
    }

    /* Test 3: compress */
    uLongf destLen_ul = (uLongf)max_dest_size;
    int ret_compress = compress(dest, &destLen_ul, source, sourceLen_ul);
    (void)ret_compress;

    /* Test 4: compress2 */
    int level = (Data[0] % 10);
    destLen_ul = (uLongf)max_dest_size;
    int ret_compress2 = compress2(dest, &destLen_ul, source, sourceLen_ul, level);
    (void)ret_compress2;

    /* Test 5: compress_z */
    z_size_t destLen_z = (z_size_t)max_dest_size;
    int ret_compress_z = compress_z(dest, &destLen_z, source, sourceLen_z);
    (void)ret_compress_z;

    /* Test 6: compress2_z */
    destLen_z = (z_size_t)max_dest_size;
    int ret_compress2_z = compress2_z(dest, &destLen_z, source, sourceLen_z, level);
    (void)ret_compress2_z;

    /* Cleanup */
    free(dest);
    free(source);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void fuzz_compress(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    uLong sourceLen = (uLong)Size;
    uLong bound = compressBound(sourceLen);
    if (bound == (uLong)-1) return;

    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) return;

    uLongf destLen = bound;
    int ret = compress(dest, &destLen, (const Bytef *)Data, sourceLen);
    (void)ret;

    free(dest);
}

static void fuzz_compress2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    uLong sourceLen = (uLong)Size;
    uLong bound = compressBound(sourceLen);
    if (bound == (uLong)-1) return;

    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) return;

    uLongf destLen = bound;
    int level = 0;
    if (Size > 0) {
        level = Data[0] % 10;
    }
    int ret = compress2(dest, &destLen, (const Bytef *)Data, sourceLen, level);
    (void)ret;

    free(dest);
}

static void fuzz_compress_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    z_size_t sourceLen = (z_size_t)Size;
    z_size_t bound = compressBound_z(sourceLen);
    if (bound == (z_size_t)-1) return;

    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) return;

    z_size_t destLen = bound;
    int ret = compress_z(dest, &destLen, (const Bytef *)Data, sourceLen);
    (void)ret;

    free(dest);
}

static void fuzz_compress2_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    z_size_t sourceLen = (z_size_t)Size;
    z_size_t bound = compressBound_z(sourceLen);
    if (bound == (z_size_t)-1) return;

    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) return;

    z_size_t destLen = bound;
    int level = 0;
    if (Size > 0) {
        level = Data[0] % 10;
    }
    int ret = compress2_z(dest, &destLen, (const Bytef *)Data, sourceLen, level);
    (void)ret;

    free(dest);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    fuzz_compress(Data, Size);
    fuzz_compress2(Data, Size);
    fuzz_compress_z(Data, Size);
    fuzz_compress2_z(Data, Size);

    return 0;
}
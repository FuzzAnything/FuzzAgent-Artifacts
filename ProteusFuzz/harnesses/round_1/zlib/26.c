// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compress2_z at compress.c:24:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_compress_bound(const uint8_t *Data, size_t Size) {
    uLong sourceLen = 0;
    if (Size >= sizeof(uLong)) {
        memcpy(&sourceLen, Data, sizeof(uLong));
        Data += sizeof(uLong);
        Size -= sizeof(uLong);
    }
    (void)compressBound(sourceLen);
}

static void test_compress2(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(uLong) + 1) return;

    uLong sourceLen;
    uLong destLen;
    uLong maxDestLen;
    int level;

    memcpy(&sourceLen, Data, sizeof(uLong));
    memcpy(&destLen, Data + sizeof(uLong), sizeof(uLong));
    memcpy(&maxDestLen, Data + 2 * sizeof(uLong), sizeof(uLong));
    level = Data[3 * sizeof(uLong)] & 0x0F;

    Data += 3 * sizeof(uLong) + 1;
    Size -= 3 * sizeof(uLong) + 1;

    if (sourceLen > Size) sourceLen = Size;
    if (maxDestLen == 0) maxDestLen = 1;

    Bytef *dest = (Bytef *)malloc(maxDestLen);
    if (!dest) return;

    destLen = maxDestLen;
    int ret = compress2(dest, &destLen, (const Bytef *)Data, sourceLen, level);
    (void)ret;

    free(dest);
}

static void test_compress(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(uLong)) return;

    uLong sourceLen;
    uLong destLen;
    uLong maxDestLen;

    memcpy(&sourceLen, Data, sizeof(uLong));
    memcpy(&maxDestLen, Data + sizeof(uLong), sizeof(uLong));

    Data += 2 * sizeof(uLong);
    Size -= 2 * sizeof(uLong);

    if (sourceLen > Size) sourceLen = Size;
    if (maxDestLen == 0) maxDestLen = 1;

    Bytef *dest = (Bytef *)malloc(maxDestLen);
    if (!dest) return;

    destLen = maxDestLen;
    int ret = compress(dest, &destLen, (const Bytef *)Data, sourceLen);
    (void)ret;

    free(dest);
}

static void test_compress_bound_z(const uint8_t *Data, size_t Size) {
    z_size_t sourceLen = 0;
    if (Size >= sizeof(z_size_t)) {
        memcpy(&sourceLen, Data, sizeof(z_size_t));
        Data += sizeof(z_size_t);
        Size -= sizeof(z_size_t);
    }
    (void)compressBound_z(sourceLen);
}

static void test_compress2_z(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(z_size_t) + 1) return;

    z_size_t sourceLen;
    z_size_t destLen;
    z_size_t maxDestLen;
    int level;

    memcpy(&sourceLen, Data, sizeof(z_size_t));
    memcpy(&destLen, Data + sizeof(z_size_t), sizeof(z_size_t));
    memcpy(&maxDestLen, Data + 2 * sizeof(z_size_t), sizeof(z_size_t));
    level = Data[3 * sizeof(z_size_t)] & 0x0F;

    Data += 3 * sizeof(z_size_t) + 1;
    Size -= 3 * sizeof(z_size_t) + 1;

    if (sourceLen > Size) sourceLen = Size;
    if (maxDestLen == 0) maxDestLen = 1;

    Bytef *dest = (Bytef *)malloc(maxDestLen);
    if (!dest) return;

    destLen = maxDestLen;
    int ret = compress2_z(dest, &destLen, (const Bytef *)Data, sourceLen, level);
    (void)ret;

    free(dest);
}

static void test_compress_z(const uint8_t *Data, size_t Size) {
    if (Size < 2 * sizeof(z_size_t)) return;

    z_size_t sourceLen;
    z_size_t destLen;
    z_size_t maxDestLen;

    memcpy(&sourceLen, Data, sizeof(z_size_t));
    memcpy(&maxDestLen, Data + sizeof(z_size_t), sizeof(z_size_t));

    Data += 2 * sizeof(z_size_t);
    Size -= 2 * sizeof(z_size_t);

    if (sourceLen > Size) sourceLen = Size;
    if (maxDestLen == 0) maxDestLen = 1;

    Bytef *dest = (Bytef *)malloc(maxDestLen);
    if (!dest) return;

    destLen = maxDestLen;
    int ret = compress_z(dest, &destLen, (const Bytef *)Data, sourceLen);
    (void)ret;

    free(dest);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int op = Data[0] % 6;
    Data++;
    Size--;

    switch (op) {
        case 0:
            test_compress_bound(Data, Size);
            break;
        case 1:
            test_compress2(Data, Size);
            break;
        case 2:
            test_compress(Data, Size);
            break;
        case 3:
            test_compress_bound_z(Data, Size);
            break;
        case 4:
            test_compress2_z(Data, Size);
            break;
        case 5:
            test_compress_z(Data, Size);
            break;
    }

    return 0;
}
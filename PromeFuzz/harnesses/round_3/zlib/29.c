// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Common buffer setup */
    size_t max_dest_size = Size * 2 + 1024;
    Bytef *dest = (Bytef *)malloc(max_dest_size);
    Bytef *source = (Bytef *)malloc(Size);
    if (!dest || !source) {
        free(dest);
        free(source);
        return 0;
    }
    memcpy(source, Data, Size);

    /* Test uncompress */
    {
        uLongf destLen = (uLongf)max_dest_size;
        uLong sourceLen = (uLong)Size;
        int ret = uncompress(dest, &destLen, source, sourceLen);
        (void)ret;
    }

    /* Test uncompress_z */
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int ret = uncompress_z(dest, &destLen, source, sourceLen);
        (void)ret;
    }

    /* Test compress2_z */
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int level = (int)(Data[0] % 10); /* 0-9 compression levels */
        int ret = compress2_z(dest, &destLen, source, sourceLen, level);
        (void)ret;
    }

    /* Test uncompress2_z */
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int ret = uncompress2_z(dest, &destLen, source, &sourceLen);
        (void)ret;
    }

    /* Test uncompress2 */
    {
        uLongf destLen = (uLongf)max_dest_size;
        uLong sourceLen = (uLong)Size;
        int ret = uncompress2(dest, &destLen, source, &sourceLen);
        (void)ret;
    }

    /* Test compress_z */
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int ret = compress_z(dest, &destLen, source, sourceLen);
        (void)ret;
    }

    /* Test with file-based operations */
    write_to_dummy_file(Data, Size);
    /* File operations could be added here if needed */
    remove_dummy_file();

    /* Cleanup */
    free(dest);
    free(source);
    return 0;
}
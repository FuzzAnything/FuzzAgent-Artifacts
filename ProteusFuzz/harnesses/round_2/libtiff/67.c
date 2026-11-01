// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

static void
setup_dummy_file(const uint8_t *data, size_t size)
{
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int
LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size)
{
    if (Size < 8) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    /* Fuzz TIFFCurrentDirOffset */
    uint64_t offset1 = TIFFCurrentDirOffset(tif);
    (void)offset1;

    /* Fuzz TIFFSetSubDirectory */
    uint64_t sub_offset = 0;
    if (Size >= 16) {
        sub_offset = 0;
        for (int i = 0; i < 8; i++) {
            sub_offset |= ((uint64_t)Data[8 + i]) << (i * 8);
        }
    }
    TIFFSetSubDirectory(tif, sub_offset);

    /* Fuzz TIFFSetDirectory */
    tdir_t dir_index = 0;
    if (Size >= 20) {
        dir_index = 0;
        for (int i = 0; i < 4; i++) {
            dir_index |= ((tdir_t)Data[16 + i]) << (i * 8);
        }
    }
    TIFFSetDirectory(tif, dir_index);

    /* Fuzz TIFFSetWriteOffset */
    toff_t write_offset = 0;
    if (Size >= 28) {
        write_offset = 0;
        for (int i = 0; i < 8; i++) {
            write_offset |= ((toff_t)Data[20 + i]) << (i * 8);
        }
    }
    TIFFSetWriteOffset(tif, write_offset);

    /* Fuzz TIFFGetStrileOffset */
    uint32_t strile = 0;
    if (Size >= 32) {
        strile = 0;
        for (int i = 0; i < 4; i++) {
            strile |= ((uint32_t)Data[28 + i]) << (i * 8);
        }
    }
    uint64_t strile_offset = TIFFGetStrileOffset(tif, strile);
    (void)strile_offset;

    /* Fuzz TIFFRewriteDirectory */
    int rewrite_result = TIFFRewriteDirectory(tif);
    (void)rewrite_result;

    TIFFClose(tif);
    return 0;
}
// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    const char *filename = NULL;
    const TIFFField *field = NULL;
    tmsize_t tile_size = 0;
    void *mem = NULL;

    if (Size < 4) {
        return 0;
    }

    write_dummy_file(Data, Size);

    tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    filename = TIFFFileName(tif);
    (void)filename;

    uint32_t tag1 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    field = TIFFFieldWithTag(tif, tag1);
    (void)field;

    uint32_t tag2 = (Data[0] << 8) | Data[1] | (Data[2] << 24) | (Data[3] << 16);
    field = TIFFFieldWithTag(tif, tag2);
    (void)field;

    uint32_t tag3 = (Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    field = TIFFFieldWithTag(tif, tag3);
    (void)field;

    uint32_t tag4 = (Data[0] << 16) | (Data[1] << 24) | Data[2] | (Data[3] << 8);
    field = TIFFFieldWithTag(tif, tag4);
    (void)field;

    tile_size = TIFFTileSize(tif);
    (void)tile_size;

    uint32_t tag5 = Data[0] ^ Data[1] ^ Data[2] ^ Data[3];
    field = TIFFFieldWithTag(tif, tag5);
    (void)field;

    if (Size > 4) {
        size_t alloc_size = (size_t)(Data[4] + 1);
        mem = _TIFFmalloc(alloc_size);
        if (mem != NULL) {
            memset(mem, 0, alloc_size);
        }
    }

    if (mem != NULL) {
        _TIFFfree(mem);
    }

    if (tif != NULL) {
        TIFFClose(tif);
    }

    return 0;
}
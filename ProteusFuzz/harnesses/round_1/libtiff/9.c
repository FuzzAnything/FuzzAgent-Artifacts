// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif1 = TIFFOpen("./dummy_file", "w");
    if (!tif1) {
        return 0;
    }

    TIFFCheckpointDirectory(tif1);

    uint32_t width = 1;
    uint32_t height = 1;
    if (Size >= 2) {
        width = Data[0];
        height = Data[1];
        if (width == 0) width = 1;
        if (height == 0) height = 1;
    }
    TIFFSetField(tif1, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif1, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif1, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif1, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif1, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    TIFFWriteDirectory(tif1);

    TIFFSetDirectory(tif1, 0);

    TIFFCreateDirectory(tif1);

    TIFFClose(tif1);
    tif1 = NULL;

    TIFF *tif2 = TIFFOpen("./dummy_file", "r+");
    if (!tif2) {
        return 0;
    }

    TIFFSetField(tif2, TIFFTAG_IMAGEWIDTH, width);
    TIFFCheckpointDirectory(tif2);
    TIFFSetField(tif2, TIFFTAG_IMAGELENGTH, height);
    TIFFCheckpointDirectory(tif2);
    TIFFWriteDirectory(tif2);

    TIFFClose(tif2);
    tif2 = NULL;

    TIFF *tif3 = TIFFOpen("./dummy_file", "r");
    if (!tif3) {
        return 0;
    }

    TIFFSetDirectory(tif3, 0);
    TIFFSetDirectory(tif3, 1);

    TIFFClose(tif3);
    tif3 = NULL;

    return 0;
}
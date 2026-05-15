// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void dummyErrorHandler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 32) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }

    uint32_t width = *(uint32_t*)Data;
    uint32_t height = *(uint32_t*)(Data + 4);
    uint16_t samples = *(uint16_t*)(Data + 8);
    uint16_t bits = *(uint16_t*)(Data + 10);
    uint32_t offset = *(uint32_t*)(Data + 12);
    uint16_t dir_index = *(uint16_t*)(Data + 16);

    TIFFWriteDirectory(tif);
    TIFFSetSubDirectory(tif, offset);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetErrorHandler(dummyErrorHandler);
    TIFFWriteDirectory(tif);
    TIFFSetErrorHandler(dummyErrorHandler);
    TIFFSetDirectory(tif, dir_index);
    uint64_t cur_offset = TIFFCurrentDirOffset(tif);
    (void)cur_offset;
    TIFFSetDirectory(tif, dir_index + 1);
    cur_offset = TIFFCurrentDirOffset(tif);
    TIFFSetSubDirectory(tif, offset + 100);
    TIFFClose(tif);
    
    // Open a new TIFF for the second TIFFClose
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        TIFFClose(tif2);
    }

    remove("./dummy_file");
    return 0;
}
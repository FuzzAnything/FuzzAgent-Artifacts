// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for writing to test directory creation functions
    TIFF *tif = TIFFOpen(filename, "w8");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // Set some basic image parameters to allow directory writing
    uint16_t width = 1;
    uint16_t height = 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);

    // Fuzz TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // Fuzz TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // Fuzz TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // Fuzz TIFFSetDirectory
    // Try setting to directory 0, 1, and a random value derived from input
    TIFFSetDirectory(tif, 0);
    if (Size > 4) {
        tdir_t dir_idx = *(const uint32_t *)(Data + 4);
        TIFFSetDirectory(tif, dir_idx);
    }

    // Fuzz TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // Cleanup
    TIFFClose(tif);
    
    // Remove dummy file
    unlink(filename);

    return 0;
}
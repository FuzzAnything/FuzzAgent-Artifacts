// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tiffio.h"

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for writing to test directory creation and writing
    TIFF *tif = TIFFOpen(filename, "w8");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes during fuzzing
    // Note: TIFFSetErrorHandlerExt takes only the handler function pointer in this version
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler);
    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warn_handler);

    // Basic setup to ensure the TIFF structure is somewhat valid for directory operations
    // Setting image width and height is often required before writing directories
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t bitspersample = 8;
    uint16_t samplesperpixel = 1;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;

    if (Size >= 2) {
        width = Data[0] | (Data[1] << 8);
    }
    if (Size >= 4) {
        height = Data[2] | (Data[3] << 8);
    }
    // Ensure non-zero dimensions to avoid immediate errors in some internal logic
    if (width == 0) width = 1;
    if (height == 0) height = 1;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);

    // Fuzzing logic for target APIs

    // 1. TIFFCreateGPSDirectory
    // Initialize GPS directory
    TIFFCreateGPSDirectory(tif);

    // 2. TIFFWriteDirectory
    // Attempt to write the directory
    TIFFWriteDirectory(tif);

    // 3. TIFFFreeDirectory
    // Free the current directory
    TIFFFreeDirectory(tif);

    // 5. TIFFCreateEXIFDirectory
    // Initialize EXIF directory
    TIFFCreateEXIFDirectory(tif);

    // 6. TIFFSetDirectory
    // Try to set a directory index. We use a value derived from input data.
    tdir_t dir_index = 0;
    if (Size > 5) {
        dir_index = (tdir_t)(Data[4] | (Data[5] << 8));
    }
    // Limit index to a reasonable range to avoid extreme memory issues, though TIFFSetDirectory handles invalid indices.
    if (dir_index > 1000) dir_index = 1000;
    
    TIFFSetDirectory(tif, dir_index);

    // Cleanup
    TIFFClose(tif);

    // Remove the dummy file
    unlink(filename);

    return 0;
}
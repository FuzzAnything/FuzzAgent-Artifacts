// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_tiff(TIFF *tif) {
    if (!tif) return;
    // Set minimal required fields to allow directory operations
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create a dummy file with input data
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    // Open TIFF in read-write mode
    TIFF *tif = TIFFOpen(filename, "rw+");
    if (!tif) {
        remove(filename);
        return 0;
    }

    // Initialize basic image parameters
    setup_tiff(tif);

    // 1. Fuzz TIFFCreateCustomDirectory
    // Passing NULL to test error handling or internal state reset
    TIFFCreateCustomDirectory(tif, NULL);

    // 2. Fuzz TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 3. Fuzz TIFFAccessTagMethods
    // Accessing internal tag methods
    TIFFTagMethods *tag_methods = TIFFAccessTagMethods(tif);
    if (tag_methods) {
        // Just ensure the pointer is valid, no dereference to avoid crashes on bad state
        (void)tag_methods;
    }

    // 4. Fuzz TIFFWriteDirectory
    // Attempt to write the directory
    TIFFWriteDirectory(tif);

    // 5. Fuzz TIFFRewriteDirectory
    // Attempt to rewrite the directory
    TIFFRewriteDirectory(tif);

    // 6. Fuzz TIFFFreeDirectory
    // Free directory resources
    TIFFFreeDirectory(tif);

    // Cleanup
    TIFFClose(tif);
    remove(filename);

    return 0;
}
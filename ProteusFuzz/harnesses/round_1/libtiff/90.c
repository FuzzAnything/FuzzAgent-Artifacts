// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
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

    // Open the TIFF file for writing
    TIFF *tif = TIFFOpen(filename, "w8");
    if (!tif) {
        return 0;
    }

    // Set error handlers to prevent crashes
    // Note: TIFFSetErrorHandlerExt and TIFFSetWarningHandlerExt take only one argument
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler);

    // Basic setup to ensure the TIFF structure is valid for directory operations
    uint16_t bitspersample = 8;
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    uint16_t samplesperpixel = 1;
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    uint32_t width = 1;
    uint32_t height = 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    uint16_t compression = COMPRESSION_NONE;
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

    // 1. Test TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 2. Test TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // 3. Test TIFFMergeFieldInfo
    // Define a custom field info array.
    // Based on the error, the struct layout is:
    // { tag, readcount, writecount, type, bit, oktochange, passcount, name, ... }
    // We use a static array to avoid dynamic allocation issues during fuzzing.
    static const TIFFFieldInfo custom_fields[] = {
        { 0xFFFF, 1, 1, TIFF_SHORT, 0, 0, 0, "CustomTag", NULL, 0, 0, 0 },
    };
    
    TIFFMergeFieldInfo(tif, custom_fields, 1);

    // 4. Test TIFFCreateCustomDirectory
    // We skip constructing a custom TIFFFieldArray manually to avoid complex struct issues.
    // The EXIF and GPS calls already exercise TIFFCreateCustomDirectory internally.

    // 5. Test TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 6. Test TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // Cleanup
    TIFFClose(tif);

    return 0;
}
// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    // Write some dummy data to the file to ensure it's not empty
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    // Open the file for writing
    TIFF *tif = TIFFOpen(filename, "w8");
    if (!tif) {
        return 0;
    }

    // Set error handlers to prevent crashes (fixed signature)
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Basic setup to ensure TIFF is in a writable state
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    // Fuzzing logic: Try various API calls based on input data
    uint8_t action = Data[0] % 8;

    switch (action) {
        case 0:
            // TIFFCreateGPSDirectory
            TIFFCreateGPSDirectory(tif);
            break;
        case 1:
            // TIFFCreateEXIFDirectory
            TIFFCreateEXIFDirectory(tif);
            break;
        case 2:
            // TIFFCreateCustomDirectory: Skip NULL input as it causes a crash in libtiff
            // The library expects a valid TIFFFieldArray pointer.
            break; 
        case 3:
            // TIFFMergeFieldInfo with empty array
            {
                static const TIFFFieldInfo empty_fields[] = {
                    {0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL}
                };
                TIFFMergeFieldInfo(tif, empty_fields, 0);
            }
            break;
        case 4:
            // TIFFMergeFieldInfo with one element
            {
                static const TIFFFieldInfo one_field[] = {
                    {0x0100, 1, 1, TIFF_SHORT, 0, 0, 0, "TestTag", NULL}
                };
                TIFFMergeFieldInfo(tif, one_field, 1);
            }
            break;
        case 5:
            // TIFFWriteDirectory
            TIFFWriteDirectory(tif);
            break;
        case 6:
            // TIFFFreeDirectory
            TIFFFreeDirectory(tif);
            break;
        case 7:
            // Multiple operations
            TIFFCreateGPSDirectory(tif);
            TIFFWriteDirectory(tif);
            TIFFFreeDirectory(tif);
            break;
        default:
            break;
    }

    // Cleanup
    TIFFClose(tif);
    remove(filename);

    return 0;
}
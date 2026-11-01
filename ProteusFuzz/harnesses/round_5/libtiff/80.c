// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
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

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF for writing
    TIFF *tif = TIFFOpen(filename, "w");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes on stderr
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Basic setup to make the TIFF valid for directory operations
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t bits = 8;
    uint16_t samples = 1;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

    // Fuzzing logic for target API functions
    
    // 1. TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // 2. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 3. TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // Re-initialize basic fields after free to allow further writes if needed
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

    // 4. TIFFCreateCustomDirectory
    // We cannot easily construct a valid TIFFFieldArray from fuzz input safely without deep parsing,
    // and the structure is opaque/incomplete in the public header. 
    // We skip direct invocation with a custom array to avoid compilation errors and undefined behavior.
    // The library's internal GPS and EXIF functions are safer to test as they use internal arrays.

    // 5. TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 6. TIFFSetDirectory
    // Try to set directory 0 (current) and potentially others if they existed
    TIFFSetDirectory(tif, 0);
    if (Size > 0) {
        // Use fuzz data to try different directory indices
        tdir_t dir_idx = Data[0] % 10; 
        TIFFSetDirectory(tif, dir_idx);
    }

    // Cleanup
    TIFFClose(tif);

    // Remove dummy file
    unlink(filename);

    return 0;
}
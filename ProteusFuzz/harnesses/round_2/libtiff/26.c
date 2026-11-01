// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
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

static void test_varargs(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVGetField(tif, tag, ap);
    va_end(ap);

    va_start(ap, tag);
    TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);

    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write the fuzzed data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the TIFF file for reading
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        // If opening fails, try to open in write mode to create a valid header
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            remove(filename);
            return 0;
        }
    }

    // Set custom error handlers
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Extract some values from the input data to use as parameters
    uint32_t tag = (Size > 0) ? Data[0] : 0;
    uint32_t tag2 = (Size > 1) ? Data[1] : 0;
    uint32_t tag3 = (Size > 2) ? Data[2] : 0;
    
    // Prepare variables for get/set operations
    uint32_t val_uint32;
    uint16_t val_uint16;
    int16_t val_int16;
    float val_float;
    double val_double;
    char *val_string = NULL;
    uint16_t *val_array = NULL;
    
    // Allocate some memory for array tests if needed
    if (Size > 3 && Data[3] > 0 && Data[3] < 100) {
        val_array = (uint16_t *)malloc(sizeof(uint16_t) * Data[3]);
        if (val_array) {
            memset(val_array, 0, sizeof(uint16_t) * Data[3]);
        }
    }

    // Test 1: TIFFGetField
    TIFFGetField(tif, tag, &val_uint32);
    TIFFGetField(tif, tag2, &val_uint16);
    TIFFGetField(tif, tag3, &val_int16);

    // Test 2: TIFFGetFieldDefaulted
    TIFFGetFieldDefaulted(tif, tag, &val_uint32);
    TIFFGetFieldDefaulted(tif, tag2, &val_uint16);
    TIFFGetFieldDefaulted(tif, tag3, &val_int16);

    // Test 3, 4, 6: TIFFVGetField, TIFFVGetFieldDefaulted, TIFFVSetField via helper
    test_varargs(tif, tag);
    test_varargs(tif, tag2);
    test_varargs(tif, tag3);

    // Test 5: TIFFUnsetField
    TIFFUnsetField(tif, tag);
    TIFFUnsetField(tif, tag2);
    TIFFUnsetField(tif, tag3);

    // Test 7: TIFFSetField (Standard wrapper)
    if (Size > 4) {
        uint16_t width = Data[4];
        uint16_t height = (Size > 5) ? Data[5] : 0;
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    }

    // Cleanup
    if (val_array) {
        free(val_array);
    }
    if (val_string) {
        free(val_string);
    }

    TIFFClose(tif);
    
    // Clean up the dummy file
    remove(filename);

    return 0;
}
// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Create a dummy file with the fuzzer data
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return NULL;
    
    // Write minimal TIFF header (BigTIFF format)
    uint8_t header[] = {
        0x4D, 0x4D, 0x00, 0x2B,  // BigTIFF magic
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00, 0x00, 0x00,  // Reserved
        0x08, 0x00, 0x00, 0x00   // TIFF version (8 for BigTIFF)
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write fuzzer data
    if (size > 0) {
        fwrite(data, 1, size, fp);
    }
    fclose(fp);
    
    // Open the file with libtiff
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return NULL;
    }
    
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    return tif;
}

static void cleanup_dummy_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
    unlink("./dummy_file");
}

// Variadic helper functions to properly use va_list APIs
static void test_vsetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVSetField(tif, tag, ap);
    (void)result;
    va_end(ap);
}

static void test_vgetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetField(tif, tag, ap);
    (void)result;
    va_end(ap);
}

static void test_vgetfielddefaulted(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetFieldDefaulted(tif, tag, ap);
    (void)result;
    va_end(ap);
}

int LLVMFuzzerTestOneInput_39(const uint8_t* data, size_t size) {
    // Limit input size to prevent excessive memory usage
    if (size > 1024 * 1024) return 0;
    
    // Create a dummy TIFF file and open it
    TIFF* tif = create_dummy_tiff(data, size);
    if (!tif) return 0;
    
    // Extract some values from fuzzer data for parameters
    // Use modulo operations to keep values in reasonable ranges
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    
    if (size >= sizeof(uint32_t)) {
        memcpy(&tag, data, sizeof(uint32_t));
        // Ensure tag is in a reasonable range
        tag = tag % 65536;
        
        if (size >= sizeof(uint32_t) + 1) {
            // Use a byte to determine data type
            uint8_t type_byte = data[sizeof(uint32_t)];
            dtype = (TIFFDataType)(type_byte % 13); // 13 is number of TIFFDataType values
        }
    }
    
    // Test TIFFFindField
    const TIFFField* field1 = TIFFFindField(tif, tag, dtype);
    (void)field1; // Result may be NULL, that's fine
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    (void)field2;
    
    // Test TIFFVSetField using helper function
    if (size >= sizeof(uint32_t) * 2) {
        uint32_t value = 0;
        memcpy(&value, data + sizeof(uint32_t), sizeof(uint32_t));
        test_vsetfield(tif, tag, value);
    }
    
    // Test TIFFGetFieldDefaulted
    if (size >= sizeof(uint32_t)) {
        uint32_t out_value = 0;
        int get_result = TIFFGetFieldDefaulted(tif, tag, &out_value);
        (void)get_result;
        (void)out_value;
    }
    
    // Test TIFFVGetField using helper function
    if (size >= sizeof(uint32_t)) {
        uint32_t out_value = 0;
        test_vgetfield(tif, tag, &out_value);
        (void)out_value;
    }
    
    // Test TIFFVGetFieldDefaulted using helper function
    if (size >= sizeof(uint32_t)) {
        uint32_t out_value = 0;
        test_vgetfielddefaulted(tif, tag, &out_value);
        (void)out_value;
    }
    
    // Cleanup
    cleanup_dummy_tiff(tif);
    
    return 0;
}
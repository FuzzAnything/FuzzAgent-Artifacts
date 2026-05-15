// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence warnings and errors during fuzzing
}

static int create_dummy_tiff(const uint8_t *data, size_t size, const char* filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    // Write minimal TIFF header
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, classic TIFF magic
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00              // End of IFD (0 entries)
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write some fuzz data if available
    if (size > 0) {
        size_t to_write = size > 1024 ? 1024 : size;
        fwrite(data, 1, to_write, fp);
    }
    
    fclose(fp);
    return 1;
}

static void test_vgetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVGetField(tif, tag, ap);
    va_end(ap);
}

static void test_vgetfield_defaulted(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
}

static void test_vsetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Silence libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Create dummy TIFF file
    const char* filename = "./dummy_file";
    if (!create_dummy_tiff(Data, Size, filename)) {
        return 0;
    }
    
    // Open TIFF file
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        remove(filename);
        return 0;
    }
    
    // Use fuzz data to generate tag values
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    
    if (Size >= 8) {
        // Use first 4 bytes for tag, next 4 for data type
        memcpy(&tag, Data, 4);
        memcpy(&dtype, Data + 4, 4);
        
        // Ensure dtype is within valid range
        if (dtype > TIFF_IFD8) {
            dtype = (TIFFDataType)(dtype % (TIFF_IFD8 + 1));
        }
    } else if (Size >= 4) {
        memcpy(&tag, Data, 4);
        dtype = TIFF_LONG;
    } else {
        tag = 256; // ImageWidth
        dtype = TIFF_LONG;
    }
    
    // Test TIFFFieldWithTag
    const TIFFField* field1 = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFFindField
    const TIFFField* field2 = TIFFFindField(tif, tag, dtype);
    
    // Test TIFFVGetField
    if (field1 || field2) {
        uint32_t value = 0;
        test_vgetfield(tif, tag, &value);
    }
    
    // Test TIFFVGetFieldDefaulted
    if (field1 || field2) {
        uint32_t value = 0;
        test_vgetfield_defaulted(tif, tag, &value);
    }
    
    // Test TIFFGetFieldDefaulted
    if (field1 || field2) {
        uint32_t value = 0;
        TIFFGetFieldDefaulted(tif, tag, &value);
    }
    
    // Test TIFFVSetField - need to open in write mode
    TIFFClose(tif);
    tif = TIFFOpen(filename, "w");
    if (tif) {
        if (field1 || field2) {
            uint32_t value = tag;
            test_vsetfield(tif, tag, value);
        }
        TIFFClose(tif);
    }
    
    // Cleanup
    remove(filename);
    return 0;
}
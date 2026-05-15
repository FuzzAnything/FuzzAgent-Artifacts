// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Create a dummy file with TIFF header and some data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    // Write minimal TIFF header (little-endian)
    unsigned char header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, TIFF magic
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
    };
    write(fd, header, sizeof(header));
    
    // Write some fuzzer data
    if (size > 0) {
        write(fd, data, size > 1024 ? 1024 : size);
    }
    
    close(fd);
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return NULL;
    }
    
    // Set error handlers to suppress messages
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);
    
    return tif;
}

int LLVMFuzzerTestOneInput_38(const uint8_t* data, size_t size) {
    if (size < 4) return 0;
    
    // Create dummy TIFF file and open it
    TIFF* tif = create_dummy_tiff(data, size);
    if (!tif) return 0;
    
    // Extract parameters from fuzzer data
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    
    // Use first 4 bytes for tag
    if (size >= 4) {
        memcpy(&tag, data, 4);
    }
    
    // Use next byte for data type (masked to valid range)
    if (size >= 5) {
        dtype = (TIFFDataType)(data[4] % (TIFF_IFD8 + 1));
    }
    
    // Test TIFFFindField with various combinations
    const TIFFField* field = TIFFFindField(tif, tag, dtype);
    
    // If field found, test other functions
    if (field) {
        // Test TIFFFieldWriteCount
        int write_count = TIFFFieldWriteCount(field);
        (void)write_count; // Use result to avoid unused variable warning
        
        // Test TIFFFieldReadCount
        int read_count = TIFFFieldReadCount(field);
        (void)read_count;
        
        // Test TIFFFieldPassCount
        int pass_count = TIFFFieldPassCount(field);
        (void)pass_count;
        
        // Test TIFFFieldSetGetSize
        int set_get_size = TIFFFieldSetGetSize(field);
        (void)set_get_size;
        
        // Test TIFFFieldSetGetCountSize
        int count_size = TIFFFieldSetGetCountSize(field);
        (void)count_size;
    }
    
    // Test with NULL field for some functions
    TIFFFieldSetGetSize(NULL);
    TIFFFieldSetGetCountSize(NULL);
    
    // Try with different tags and types using remaining data
    if (size > 8) {
        for (size_t i = 5; i + 4 < size && i < 50; i += 4) {
            uint32_t alt_tag = 0;
            memcpy(&alt_tag, data + i, 4);
            
            TIFFDataType alt_dtype = TIFF_NOTYPE;
            if (i + 5 < size) {
                alt_dtype = (TIFFDataType)(data[i + 4] % (TIFF_IFD8 + 1));
            }
            
            const TIFFField* alt_field = TIFFFindField(tif, alt_tag, alt_dtype);
            if (alt_field) {
                TIFFFieldWriteCount(alt_field);
                TIFFFieldReadCount(alt_field);
                TIFFFieldPassCount(alt_field);
                TIFFFieldSetGetSize(alt_field);
                TIFFFieldSetGetCountSize(alt_field);
            }
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Create a dummy TIFF file with minimal valid header
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    // Write minimal TIFF header (little-endian)
    unsigned char header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, magic 42
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00              // Number of directory entries (0)
    };
    write(fd, header, sizeof(header));
    
    // Write fuzz data if available
    if (size > 0 && data) {
        write(fd, data, size > 1024 ? 1024 : size);
    }
    
    close(fd);
    
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Open the dummy file
    return TIFFOpen("./dummy_file", "r");
}

int LLVMFuzzerTestOneInput_49(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) return 0;
    
    // Extract parameters from fuzz data
    uint32_t tag = *(const uint32_t*)Data;
    TIFFDataType dtype = (TIFFDataType)(Data[0] % 13); // TIFFDataType has 13 values
    
    // Test TIFFFindField with fuzzed parameters
    const TIFFField* field = TIFFFindField(tif, tag, dtype);
    
    if (field) {
        // Test all field-related functions
        int pass_count = TIFFFieldPassCount(field);
        int write_count = TIFFFieldWriteCount(field);
        int setget_count_size = TIFFFieldSetGetCountSize(field);
        int read_count = TIFFFieldReadCount(field);
        int setget_size = TIFFFieldSetGetSize(field);
        
        // Use results to avoid optimization
        volatile int dummy = pass_count + write_count + setget_count_size + 
                           read_count + setget_size;
        (void)dummy;
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
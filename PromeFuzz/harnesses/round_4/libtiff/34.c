// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
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
    // Create a dummy TIFF file with the fuzzer input
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    // Write minimal TIFF header (little-endian, classic TIFF)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, magic number 42
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00              // Padding to make it 10 bytes
    };
    write(fd, header, sizeof(header));
    
    // Write some fuzzer data as IFD entries
    if (size > 0) {
        // Write IFD entry count (max 10 entries to keep it small)
        uint16_t entry_count = size > 20 ? 10 : (size / 2);
        if (entry_count == 0) entry_count = 1;
        write(fd, &entry_count, 2);
        
        // Write dummy IFD entries using fuzzer data
        size_t entries_to_write = entry_count * 12; // 12 bytes per IFD entry
        size_t data_to_use = size > entries_to_write ? entries_to_write : size;
        if (data_to_use > 0) {
            write(fd, data, data_to_use);
        }
        
        // Write next IFD offset (0 = end)
        uint32_t next_ifd = 0;
        write(fd, &next_ifd, 4);
    }
    
    close(fd);
    
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Open the dummy file
    return TIFFOpen("./dummy_file", "r");
}

static void test_field_functions(TIFF* tif, uint32_t tag, TIFFDataType dtype, const char* name) {
    // Test TIFFFindField
    const TIFFField* field1 = TIFFFindField(tif, tag, dtype);
    if (field1) {
        // Test TIFFFieldTag on the found field
        uint32_t found_tag = TIFFFieldTag(field1);
        (void)found_tag;
        
        // Test TIFFFieldName
        const char* field_name = TIFFFieldName(field1);
        (void)field_name;
        
        // Test TIFFFieldDataType
        TIFFDataType field_type = TIFFFieldDataType(field1);
        (void)field_type;
    }
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    if (field2) {
        uint32_t tag2 = TIFFFieldTag(field2);
        (void)tag2;
    }
    
    // Test TIFFFieldWithName if name is provided
    if (name && name[0] != '\0') {
        const TIFFField* field3 = TIFFFieldWithName(tif, name);
        if (field3) {
            uint32_t tag3 = TIFFFieldTag(field3);
            (void)tag3;
        }
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Extract parameters from fuzzer input
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    char name_buf[256] = {0};
    
    // First 4 bytes for tag
    if (Size >= 4) {
        memcpy(&tag, Data, 4);
    }
    
    // Next byte for data type (mask to valid range)
    if (Size >= 5) {
        dtype = (TIFFDataType)(Data[4] % (TIFF_IFD8 + 1)); // TIFF_IFD8 is last enum value
    }
    
    // Remaining bytes for field name (null-terminated)
    if (Size >= 6) {
        size_t name_len = Size - 5;
        if (name_len > 255) name_len = 255;
        memcpy(name_buf, Data + 5, name_len);
        name_buf[name_len] = '\0';
        
        // Ensure name is printable ASCII for safety
        for (size_t i = 0; i < name_len; i++) {
            if (name_buf[i] < 32 || name_buf[i] > 126) {
                name_buf[i] = 'A' + (name_buf[i] % 26);
            }
        }
    }
    
    // Create dummy TIFF file and open it
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Test the API functions
    test_field_functions(tif, tag, dtype, name_buf);
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
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
#include <stdarg.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    // Initialize error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Create dummy TIFF file with fuzzer data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    // Write minimal TIFF header (8 bytes) followed by fuzzer data
    unsigned char header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    write(fd, header, sizeof(header));
    write(fd, Data, Size > 1024 ? 1024 : Size); // Limit size for safety
    close(fd);

    // Open the dummy TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Use fuzzer data to generate parameters for API calls
    // We'll use the data to create tags, indices, and field info
    
    // Generate parameters from fuzzer data
    uint32_t tag1 = 0, tag2 = 0, tag3 = 0;
    int index = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    
    if (Size >= 4) tag1 = *(uint32_t*)Data;
    if (Size >= 8) tag2 = *(uint32_t*)(Data + 4);
    if (Size >= 12) tag3 = *(uint32_t*)(Data + 8);
    if (Size >= 16) index = *(int*)(Data + 12);
    if (Size >= 20) dtype = *(TIFFDataType*)(Data + 16);
    
    // Limit index range
    if (index < 0) index = -index;
    index = index % 100;
    
    // Test TIFFFindField
    const TIFFField* field1 = TIFFFindField(tif, tag1, dtype);
    
    // Test TIFFGetTagListEntry
    uint32_t tag_entry = TIFFGetTagListEntry(tif, index);
    
    // Test TIFFUnsetField
    int unset_result = TIFFUnsetField(tif, tag2);
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag3);
    
    // Test TIFFFieldTag if we found a field
    if (field1) {
        uint32_t field_tag = TIFFFieldTag(field1);
    }
    if (field2) {
        uint32_t field_tag = TIFFFieldTag(field2);
    }
    
    // Test TIFFMergeFieldInfo with dummy field info
    if (Size >= 24) {
        // Create a minimal TIFFFieldInfo array from fuzzer data
        TIFFFieldInfo field_info[1];
        field_info[0].field_tag = tag1;
        field_info[0].field_type = dtype;
        field_info[0].field_readcount = 1;
        field_info[0].field_writecount = 1;
        field_info[0].field_bit = 0;
        field_info[0].field_oktochange = 1;
        field_info[0].field_passcount = 0;
        field_info[0].field_name = "dummy";
        
        int merge_result = TIFFMergeFieldInfo(tif, field_info, 1);
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
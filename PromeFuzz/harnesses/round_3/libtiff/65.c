// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    create_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use fuzz data to generate parameters
    size_t offset = 0;
    
    // Generate tag values from fuzz data
    uint32_t tag1 = 0, tag2 = 0, tag3 = 0;
    int tag_index = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    
    if (Size - offset >= sizeof(uint32_t)) {
        memcpy(&tag1, Data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
    if (Size - offset >= sizeof(uint32_t)) {
        memcpy(&tag2, Data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
    if (Size - offset >= sizeof(uint32_t)) {
        memcpy(&tag3, Data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
    if (Size - offset >= sizeof(int)) {
        memcpy(&tag_index, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    if (Size - offset >= 1) {
        dtype = (TIFFDataType)(Data[offset] % (TIFF_IFD8 + 1));
        offset++;
    }
    
    // Test TIFFFindField
    const TIFFField *field1 = TIFFFindField(tif, tag1, dtype);
    
    // Test TIFFGetTagListEntry
    uint32_t tag_entry = TIFFGetTagListEntry(tif, tag_index);
    (void)tag_entry; // Prevent unused warning
    
    // Test TIFFFieldWithTag
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag2);
    
    // Test TIFFFieldTag if we have valid fields
    if (field1) {
        uint32_t tag_val1 = TIFFFieldTag(field1);
        (void)tag_val1;
    }
    if (field2) {
        uint32_t tag_val2 = TIFFFieldTag(field2);
        (void)tag_val2;
    }
    
    // Test TIFFUnsetField
    int unset_result = TIFFUnsetField(tif, tag3);
    (void)unset_result;
    
    // Test TIFFMergeFieldInfo with dummy field info
    // We need to create a minimal TIFFFieldInfo array
    // Note: TIFFFieldInfo is an opaque type in public API, but we can try
    // to use what's available from the headers
    if (Size - offset >= sizeof(TIFFFieldInfo) * 2) {
        // Create a small array with data from fuzz input
        TIFFFieldInfo *field_infos = (TIFFFieldInfo*)(Data + offset);
        uint32_t num_fields = 1; // Use 1 field to avoid complex validation
        
        // Try to merge - this may fail but that's OK for fuzzing
        int merge_result = TIFFMergeFieldInfo(tif, field_infos, num_fields);
        (void)merge_result;
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}
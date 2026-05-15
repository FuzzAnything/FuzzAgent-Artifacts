// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    static uint8_t dummy_data[1024] = {0};
    tmsize_t* offset = (tmsize_t*)handle;
    tmsize_t available = 1024 - *offset;
    tmsize_t to_read = size < available ? size : available;
    if (to_read > 0) {
        memcpy(buf, dummy_data + *offset, to_read);
        *offset += to_read;
    }
    return to_read;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    return size;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    static tmsize_t pos = 0;
    tmsize_t* ppos = (tmsize_t*)handle;
    switch (whence) {
        case SEEK_SET:
            *ppos = (tmsize_t)offset;
            break;
        case SEEK_CUR:
            *ppos += (tmsize_t)offset;
            break;
        case SEEK_END:
            *ppos = 1024 + (tmsize_t)offset;
            break;
    }
    if (*ppos < 0) *ppos = 0;
    if (*ppos > 1024) *ppos = 1024;
    return *ppos;
}

static int dummy_close(thandle_t handle) {
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    return 1024;
}

static TIFF* create_dummy_tiff() {
    static tmsize_t offset = 0;
    offset = 0;
    
    TIFF* tif = TIFFClientOpen("dummy", "w", (thandle_t)&offset,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, NULL, NULL);
    
    return tif;
}

static void test_vsetfield(TIFF* tif, uint32_t tag, uint32_t value) {
    // Create a va_list with a single argument
    va_list ap;
    // We need to use a variadic function to properly initialize va_list
    // Create a helper function that uses variadic arguments
    // But for simplicity, we'll use a different approach
    // We'll create a small wrapper that calls TIFFSetField instead
    // since TIFFSetField internally calls TIFFVSetField
    TIFFSetField(tif, tag, value);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_dummy_tiff();
    if (!tif) return 0;
    
    // Test TIFFMergeFieldInfo
    if (Size >= 32) {
        // Create a dummy TIFFFieldInfo array
        TIFFFieldInfo field_info[1];
        // Initialize with data from fuzzer input
        memcpy(&field_info[0], Data, sizeof(TIFFFieldInfo) < 32 ? sizeof(TIFFFieldInfo) : 32);
        TIFFMergeFieldInfo(tif, field_info, 1);
    }
    
    // Test TIFFVSetField via TIFFSetField
    if (Size >= 8) {
        uint32_t tag;
        memcpy(&tag, Data, 4);
        uint32_t value;
        memcpy(&value, Data + 4, 4);
        test_vsetfield(tif, tag, value);
    }
    
    // Test TIFFGetTagListCount
    int tag_count = TIFFGetTagListCount(tif);
    
    // Test TIFFUnsetField
    if (Size >= 4) {
        uint32_t tag_to_unset;
        memcpy(&tag_to_unset, Data, 4);
        TIFFUnsetField(tif, tag_to_unset);
    }
    
    // Test TIFFGetTagListEntry
    if (tag_count > 0) {
        for (int i = 0; i < tag_count && i < 10; i++) {
            TIFFGetTagListEntry(tif, i);
        }
    }
    
    // Test TIFFCreateCustomDirectory
    // Pass NULL since we can't properly construct TIFFFieldArray
    TIFFCreateCustomDirectory(tif, NULL);
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}
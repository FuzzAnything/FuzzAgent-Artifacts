// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

static void test_vsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);
    
    // Try to open the file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // If opening fails, try creating a new TIFF file
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Extract some values from fuzzer data for parameters
    // Use modulo to keep values in reasonable ranges
    uint32_t tag = 0;
    int tag_index = 0;
    uint32_t field_count = 0;
    
    if (Size >= 4) {
        memcpy(&tag, Data, 4);
        tag_index = Data[0] % 100;  // Reasonable index range
        field_count = Data[1] % 50; // Reasonable field count
    }
    
    // Test TIFFGetTagListCount
    int tag_count = TIFFGetTagListCount(tif);
    
    // Test TIFFGetTagListEntry with various indices
    if (tag_count > 0) {
        // Try valid indices
        for (int i = 0; i < 3 && i < tag_count; i++) {
            TIFFGetTagListEntry(tif, i);
        }
        // Try out of bounds
        TIFFGetTagListEntry(tif, -1);
        TIFFGetTagListEntry(tif, tag_count);
        TIFFGetTagListEntry(tif, tag_count + 100);
    }
    
    // Test TIFFUnsetField with various tags
    TIFFUnsetField(tif, tag);
    TIFFUnsetField(tif, 0);
    TIFFUnsetField(tif, 65535);
    
    // Test TIFFVSetField using helper function
    test_vsetfield(tif, tag);
    test_vsetfield(tif, tag + 1);
    
    // Removed: Test TIFFCreateCustomDirectory with NULL (since we can't create a proper TIFFFieldArray)
    // This was causing the crash because TIFFCreateCustomDirectory requires a valid TIFFFieldArray
    // TIFFCreateCustomDirectory(tif, NULL);
    
    // Test TIFFMergeFieldInfo with NULL array (but only if count is 0)
    TIFFMergeFieldInfo(tif, NULL, 0);
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
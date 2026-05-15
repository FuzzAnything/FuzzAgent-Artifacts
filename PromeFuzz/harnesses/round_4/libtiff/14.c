// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
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
#include <stdarg.h>

static void dummyErrorHandler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress error output
}

static void dummyErrorHandler2(const char* module, const char* fmt, va_list ap) {
    // Another dummy handler
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening in write mode if read+write fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) return 0;
    }
    
    // Create a second TIFF handle for testing
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    
    // Call functions in specified order
    
    // 1. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 2. TIFFSetSubDirectory
    uint64_t offset = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&offset, Data, sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, offset);
    
    // 3. TIFFSetField
    uint32_t tag = 256; // IMAGEWIDTH
    uint32_t value = 100;
    if (Size >= 2 * sizeof(uint32_t)) {
        memcpy(&tag, Data, sizeof(uint32_t));
        memcpy(&value, Data + sizeof(uint32_t), sizeof(uint32_t));
    }
    TIFFSetField(tif, tag, value);
    
    // 4. TIFFSetErrorHandler
    TIFFErrorHandler prev_handler = TIFFSetErrorHandler(dummyErrorHandler);
    
    // 5. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 6. TIFFSetErrorHandler
    TIFFSetErrorHandler(dummyErrorHandler2);
    
    // 7. TIFFSetDirectory
    tdir_t dir_index = 0;
    if (Size >= sizeof(tdir_t)) {
        memcpy(&dir_index, Data, sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_index);
    
    // 8. TIFFCurrentDirOffset
    uint64_t current_offset = TIFFCurrentDirOffset(tif);
    
    // 9. TIFFSetDirectory
    tdir_t dir_index2 = 1;
    if (Size >= 2 * sizeof(tdir_t)) {
        memcpy(&dir_index2, Data + sizeof(tdir_t), sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_index2);
    
    // 10. TIFFCurrentDirOffset
    current_offset = TIFFCurrentDirOffset(tif);
    
    // 11. TIFFSetSubDirectory
    uint64_t offset2 = 100;
    if (Size >= 2 * sizeof(uint64_t)) {
        memcpy(&offset2, Data + sizeof(uint64_t), sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, offset2);
    
    // 12. TIFFClose
    TIFFClose(tif);
    
    // 13. TIFFClose
    if (tif2) {
        TIFFClose(tif2);
    }
    
    // Restore original error handler
    TIFFSetErrorHandler(prev_handler);
    
    return 0;
}
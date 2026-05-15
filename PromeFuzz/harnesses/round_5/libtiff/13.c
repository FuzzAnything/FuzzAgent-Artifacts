// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tiffio.h"

static void dummyErrorHandler(const char* module, const char* fmt, va_list ap) {
    // Empty error handler to suppress error output during fuzzing
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create a dummy file for TIFF operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    
    // Write fuzzer data to the file
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Open the TIFF file for writing
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }
    
    // Set some basic fields first to make directory writing possible
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // 1. First TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 2. TIFFSetSubDirectory - use a dummy offset
    uint64_t offset = 0;
    if (Size >= sizeof(uint64_t)) {
        offset = *(uint64_t*)Data;
    }
    TIFFSetSubDirectory(tif, offset);
    
    // 3. TIFFSetField - try to set various fields with fuzzer data
    if (Size >= sizeof(uint32_t)) {
        uint32_t tag = *(uint32_t*)Data;
        // Use modulo to get a reasonable value
        uint32_t value = tag % 1000;
        TIFFSetField(tif, tag, value);
    }
    
    // 4. First TIFFSetErrorHandler
    TIFFErrorHandler old_handler = TIFFSetErrorHandler(dummyErrorHandler);
    
    // 5. Second TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 6. Second TIFFSetErrorHandler - restore original handler
    TIFFSetErrorHandler(old_handler);
    
    // 7. First TIFFSetDirectory
    tdir_t dir_index = 0;
    if (Size >= sizeof(tdir_t)) {
        dir_index = *(tdir_t*)Data;
    }
    TIFFSetDirectory(tif, dir_index);
    
    // 8. First TIFFCurrentDirOffset
    uint64_t current_offset = TIFFCurrentDirOffset(tif);
    (void)current_offset; // Use result to avoid unused variable warning
    
    // 9. Second TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index + 1);
    
    // 10. Second TIFFCurrentDirOffset
    current_offset = TIFFCurrentDirOffset(tif);
    (void)current_offset;
    
    // 11. Second TIFFSetSubDirectory
    if (Size >= 2 * sizeof(uint64_t)) {
        offset = *((uint64_t*)Data + 1);
    }
    TIFFSetSubDirectory(tif, offset);
    
    // 12. First TIFFClose
    TIFFClose(tif);
    
    // 13. Second TIFFClose - try to close a NULL pointer
    // This tests error handling for invalid close operations
    TIFFClose(NULL);
    
    // Clean up the dummy file
    remove("./dummy_file");
    
    return 0;
}
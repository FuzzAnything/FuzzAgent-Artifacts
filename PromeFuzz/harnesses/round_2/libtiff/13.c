// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void dummyErrorHandler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress error output during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file for writing
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }
    
    // First TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // TIFFSetSubDirectory - use fuzzer data for offset
    uint64_t subdir_offset = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&subdir_offset, Data, sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, subdir_offset);
    
    // TIFFSetField - try to set various fields using fuzzer data
    if (Size >= 12) {
        uint32_t tag = 0;
        uint32_t value1 = 0;
        uint32_t value2 = 0;
        
        // Extract values from fuzzer data
        memcpy(&tag, Data, sizeof(uint32_t));
        memcpy(&value1, Data + 4, sizeof(uint32_t));
        memcpy(&value2, Data + 8, sizeof(uint32_t));
        
        // Try different field types based on fuzzer data
        if (Size % 3 == 0) {
            TIFFSetField(tif, tag, value1);
        } else if (Size % 3 == 1) {
            TIFFSetField(tif, tag, value1, value2);
        } else {
            TIFFSetField(tif, tag, (double)value1);
        }
    }
    
    // First TIFFSetErrorHandler
    TIFFSetErrorHandler(dummyErrorHandler);
    
    // Second TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // Second TIFFSetErrorHandler
    TIFFSetErrorHandler(dummyErrorHandler);
    
    // First TIFFSetDirectory
    tdir_t dir_index = 0;
    if (Size >= sizeof(tdir_t)) {
        memcpy(&dir_index, Data, sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_index);
    
    // First TIFFCurrentDirOffset
    TIFFCurrentDirOffset(tif);
    
    // Second TIFFSetDirectory - use different index
    tdir_t dir_index2 = 1;
    if (Size >= sizeof(tdir_t) * 2) {
        memcpy(&dir_index2, Data + sizeof(tdir_t), sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_index2);
    
    // Second TIFFCurrentDirOffset
    TIFFCurrentDirOffset(tif);
    
    // Second TIFFSetSubDirectory - use different offset
    uint64_t subdir_offset2 = 100;
    if (Size >= sizeof(uint64_t) * 2) {
        memcpy(&subdir_offset2, Data + sizeof(uint64_t), sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, subdir_offset2);
    
    // First TIFFClose
    TIFFClose(tif);
    
    // Open another TIFF file for the second close
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Second TIFFClose
        TIFFClose(tif);
    }
    
    return 0;
}
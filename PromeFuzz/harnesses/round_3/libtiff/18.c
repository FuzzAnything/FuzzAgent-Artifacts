// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Step 1: Allocate TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Step 2: Set memory limit (use a value derived from input)
    tmsize_t max_mem = 0;
    if (Size >= sizeof(tmsize_t)) {
        max_mem = *(tmsize_t*)Data;
    }
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_mem);
    
    // Step 3: Open TIFF file with options
    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);
    
    // Step 4: Free options (regardless of whether TIFF opened successfully)
    TIFFOpenOptionsFree(opts);
    
    // If TIFF opened successfully, proceed with operations
    if (tif) {
        // Step 5: Close TIFF (as per the order, but we need to reopen for writing)
        TIFFClose(tif);
        
        // Reopen for writing since we closed it
        tif = TIFFOpenExt("./dummy_file", "w", NULL);
        if (tif) {
            // Step 6 & 7: Set some fields (use values from fuzzer input)
            uint32_t tag1 = TIFFTAG_IMAGEWIDTH;
            uint32_t tag2 = TIFFTAG_IMAGELENGTH;
            uint32_t value1 = 1;
            uint32_t value2 = 1;
            
            if (Size >= 8) {
                value1 = *(uint32_t*)Data;
                value2 = *(uint32_t*)(Data + 4);
            }
            
            TIFFSetField(tif, tag1, value1);
            TIFFSetField(tif, tag2, value2);
            
            // Step 8: Write directory
            TIFFWriteDirectory(tif);
            
            // Step 9: Close TIFF
            TIFFClose(tif);
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}
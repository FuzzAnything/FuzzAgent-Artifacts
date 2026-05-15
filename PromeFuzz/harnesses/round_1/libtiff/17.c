// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
#include <string.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Step 1: Allocate TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Step 2: Set max single memory allocation limit
    // Use a reasonable value derived from fuzzer input to avoid huge allocations
    tmsize_t max_alloc = (tmsize_t)(Size % 1024) + 1;
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);
    
    // Step 3: Open TIFF file with options
    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);
    
    // Step 4: Free options
    TIFFOpenOptionsFree(opts);
    
    // Step 5: First TIFFClose (if file was opened)
    if (tif) {
        TIFFClose(tif);
    }
    
    // Re-open file for writing operations
    tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }
    
    // Step 6: Set first field
    // Use fuzzer data to determine field values
    uint32_t width = (Size > 0) ? (uint32_t)Data[0] : 1;
    uint32_t height = (Size > 1) ? (uint32_t)Data[1] : 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    
    // Step 7: Set second field
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    
    // Step 8: Write directory
    TIFFWriteDirectory(tif);
    
    // Step 9: Final close
    TIFFClose(tif);
    
    return 0;
}
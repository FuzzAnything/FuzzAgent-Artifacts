// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Set up error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening in write mode if read fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Test TIFFIsUpSampled - can be called on any valid TIFF handle
    TIFFIsUpSampled(tif);
    
    // Test TIFFReadDirectory if we opened in read mode
    if (TIFFCurrentDirectory(tif) == 0) {
        // Try to read first directory
        int read_result = TIFFReadDirectory(tif);
        
        // Continue reading directories until failure
        while (read_result > 0) {
            read_result = TIFFReadDirectory(tif);
        }
    }
    
    // Test TIFFSetSubDirectory with various offsets
    // Use fuzzer data to generate offsets if available
    uint64_t offset = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&offset, Data, sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, offset);
    
    // Test TIFFCheckpointDirectory
    TIFFCheckpointDirectory(tif);
    
    // Test TIFFCreateDirectory
    TIFFCreateDirectory(tif);
    
    // Test TIFFRewriteDirectory
    TIFFRewriteDirectory(tif);
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
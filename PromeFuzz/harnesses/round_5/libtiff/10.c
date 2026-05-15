// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include <stdio.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Use a dummy file for all operations
    const char* filename = "./dummy_file";
    
    // Write input data to dummy file
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // First block: Open, WriteDirectory, Close
    TIFF* tif1 = TIFFOpen(filename, "w");
    if (tif1) {
        TIFFWriteDirectory(tif1);
        TIFFClose(tif1);
    }
    
    // Second block: Open, RewriteDirectory, CreateDirectory, WriteDirectory, NumberOfDirectories, Close
    TIFF* tif2 = TIFFOpen(filename, "r+");
    if (tif2) {
        TIFFRewriteDirectory(tif2);
        TIFFCreateDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFNumberOfDirectories(tif2);
        TIFFClose(tif2);
    }
    
    // Third block: Open, SetSubDirectory, WriteDirectory, NumberOfDirectories, UnlinkDirectory, Close, Close
    TIFF* tif3 = TIFFOpen(filename, "r+");
    if (tif3) {
        // Use first 8 bytes of data for offset, or 0 if not enough
        uint64_t offset = 0;
        if (Size >= sizeof(uint64_t)) {
            memcpy(&offset, Data, sizeof(uint64_t));
        }
        
        TIFFSetSubDirectory(tif3, offset);
        TIFFWriteDirectory(tif3);
        TIFFNumberOfDirectories(tif3);
        
        // Use next 4 bytes for directory number, or 0 if not enough
        tdir_t dir_num = 0;
        if (Size >= sizeof(uint64_t) + sizeof(tdir_t)) {
            memcpy(&dir_num, Data + sizeof(uint64_t), sizeof(tdir_t));
        }
        
        TIFFUnlinkDirectory(tif3, dir_num);
        TIFFClose(tif3);
        
        // Double close (as per specification)
        TIFFClose(tif3);
    }
    
    // Clean up dummy file
    remove(filename);
    
    return 0;
}
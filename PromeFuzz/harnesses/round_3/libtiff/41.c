// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
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
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer data to file
    if (write_to_file("./dummy_file", Data, Size) != 0) {
        return 0;
    }
    
    // Try opening the file in different modes
    const char* modes[] = {"r", "r+", "w", "w+"};
    for (int mode_idx = 0; mode_idx < 4; mode_idx++) {
        TIFF* tif = TIFFOpen("./dummy_file", modes[mode_idx]);
        if (!tif) continue;
        
        // Test TIFFReadDirectory - read all directories
        while (TIFFReadDirectory(tif) > 0) {
            // Test TIFFLastDirectory after each read
            TIFFLastDirectory(tif);
            
            // Test TIFFCheckpointDirectory
            TIFFCheckpointDirectory(tif);
            
            // Test TIFFRewriteDirectory
            TIFFRewriteDirectory(tif);
            
            // Extract an offset from fuzzer data for TIFFSetSubDirectory
            uint64_t offset = 0;
            if (Size >= sizeof(uint64_t)) {
                memcpy(&offset, Data, sizeof(uint64_t));
            }
            
            // Test TIFFSetSubDirectory with various offsets
            TIFFSetSubDirectory(tif, offset);
            TIFFSetSubDirectory(tif, TIFFCurrentDirOffset(tif));
            TIFFSetSubDirectory(tif, 0);
            TIFFSetSubDirectory(tif, UINT64_MAX);
        }
        
        // Reset directory to first one
        TIFFSetDirectory(tif, 0);
        
        // Test TIFFCreateDirectory
        TIFFCreateDirectory(tif);
        
        // Test TIFFCheckpointDirectory on newly created directory
        TIFFCheckpointDirectory(tif);
        
        // Test TIFFRewriteDirectory on newly created directory
        TIFFRewriteDirectory(tif);
        
        // Test TIFFLastDirectory after directory operations
        TIFFLastDirectory(tif);
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Remove temporary file
    unlink("./dummy_file");
    
    return 0;
}
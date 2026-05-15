// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer data to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Try to open the file with different modes
    const char* modes[] = {"r", "w", "a", "r+", "w+", "a+"};
    
    for (size_t mode_idx = 0; mode_idx < sizeof(modes)/sizeof(modes[0]); mode_idx++) {
        TIFF* tif = TIFFOpen("./dummy_file", modes[mode_idx]);
        if (!tif) {
            continue;
        }
        
        // Test TIFFCreateDirectory
        if (Size > 0 && (Data[0] & 0x01)) {
            TIFFCreateDirectory(tif);
        }
        
        // Test TIFFWriteDirectory
        if (Size > 1 && (Data[1] & 0x01)) {
            TIFFWriteDirectory(tif);
        }
        
        // Test TIFFRewriteDirectory
        if (Size > 2 && (Data[2] & 0x01)) {
            TIFFRewriteDirectory(tif);
        }
        
        // Test TIFFReadDirectory multiple times
        if (Size > 3) {
            int max_reads = Data[3] % 10; // Limit to reasonable number
            for (int i = 0; i < max_reads; i++) {
                int ret = TIFFReadDirectory(tif);
                if (ret <= 0) break;
            }
        }
        
        // Test TIFFFreeDirectory
        if (Size > 4 && (Data[4] & 0x01)) {
            TIFFFreeDirectory(tif);
        }
        
        // Clean up
        TIFFCleanup(tif);
        
        // Reopen for more testing
        tif = TIFFOpen("./dummy_file", modes[mode_idx]);
        if (tif) {
            // Test combination of operations
            if (Size > 5) {
                // Create, write, then read directories
                if (Data[5] & 0x01) {
                    TIFFCreateDirectory(tif);
                    TIFFWriteDirectory(tif);
                }
                
                // Try to read after writing
                if (Data[5] & 0x02) {
                    TIFFReadDirectory(tif);
                }
                
                // Create and free without writing
                if (Data[5] & 0x04) {
                    TIFFCreateDirectory(tif);
                    TIFFFreeDirectory(tif);
                }
                
                // Rewrite if possible
                if (Data[5] & 0x08) {
                    TIFFRewriteDirectory(tif);
                }
            }
            
            TIFFCleanup(tif);
        }
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}
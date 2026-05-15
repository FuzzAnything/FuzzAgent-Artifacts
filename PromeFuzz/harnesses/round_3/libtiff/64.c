// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
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
    // Silent error handler
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Set error handlers to silent
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write data to dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink("./dummy_file");
        return 0;
    }
    close(fd);
    
    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening for writing if read fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Fuzz target functions in various sequences
    TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
    (void)methods; // Use result to avoid unused variable warning
    
    // Try different sequences based on input data
    uint8_t sequence = Size > 0 ? Data[0] % 8 : 0;
    
    switch (sequence) {
        case 0:
            // Sequence 1: Access tag methods -> Create directory -> Free directory
            TIFFAccessTagMethods(tif);
            TIFFCreateDirectory(tif);
            TIFFFreeDirectory(tif);
            break;
            
        case 1:
            // Sequence 2: Create custom directory -> Write custom directory
            {
                uint64_t offset = 0;
                TIFFCreateCustomDirectory(tif, NULL);
                TIFFWriteCustomDirectory(tif, &offset);
            }
            break;
            
        case 2:
            // Sequence 3: Create directory -> Free directory -> Access tag methods
            TIFFCreateDirectory(tif);
            TIFFFreeDirectory(tif);
            TIFFAccessTagMethods(tif);
            break;
            
        case 3:
            // Sequence 4: Create custom directory -> Read custom directory
            TIFFCreateCustomDirectory(tif, NULL);
            TIFFReadCustomDirectory(tif, 0, NULL);
            break;
            
        case 4:
            // Sequence 5: Create directory -> Create custom directory -> Free directory
            TIFFCreateDirectory(tif);
            TIFFCreateCustomDirectory(tif, NULL);
            TIFFFreeDirectory(tif);
            break;
            
        case 5:
            // Sequence 6: Write custom directory (may fail) -> Read custom directory
            {
                uint64_t offset = 0;
                TIFFWriteCustomDirectory(tif, &offset);
                TIFFReadCustomDirectory(tif, offset, NULL);
            }
            break;
            
        case 6:
            // Sequence 7: Multiple directory operations
            for (int i = 0; i < 3 && i < (int)Size; i++) {
                if (Data[i] % 2 == 0) {
                    TIFFCreateDirectory(tif);
                } else {
                    TIFFCreateCustomDirectory(tif, NULL);
                }
                TIFFAccessTagMethods(tif);
            }
            TIFFFreeDirectory(tif);
            break;
            
        case 7:
            // Sequence 8: Complex sequence with all functions
            TIFFAccessTagMethods(tif);
            TIFFCreateDirectory(tif);
            TIFFAccessTagMethods(tif);
            TIFFFreeDirectory(tif);
            
            TIFFCreateCustomDirectory(tif, NULL);
            {
                uint64_t offset = 0;
                TIFFWriteCustomDirectory(tif, &offset);
                TIFFReadCustomDirectory(tif, offset, NULL);
            }
            break;
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
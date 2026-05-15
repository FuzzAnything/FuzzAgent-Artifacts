// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>

static void dummy_cleanup(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    close(fd);
    
    // Open TIFF file with the dummy data
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as new file if reading fails
        tif = TIFFOpen("./dummy_file", "w+");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Use fuzzer data to decide which operations to perform
    uint8_t op_selector = Data[0] % 6;
    
    switch (op_selector) {
        case 0: {
            // Test TIFFCreateDirectory
            int result = TIFFCreateDirectory(tif);
            (void)result; // Use result to avoid unused variable warning
            break;
        }
        case 1: {
            // Test TIFFWriteCustomDirectory
            uint64_t offset = 0;
            if (Size > sizeof(uint64_t)) {
                memcpy(&offset, Data, sizeof(uint64_t));
            }
            int result = TIFFWriteCustomDirectory(tif, &offset);
            (void)result;
            break;
        }
        case 2: {
            // Test TIFFCreateCustomDirectory
            // TIFFFieldArray is an opaque type, so we use NULL
            int result = TIFFCreateCustomDirectory(tif, NULL);
            (void)result;
            break;
        }
        case 3: {
            // Test TIFFFreeDirectory
            TIFFFreeDirectory(tif);
            break;
        }
        case 4: {
            // Test TIFFReadCustomDirectory
            toff_t diroff = 0;
            if (Size > sizeof(toff_t)) {
                memcpy(&diroff, Data, sizeof(toff_t));
            }
            
            // TIFFFieldArray is an opaque type, so we use NULL
            int result = TIFFReadCustomDirectory(tif, diroff, NULL);
            (void)result;
            break;
        }
        case 5: {
            // Test TIFFAccessTagMethods
            TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
            (void)methods; // Use methods to avoid unused variable warning
            break;
        }
    }
    
    // Clean up
    dummy_cleanup(tif);
    unlink("./dummy_file");
    
    return 0;
}
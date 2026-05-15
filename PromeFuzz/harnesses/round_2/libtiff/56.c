// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
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

static int dummy_fd = -1;

static void cleanup_dummy_file(void) {
    if (dummy_fd != -1) {
        close(dummy_fd);
        dummy_fd = -1;
    }
    unlink("./dummy_file");
}

static int setup_dummy_file(const uint8_t *data, size_t size) {
    cleanup_dummy_file();
    
    dummy_fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (dummy_fd == -1) {
        return 0;
    }
    
    if (write(dummy_fd, data, size) != (ssize_t)size) {
        cleanup_dummy_file();
        return 0;
    }
    
    return 1;
}

static TIFF* open_tiff_for_fuzzing(const char* mode) {
    return TIFFOpen("./dummy_file", mode);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }
    
    if (!setup_dummy_file(Data, Size)) {
        return 0;
    }
    
    TIFF* tif = NULL;
    uint64_t offset = 0;
    int result = 0;
    
    // Use first byte to determine operation mode
    uint8_t mode = Data[0] % 4;
    
    switch (mode) {
        case 0: {
            // Test TIFFCreateDirectory and TIFFFreeDirectory
            tif = open_tiff_for_fuzzing("w");
            if (tif) {
                result = TIFFCreateDirectory(tif);
                if (result) {
                    TIFFFreeDirectory(tif);
                }
                TIFFClose(tif);
            }
            break;
        }
        
        case 1: {
            // Test TIFFAccessTagMethods
            tif = open_tiff_for_fuzzing("r");
            if (tif) {
                TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
                (void)methods; // Use result to avoid unused variable warning
                TIFFClose(tif);
            }
            break;
        }
        
        case 2: {
            // Test TIFFCreateCustomDirectory and TIFFWriteCustomDirectory
            tif = open_tiff_for_fuzzing("w");
            if (tif) {
                // Pass NULL for infoarray since we can't construct a valid TIFFFieldArray
                result = TIFFCreateCustomDirectory(tif, NULL);
                if (result) {
                    TIFFWriteCustomDirectory(tif, &offset);
                }
                TIFFClose(tif);
            }
            break;
        }
        
        case 3: {
            // Test TIFFReadCustomDirectory
            tif = open_tiff_for_fuzzing("r");
            if (tif) {
                // Use data to determine offset
                toff_t diroff = (toff_t)(Data[2] | (Data[3] << 8) | 
                                        (Data[4] << 16) | (Data[5] << 24));
                
                // Pass NULL for infoarray since we can't construct a valid TIFFFieldArray
                TIFFReadCustomDirectory(tif, diroff, NULL);
                TIFFClose(tif);
            }
            break;
        }
    }
    
    cleanup_dummy_file();
    return 0;
}
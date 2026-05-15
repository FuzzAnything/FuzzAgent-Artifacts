// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Create a dummy file with the fuzzer input
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    // Write fuzzer data to file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink("./dummy_file");
        return 0;
    }
    close(fd);
    
    // Open the TIFF file - try both read and write modes
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try to create a new file if reading fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Extract some values from the input for parameterized functions
    uint64_t subdir_offset = 0;
    tdir_t dir_index = 0;
    
    if (Size >= sizeof(uint64_t) + sizeof(tdir_t)) {
        memcpy(&subdir_offset, Data, sizeof(uint64_t));
        memcpy(&dir_index, Data + sizeof(uint64_t), sizeof(tdir_t));
    } else if (Size >= sizeof(tdir_t)) {
        memcpy(&dir_index, Data, sizeof(tdir_t));
        subdir_offset = Size;
    } else {
        subdir_offset = Size;
        dir_index = Size;
    }
    
    // Fuzz TIFFCreateDirectory
    if (TIFFCurrentDirectory(tif) >= 0) {
        TIFFCreateDirectory(tif);
    }
    
    // Fuzz TIFFRewriteDirectory
    if (TIFFCurrentDirectory(tif) >= 0) {
        TIFFRewriteDirectory(tif);
    }
    
    // Fuzz TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, subdir_offset);
    
    // Fuzz TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);
    
    // Fuzz TIFFReadDirectory
    // Try to read multiple directories
    for (int i = 0; i < 10; i++) {
        int ret = TIFFReadDirectory(tif);
        if (ret <= 0) break;
    }
    
    // Fuzz TIFFLastDirectory
    TIFFLastDirectory(tif);
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}
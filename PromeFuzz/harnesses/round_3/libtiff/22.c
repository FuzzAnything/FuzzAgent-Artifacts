// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer data to dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    // Write data to file
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Open TIFF file for writing (we need write access for some functions)
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        // Try opening for reading if write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Call functions in specified order
    // 1. TIFFWriteDirectory
    int result1 = TIFFWriteDirectory(tif);
    
    // 2. TIFFSetDirectory
    int result2 = TIFFSetDirectory(tif, 0);
    
    // 3. TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    
    // 4. TIFFSetDirectory (again, try different indices)
    int result3 = TIFFSetDirectory(tif, 0);
    if (Size > 0) {
        // Use fuzzer data to determine directory index
        tdir_t dir_index = (tdir_t)(Data[0] % 10);
        TIFFSetDirectory(tif, dir_index);
    }
    
    // 5. TIFFForceStrileArrayWriting
    int result4 = TIFFForceStrileArrayWriting(tif);
    
    // 6. TIFFFlushData
    int result5 = TIFFFlushData(tif);
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    // Use results to avoid compiler optimizations
    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)offset;
    
    return 0;
}
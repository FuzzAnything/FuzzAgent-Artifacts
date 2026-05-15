// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Clean up and return if file can't be opened as TIFF
        remove("./dummy_file");
        return 0;
    }
    
    // Open a second TIFF handle for testing multiple handles
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    
    // First TIFFSetDirectory call (try directory 0)
    TIFFSetDirectory(tif, 0);
    
    // Second TIFFSetDirectory call (try directory 1)
    TIFFSetDirectory(tif, 1);
    
    // TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset; // Use offset to avoid unused variable warning
    
    // TIFFGetSeekProc
    TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
    (void)seek_proc;
    
    // TIFFIsByteSwapped
    int swapped = TIFFIsByteSwapped(tif);
    (void)swapped;
    
    // TIFFSwabLong - use a dummy value
    uint32_t dummy_long = 0x12345678;
    TIFFSwabLong(&dummy_long);
    
    // TIFFGetWriteProc
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    (void)write_proc;
    
    // TIFFNumberOfDirectories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    // Third TIFFSetDirectory call (try last directory if exists)
    if (num_dirs > 0) {
        TIFFSetDirectory(tif, num_dirs - 1);
    } else {
        TIFFSetDirectory(tif, 0);
    }
    
    // Fourth TIFFSetDirectory call (try directory 0 again)
    TIFFSetDirectory(tif, 0);
    
    // First TIFFClose
    TIFFClose(tif);
    
    // Second TIFFClose (close second handle if opened)
    if (tif2) {
        TIFFClose(tif2);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}
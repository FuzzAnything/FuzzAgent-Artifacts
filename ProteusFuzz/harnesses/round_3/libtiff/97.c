// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for the fuzzer to work with
    const char *filename = "./dummy_file";
    
    // Write the fuzzing data to the file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the TIFF file for reading and writing
    TIFF *tif = TIFFOpen(filename, "r+");
    if (!tif) {
        // Try opening for writing if read+write fails (e.g., corrupt header)
        tif = TIFFOpen(filename, "w");
    }
    
    if (!tif) {
        return 0;
    }

    // Set error handlers to prevent crashes from printing
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warn_handler);

    // Fuzzing logic: Try various directory operations
    
    // 1. Try creating a standard directory
    TIFFCreateDirectory(tif);

    // 2. Try setting a directory index (fuzz the index)
    if (Size > 0) {
        tdir_t dir_index = Data[0];
        TIFFSetDirectory(tif, dir_index);
    }

    // 3. Try creating an EXIF directory
    TIFFCreateEXIFDirectory(tif);

    // 4. Try creating a custom directory
    // We skip this to avoid passing invalid pointers as per requirements.

    // 5. Try writing the directory
    TIFFWriteDirectory(tif);

    // 6. Try freeing the directory
    TIFFFreeDirectory(tif);

    // 7. Try creating a directory again after free
    TIFFCreateDirectory(tif);

    // 8. Try setting directory again with bounds checking
    if (Size > 2) {
        tdir_t dir_index2 = (tdir_t)(Data[1] | (Data[2] << 8));
        TIFFSetDirectory(tif, dir_index2);
    }

    // 9. Try writing again
    TIFFWriteDirectory(tif);

    // Cleanup
    TIFFClose(tif);

    // Remove the dummy file
    remove(filename);

    return 0;
}
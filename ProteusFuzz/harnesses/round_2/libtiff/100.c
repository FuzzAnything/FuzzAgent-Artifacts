// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    TIFF *tif = NULL;
    int ret;
    tdir_t dir_index;

    // Write input data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for writing to test directory creation APIs
    tif = TIFFOpen(filename, "w8");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes during fuzzing
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler);

    // Test 2: TIFFCreateEXIFDirectory
    ret = TIFFCreateEXIFDirectory(tif);
    (void)ret; // Ignore return for fuzzing stability

    // Test 3: TIFFCreateGPSDirectory
    ret = TIFFCreateGPSDirectory(tif);
    (void)ret;

    // Test 4: TIFFWriteDirectory
    // Try to write the directory. This might fail if no tags are set, but that's expected.
    ret = TIFFWriteDirectory(tif);
    (void)ret;

    // Test 5: TIFFSetDirectory
    // Use a value derived from input data to explore different directory indices
    if (Size > 0) {
        dir_index = Data[0];
        ret = TIFFSetDirectory(tif, dir_index);
        (void)ret;
    }
    
    // Try setting directory to 0 explicitly
    ret = TIFFSetDirectory(tif, 0);
    (void)ret;

    // Test 6: TIFFFreeDirectory
    // Free the directory resources
    TIFFFreeDirectory(tif);

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    // Remove the dummy file
    unlink(filename);

    return 0;
}
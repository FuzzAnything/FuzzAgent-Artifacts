// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to file to simulate a TIFF file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for reading/writing
    TIFF *tif = TIFFOpen(filename, "rw+");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes during fuzzing
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Fuzz TIFFCreateDirectory
    if (Size > 0 && Data[0] % 2 == 0) {
        TIFFCreateDirectory(tif);
    }

    // Fuzz TIFFFreeDirectory
    if (Size > 1 && Data[1] % 3 == 0) {
        TIFFFreeDirectory(tif);
    }

    // Fuzz TIFFWriteDirectory
    if (Size > 2 && Data[2] % 5 == 0) {
        TIFFWriteDirectory(tif);
    }

    // Fuzz TIFFRewriteDirectory
    if (Size > 3 && Data[3] % 7 == 0) {
        TIFFRewriteDirectory(tif);
    }

    // Fuzz TIFFCreateEXIFDirectory
    if (Size > 4 && Data[4] % 11 == 0) {
        TIFFCreateEXIFDirectory(tif);
    }

    // Cleanup
    TIFFClose(tif);

    // Remove the dummy file
    unlink(filename);

    return 0;
}
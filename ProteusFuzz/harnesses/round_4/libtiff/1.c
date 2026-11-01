// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static int dummy_error_handler(TIFF *tif, void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_warn_handler(TIFF *tif, void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to open
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove(filename);
        return 0;
    }

    // Set custom error and warning handlers
    TIFFOpenOptionsSetErrorHandlerExtR(opts, dummy_error_handler, NULL);
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warn_handler, NULL);

    // Attempt to open the file
    TIFF *tif = TIFFOpenExt(filename, "r", opts);

    if (tif) {
        // Trigger error and warning handlers
        TIFFErrorExtR(tif, "FuzzerTest", "Simulated error: %s", "test");
        TIFFWarningExtR(tif, "FuzzerTest", "Simulated warning: %s", "test");

        // Close the TIFF file
        TIFFClose(tif);
    }

    // Free the options structure
    TIFFOpenOptionsFree(opts);

    // Clean up the dummy file
    remove(filename);

    return 0;
}
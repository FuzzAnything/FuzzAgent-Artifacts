// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static int dummy_error_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_warn_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static void setup_handlers(void) {
    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warning_handler_ext);
    TIFFSetWarningHandler((TIFFErrorHandler)dummy_warning_handler);
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler_ext);
    TIFFSetErrorHandler((TIFFErrorHandler)dummy_error_handler);
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    setup_handlers();

    const char *module = "FuzzModule";
    char *fmt = NULL;
    char *arg1 = NULL;
    char *arg2 = NULL;

    // Create safe format strings and arguments from input
    if (Size >= 8) {
        size_t fmt_len = Data[0] % 20;
        if (fmt_len > 0 && fmt_len < Size - 2) {
            fmt = malloc(fmt_len + 1);
            if (fmt) {
                memcpy(fmt, Data + 1, fmt_len);
                fmt[fmt_len] = '\0';
                // Ensure format string is safe (no %s, %d, etc. that could cause issues)
                // Replace any % with %%
                for (size_t i = 0; i < fmt_len; i++) {
                    if (fmt[i] == '%') {
                        memmove(fmt + i + 1, fmt + i, fmt_len - i + 1);
                        fmt[i] = '%';
                        fmt_len++;
                    }
                }
            }
        }
    }
    if (!fmt) {
        fmt = "%s";
    }

    if (Size >= 16) {
        size_t arg1_len = Data[1] % 20;
        if (arg1_len > 0 && arg1_len < Size - 4) {
            arg1 = malloc(arg1_len + 1);
            if (arg1) {
                memcpy(arg1, Data + 2, arg1_len);
                arg1[arg1_len] = '\0';
            }
        }
    }
    if (!arg1) {
        arg1 = "test";
    }

    if (Size >= 24) {
        size_t arg2_len = Data[2] % 20;
        if (arg2_len > 0 && arg2_len < Size - 6) {
            arg2 = malloc(arg2_len + 1);
            if (arg2) {
                memcpy(arg2, Data + 3, arg2_len);
                arg2[arg2_len] = '\0';
            }
        }
    }
    if (!arg2) {
        arg2 = "data";
    }

    // Test TIFFWarning
    TIFFWarning(module, fmt, arg1);

    // Test TIFFWarningExt
    thandle_t dummy_handle = (thandle_t)0x12345678;
    TIFFWarningExt(dummy_handle, module, fmt, arg1);

    // Test TIFFError
    TIFFError(module, fmt, arg1);

    // Test TIFFErrorExt
    TIFFErrorExt(dummy_handle, module, fmt, arg1);

    // Create a dummy file for TIFF*ExtR tests
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write minimal TIFF header (4 bytes)
        uint8_t header[4] = {0x49, 0x49, 0x2A, 0x00}; // Little-endian TIFF
        fwrite(header, 1, 4, f);
        fclose(f);

        // Open TIFF file
        TIFF *tif = TIFFOpen("./dummy_file", "r");
        if (tif) {
            // Test TIFFWarningExtR
            TIFFWarningExtR(tif, module, fmt, arg1);

            // Test TIFFErrorExtR
            TIFFErrorExtR(tif, module, fmt, arg1);

            // Set custom handlers on the TIFF instance using the standard API
            // Note: TIFFSetWarningExtR and TIFFSetErrorExtR are internal or not exposed in all versions.
            // We rely on the global handlers or the fact that TIFFWarningExtR/TIFFErrorExtR
            // check the tif_warnhandler/tif_errorhandler fields if set via internal mechanisms.
            // Since we cannot call the non-existent setters, we just invoke the functions again
            // to exercise the code path with the open TIFF handle.
            
            // Invoke again to test different states
            TIFFWarningExtR(tif, module, fmt, arg2);
            TIFFErrorExtR(tif, module, fmt, arg2);

            TIFFClose(tif);
        }
    }

    // Cleanup
    if (fmt != "%s") {
        free((void *)fmt);
    }
    if (arg1 != "test") {
        free((void *)arg1);
    }
    if (arg2 != "data") {
        free((void *)arg2);
    }

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}
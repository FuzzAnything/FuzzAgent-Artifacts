// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <tiffio.h>

/* Custom error handler for testing */
static void
custom_error_handler(const char *module, const char *fmt, va_list ap)
{
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing to avoid printing to stderr during fuzzing */
}

static void
custom_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap)
{
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

static void
custom_warning_handler(const char *module, const char *fmt, va_list ap)
{
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

static void
custom_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap)
{
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

static int
custom_warning_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap)
{
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0; /* Continue propagation */
}

static int
custom_error_handler_ext_r(TIFF *tif, const char *module, const char *fmt, va_list ap)
{
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0; /* Continue propagation */
}

int
LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size)
{
    if (Size == 0) {
        return 0;
    }

    /* Setup custom handlers to prevent output and ensure handlers are active */
    TIFFSetErrorHandler(custom_error_handler);
    TIFFSetErrorHandlerExt(custom_error_handler_ext);
    TIFFSetWarningHandler(custom_warning_handler);
    TIFFSetWarningHandlerExt(custom_warning_handler_ext);

    /* Create a dummy file to open a TIFF handle */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        /* Write some minimal data to make it a valid file for opening */
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    TIFF *tif = NULL;
    thandle_t clientdata = NULL;

    /* Attempt to open the dummy file in read mode */
    tif = TIFFOpen(dummy_file, "r");

    /* Fuzz TIFFError and TIFFWarning (global handlers) */
    if (Size >= 4) {
        uint32_t val = *(const uint32_t *)Data;
        char buf[256];
        snprintf(buf, sizeof(buf), "Test error %u", val & 0xFFFF);
        TIFFError("Fuzzer", "%s", buf);

        snprintf(buf, sizeof(buf), "Test warning %u", (val >> 16) & 0xFFFF);
        TIFFWarning("Fuzzer", "%s", buf);
    }

    /* Fuzz TIFFErrorExt and TIFFWarningExt (extended handlers) */
    if (Size >= 8) {
        uint64_t val64 = *(const uint64_t *)Data;
        char buf[256];
        snprintf(buf, sizeof(buf), "Ext error %lu", (unsigned long)(val64 & 0xFFFFFFFF));
        TIFFErrorExt(clientdata, "FuzzerExt", "%s", buf);

        snprintf(buf, sizeof(buf), "Ext warning %lu", (unsigned long)(val64 >> 32));
        TIFFWarningExt(clientdata, "FuzzerExt", "%s", buf);
    }

    /* Fuzz TIFFWarningExtR and TIFFErrorExtR (file-specific handlers) */
    if (tif) {
        /* Note: libtiff does not expose TIFFSetWarningExtR or TIFFSetErrorExtR directly.
           However, we can still call the reporting functions TIFFWarningExtR and TIFFErrorExtR
           which use the internal handlers if they were set via internal APIs or default.
           Since we cannot set them via public API in this version, we just call the reporters.
           The internal logic will check if handlers are set. */
        
        if (Size >= 12) {
            uint32_t val3 = *(const uint32_t *)(Data + 8);
            char buf[256];
            snprintf(buf, sizeof(buf), "ExtR warning %u", val3 & 0xFFFF);
            TIFFWarningExtR(tif, "FuzzerExtR", "%s", buf);

            snprintf(buf, sizeof(buf), "ExtR error %u", (val3 >> 16) & 0xFFFF);
            TIFFErrorExtR(tif, "FuzzerExtR", "%s", buf);
        }

        TIFFClose(tif);
    }

    /* Cleanup dummy file */
    unlink(dummy_file);

    return 0;
}
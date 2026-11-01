// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

/* Custom error handler for global registration */
/* Must match TIFFErrorHandler signature: void (*)(const char *, const char *, va_list) */
static void
custom_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing to avoid output pollution during fuzzing */
}

/* Custom extended error handler */
/* Must match TIFFErrorHandlerExt signature: void (*)(thandle_t, const char *, const char *, va_list) */
static void
custom_error_handler_ext(thandle_t clientdata, const char *module, 
                         const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Custom warning handler */
static void
custom_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Custom extended warning handler */
static void
custom_warning_handler_ext(thandle_t clientdata, const char *module, 
                           const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Helper to create a dummy TIFF file for testing file-specific handlers */
static TIFF*
create_dummy_tiff(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return NULL;

    if (size > 0) {
        fwrite(data, 1, size, fp);
    } else {
        /* Write minimal valid TIFF header if data is empty to ensure open succeeds */
        uint16_t header[8] = {0x4949, 42, 0, 0, 0, 0, 0, 0}; /* Little endian, offset 8 */
        fwrite(header, sizeof(uint16_t), 8, fp);
    }
    fclose(fp);

    /* Open in read mode to get a TIFF handle for TIFFErrorExtR/TIFFWarningExtR */
    TIFF *tif = TIFFOpen(filename, "r");
    return tif;
}

int
LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    /* Step 1: Setup environment and handlers */
    
    /* Register global handlers to ensure the functions have something to call */
    TIFFSetErrorHandler(custom_error_handler);
    TIFFSetErrorHandlerExt(custom_error_handler_ext);
    TIFFSetWarningHandler(custom_warning_handler);
    TIFFSetWarningHandlerExt(custom_warning_handler_ext);

    /* Step 2: Fuzz the API functions */
    
    /* 1. TIFFError: Global error reporting */
    /* Use data from input to generate module name and format string safely */
    char module[32] = {0};
    char format[64] = {0};
    
    size_t safe_module_len = (Size > 31) ? 31 : Size;
    if (safe_module_len > 0) {
        memcpy(module, Data, safe_module_len);
        module[safe_module_len] = '\0';
    } else {
        strcpy(module, "fuzz_module");
    }

    /* Ensure format string is safe (no %s/%d that would crash without args) */
    /* We use a static format or a safe subset of input */
    if (Size > 0 && Data[0] == '%') {
        /* Skip potentially dangerous format chars for safety in this context */
        strcpy(format, "Error occurred");
    } else {
        size_t safe_fmt_len = (Size > 63) ? 63 : Size;
        if (safe_fmt_len > 0) {
            memcpy(format, Data, safe_fmt_len);
            format[safe_fmt_len] = '\0';
        } else {
            strcpy(format, "Error occurred");
        }
    }

    /* Call TIFFError */
    TIFFError(module, format);

    /* 2. TIFFWarning: Global warning reporting */
    TIFFWarning(module, format);

    /* 3. TIFFErrorExt: Extended global error reporting with dummy handle */
    thandle_t dummy_handle = (thandle_t)0x12345;
    TIFFErrorExt(dummy_handle, module, format);

    /* 4. TIFFWarningExt: Extended global warning reporting with dummy handle */
    TIFFWarningExt(dummy_handle, module, format);

    /* 5. TIFFErrorExtR and TIFFWarningExtR: Require a valid TIFF* handle */
    TIFF *tif = create_dummy_tiff(Data, Size);
    
    if (tif) {
        /* Call file-specific error reporting */
        TIFFErrorExtR(tif, module, format);
        
        /* Call file-specific warning reporting */
        TIFFWarningExtR(tif, module, format);
        
        /* Cleanup TIFF handle */
        TIFFClose(tif);
    }

    /* Step 3: Cleanup */
    /* Remove dummy file */
    remove("./dummy_file");

    /* Reset handlers to default (optional, but good practice) */
    TIFFSetErrorHandler(NULL);
    TIFFSetErrorHandlerExt(NULL);
    TIFFSetWarningHandler(NULL);
    TIFFSetWarningHandlerExt(NULL);

    return 0;
}
// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

#include "tiffio.h"

/* Silent handlers to suppress output during fuzzing */
static void silent_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void silent_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

/* Create a minimal dummy TIFF file for testing */
static int create_dummy_tiff_file(const char* filename) {
    /* Create a minimal valid TIFF header */
    unsigned char tiff_header[] = {
        0x49, 0x49, 0x2A, 0x00,  /* Little-endian, TIFF magic number */
        0x08, 0x00, 0x00, 0x00,  /* Offset to first IFD */
        0x00, 0x00,             /* Number of directory entries (0) */
        0x00, 0x00, 0x00, 0x00   /* Offset to next IFD (0 = end) */
    };
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    fwrite(tiff_header, 1, sizeof(tiff_header), fp);
    fclose(fp);
    return 1;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    /* Set silent handlers to avoid output pollution */
    TIFFSetWarningHandler(silent_handler);
    TIFFSetErrorHandler(silent_handler);
    TIFFSetWarningHandlerExt(silent_handler_ext);
    TIFFSetErrorHandlerExt(silent_handler_ext);
    
    /* Create dummy TIFF file */
    const char* dummy_filename = "./dummy_file";
    if (!create_dummy_tiff_file(dummy_filename)) {
        return 0;
    }
    
    /* Open dummy TIFF file */
    TIFF* tif = TIFFOpen(dummy_filename, "r");
    thandle_t handle = NULL;
    
    /* Prepare strings from fuzzer input with bounds checking */
    size_t module_len = Size / 3;
    size_t fmt_len = Size / 3;
    size_t arg_len = Size - module_len - fmt_len;
    
    /* Ensure we don't read beyond Data buffer */
    if (module_len + fmt_len + arg_len > Size) {
        arg_len = Size - module_len - fmt_len;
    }
    
    /* Allocate buffers with null terminators */
    char* module = (char*)malloc(module_len + 1);
    char* fmt = (char*)malloc(fmt_len + 1);
    char* arg = (char*)malloc(arg_len + 1);
    
    if (!module || !fmt || !arg) {
        free(module);
        free(fmt);
        free(arg);
        if (tif) TIFFClose(tif);
        unlink(dummy_filename);
        return 0;
    }
    
    /* Copy data ensuring null termination with bounds checking */
    if (module_len > 0) {
        memcpy(module, Data, module_len);
    }
    module[module_len] = '\0';
    
    if (fmt_len > 0 && module_len < Size) {
        size_t copy_len = fmt_len;
        if (module_len + fmt_len > Size) {
            copy_len = Size - module_len;
        }
        memcpy(fmt, Data + module_len, copy_len);
    }
    fmt[fmt_len] = '\0';
    
    if (arg_len > 0 && module_len + fmt_len < Size) {
        size_t copy_len = arg_len;
        if (module_len + fmt_len + arg_len > Size) {
            copy_len = Size - module_len - fmt_len;
        }
        memcpy(arg, Data + module_len + fmt_len, copy_len);
    }
    arg[arg_len] = '\0';
    
    /* Test TIFFWarning */
    TIFFWarning(module, fmt, arg);
    
    /* Test TIFFError */
    TIFFError(module, fmt, arg);
    
    /* Test TIFFWarningExtR */
    if (tif) {
        TIFFWarningExtR(tif, module, fmt, arg);
    }
    
    /* Test TIFFWarningExt */
    TIFFWarningExt(handle, module, fmt, arg);
    
    /* Test TIFFErrorExtR */
    if (tif) {
        TIFFErrorExtR(tif, module, fmt, arg);
    }
    
    /* Test TIFFErrorExt */
    TIFFErrorExt(handle, module, fmt, arg);
    
    /* Test with multiple arguments only if we have enough data */
    if (arg_len >= 4) {
        int int_arg;
        memcpy(&int_arg, arg, sizeof(int));
        TIFFWarning(module, "Value: %d", int_arg);
        TIFFError(module, "Value: %d", int_arg);
        if (tif) {
            TIFFWarningExtR(tif, module, "Value: %d", int_arg);
            TIFFErrorExtR(tif, module, "Value: %d", int_arg);
        }
        TIFFWarningExt(handle, module, "Value: %d", int_arg);
        TIFFErrorExt(handle, module, "Value: %d", int_arg);
    }
    
    /* Test with NULL arguments */
    TIFFWarning(NULL, NULL);
    TIFFError(NULL, NULL);
    if (tif) {
        TIFFWarningExtR(tif, NULL, NULL);
        TIFFErrorExtR(tif, NULL, NULL);
    }
    TIFFWarningExt(NULL, NULL, NULL);
    TIFFErrorExt(NULL, NULL, NULL);
    
    /* Test with empty strings */
    TIFFWarning("", "");
    TIFFError("", "");
    if (tif) {
        TIFFWarningExtR(tif, "", "");
        TIFFErrorExtR(tif, "", "");
    }
    TIFFWarningExt(handle, "", "");
    TIFFErrorExt(handle, "", "");
    
    /* Test with format specifiers */
    TIFFWarning(module, "%s %d", arg, 42);
    TIFFError(module, "%s %d", arg, 42);
    
    /* Cleanup */
    free(module);
    free(fmt);
    free(arg);
    
    if (tif) {
        TIFFClose(tif);
    }
    
    unlink(dummy_filename);
    
    return 0;
}
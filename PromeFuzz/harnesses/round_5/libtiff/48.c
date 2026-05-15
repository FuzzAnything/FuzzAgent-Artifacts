// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "tiffio.h"

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for TIFF operations if needed
    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);

    // Set up dummy handlers to avoid console output
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);

    // Create a dummy TIFF structure
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    
    // Use the first byte to decide which functions to call
    uint8_t selector = Data[0] % 6;
    
    // Prepare module and message strings from input data
    const char* module = "fuzz_module";
    const char* message = "fuzz_message";
    
    if (Size > 10) {
        // Use a portion of input for module and message if we have enough data
        size_t module_len = (Data[1] % (Size - 2)) + 1;
        if (module_len > Size - 2) {
            module_len = Size - 2;
        }
        size_t msg_len = Size - 2 - module_len;
        
        if (module_len > 0 && msg_len > 0) {
            char* mod_buf = (char*)malloc(module_len + 1);
            char* msg_buf = (char*)malloc(msg_len + 1);
            
            if (mod_buf && msg_buf) {
                memcpy(mod_buf, Data + 2, module_len);
                memcpy(msg_buf, Data + 2 + module_len, msg_len);
                mod_buf[module_len] = '\0';
                msg_buf[msg_len] = '\0';
                
                module = mod_buf;
                message = msg_buf;
                
                // Call the selected function
                switch (selector) {
                    case 0:
                        TIFFWarningExt((thandle_t)tif, module, "%s", message);
                        break;
                    case 1:
                        TIFFError(module, "%s", message);
                        break;
                    case 2:
                        if (tif) {
                            TIFFWarningExtR(tif, module, "%s", message);
                        }
                        break;
                    case 3:
                        if (tif) {
                            TIFFErrorExtR(tif, module, "%s", message);
                        }
                        break;
                    case 4:
                        TIFFWarning(module, "%s", message);
                        break;
                    case 5:
                        TIFFErrorExt((thandle_t)tif, module, "%s", message);
                        break;
                }
                
                free(mod_buf);
                free(msg_buf);
            }
        }
    } else {
        // Minimal test with default strings
        switch (selector) {
            case 0:
                TIFFWarningExt((thandle_t)tif, module, "%s", message);
                break;
            case 1:
                TIFFError(module, "%s", message);
                break;
            case 2:
                if (tif) {
                    TIFFWarningExtR(tif, module, "%s", message);
                }
                break;
            case 3:
                if (tif) {
                    TIFFErrorExtR(tif, module, "%s", message);
                }
                break;
            case 4:
                TIFFWarning(module, "%s", message);
                break;
            case 5:
                TIFFErrorExt((thandle_t)tif, module, "%s", message);
                break;
        }
    }

    // Clean up
    if (tif) {
        TIFFClose(tif);
    }
    remove("./dummy_file");
    
    return 0;
}
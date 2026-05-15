// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

#define MAX_BUF_SIZE 4096
#define MAX_INPUT_SIZE 1024

/* Dummy table resolver for lou_registerTableResolver */
static char **dummy_resolver(const char *table, const char *base) {
    static char *paths[] = {"./dummy_file", NULL};
    return paths;
}

/* Helper to convert input bytes to widechar array with size limit */
static widechar *bytes_to_widechar(const uint8_t *data, size_t size, int *out_len) {
    if (size == 0) {
        *out_len = 0;
        return NULL;
    }
    
    /* Limit input size to avoid excessive allocations */
    int max_chars = (size > MAX_INPUT_SIZE) ? MAX_INPUT_SIZE : (int)size;
    widechar *buf = malloc(max_chars * sizeof(widechar));
    if (!buf) {
        *out_len = 0;
        return NULL;
    }
    
    *out_len = max_chars;
    for (int i = 0; i < max_chars; i++) {
        buf[i] = (widechar)data[i];
    }
    return buf;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Use a fixed table for fuzzing */
    const char *table_list = "en-us-g2.ctb";

    /* Prepare dummy file for table resolution if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        size_t write_size = (Size > MAX_INPUT_SIZE) ? MAX_INPUT_SIZE : Size;
        if (write_size > 0) {
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }

    /* Determine which function to test based on first byte */
    int function_selector = Data[0] % 6;
    const uint8_t *func_data = Data + 1;
    size_t func_size = (Size > 1) ? Size - 1 : 0;

    switch (function_selector) {
        case 0: { /* lou_translateString */
            int inlen = 0;
            widechar *inbuf = bytes_to_widechar(func_data, func_size, &inlen);
            if (!inbuf) break;
            
            int outlen = MAX_BUF_SIZE;
            widechar *outbuf = malloc(outlen * sizeof(widechar));
            
            /* Allocate typeform and spacing with safe sizes based on output buffer */
            formtype *typeform = malloc(outlen * sizeof(formtype));
            char *spacing = malloc(outlen * sizeof(char));
            
            if (outbuf && typeform && spacing) {
                lou_translateString(table_list, inbuf, &inlen, outbuf, &outlen,
                                    typeform, spacing, 0);
            }
            
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
        case 1: { /* lou_registerTableResolver */
            lou_registerTableResolver(dummy_resolver);
            break;
        }
        case 2: { /* lou_backTranslate */
            int inlen = 0;
            widechar *inbuf = bytes_to_widechar(func_data, func_size, &inlen);
            if (!inbuf) break;
            
            int outlen = MAX_BUF_SIZE;
            widechar *outbuf = malloc(outlen * sizeof(widechar));
            
            /* Allocate buffers based on output size, not input size */
            formtype *typeform = malloc(outlen * sizeof(formtype));
            char *spacing = malloc(outlen * sizeof(char));
            int *outputPos = malloc(inlen * sizeof(int));
            int *inputPos = malloc(outlen * sizeof(int));
            int cursorPos = 0;
            
            if (outbuf && typeform && spacing && outputPos && inputPos) {
                lou_backTranslate(table_list, inbuf, &inlen, outbuf, &outlen,
                                  typeform, spacing, outputPos, inputPos, &cursorPos, 0);
            }
            
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            free(outputPos);
            free(inputPos);
            break;
        }
        case 3: { /* lou_getTypeformForEmphClass */
            /* Use up to 64 bytes for emphasis class name */
            size_t emph_len = (func_size > 64) ? 64 : func_size;
            char *emph_class = malloc(emph_len + 1);
            if (emph_class) {
                memcpy(emph_class, func_data, emph_len);
                emph_class[emph_len] = '\0';
                formtype result = lou_getTypeformForEmphClass(table_list, emph_class);
                (void)result;
                free(emph_class);
            }
            break;
        }
        case 4: { /* lou_compileString */
            /* Ensure table is loaded first by attempting translation */
            int dummy_inlen = 4, dummy_outlen = MAX_BUF_SIZE;
            widechar dummy_inbuf[] = {'t','e','s','t'};
            widechar dummy_outbuf[MAX_BUF_SIZE];
            formtype dummy_typeform[MAX_BUF_SIZE];
            char dummy_spacing[MAX_BUF_SIZE];
            
            lou_translateString(table_list, dummy_inbuf, &dummy_inlen,
                                dummy_outbuf, &dummy_outlen,
                                dummy_typeform, dummy_spacing, 0);
            
            /* Now compile a string from fuzzer input */
            size_t compile_len = (func_size > 128) ? 128 : func_size;
            char *compile_str = malloc(compile_len + 1);
            if (compile_str) {
                memcpy(compile_str, func_data, compile_len);
                compile_str[compile_len] = '\0';
                int result = lou_compileString(table_list, compile_str);
                (void)result;
                free(compile_str);
            }
            break;
        }
        case 5: { /* lou_backTranslateString */
            int inlen = 0;
            widechar *inbuf = bytes_to_widechar(func_data, func_size, &inlen);
            if (!inbuf) break;
            
            int outlen = MAX_BUF_SIZE;
            widechar *outbuf = malloc(outlen * sizeof(widechar));
            
            /* Allocate typeform and spacing based on output buffer size */
            formtype *typeform = malloc(outlen * sizeof(formtype));
            char *spacing = malloc(outlen * sizeof(char));
            
            if (outbuf && typeform && spacing) {
                lou_backTranslateString(table_list, inbuf, &inlen, outbuf, &outlen,
                                        typeform, spacing, 0);
            }
            
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
    }

    /* Clean up dummy file */
    remove("./dummy_file");
    return 0;
}
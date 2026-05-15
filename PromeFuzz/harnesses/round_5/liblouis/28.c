// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Write dummy table file
    write_dummy_file(Data, Size % (Size/2 + 1));
    
    // Load a table first for safety
    const char *tableList = lou_getTable("./dummy_file");
    if (!tableList) {
        // If table loading fails, use a default name
        tableList = "en-us-g2.ctb";
    }

    // Extract function selector from first byte
    uint8_t func_selector = Data[0] % 6;
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;

    // Common variables for translation functions
    int inlen, outlen;
    widechar *inbuf = NULL;
    widechar *outbuf = NULL;
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    int mode = 0;

    // Set up buffers based on remaining data
    if (func_size > 0) {
        // For inbuf, use half of remaining data or at least 1 widechar
        inlen = (func_size / 2) / sizeof(widechar);
        if (inlen < 1) inlen = 1;
        if (inlen > 1024) inlen = 1024;
        
        inbuf = (widechar *)malloc(inlen * sizeof(widechar));
        if (inbuf) {
            size_t copy_bytes = (func_size / 2) > (inlen * sizeof(widechar)) ? 
                               (inlen * sizeof(widechar)) : (func_size / 2);
            memcpy(inbuf, func_data, copy_bytes);
        }

        // For outbuf, use the other half
        outlen = 2048; // Fixed size for output buffer
        outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        
        // Allocate typeform and spacing buffers
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        if (typeform) {
            memset(typeform, 0, outlen * sizeof(formtype));
        }
        
        spacing = (char *)malloc(outlen * sizeof(char));
        if (spacing) {
            memset(spacing, 0, outlen * sizeof(char));
        }
        
        // Allocate position arrays for lou_translate and lou_backTranslate
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
    }

    switch (func_selector) {
        case 0: // lou_compileString
            if (func_size > 0) {
                char *inString = (char *)malloc(func_size + 1);
                if (inString) {
                    memcpy(inString, func_data, func_size);
                    inString[func_size] = '\0';
                    lou_compileString(tableList, inString);
                    free(inString);
                }
            }
            break;
            
        case 1: // lou_backTranslateString
            if (inbuf && outbuf) {
                int local_inlen = inlen;
                int local_outlen = outlen;
                lou_backTranslateString(tableList, inbuf, &local_inlen, 
                                      outbuf, &local_outlen, typeform, spacing, mode);
            }
            break;
            
        case 2: // lou_translateString
            if (inbuf && outbuf) {
                int local_inlen = inlen;
                int local_outlen = outlen;
                lou_translateString(tableList, inbuf, &local_inlen, 
                                  outbuf, &local_outlen, typeform, spacing, mode);
            }
            break;
            
        case 3: // lou_translate
            if (inbuf && outbuf && outputPos && inputPos) {
                int local_inlen = inlen;
                int local_outlen = outlen;
                lou_translate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                            typeform, spacing, outputPos, inputPos, &cursorPos, mode);
            }
            break;
            
        case 4: // lou_getTypeformForEmphClass
            if (func_size > 0) {
                char *emphClass = (char *)malloc(func_size + 1);
                if (emphClass) {
                    memcpy(emphClass, func_data, func_size);
                    emphClass[func_size] = '\0';
                    lou_getTypeformForEmphClass(tableList, emphClass);
                    free(emphClass);
                }
            }
            break;
            
        case 5: // lou_backTranslate
            if (inbuf && outbuf && outputPos && inputPos) {
                int local_inlen = inlen;
                int local_outlen = outlen;
                lou_backTranslate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                                typeform, spacing, outputPos, inputPos, &cursorPos, mode);
            }
            break;
    }

    // Cleanup
    if (inbuf) free(inbuf);
    if (outbuf) free(outbuf);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);
    
    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}
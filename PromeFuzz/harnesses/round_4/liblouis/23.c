// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
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

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Prepare a dummy table file
    write_dummy_file(Data, Size);
    
    // Load a table using the dummy file
    const char *tableList = "./dummy_file";
    void *table = lou_getTable(tableList);
    
    // Only proceed if table loaded successfully
    if (table != NULL) {
        // Split input for different parameters
        size_t half = Size / 2;
        const uint8_t *first_half = Data;
        const uint8_t *second_half = Data + half;
        size_t first_size = half;
        size_t second_size = Size - half;
        
        // 1. Test lou_compileString
        if (first_size > 0 && second_size > 0) {
            char *tableStr = (char *)malloc(first_size + 1);
            char *inStr = (char *)malloc(second_size + 1);
            if (tableStr && inStr) {
                memcpy(tableStr, first_half, first_size);
                memcpy(inStr, second_half, second_size);
                tableStr[first_size] = '\0';
                inStr[second_size] = '\0';
                lou_compileString(tableList, inStr);
                free(tableStr);
                free(inStr);
            }
        }
        
        // Prepare parameters for translation functions
        int inlen = (first_size > 0) ? (first_size / sizeof(widechar)) : 10;
        if (inlen < 1) inlen = 1;
        int outlen = inlen * 2;
        
        widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
        widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
        char *spacing = (char *)malloc(outlen * sizeof(char));
        int *outputPos = (int *)malloc(inlen * sizeof(int));
        int *inputPos = (int *)malloc(outlen * sizeof(int));
        int cursorPos = 0;
        
        if (inbuf && outbuf) {
            // Initialize buffers
            for (int i = 0; i < inlen && i < (int)(first_size / sizeof(widechar)); i++) {
                inbuf[i] = ((widechar *)first_half)[i];
            }
            for (int i = 0; i < outlen; i++) {
                outbuf[i] = 0;
            }
            
            // Initialize optional parameters
            if (typeform) {
                memset(typeform, 0, outlen * sizeof(formtype));
            }
            if (spacing) {
                memset(spacing, 0, outlen * sizeof(char));
            }
            if (outputPos) {
                memset(outputPos, 0, inlen * sizeof(int));
            }
            if (inputPos) {
                memset(inputPos, 0, outlen * sizeof(int));
            }
            
            int mode = 0;
            
            // 2. Test lou_translateString
            int inlen_copy = inlen;
            int outlen_copy = outlen;
            lou_translateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy, 
                               typeform, spacing, mode);
            
            // 3. Test lou_backTranslate
            inlen_copy = inlen;
            outlen_copy = outlen;
            lou_backTranslate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                             typeform, spacing, outputPos, inputPos, &cursorPos, mode);
            
            // 4. Test lou_backTranslateString
            inlen_copy = inlen;
            outlen_copy = outlen;
            lou_backTranslateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                                   typeform, spacing, mode);
            
            // 5. Test lou_translate
            inlen_copy = inlen;
            outlen_copy = outlen;
            lou_translate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
            
            // 6. Test lou_getTypeformForEmphClass
            if (second_size > 0) {
                char *emphClass = (char *)malloc(second_size + 1);
                if (emphClass) {
                    memcpy(emphClass, second_half, second_size);
                    emphClass[second_size] = '\0';
                    lou_getTypeformForEmphClass(tableList, emphClass);
                    free(emphClass);
                }
            }
        }
        
        // Free allocated memory
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        
        // Free the table
        lou_free();
    }
    
    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}
// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
    if (size < 2) return 0;

    // Write data to dummy file for potential table loading
    write_dummy_file(data, size);

    // Extract parameters from fuzz data
    size_t offset = 0;
    
    // First byte: function selector (0-5)
    uint8_t func_selector = data[offset++];
    if (func_selector > 5) func_selector = func_selector % 6;
    
    // Next bytes: tableList string (null-terminated)
    size_t table_len = 0;
    while (offset + table_len < size && data[offset + table_len] != 0) {
        table_len++;
    }
    if (offset + table_len >= size) {
        cleanup_dummy_file();
        return 0;
    }
    
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) {
        cleanup_dummy_file();
        return 0;
    }
    memcpy(tableList, data + offset, table_len);
    tableList[table_len] = '\0';
    offset += table_len + 1;
    
    // Remaining bytes: input string/data
    size_t remaining = size - offset;
    if (remaining == 0) {
        free(tableList);
        cleanup_dummy_file();
        return 0;
    }

    // Buffer sizes
    int inlen = (remaining > 1024) ? 1024 : remaining;
    int outlen = inlen * 2;  // Conservative output buffer size
    
    // Allocate buffers
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    char *spacing = (char *)malloc(outlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;
    
    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(tableList);
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        cleanup_dummy_file();
        return 0;
    }
    
    // Initialize buffers
    memset(inbuf, 0, inlen * sizeof(widechar));
    memset(outbuf, 0, outlen * sizeof(widechar));
    memset(typeform, 0, outlen * sizeof(formtype));
    memset(spacing, 0, outlen * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));
    
    // Convert input data to widechar (simple byte-to-widechar mapping)
    for (int i = 0; i < inlen && offset + i < size; i++) {
        inbuf[i] = (widechar)data[offset + i];
    }
    
    // Load table first to ensure valid tableList
    const void *table = lou_getTable(tableList);
    
    // Call selected function
    switch (func_selector) {
        case 0:  // lou_compileString
            if (table != NULL) {  // Only call if table was successfully loaded
                lou_compileString(tableList, (const char *)inbuf);
            }
            break;
            
        case 1:  // lou_translateString
            lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                              typeform, spacing, 0);
            break;
            
        case 2:  // lou_backTranslate
            lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen,
                            typeform, spacing, outputPos, inputPos, &cursorPos, 0);
            break;
            
        case 3:  // lou_backTranslateString
            lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                  typeform, spacing, 0);
            break;
            
        case 4:  // lou_getTable
            // Already called above, but call again for fuzzing
            table = lou_getTable(tableList);
            break;
            
        case 5:  // lou_translate
            lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                         typeform, spacing, outputPos, inputPos, &cursorPos, 0);
            break;
    }
    
    // Cleanup
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    cleanup_dummy_file();
    
    return 0;
}
// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

#define MAX_STR_LEN 100
#define MAX_BUF_LEN 1000

static void init_widechar_buffer(const uint8_t *data, size_t size, widechar *buf, int *len) {
    int max_chars = size / sizeof(widechar);
    if (max_chars > MAX_BUF_LEN) max_chars = MAX_BUF_LEN;
    if (max_chars < 0) max_chars = 0;
    
    *len = max_chars;
    if (max_chars > 0) {
        memcpy(buf, data, max_chars * sizeof(widechar));
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    // Extract strings for tableList, emphClass, and compileString
    char tableList[MAX_STR_LEN + 1] = {0};
    char emphClass[MAX_STR_LEN + 1] = {0};
    char compileString[MAX_STR_LEN + 1] = {0};
    
    size_t offset = 0;
    
    // Extract tableList
    size_t tableList_len = 0;
    while (offset < Size && tableList_len < MAX_STR_LEN && Data[offset] != 0) {
        tableList[tableList_len++] = Data[offset++];
    }
    tableList[tableList_len] = 0;
    if (offset >= Size) offset = Size - 1;
    offset++; // Skip null terminator
    
    // Extract emphClass
    size_t emphClass_len = 0;
    while (offset < Size && emphClass_len < MAX_STR_LEN && Data[offset] != 0) {
        emphClass[emphClass_len++] = Data[offset++];
    }
    emphClass[emphClass_len] = 0;
    if (offset >= Size) offset = Size - 1;
    offset++; // Skip null terminator
    
    // Extract compileString
    size_t compileString_len = 0;
    while (offset < Size && compileString_len < MAX_STR_LEN && Data[offset] != 0) {
        compileString[compileString_len++] = Data[offset++];
    }
    compileString[compileString_len] = 0;
    if (offset >= Size) offset = Size - 1;
    offset++; // Skip null terminator
    
    // Write table data to dummy file for lou_compileString
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(tableList, 1, tableList_len, fp);
        fclose(fp);
    }
    
    // Load table first to avoid crash in lou_compileString
    void *table = lou_getTable("./dummy_file");
    
    // Test lou_getTypeformForEmphClass
    lou_getTypeformForEmphClass(tableList, emphClass);
    
    // Test lou_compileString (only if table was loaded successfully)
    if (table != NULL) {
        lou_compileString(tableList, compileString);
    }
    
    // Prepare buffers for translation functions
    widechar inbuf[MAX_BUF_LEN];
    widechar outbuf[MAX_BUF_LEN];
    int inlen = 0, outlen = MAX_BUF_LEN;
    
    // Initialize input buffer with remaining data
    if (offset < Size) {
        size_t remaining = Size - offset;
        init_widechar_buffer(Data + offset, remaining, inbuf, &inlen);
    }
    
    // Allocate auxiliary buffers
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    char *spacing = (char *)malloc(outlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;
    
    if (typeform) memset(typeform, 0, outlen * sizeof(formtype));
    if (spacing) memset(spacing, 0, outlen * sizeof(char));
    if (outputPos) memset(outputPos, 0, inlen * sizeof(int));
    if (inputPos) memset(inputPos, 0, outlen * sizeof(int));
    
    int mode = 0;
    if (Size > 0) mode = Data[0] % 4;
    
    // Test lou_backTranslate
    if (inlen > 0 && outlen > 0) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // Test lou_translate
    if (inlen > 0 && outlen > 0) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                     typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // Test lou_backTranslateString
    if (inlen > 0 && outlen > 0) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                               typeform, spacing, mode);
    }
    
    // Test lou_translateString
    if (inlen > 0 && outlen > 0) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                           typeform, spacing, mode);
    }
    
    // Cleanup
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    
    // Free loaded table
    if (table != NULL) {
        lou_free();
    }
    
    return 0;
}
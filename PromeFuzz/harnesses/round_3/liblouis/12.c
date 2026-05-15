// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void write_dummy_table_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (!f) return;
    // Minimal valid liblouis table
    fprintf(f, "# Dummy table for fuzzing\n");
    fprintf(f, "include en-us-comp8.ctb\n");
    fprintf(f, "space \\s\n");
    fprintf(f, "a 1\n");
    fprintf(f, "b 12\n");
    fprintf(f, "c 14\n");
    fprintf(f, "dots 123456\n");
    fprintf(f, "numsign 3456\n");
    fprintf(f, "punctuation \\p\n");
    fclose(f);
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    write_dummy_table_file();
    const char *tableList = "./dummy_file";
    
    // Use first byte to control various options
    uint8_t options = Data[0];
    int use_typeform = options & 1;
    int use_spacing = (options >> 1) & 1;
    int use_positions = (options >> 2) & 1;
    int mode = (options >> 3) & 3;
    
    // Skip first byte for actual data
    const uint8_t *data_ptr = Data + 1;
    size_t data_size = Size - 1;
    
    // 1. Test lou_getTypeformForEmphClass
    if (data_size > 2) {
        size_t split = data_size / 2;
        char *emphClass = (char *)malloc(split + 1);
        if (emphClass) {
            memcpy(emphClass, data_ptr, split);
            emphClass[split] = '\0';
            lou_getTypeformForEmphClass(tableList, emphClass);
            free(emphClass);
        }
    }
    
    // Prepare buffers for translation functions
    size_t max_len = 256;
    if (data_size < max_len) max_len = data_size;
    
    int inlen = (int)(max_len / sizeof(widechar));
    if (inlen < 1) inlen = 1;
    int outlen = inlen * 2; // Output can be larger
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        return 0;
    }
    
    // Fill input buffer with data
    size_t copy_size = data_size < (size_t)(inlen * sizeof(widechar)) ? 
                      data_size : (size_t)(inlen * sizeof(widechar));
    memcpy(inbuf, data_ptr, copy_size);
    
    // Prepare optional buffers
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;
    
    if (use_typeform) {
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        if (typeform) memset(typeform, 0, outlen * sizeof(formtype));
    }
    
    if (use_spacing) {
        spacing = (char *)malloc(outlen * sizeof(char));
        if (spacing) memset(spacing, 0, outlen * sizeof(char));
    }
    
    if (use_positions) {
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
        if (outputPos) memset(outputPos, 0, inlen * sizeof(int));
        if (inputPos) memset(inputPos, 0, outlen * sizeof(int));
        cursorPos = inlen > 0 ? inlen / 2 : 0;
    }
    
    // 2. Test lou_backTranslate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // 3. Test lou_translate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                     typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // 4. Test lou_backTranslateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                               typeform, spacing, mode);
    }
    
    // 5. Test lou_translatePrehyphenated
    if (use_positions) {
        inputHyphens = (char *)malloc(inlen * sizeof(char));
        outputHyphens = (char *)malloc(outlen * sizeof(char));
        if (inputHyphens && outputHyphens) {
            memset(inputHyphens, 0, inlen * sizeof(char));
            memset(outputHyphens, 0, outlen * sizeof(char));
            
            int local_inlen = inlen;
            int local_outlen = outlen;
            lou_translatePrehyphenated(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                                      typeform, spacing, outputPos, inputPos, &cursorPos,
                                      inputHyphens, outputHyphens, mode);
        }
        free(inputHyphens);
        free(outputHyphens);
    }
    
    // 6. Test lou_translateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                           typeform, spacing, mode);
    }
    
    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    
    return 0;
}
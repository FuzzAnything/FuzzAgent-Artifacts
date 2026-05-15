// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void write_dummy_table_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (!f) return;
    // Minimal table that defines some basic mappings
    const char *table_content = 
        "# Minimal table for fuzzing\n"
        "locale en\n"
        "displayName \"Fuzz Table\"\n"
        "\n"
        "# Some basic mappings\n"
        "a 1\n"
        "b 12\n"
        "c 14\n"
        "d 145\n"
        "e 15\n"
        "f 124\n"
        "g 1245\n"
        "h 125\n"
        "i 24\n"
        "j 245\n"
        "\n"
        "# Backward translations\n"
        "1 a\n"
        "12 b\n"
        "14 c\n"
        "145 d\n"
        "15 e\n"
        "124 f\n"
        "1245 g\n"
        "125 h\n"
        "24 i\n"
        "245 j\n";
    fwrite(table_content, 1, strlen(table_content), f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    static int table_written = 0;
    if (!table_written) {
        write_dummy_table_file();
        table_written = 1;
    }

    // Ensure minimum size for basic operations
    if (Size < 2) return 0;

    // Use first byte to determine configuration flags
    uint8_t flags = Data[0];
    int use_typeform = flags & 0x01;
    int use_spacing = flags & 0x02;
    int mode = (flags >> 2) & 0x0F;  // 4 bits for mode

    // Adjust data pointer and size
    const uint8_t *real_data = Data + 1;
    size_t real_size = Size - 1;

    // Convert input to widechar buffer
    size_t max_widechars = real_size / sizeof(widechar);
    if (max_widechars < 1) return 0;
    
    widechar *inbuf = (widechar *)malloc((max_widechars + 1) * sizeof(widechar));
    if (!inbuf) return 0;
    
    // Copy data, ensuring null termination
    size_t copy_bytes = max_widechars * sizeof(widechar);
    memcpy(inbuf, real_data, copy_bytes);
    inbuf[max_widechars] = 0;
    
    int inlen = (int)max_widechars;
    int outlen = inlen * 4;  // Conservative estimate for output buffer
    
    // Allocate output buffer
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    
    // Allocate typeform if requested
    formtype *typeform = NULL;
    if (use_typeform) {
        // Allocate based on output buffer size as per hints
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        if (typeform) {
            memset(typeform, 0, outlen * sizeof(formtype));
        }
    }
    
    // Allocate spacing if requested
    char *spacing = NULL;
    if (use_spacing) {
        // Allocate based on output buffer size as per hints
        spacing = (char *)malloc(outlen * sizeof(char));
        if (spacing) {
            memset(spacing, 0, outlen * sizeof(char));
        }
    }
    
    const char *tableList = "./dummy_file";
    
    // 1. Test lou_getTable
    const void *table = lou_getTable(tableList);
    (void)table;  // Ignore result
    
    // 2. Test lou_translateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        char *local_spacing = NULL;
        
        if (use_spacing) {
            // For translateString, spacing should be at least inlen+1
            local_spacing = (char *)malloc((local_inlen + 1) * sizeof(char));
            if (local_spacing) {
                memset(local_spacing, 0, (local_inlen + 1) * sizeof(char));
            }
        }
        
        lou_translateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                           typeform, local_spacing, mode);
        
        free(local_spacing);
    }
    
    // 3. Test lou_translate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        
        // Allocate position arrays
        int *outputPos = (int *)malloc(local_inlen * sizeof(int));
        int *inputPos = (int *)malloc(local_outlen * sizeof(int));
        int cursorPos = 0;
        
        if (outputPos && inputPos) {
            memset(outputPos, 0, local_inlen * sizeof(int));
            memset(inputPos, 0, local_outlen * sizeof(int));
            
            lou_translate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
        }
        
        free(outputPos);
        free(inputPos);
    }
    
    // 4. Test lou_backTranslate
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        
        int *outputPos = (int *)malloc(local_inlen * sizeof(int));
        int *inputPos = (int *)malloc(local_outlen * sizeof(int));
        int cursorPos = 0;
        
        if (outputPos && inputPos) {
            memset(outputPos, 0, local_inlen * sizeof(int));
            memset(inputPos, 0, local_outlen * sizeof(int));
            
            lou_backTranslate(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                            typeform, spacing, outputPos, inputPos, &cursorPos, mode);
        }
        
        free(outputPos);
        free(inputPos);
    }
    
    // 5. Test lou_backTranslateString
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        
        lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                               typeform, spacing, mode);
    }
    
    // 6. Test lou_translatePrehyphenated
    {
        int local_inlen = inlen;
        int local_outlen = outlen;
        
        int *outputPos = (int *)malloc(local_inlen * sizeof(int));
        int *inputPos = (int *)malloc(local_outlen * sizeof(int));
        int cursorPos = 0;
        char *inputHyphens = NULL;
        char *outputHyphens = NULL;
        
        if (use_spacing) {
            inputHyphens = (char *)malloc(local_inlen * sizeof(char));
            outputHyphens = (char *)malloc(local_outlen * sizeof(char));
            if (inputHyphens && outputHyphens) {
                memset(inputHyphens, 0, local_inlen * sizeof(char));
                memset(outputHyphens, 0, local_outlen * sizeof(char));
            }
        }
        
        if (outputPos && inputPos) {
            memset(outputPos, 0, local_inlen * sizeof(int));
            memset(inputPos, 0, local_outlen * sizeof(int));
            
            lou_translatePrehyphenated(tableList, inbuf, &local_inlen, outbuf, &local_outlen,
                                      typeform, spacing, outputPos, inputPos, &cursorPos,
                                      inputHyphens, outputHyphens, mode);
        }
        
        free(outputPos);
        free(inputPos);
        free(inputHyphens);
        free(outputHyphens);
    }
    
    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    
    return 0;
}
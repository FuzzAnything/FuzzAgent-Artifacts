// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Extract function selector from first byte
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Use a portion of data for table list (null-terminated)
    size_t table_len = remaining_size > 0 ? (remaining_data[0] % 64) + 1 : 0;
    if (table_len > remaining_size) table_len = remaining_size;
    
    char *tableList = NULL;
    if (table_len > 0) {
        tableList = (char *)malloc(table_len + 1);
        if (!tableList) return 0;
        memcpy(tableList, remaining_data, table_len);
        tableList[table_len] = '\0';
    } else {
        tableList = strdup("en-us-g2.ctb");
    }
    
    // Prepare input buffer (widechar array)
    const uint8_t *input_start = remaining_data + table_len;
    size_t input_bytes = remaining_size - table_len;
    if (input_bytes > 1024) input_bytes = 1024;
    
    int inlen = input_bytes / sizeof(widechar);
    if (inlen < 1) inlen = 1;
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return 0;
    }
    
    if (input_bytes > 0) {
        memcpy(inbuf, input_start, input_bytes < inlen * sizeof(widechar) ? 
               input_bytes : inlen * sizeof(widechar));
    } else {
        inbuf[0] = 'A';
    }
    
    // Call functions based on selector
    switch (selector % 6) {
        case 0: {  // lou_translateString
            int outlen = inlen * 4;  // Conservative estimate
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = NULL;
            char *spacing = NULL;
            
            // Sometimes allocate typeform and spacing
            if (selector & 0x10) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                for (int i = 0; i < outlen; i++) typeform[i] = 0;
            }
            if (selector & 0x20) {
                spacing = (char *)malloc(outlen * sizeof(char));
                for (int i = 0; i < outlen; i++) spacing[i] = 0;
            }
            
            lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                               typeform, spacing, selector & 0x0F);
            
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
        
        case 1:  // lou_charSize
            lou_charSize();
            break;
            
        case 2: {  // lou_readCharFromFile
            write_dummy_file(remaining_data, remaining_size);
            int mode = 1;
            while (lou_readCharFromFile("./dummy_file", &mode) != EOF) {
                // Read until EOF
            }
            remove("./dummy_file");
            break;
        }
        
        case 3:  // lou_checkTable
            lou_checkTable(tableList);
            break;
            
        case 4: {  // lou_charToDots
            int length = inlen;
            widechar *outbuf = (widechar *)malloc(length * sizeof(widechar));
            if (outbuf) {
                lou_charToDots(tableList, inbuf, outbuf, length, selector & 0x0F);
                free(outbuf);
            }
            break;
        }
        
        case 5: {  // lou_backTranslateString
            int outlen = inlen * 4;
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = NULL;
            char *spacing = NULL;
            
            // For back-translation, allocate buffers based on output length
            if (selector & 0x10) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                for (int i = 0; i < outlen; i++) typeform[i] = 0;
            }
            if (selector & 0x20) {
                spacing = (char *)malloc(outlen * sizeof(char));
                for (int i = 0; i < outlen; i++) spacing[i] = 0;
            }
            
            lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                   typeform, spacing, selector & 0x0F);
            
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
    }
    
    // Cleanup
    free(inbuf);
    free(tableList);
    
    return 0;
}
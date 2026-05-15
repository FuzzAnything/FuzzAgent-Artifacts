// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
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
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to select function and mode bits
    uint8_t selector = Data[0];
    uint8_t mode_bits = Data[1];
    const uint8_t *fuzz_data = Data + 2;
    size_t fuzz_size = Size - 2;

    // Extract table list string (null-terminated)
    size_t table_len = fuzz_size > 256 ? 256 : fuzz_size;
    char tableList[257];
    memcpy(tableList, fuzz_data, table_len);
    tableList[table_len] = '\0';
    
    // Prepare widechar buffers
    size_t widechar_count = fuzz_size / sizeof(widechar);
    if (widechar_count > 1024) widechar_count = 1024;
    
    widechar *inbuf = NULL;
    widechar *outbuf = NULL;
    
    if (widechar_count > 0) {
        inbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
        outbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
        
        if (!inbuf || !outbuf) {
            free(inbuf);
            free(outbuf);
            return 0;
        }
        
        // Fill inbuf with fuzz data
        memcpy(inbuf, fuzz_data, widechar_count * sizeof(widechar));
        
        // Fill outbuf with zeros
        memset(outbuf, 0, widechar_count * sizeof(widechar));
    } else {
        // Early return if no data for buffers
        return 0;
    }

    int length = (int)(widechar_count > 100 ? 100 : widechar_count);
    int mode = mode_bits & 0x0F;  // Use lower 4 bits for mode

    // Select function based on selector
    switch (selector % 6) {
        case 0: {
            // lou_dotsToChar
            lou_dotsToChar(tableList, inbuf, outbuf, length, mode);
            break;
        }
        case 1: {
            // lou_charToDots
            lou_charToDots(tableList, inbuf, outbuf, length, mode);
            break;
        }
        case 2: {
            // lou_translateString
            int inlen = length;
            int outlen = length * 2;  // Allow expansion
            formtype *typeform = NULL;
            char *spacing = NULL;
            
            // Sometimes allocate typeform and spacing
            if (mode_bits & 0x10) {
                typeform = (formtype *)malloc((inlen + 1) * sizeof(formtype));
                spacing = (char *)malloc((inlen + 1) * sizeof(char));
                if (typeform && spacing) {
                    memset(typeform, 0, (inlen + 1) * sizeof(formtype));
                    memset(spacing, 0, (inlen + 1) * sizeof(char));
                }
            }
            
            widechar *translate_outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            if (translate_outbuf) {
                lou_translateString(tableList, inbuf, &inlen, translate_outbuf, 
                                   &outlen, typeform, spacing, mode);
                free(translate_outbuf);
            }
            
            free(typeform);
            free(spacing);
            break;
        }
        case 3: {
            // lou_checkTable
            lou_checkTable(tableList);
            break;
        }
        case 4: {
            // lou_backTranslateString
            int inlen = length;
            int outlen = length * 2;
            formtype *typeform = NULL;
            char *spacing = NULL;
            
            if (mode_bits & 0x20) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                spacing = (char *)malloc(outlen * sizeof(char));
                if (typeform && spacing) {
                    memset(typeform, 0, outlen * sizeof(formtype));
                    memset(spacing, 0, outlen * sizeof(char));
                }
            }
            
            widechar *back_outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            if (back_outbuf) {
                lou_backTranslateString(tableList, inbuf, &inlen, back_outbuf, 
                                       &outlen, typeform, spacing, mode);
                free(back_outbuf);
            }
            
            free(typeform);
            free(spacing);
            break;
        }
        case 5: {
            // lou_getTable
            const void *table = lou_getTable(tableList);
            (void)table;  // Prevent unused variable warning
            break;
        }
    }

    // Cleanup
    free(inbuf);
    free(outbuf);

    // Also test with file-based input for some functions
    if (Size > 10) {
        write_dummy_file(Data, Size > 4096 ? 4096 : Size);
        
        // Test lou_checkTable with file path (some tables might be loaded from file)
        char file_table[300];
        snprintf(file_table, sizeof(file_table), "./dummy_file,%s", tableList);
        lou_checkTable(file_table);
        
        // Remove dummy file
        remove("./dummy_file");
    }

    return 0;
}
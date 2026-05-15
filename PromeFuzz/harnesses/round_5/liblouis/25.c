// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
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

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Write data to dummy file for potential table loading
    write_dummy_file(Data, Size);

    // Extract tableList string (null-terminated)
    size_t tableListLen = Size < 100 ? Size : 100;
    char *tableList = malloc(tableListLen + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, tableListLen);
    tableList[tableListLen] = '\0';

    // 1. Test lou_checkTable
    lou_checkTable(tableList);
    lou_checkTable("./dummy_file");

    // 2. Test lou_getTable
    const void *table = lou_getTable(tableList);
    if (table) {
        // If table loaded successfully, test other functions
        // 3. Test lou_compileString
        size_t inStringLen = Size < 50 ? Size : 50;
        char *inString = malloc(inStringLen + 1);
        if (inString) {
            memcpy(inString, Data, inStringLen);
            inString[inStringLen] = '\0';
            lou_compileString(tableList, inString);
            free(inString);
        }

        // Prepare buffers for conversion functions
        int length = Size < 20 ? Size : 20;
        widechar *inbuf = malloc(length * sizeof(widechar));
        widechar *outbuf = malloc(length * sizeof(widechar));
        if (inbuf && outbuf) {
            // Convert input data to widechar
            for (int i = 0; i < length; i++) {
                inbuf[i] = (widechar)Data[i % Size];
            }

            // 4. Test lou_charToDots
            lou_charToDots(tableList, inbuf, outbuf, length, 0);
            lou_charToDots(tableList, inbuf, outbuf, length, 1);

            // 5. Test lou_dotsToChar
            lou_dotsToChar(tableList, inbuf, outbuf, length, 0);

            // 6. Test lou_translateString
            int inlen = length;
            int outlen = length * 2;
            widechar *translate_outbuf = malloc(outlen * sizeof(widechar));
            formtype *typeform = malloc(length * sizeof(formtype));
            char *spacing = malloc((length + 1) * sizeof(char));
            
            if (translate_outbuf) {
                lou_translateString(tableList, inbuf, &inlen, translate_outbuf, &outlen, NULL, NULL, 0);
                
                if (typeform && spacing) {
                    // Initialize arrays
                    for (int i = 0; i < length; i++) {
                        typeform[i] = 0;
                        spacing[i] = 0;
                    }
                    spacing[length] = 0;
                    
                    lou_translateString(tableList, inbuf, &inlen, translate_outbuf, &outlen, typeform, spacing, 0);
                }
                
                free(translate_outbuf);
            }
            
            free(typeform);
            free(spacing);
        }
        
        free(inbuf);
        free(outbuf);
    }

    // Also test with dummy file as tableList
    const void *table2 = lou_getTable("./dummy_file");
    if (table2) {
        // Test compileString with dummy file
        char dummy_inString[] = "test entry";
        lou_compileString("./dummy_file", dummy_inString);
    }

    // Cleanup
    free(tableList);
    remove_dummy_file();
    
    return 0;
}
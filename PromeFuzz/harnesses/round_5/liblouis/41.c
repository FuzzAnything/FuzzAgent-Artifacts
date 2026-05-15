// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_freeEmphClasses at compileTranslationTable.c:5095:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
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

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into parts for different parameters
    size_t split1 = Size / 3;
    size_t split2 = 2 * Size / 3;
    
    // Create null-terminated strings from input data
    char *tableList = malloc(split1 + 1);
    char *emphClass = malloc(split2 - split1 + 1);
    char *query = malloc(Size - split2 + 1);
    
    if (!tableList || !emphClass || !query) {
        free(tableList);
        free(emphClass);
        free(query);
        return 0;
    }
    
    memcpy(tableList, Data, split1);
    tableList[split1] = '\0';
    
    memcpy(emphClass, Data + split1, split2 - split1);
    emphClass[split2 - split1] = '\0';
    
    memcpy(query, Data + split2, Size - split2);
    query[Size - split2] = '\0';
    
    // Test lou_getEmphClasses and lou_freeEmphClasses
    const char **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }
    
    // Test lou_dotsToChar
    if (Size >= 10) {
        int length = (Size - 10) / sizeof(widechar);
        if (length > 0) {
            widechar *inbuf = (widechar *)(Data);
            widechar *outbuf = malloc(length * sizeof(widechar));
            if (outbuf) {
                lou_dotsToChar(tableList, inbuf, outbuf, length, 0);
                free(outbuf);
            }
        }
    }
    
    // Test lou_findTable
    char *foundTable = lou_findTable(query);
    if (foundTable) {
        free(foundTable);
    }
    
    // Test lou_getTypeformForEmphClass
    lou_getTypeformForEmphClass(tableList, emphClass);
    
    // Test with dummy file
    write_dummy_file(Data, Size);
    
    // Test lou_getEmphClasses with dummy file reference
    char dummyTableList[] = "./dummy_file";
    const char **dummyEmphClasses = lou_getEmphClasses(dummyTableList);
    if (dummyEmphClasses) {
        lou_freeEmphClasses(dummyEmphClasses);
    }
    
    // Test lou_dotsToChar with dummy file
    if (Size >= 4) {
        int dummyLength = 2;
        widechar dummyInbuf[2] = {0x2800, 0x2801};
        widechar dummyOutbuf[2];
        lou_dotsToChar(dummyTableList, dummyInbuf, dummyOutbuf, dummyLength, 0);
    }
    
    remove_dummy_file();
    
    // Clean up
    free(tableList);
    free(emphClass);
    free(query);
    
    return 0;
}
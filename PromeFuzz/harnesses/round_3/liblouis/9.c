// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "liblouis.h"

#define MAX_SIZE 1024

static char **dummy_resolver(const char *table, const char *base) {
    (void)table;
    (void)base;
    return NULL;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file for table loading if needed
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size < 256 ? Size : 256, f);
        fclose(f);
    }

    // Extract strings from fuzzer input
    char tableList[256] = "dummy_file";
    char emphClass[256] = {0};
    char inString[256] = {0};
    
    if (Size > 1) {
        size_t len = Size < 255 ? Size - 1 : 255;
        memcpy(emphClass, Data + 1, len);
        emphClass[len] = '\0';
    }
    
    if (Size > 2) {
        size_t len = Size < 255 ? Size - 2 : 255;
        memcpy(inString, Data + 2, len);
        inString[len] = '\0';
    }

    // Function 1: lou_getTypeformForEmphClass
    lou_getTypeformForEmphClass(tableList, emphClass);

    // Function 2: lou_registerTableResolver
    lou_registerTableResolver(dummy_resolver);

    // Function 3: lou_compileString
    // First try to load the table
    const char *table = lou_getTable(tableList);
    if (table != NULL) {
        lou_compileString(tableList, inString);
    }

    // Functions 4-6: Translation functions
    int inlen = 0, outlen = 0, cursorPos = 0;
    widechar inbuf[MAX_SIZE] = {0};
    widechar outbuf[MAX_SIZE] = {0};
    formtype typeform[MAX_SIZE] = {0};
    char spacing[MAX_SIZE] = {0};
    int outputPos[MAX_SIZE] = {0};
    int inputPos[MAX_SIZE] = {0};
    
    // Prepare input buffer from fuzzer data
    if (Size > 10) {
        inlen = (Size - 10) / 2;
        if (inlen > MAX_SIZE) inlen = MAX_SIZE;
        if (inlen > 0) {
            size_t offset = 10;
            for (int i = 0; i < inlen; i++) {
                if (offset + 1 < Size) {
                    inbuf[i] = (widechar)(Data[offset] << 8) | Data[offset + 1];
                    offset += 2;
                }
            }
        }
    }
    
    outlen = MAX_SIZE;
    int mode = Data[0] % 4;

    // Function 4: lou_backTranslate
    lou_backTranslate(tableList, inbuf, &inlen, outbuf, &outlen, 
                     typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // Reset lengths for next functions
    if (Size > 10) {
        inlen = (Size - 10) / 2;
        if (inlen > MAX_SIZE) inlen = MAX_SIZE;
    } else {
        inlen = 0;
    }
    outlen = MAX_SIZE;

    // Function 5: lou_backTranslateString
    lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                           typeform, spacing, mode);

    // Reset lengths for next function
    if (Size > 10) {
        inlen = (Size - 10) / 2;
        if (inlen > MAX_SIZE) inlen = MAX_SIZE;
    } else {
        inlen = 0;
    }
    outlen = MAX_SIZE;

    // Function 6: lou_translateString
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen,
                       typeform, spacing, mode);

    // Clean up
    unlink("./dummy_file");
    return 0;
}
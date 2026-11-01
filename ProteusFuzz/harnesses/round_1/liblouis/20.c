// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static int fuzzer_run(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a null-terminated string from input for table names and input text
    // We use a subset of data for table names to increase chance of valid table names
    size_t tableLen = (Size > 32) ? 32 : Size;
    char *tableList = (char *)malloc(tableLen + 1);
    char *inString = (char *)malloc(Size + 1);
    widechar *inbuf = (widechar *)malloc((Size + 1) * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc((Size + 1) * sizeof(widechar));
    formtype *typeform = (formtype *)malloc((Size + 1) * sizeof(formtype));
    char *spacing = (char *)malloc(Size + 1);
    int *outputPos = (int *)malloc((Size + 1) * sizeof(int));
    int *inputPos = (int *)malloc((Size + 1) * sizeof(int));
    int *cursorPos = (int *)malloc(sizeof(int));

    if (!tableList || !inString || !inbuf || !outbuf || !typeform || 
        !spacing || !outputPos || !inputPos || !cursorPos) {
        free(tableList); free(inString); free(inbuf); free(outbuf);
        free(typeform); free(spacing); free(outputPos); free(inputPos); free(cursorPos);
        return 0;
    }

    // Initialize buffers
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';
    
    memcpy(inString, Data, Size);
    inString[Size] = '\0';

    // Convert input string to widechar
    int inlen = Size;
    for (int i = 0; i < Size; i++) {
        inbuf[i] = (unsigned char)Data[i];
    }
    inbuf[Size] = 0;

    int outlen = Size;
    // Vary mode safely
    int mode = (Size % 4) * 100 - 200; 

    // Initialize optional arrays
    for (int i = 0; i <= Size; i++) {
        typeform[i] = (Size % 2) ? 1 : 0;
        spacing[i] = (char)(Data[i % Size]);
        outputPos[i] = i;
        inputPos[i] = i;
    }
    *cursorPos = Size % (Size + 1);

    // Test lou_getTable first to check validity before using tableList in other functions
    const void *table = lou_getTable(tableList);
    
    // Only proceed with table-dependent operations if table is valid
    if (table != NULL) {
        // Test lou_compileString only if table is valid to avoid internal null pointer dereference
        (void)lou_compileString(tableList, inString);

        // Test lou_getEmphClasses
        char const **emphClasses = lou_getEmphClasses(tableList);
        if (emphClasses) {
            for (int i = 0; emphClasses[i] != NULL; i++) {
                // Test lou_getTypeformForEmphClass
                (void)lou_getTypeformForEmphClass(tableList, emphClasses[i]);
            }
        }

        // Test lou_translateString
        int ret1 = lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
        (void)ret1;

        // Test lou_translate
        int ret2 = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, cursorPos, mode);
        (void)ret2;
    }

    // Cleanup
    free(tableList);
    free(inString);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);

    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    return fuzzer_run(Data, Size);
}
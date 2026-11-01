// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

static void free_emph_classes(char const **classes) {
    if (classes) {
        size_t i = 0;
        while (classes[i] != NULL) {
            free((void *)classes[i]);
            i++;
        }
        free((void *)classes);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a safe table list string from input, ensuring it's null-terminated
    // We limit the table list to a reasonable size to avoid excessive allocation
    size_t tableListSize = (Size < 256) ? Size : 256;
    char tableList[257];
    memcpy(tableList, Data, tableListSize);
    tableList[tableListSize] = '\0';

    // Prepare buffers for translation
    // Use a fixed size for fuzzing to prevent OOM, but large enough to catch issues
    const int bufferSize = 1024;
    widechar inbuf[bufferSize];
    widechar outbuf[bufferSize];
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    int inlen = 0;
    int outlen = bufferSize;

    // Initialize input buffer with fuzz data
    // Ensure we don't exceed buffer size
    size_t inputChars = (Size < bufferSize) ? Size : bufferSize;
    for (size_t i = 0; i < inputChars; i++) {
        inbuf[i] = Data[i];
    }
    inlen = inputChars;

    // Allocate optional buffers if we want to test with them
    // We'll use a simple heuristic: if the first byte is odd, allocate them
    if (Data[0] % 2 != 0) {
        typeform = calloc(bufferSize, sizeof(formtype));
        spacing = calloc(bufferSize, sizeof(char));
        outputPos = calloc(bufferSize, sizeof(int));
        inputPos = calloc(bufferSize, sizeof(int));
        
        // Fill typeform with random values
        for (int i = 0; i < bufferSize; i++) {
            typeform[i] = Data[i % Size];
        }
        // Fill spacing with random values
        for (int i = 0; i < bufferSize; i++) {
            spacing[i] = Data[i % Size];
        }
    }

    // 1. Test lou_getTable
    const void *table = lou_getTable(tableList);
    // No cleanup needed for lou_getTable return value as per typical liblouis usage
    // (The table is managed internally or the caller doesn't free it directly in this context)

    // 2. Test lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    free_emph_classes(emphClasses);

    // 3. Test lou_getTypeformForEmphClass
    // We try to get typeform for the first few bytes interpreted as a class name
    char classBuf[16];
    size_t classLen = (Size < 15) ? Size : 15;
    memcpy(classBuf, Data, classLen);
    classBuf[classLen] = '\0';
    formtype tf = lou_getTypeformForEmphClass(tableList, classBuf);
    (void)tf; // Suppress unused variable warning

    // 4. Test lou_backTranslateString
    int outlen_str = bufferSize;
    int inlen_str = inlen;
    int mode = (Data[0] % 3) - 1; // -1, 0, 1
    int ret = lou_backTranslateString(tableList, inbuf, &inlen_str, outbuf, &outlen_str, 
                                      typeform, spacing, mode);
    (void)ret;

    // 5. Test lou_backTranslate
    int outlen_bt = bufferSize;
    int inlen_bt = inlen;
    int cursorPos_bt = 0;
    int *outputPos_bt = outputPos;
    int *inputPos_bt = inputPos;
    int mode_bt = (Data[0] % 3) - 1;
    
    // Re-initialize lengths for this call
    inlen_bt = inlen;
    outlen_bt = bufferSize;
    
    ret = lou_backTranslate(tableList, inbuf, &inlen_bt, outbuf, &outlen_bt, 
                            typeform, spacing, outputPos_bt, inputPos_bt, 
                            &cursorPos_bt, mode_bt);
    (void)ret;

    // 6. Test lou_translate
    int outlen_tr = bufferSize;
    int inlen_tr = inlen;
    int cursorPos_tr = 0;
    int *outputPos_tr = outputPos;
    int *inputPos_tr = inputPos;
    int mode_tr = (Data[0] % 3) - 1;

    // Re-initialize lengths for this call
    inlen_tr = inlen;
    outlen_tr = bufferSize;

    ret = lou_translate(tableList, inbuf, &inlen_tr, outbuf, &outlen_tr,
                        typeform, spacing, outputPos_tr, inputPos_tr,
                        &cursorPos_tr, mode_tr);
    (void)ret;

    // Cleanup
    if (typeform) free(typeform);
    if (spacing) free(spacing);
    if (outputPos) free(outputPos);
    if (inputPos) free(inputPos);

    return 0;
}
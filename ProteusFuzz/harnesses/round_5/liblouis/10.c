// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_SIZE 4096

static void free_emph_classes(char const **classes) {
    if (classes == NULL) return;
    for (int i = 0; classes[i] != NULL; i++) {
        free((void *)classes[i]);
    }
    free((void *)classes);
}

static int is_valid_table_name(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    // Try to find a null terminator or reasonable table name pattern
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\0') return 1;
        // Basic check for printable chars, allow dots and underscores
        if (data[i] >= 32 && data[i] <= 126) continue;
        if (data[i] == '.' || data[i] == '_' || data[i] == '-') continue;
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate buffers for translation
    widechar *inbuf = (widechar *)malloc(MAX_SIZE * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(MAX_SIZE * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(MAX_SIZE * sizeof(formtype));
    char *spacing = (char *)malloc(MAX_SIZE * sizeof(char));
    int *outputPos = (int *)malloc(MAX_SIZE * sizeof(int));
    int *inputPos = (int *)malloc(MAX_SIZE * sizeof(int));

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        free(outputPos); free(inputPos);
        return 0;
    }

    // Prepare table list string from input
    char tableList[MAX_SIZE];
    size_t tableLen = (Size < MAX_SIZE - 1) ? Size : MAX_SIZE - 1;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Ensure tableList is valid (contains a null terminator and printable chars)
    // If not, try to sanitize or use a default
    int validTable = 1;
    for (size_t i = 0; i < tableLen; i++) {
        if (tableList[i] < 32 || tableList[i] > 126) {
            if (tableList[i] != '.' && tableList[i] != '_' && tableList[i] != '-') {
                validTable = 0;
                break;
            }
        }
    }
    if (!validTable) {
        // Fallback to a known table name if input is garbage
        strcpy(tableList, "en-gb-8");
    }

    // 1. Test lou_getEmphClasses
    char const **emphClasses = lou_getEmphClasses(tableList);
    if (emphClasses != NULL) {
        // Just iterate to ensure no crash, then free
        for (int i = 0; emphClasses[i] != NULL; i++) {
            (void)emphClasses[i]; // Use to avoid warning
        }
        free_emph_classes(emphClasses);
    }

    // 2. Test lou_getTable
    const void *table = lou_getTable(tableList);
    // We don't free table as it's managed by liblouis internally

    // 3. Test lou_getTypeformForEmphClass
    // Try with a few common class names or parts of input
    const char *testClasses[] = {"italic", "bold", "computer", NULL};
    for (int i = 0; testClasses[i] != NULL; i++) {
        formtype ft = lou_getTypeformForEmphClass(tableList, testClasses[i]);
        (void)ft;
    }
    // Also try using input data as class name if it looks like a string
    if (Size > 0 && Size < 20) {
        char className[21];
        memcpy(className, Data, Size);
        className[Size] = '\0';
        formtype ft2 = lou_getTypeformForEmphClass(tableList, className);
        (void)ft2;
    }

    // Prepare input buffer for translation
    // Convert raw input to widechar, filtering non-printable if needed
    int inlen = 0;
    for (size_t i = 0; i < Size && inlen < MAX_SIZE - 1; i++) {
        if (Data[i] >= 32 && Data[i] <= 126) {
            inbuf[inlen++] = (widechar)Data[i];
        }
    }
    inbuf[inlen] = 0;

    int outlen = MAX_SIZE;
    int cursorPos = 0;

    // Initialize optional arrays
    memset(typeform, 0, MAX_SIZE * sizeof(formtype));
    memset(spacing, 0, MAX_SIZE * sizeof(char));
    memset(outputPos, 0, MAX_SIZE * sizeof(int));
    memset(inputPos, 0, MAX_SIZE * sizeof(int));

    // 4. Test lou_translate
    int mode = 0; // Default mode
    int ret = lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                            typeform, spacing, outputPos, inputPos,
                            &cursorPos, mode);
    (void)ret;

    // 5. Test lou_backTranslate
    // Prepare braille input (reuse outbuf from previous step or use raw data)
    int brailleInLen = outlen;
    // Ensure we have valid braille chars (0-127)
    for (int i = 0; i < brailleInLen && i < MAX_SIZE; i++) {
        if (outbuf[i] > 127) outbuf[i] = 0x00;
    }
    
    int textOutLen = MAX_SIZE;
    cursorPos = 0;
    ret = lou_backTranslate(tableList, outbuf, &brailleInLen, inbuf, &textOutLen,
                            typeform, spacing, outputPos, inputPos,
                            &cursorPos, mode);
    (void)ret;

    // 6. Test lou_backTranslateString
    int brailleStrLen = brailleInLen;
    int textStrOutLen = MAX_SIZE;
    ret = lou_backTranslateString(tableList, outbuf, &brailleStrLen, inbuf, &textStrOutLen,
                                  typeform, spacing, mode);
    (void)ret;

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    return 0;
}
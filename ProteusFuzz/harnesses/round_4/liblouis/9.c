// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Limit size to prevent excessive allocation
    if (Size > MAX_SIZE) Size = MAX_SIZE;

    // Allocate buffers
    widechar *inbuf = (widechar *)malloc(Size * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc((Size + 100) * sizeof(widechar));
    int *inlen = (int *)malloc(sizeof(int));
    int *outlen = (int *)malloc(sizeof(int));
    formtype *typeform = (formtype *)malloc(Size * sizeof(formtype));
    char *spacing = (char *)malloc(Size * sizeof(char));
    int *outputPos = (int *)malloc(Size * sizeof(int));
    int *inputPos = (int *)malloc((Size + 100) * sizeof(int));
    int cursorPos = 0;
    char *hyphens = (char *)malloc(Size * sizeof(char));
    char *inputHyphens = (char *)malloc(Size * sizeof(char));
    char *outputHyphens = (char *)malloc((Size + 100) * sizeof(char));

    if (!inbuf || !outbuf || !inlen || !outlen || !typeform || !spacing ||
        !outputPos || !inputPos || !hyphens || !inputHyphens || !outputHyphens) {
        free(inbuf); free(outbuf); free(inlen); free(outlen);
        free(typeform); free(spacing); free(outputPos); free(inputPos);
        free(hyphens); free(inputHyphens); free(outputHyphens);
        return 0;
    }

    // Initialize buffers
    memset(inbuf, 0, Size * sizeof(widechar));
    memset(outbuf, 0, (Size + 100) * sizeof(widechar));
    memset(typeform, 0, Size * sizeof(formtype));
    memset(spacing, 0, Size * sizeof(char));
    memset(outputPos, 0, Size * sizeof(int));
    memset(inputPos, 0, (Size + 100) * sizeof(int));
    memset(hyphens, 0, Size * sizeof(char));
    memset(inputHyphens, 0, Size * sizeof(char));
    memset(outputHyphens, 0, (Size + 100) * sizeof(char));

    // Convert input data to widechar
    for (size_t i = 0; i < Size; i++) {
        inbuf[i] = (widechar)Data[i];
        typeform[i] = (formtype)(Data[i] % 256);
        spacing[i] = (char)Data[i];
        inputHyphens[i] = (char)Data[i];
    }

    *inlen = (int)Size;
    *outlen = (int)(Size + 100);

    // Table name (use a common one or derived from input)
    const char *tableList = "en-gb-8";
    if (Size > 10) {
        // Create a dynamic table name from input (safe subset)
        static char dynTable[32];
        int len = (Size < 31) ? Size : 31;
        for (int i = 0; i < len; i++) {
            dynTable[i] = (Data[i] >= 'a' && Data[i] <= 'z') ? Data[i] : 'a';
        }
        dynTable[len] = '\0';
        tableList = dynTable;
    }

    // Fuzz lou_translate
    lou_translate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing,
                  outputPos, inputPos, &cursorPos, 0);
    lou_translate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing,
                  outputPos, inputPos, &cursorPos, 1);

    // Fuzz lou_backTranslate
    lou_backTranslate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing,
                      outputPos, inputPos, &cursorPos, 0);
    lou_backTranslate(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing,
                      outputPos, inputPos, &cursorPos, 1);

    // Fuzz lou_hyphenate
    lou_hyphenate(tableList, inbuf, *inlen, hyphens, 0);
    lou_hyphenate(tableList, inbuf, *inlen, hyphens, 1);

    // Fuzz lou_dotsToChar
    lou_dotsToChar(tableList, inbuf, outbuf, *inlen, 0);
    lou_dotsToChar(tableList, inbuf, outbuf, *inlen, 1);

    // Fuzz lou_translatePrehyphenated
    lou_translatePrehyphenated(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing,
                               outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, 0);
    lou_translatePrehyphenated(tableList, inbuf, inlen, outbuf, outlen, typeform, spacing,
                               outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, 1);

    // Fuzz lou_getTable
    const void *table = lou_getTable(tableList);
    (void)table; // Suppress unused warning

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(inlen);
    free(outlen);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(hyphens);
    free(inputHyphens);
    free(outputHyphens);

    return 0;
}
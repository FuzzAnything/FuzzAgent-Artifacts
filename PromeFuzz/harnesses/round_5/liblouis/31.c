// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
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

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Prepare table list string
    size_t table_list_len = Size > 100 ? 100 : Size;
    char tableList[101];
    memcpy(tableList, Data, table_list_len);
    tableList[table_list_len] = '\0';

    // Write dummy file for lou_readCharFromFile
    write_dummy_file(Data, Size > 1024 ? 1024 : Size);

    // 1. Test lou_compileString
    // First try to load a table to ensure tableList is valid
    const void *table = lou_getTable(tableList);
    if (table != NULL) {
        size_t compile_str_len = Size > 200 ? 200 : Size;
        char inString[201];
        memcpy(inString, Data + (Size > 50 ? 50 : 0), 
               compile_str_len > 200 ? 200 : compile_str_len);
        inString[compile_str_len] = '\0';
        lou_compileString(tableList, inString);
    }

    // 2. Test lou_charSize
    int char_size = lou_charSize();
    (void)char_size; // Suppress unused warning

    // 3. Test lou_backTranslateString
    if (Size >= sizeof(widechar) * 2) {
        int inlen = (Size / sizeof(widechar)) > 50 ? 50 : Size / sizeof(widechar);
        int outlen = 100;
        widechar *inbuf = (widechar *)Data;
        widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        formtype *typeform = NULL;
        char *spacing = NULL;
        int mode = 0;

        // Alternate between NULL and allocated buffers
        if (Size % 2 == 0) {
            typeform = (formtype *)malloc(outlen * sizeof(formtype));
            spacing = (char *)malloc(outlen * sizeof(char));
        }

        lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                               typeform, spacing, mode);

        free(outbuf);
        if (typeform) free(typeform);
        if (spacing) free(spacing);
    }

    // 4. Test lou_readCharFromFile
    int mode = 1;
    int result = lou_readCharFromFile("./dummy_file", &mode);
    (void)result; // Suppress unused warning

    // 5. Test lou_translateString
    if (Size >= sizeof(widechar) * 2) {
        int inlen = (Size / sizeof(widechar)) > 50 ? 50 : Size / sizeof(widechar);
        int outlen = 100;
        widechar *inbuf = (widechar *)Data;
        widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        formtype *typeform = NULL;
        char *spacing = NULL;
        int mode = 0;

        // Alternate between NULL and allocated buffers
        if (Size % 3 == 0) {
            typeform = (formtype *)malloc(inlen * sizeof(formtype));
            spacing = (char *)malloc((inlen + 1) * sizeof(char));
        }

        lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                           typeform, spacing, mode);

        free(outbuf);
        if (typeform) free(typeform);
        if (spacing) free(spacing);
    }

    // 6. Test lou_getTypeformForEmphClass
    size_t emph_len = Size > 50 ? 50 : Size;
    char emphClass[51];
    memcpy(emphClass, Data + (Size > 100 ? 100 : 0), emph_len);
    emphClass[emph_len] = '\0';
    formtype typeform_result = lou_getTypeformForEmphClass(tableList, emphClass);
    (void)typeform_result; // Suppress unused warning

    return 0;
}
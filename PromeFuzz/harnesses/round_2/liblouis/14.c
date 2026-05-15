// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

#define MAX_BUF 1024

static char **dummy_resolver(const char *table, const char *base) {
    char **result = (char **)malloc(2 * sizeof(char *));
    if (result) {
        result[0] = strdup("./dummy_file");
        result[1] = NULL;
    }
    return result;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Prepare dummy file for table resolution
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Split input for different parameters
    size_t half = Size / 2;
    const char *tableList = "en-us-g2.ctb";
    const char *emphClass = "italic";
    int mode = 0;

    // Prepare buffers with safe limits
    int inlen = half / sizeof(widechar);
    if (inlen > MAX_BUF) inlen = MAX_BUF;
    if (inlen <= 0) inlen = 1;
    
    int outlen = MAX_BUF;
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    // Allocate typeform with output buffer size for lou_backTranslate safety
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    // Allocate spacing with output buffer size + 1 for lou_backTranslate safety
    char *spacing = (char *)malloc((outlen + 1) * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;

    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform);
        free(spacing); free(outputPos); free(inputPos);
        return 0;
    }

    // Initialize buffers
    size_t copy_size = half > inlen * sizeof(widechar) ? inlen * sizeof(widechar) : half;
    memcpy(inbuf, Data, copy_size);
    memset(typeform, 0, outlen * sizeof(formtype));
    memset(spacing, 0, (outlen + 1) * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));

    // 1. lou_backTranslate
    int inlen_back = inlen;
    int outlen_back = outlen;
    lou_backTranslate(tableList, inbuf, &inlen_back, outbuf, &outlen_back,
                      typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // 2. lou_registerTableResolver
    lou_registerTableResolver(dummy_resolver);

    // 3. lou_getTypeformForEmphClass
    lou_getTypeformForEmphClass(tableList, emphClass);

    // 4. lou_translateString
    int inlen_trans = inlen;
    int outlen_trans = outlen;
    // Reinitialize typeform for input buffer size for lou_translateString
    memset(typeform, 0, outlen * sizeof(formtype));
    lou_translateString(tableList, inbuf, &inlen_trans, outbuf, &outlen_trans,
                        typeform, spacing, mode);

    // 5. lou_translate
    int inlen_trans2 = inlen;
    int outlen_trans2 = outlen;
    // Reinitialize typeform for output buffer size as per hint
    memset(typeform, 0, outlen * sizeof(formtype));
    lou_translate(tableList, inbuf, &inlen_trans2, outbuf, &outlen_trans2,
                  typeform, spacing, outputPos, inputPos, &cursorPos, mode);

    // 6. lou_compileString
    char *tableString = (char *)malloc(half + 1);
    if (tableString) {
        size_t table_copy_size = Size - half > half ? half : Size - half;
        memcpy(tableString, Data + half, table_copy_size);
        tableString[table_copy_size] = '\0';
        lou_compileString(tableList, tableString);
        free(tableString);
    }

    // Cleanup
    free(inbuf); free(outbuf); free(typeform);
    free(spacing); free(outputPos); free(inputPos);
    remove("./dummy_file");

    return 0;
}
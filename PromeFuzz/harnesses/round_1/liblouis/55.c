// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Dummy table resolver for lou_registerTableResolver */
static char **dummy_resolver(const char *table, const char *base) {
    static char *result = NULL;
    (void)table;
    (void)base;
    return &result;
}

/* Main fuzzing entry point */
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    /* Early exit for insufficient data */
    if (Size < 1) return 0;

    /* Use a fixed table list for testing */
    const char *tableList = "en-us-g2.ctb";

    /* Extract mode from first byte */
    int mode = Data[0] & 0x0F; /* Limit to 0-15 */

    /* Determine input length for widechar buffers */
    size_t max_inlen = (Size - 1) / sizeof(widechar);
    if (max_inlen < 1) max_inlen = 1;

    /* Allocate input buffer */
    widechar *inbuf = (widechar *)malloc(max_inlen * sizeof(widechar));
    if (!inbuf) return 0;

    /* Fill input buffer with fuzz data (skip first byte used for mode) */
    size_t actual_inlen = max_inlen;
    if (Size > 1) {
        size_t bytes_to_copy = Size - 1;
        if (bytes_to_copy > max_inlen * sizeof(widechar))
            bytes_to_copy = max_inlen * sizeof(widechar);
        memcpy(inbuf, Data + 1, bytes_to_copy);
        actual_inlen = bytes_to_copy / sizeof(widechar);
        if (actual_inlen < 1) actual_inlen = 1;
    } else {
        /* If no data beyond mode, use a single default character */
        inbuf[0] = 'A';
    }

    /* Prepare lengths (will be updated by library functions) */
    int inlen = (int)actual_inlen;
    int outlen = inlen * 4; /* Conservative output buffer size */

    /* Allocate output buffer */
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }

    /* Allocate typeform and spacing buffers with safe sizes */
    formtype *typeform = (formtype *)malloc(outlen * 2 * sizeof(formtype));
    char *spacing = (char *)malloc((outlen * 2 + 1) * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;

    /* Initialize buffers to zero if allocated */
    if (typeform) memset(typeform, 0, outlen * 2 * sizeof(formtype));
    if (spacing) memset(spacing, 0, (outlen * 2 + 1) * sizeof(char));
    if (outputPos) memset(outputPos, 0, inlen * sizeof(int));
    if (inputPos) memset(inputPos, 0, outlen * sizeof(int));

    /* 1. lou_translateString */
    if (typeform && spacing) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_translateString(tableList, inbuf, &local_inlen, outbuf,
                            &local_outlen, typeform, spacing, mode);
    }

    /* 2. lou_registerTableResolver */
    lou_registerTableResolver(dummy_resolver);

    /* 3. lou_backTranslate */
    if (typeform && spacing && outputPos && inputPos) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslate(tableList, inbuf, &local_inlen, outbuf,
                          &local_outlen, typeform, spacing, outputPos,
                          inputPos, &cursorPos, mode);
    }

    /* 4. lou_getTypeformForEmphClass */
    /* Use a small subset of data to create emphasis class string */
    char emphClass[32] = {0};
    size_t emph_len = Size < sizeof(emphClass) - 1 ? Size : sizeof(emphClass) - 1;
    if (emph_len > 0) {
        memcpy(emphClass, Data, emph_len);
        emphClass[emph_len] = '\0';
        lou_getTypeformForEmphClass(tableList, emphClass);
    }

    /* 5. lou_compileString */
    /* Write table data to a dummy file first */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    /* Then try to compile a simple rule */
    const char *simple_rule = "include en-us-g2.ctb";
    lou_compileString(tableList, simple_rule);

    /* 6. lou_backTranslateString */
    if (typeform && spacing) {
        int local_inlen = inlen;
        int local_outlen = outlen;
        lou_backTranslateString(tableList, inbuf, &local_inlen, outbuf,
                                &local_outlen, typeform, spacing, mode);
    }

    /* Cleanup */
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}
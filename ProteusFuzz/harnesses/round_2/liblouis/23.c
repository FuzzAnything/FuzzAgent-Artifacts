// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_registerTableResolver at compileTranslationTable.c:4865:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void cleanup_buffers(widechar *inbuf, widechar *outbuf, formtype *typeform, char *spacing) {
    if (inbuf) free(inbuf);
    if (outbuf) free(outbuf);
    if (typeform) free(typeform);
    if (spacing) free(spacing);
}

// Dummy resolver function matching the required signature: char **(*)(const char *, const char *)
static char **dummy_resolver(const char *table, const char *base) {
    (void)table;
    (void)base;
    // Return NULL to indicate no custom resolution found
    return NULL;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file if needed
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Allocate buffers for fuzzing
    size_t max_len = Size > 1024 ? 1024 : Size;
    widechar *inbuf = (widechar *)malloc(max_len * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(max_len * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(max_len * sizeof(formtype));
    char *spacing = (char *)malloc(max_len * sizeof(char));
    int inlen = (int)max_len;
    int outlen = (int)max_len;

    if (!inbuf || !outbuf || !typeform || !spacing) {
        cleanup_buffers(inbuf, outbuf, typeform, spacing);
        return 0;
    }

    // Initialize buffers with fuzz data
    for (size_t i = 0; i < max_len; i++) {
        inbuf[i] = (widechar)Data[i];
        typeform[i] = (formtype)(Data[i] & 0xFFFF);
        spacing[i] = (char)Data[i];
    }

    // Prepare table list string (ensure null termination)
    char *tableListCopy = (char *)malloc(Size + 1);
    if (!tableListCopy) {
        cleanup_buffers(inbuf, outbuf, typeform, spacing);
        return 0;
    }
    memcpy(tableListCopy, Data, Size);
    tableListCopy[Size] = '\0';

    // Test lou_checkTable
    lou_checkTable(tableListCopy);

    // Test lou_getTable
    const void *table = lou_getTable(tableListCopy);
    if (table) {
        // Table loaded successfully, no direct cleanup needed for lou_getTable
    }

    // Test lou_registerTableResolver with the correct function pointer type
    lou_registerTableResolver(dummy_resolver);

    // Test lou_charToDots
    int mode = 0;
    if (Size > 0) {
        mode = (int)Data[0];
    }
    lou_charToDots(tableListCopy, inbuf, outbuf, inlen, mode);

    // Test lou_dotsToChar
    lou_dotsToChar(tableListCopy, inbuf, outbuf, inlen, mode);

    // Test lou_translateString
    lou_translateString(
        tableListCopy,
        inbuf, &inlen,
        outbuf, &outlen,
        typeform, spacing, mode
    );

    // Cleanup
    cleanup_buffers(inbuf, outbuf, typeform, spacing);
    free(tableListCopy);

    return 0;
}
// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_indexTables at metadata.c:945:1 in liblouis.h
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
#include <ctype.h>

static void ensure_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

static void sanitize_string(const uint8_t *Data, size_t Size, char *out, size_t outSize) {
    size_t i = 0;
    size_t j = 0;
    if (outSize == 0) return;
    
    while (i < Size && j < outSize - 1) {
        unsigned char c = Data[i];
        if (c == 0 || c == '\n' || c == '\r') {
            c = ' ';
        }
        if (c >= 32 && c < 127) {
            out[j++] = (char)c;
        }
        i++;
    }
    out[j] = '\0';
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure dummy file exists for lou_readCharFromFile
    ensure_dummy_file(Data, Size);

    char query[256];
    sanitize_string(Data, Size, query, sizeof(query));

    // Fuzz lou_findTable
    char *table = lou_findTable(query);
    if (table) {
        free(table);
    }

    // Fuzz lou_checkTable
    int exists = lou_checkTable(query);
    (void)exists;

    // Fuzz lou_getTable
    const void *tbl = lou_getTable(query);
    (void)tbl;

    // Prepare table list for indexing
    char *tables[10];
    int tableCount = 0;
    char tempQuery[256];
    
    // Create multiple table names from input
    for (int i = 0; i < 5 && i < Size; i++) {
        size_t chunkSize = (Size - i) / (5 - i);
        if (chunkSize == 0) chunkSize = 1;
        sanitize_string(&Data[i], chunkSize, tempQuery, sizeof(tempQuery));
        tables[tableCount] = strdup(tempQuery);
        tableCount++;
        i += chunkSize;
    }
    tables[tableCount] = NULL;

    // Fuzz lou_indexTables
    if (tableCount > 0) {
        lou_indexTables((const char **)tables);
    }

    // Cleanup table names
    for (int i = 0; i < tableCount; i++) {
        free(tables[i]);
    }

    // Fuzz lou_translateString
    // Prepare input buffer (widechar)
    size_t inLen = Size > 1000 ? 1000 : Size;
    widechar *inbuf = (widechar *)malloc(inLen * sizeof(widechar));
    if (!inbuf) return 0;
    
    for (size_t i = 0; i < inLen; i++) {
        // Ensure valid widechar values (0-65535)
        inbuf[i] = (widechar)(Data[i] & 0xFFFF);
    }
    
    int inlen = (int)inLen;
    
    // Prepare output buffer
    widechar *outbuf = (widechar *)malloc(inLen * 2 * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    
    int outlen = (int)(inLen * 2);
    
    // Prepare typeform and spacing buffers
    formtype *typeform = (formtype *)malloc(inLen * sizeof(formtype));
    char *spacing = (char *)malloc(inLen * sizeof(char));
    
    if (typeform && spacing) {
        for (size_t i = 0; i < inLen; i++) {
            typeform[i] = (formtype)(Data[i] & 0xFF);
            spacing[i] = (char)(Data[i] & 0x7F);
        }
    }

    // Try different modes
    int mode = Data[0] % 4;
    
    // Use a known table or the query if valid
    const char *tableList = "en-gb:computer";
    if (exists) {
        tableList = query;
    }

    int result = lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
    (void)result;

    // Fuzz lou_readCharFromFile
    int modeFile = 1;
    int ch = lou_readCharFromFile("./dummy_file", &modeFile);
    (void)ch;
    
    // Try reading again with mode 0
    modeFile = 0;
    ch = lou_readCharFromFile("./dummy_file", &modeFile);
    (void)ch;

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);

    return 0;
}
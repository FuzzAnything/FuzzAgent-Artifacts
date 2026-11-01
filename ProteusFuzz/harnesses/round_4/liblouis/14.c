// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "liblouis.h"

static void init_buffers(widechar *buf, int *len, size_t max_len, const uint8_t *data, size_t size) {
    if (buf == NULL || len == NULL) return;
    *len = 0;
    if (size == 0 || max_len == 0) return;
    size_t copy_len = (size < max_len - 1) ? size : max_len - 1;
    for (size_t i = 0; i < copy_len; i++) {
        buf[i] = (widechar)data[i];
    }
    buf[copy_len] = 0;
    *len = (int)copy_len;
}

static void create_dummy_table(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "output: en-gb\n");
        fprintf(f, "format: en-gb\n");
        fprintf(f, "rule: . .\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const int MAX_LEN = 4096;
    const int MAX_TABLES = 10;
    
    widechar inbuf[MAX_LEN];
    widechar outbuf[MAX_LEN];
    formtype typeform[MAX_LEN];
    char spacing[MAX_LEN];
    int outputPos[MAX_LEN];
    int inputPos[MAX_LEN];
    int cursorPos = 0;
    char inputHyphens[MAX_LEN];
    char outputHyphens[MAX_LEN];
    
    int inlen = MAX_LEN;
    int outlen = MAX_LEN;
    
    // Initialize buffers
    memset(inbuf, 0, sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(typeform, 0, sizeof(typeform));
    memset(spacing, 0, sizeof(spacing));
    memset(outputPos, 0, sizeof(outputPos));
    memset(inputPos, 0, sizeof(inputPos));
    memset(inputHyphens, 0, sizeof(inputHyphens));
    memset(outputHyphens, 0, sizeof(outputHyphens));

    // Prepare input buffer from fuzz data
    init_buffers(inbuf, &inlen, MAX_LEN, Data, Size);

    // Prepare table list from fuzz data (ensure null termination)
    char tableList[MAX_LEN];
    size_t tableLen = (Size < MAX_LEN - 1) ? Size : MAX_LEN - 1;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    // Ensure tableList is not empty for functions requiring it
    if (tableList[0] == '\0') {
        tableList[0] = 'e';
        tableList[1] = 'n';
        tableList[2] = '-';
        tableList[3] = 'g';
        tableList[4] = 'b';
        tableList[5] = '\0';
    }

    // Test lou_checkTable
    lou_checkTable(tableList);

    // Test lou_compileString
    char compileString[MAX_LEN];
    memcpy(compileString, Data, (Size < MAX_LEN - 1) ? Size : MAX_LEN - 1);
    compileString[(Size < MAX_LEN - 1) ? Size : MAX_LEN - 1] = '\0';
    lou_compileString(tableList, compileString);

    // Test lou_indexTables
    const char *tables[MAX_TABLES];
    int tableCount = 0;
    char *token = strtok(tableList, ",");
    while (token != NULL && tableCount < MAX_TABLES - 1) {
        tables[tableCount++] = token;
        token = strtok(NULL, ",");
    }
    tables[tableCount] = NULL;
    if (tableCount > 0) {
        lou_indexTables(tables);
    }

    // Test lou_translate
    int cursorPosVal = (Size > 0) ? Data[0] % (inlen + 1) : 0;
    int mode = (Size > 1) ? Data[1] % 10 : 0;
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPosVal, mode);

    // Test lou_translateString
    inlen = MAX_LEN;
    outlen = MAX_LEN;
    init_buffers(inbuf, &inlen, MAX_LEN, Data, Size);
    mode = (Size > 2) ? Data[2] % 10 : 0;
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);

    // Test lou_translatePrehyphenated
    inlen = MAX_LEN;
    outlen = MAX_LEN;
    init_buffers(inbuf, &inlen, MAX_LEN, Data, Size);
    cursorPosVal = (Size > 3) ? Data[3] % (inlen + 1) : 0;
    mode = (Size > 4) ? Data[4] % 10 : 0;
    // Prepare hyphen arrays
    for (int i = 0; i < inlen; i++) {
        inputHyphens[i] = (i < Size) ? (char)(Data[i] % 2 ? '-' : ' ') : ' ';
    }
    lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, outputPos, inputPos, &cursorPosVal, inputHyphens, outputHyphens, mode);

    return 0;
}
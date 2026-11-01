// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void logCallback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    lou_registerLogCallback(logCallback);

    const char *tableList = (const char *)Data;
    size_t tableLen = Size;
    char *safeTable = NULL;
    
    if (tableLen > 0 && tableList[tableLen - 1] != '\0') {
        safeTable = malloc(tableLen + 1);
        if (!safeTable) return 0;
        memcpy(safeTable, Data, tableLen);
        safeTable[tableLen] = '\0';
        tableList = safeTable;
    }

    int checkResult = lou_checkTable(tableList);
    (void)checkResult;

    lou_free();

    lou_free();

    if (Size < 2) {
        if (safeTable) free(safeTable);
        return 0;
    }

    size_t inLen = Size - 1;
    widechar *inbuf = malloc(inLen * sizeof(widechar));
    if (!inbuf) {
        if (safeTable) free(safeTable);
        return 0;
    }

    for (size_t i = 0; i < inLen; i++) {
        inbuf[i] = (widechar)(Data[i] & 0x7F);
    }

    int inputLength = (int)inLen;
    int outputLength = (int)(inLen * 2);
    widechar *outbuf = malloc(outputLength * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        if (safeTable) free(safeTable);
        return 0;
    }

    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;

    int backTranslateResult = lou_backTranslateString(
        tableList,
        inbuf,
        &inputLength,
        outbuf,
        &outputLength,
        typeform,
        spacing,
        mode
    );
    (void)backTranslateResult;

    lou_free();

    free(inbuf);
    free(outbuf);
    if (safeTable) free(safeTable);

    return 0;
}
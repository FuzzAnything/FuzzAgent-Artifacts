// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>

static void log_callback(logLevels level, const char *message) {
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    lou_registerLogCallback(log_callback);

    const char *tableList = (const char *)Data;
    size_t tableLen = Size;
    
    char *safeTableList = NULL;
    if (tableList[tableLen - 1] != '\0') {
        safeTableList = malloc(tableLen + 1);
        if (!safeTableList) return 0;
        memcpy(safeTableList, Data, tableLen);
        safeTableList[tableLen] = '\0';
        tableList = safeTableList;
    }

    int checkResult = lou_checkTable(tableList);
    (void)checkResult;

    lou_free();
    lou_free();

    if (Size < 2) {
        if (safeTableList) free(safeTableList);
        return 0;
    }

    size_t inBufSize = Size - 1;
    widechar *inbuf = malloc(inBufSize * sizeof(widechar));
    if (!inbuf) {
        if (safeTableList) free(safeTableList);
        return 0;
    }

    for (size_t i = 0; i < inBufSize; i++) {
        inbuf[i] = (widechar)Data[i + 1];
    }

    int inlen = (int)inBufSize;
    widechar *outbuf = malloc(inBufSize * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        if (safeTableList) free(safeTableList);
        return 0;
    }
    int outlen = (int)inBufSize;

    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;

    int translateResult = lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing, mode);
    (void)translateResult;

    lou_free();

    free(inbuf);
    free(outbuf);
    if (safeTableList) free(safeTableList);

    return 0;
}
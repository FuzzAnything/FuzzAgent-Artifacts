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

static void logCallback(logLevels level, const char *message) {
    (void)level;
    (void)message;
}

static void writeDummyFile(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for testing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    writeDummyFile();

    lou_registerLogCallback(logCallback);

    char tableList[256];
    size_t tableSize = Size < sizeof(tableList) - 1 ? Size : sizeof(tableList) - 1;
    memcpy(tableList, Data, tableSize);
    tableList[tableSize] = '\0';

    if (tableSize == 0) {
        strcpy(tableList, "en-ga");
    }

    int checkResult = lou_checkTable(tableList);
    (void)checkResult;

    lou_free();

    lou_free();

    widechar *inbuf = NULL;
    widechar *outbuf = NULL;
    int inlen = 0;
    int outlen = 0;

    if (Size > 0) {
        inbuf = (widechar *)malloc(Size * sizeof(widechar));
        if (!inbuf) {
            return 0;
        }
        for (size_t i = 0; i < Size; i++) {
            inbuf[i] = (widechar)Data[i];
        }
        inlen = (int)Size;
    }

    if (inbuf) {
        outbuf = (widechar *)malloc((Size + 1) * sizeof(widechar));
        if (!outbuf) {
            free(inbuf);
            return 0;
        }
        outlen = (int)(Size + 1);
    }

    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;

    if (inbuf && outbuf) {
        int translateResult = lou_translateString(
            tableList,
            inbuf, &inlen,
            outbuf, &outlen,
            typeform, spacing, mode
        );
        (void)translateResult;
    }

    if (inbuf) {
        free(inbuf);
    }
    if (outbuf) {
        free(outbuf);
    }

    lou_free();

    return 0;
}
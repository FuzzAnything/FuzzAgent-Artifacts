// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void fuzz_lou_readCharFromFile(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    int mode = 1;
    lou_readCharFromFile("./dummy_file", &mode);
    
    remove("./dummy_file");
}

static void fuzz_lou_hyphenate(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t table_len = data[0] % 32 + 1;
    if (table_len + 1 > size) return;
    
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return;
    
    memcpy(tableList, data + 1, table_len);
    tableList[table_len] = '\0';
    
    size_t remaining = size - table_len - 1;
    if (remaining < 2) {
        free(tableList);
        return;
    }
    
    int inlen = (data[table_len + 1] % 64) + 1;
    if (inlen * sizeof(widechar) > remaining - 1) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    memcpy(inbuf, data + table_len + 2, inlen * sizeof(widechar));
    
    char *hyphens = (char *)malloc(inlen + 1);
    if (!hyphens) {
        free(tableList);
        free(inbuf);
        return;
    }
    
    int mode = (data[table_len + 1 + inlen * sizeof(widechar)] % 2);
    lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
    
    free(tableList);
    free(inbuf);
    free(hyphens);
}

static void fuzz_lou_translate(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t table_len = data[0] % 32 + 1;
    if (table_len + 1 > size) return;
    
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return;
    
    memcpy(tableList, data + 1, table_len);
    tableList[table_len] = '\0';
    
    size_t remaining = size - table_len - 1;
    if (remaining < 4) {
        free(tableList);
        return;
    }
    
    int inlen = (data[table_len + 1] % 128) + 1;
    int outlen = (data[table_len + 2] % 256) + 1;
    
    if (inlen * sizeof(widechar) > remaining - 2) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    memcpy(inbuf, data + table_len + 3, inlen * sizeof(widechar));
    
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(tableList);
        free(inbuf);
        return;
    }
    
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    if (typeform) {
        memset(typeform, 0, outlen * sizeof(formtype));
    }
    
    char *spacing = (char *)malloc(inlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = (data[table_len + 3 + inlen * sizeof(widechar)] % inlen);
    int mode = (data[table_len + 4 + inlen * sizeof(widechar)] % 4);
    
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, typeform, spacing,
                  outputPos, inputPos, &cursorPos, mode);
    
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
}

static void fuzz_lou_charSize(void) {
    lou_charSize();
}

static void fuzz_lou_translatePrehyphenated(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t table_len = data[0] % 32 + 1;
    if (table_len + 1 > size) return;
    
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return;
    
    memcpy(tableList, data + 1, table_len);
    tableList[table_len] = '\0';
    
    size_t remaining = size - table_len - 1;
    if (remaining < 4) {
        free(tableList);
        return;
    }
    
    int inlen = (data[table_len + 1] % 128) + 1;
    int outlen = (data[table_len + 2] % 256) + 1;
    
    if (inlen * sizeof(widechar) > remaining - 2) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    memcpy(inbuf, data + table_len + 3, inlen * sizeof(widechar));
    
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(tableList);
        free(inbuf);
        return;
    }
    
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    if (typeform) {
        memset(typeform, 0, outlen * sizeof(formtype));
    }
    
    char *spacing = (char *)malloc(inlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = (data[table_len + 3 + inlen * sizeof(widechar)] % inlen);
    
    char *inputHyphens = (char *)malloc(inlen + 1);
    char *outputHyphens = (char *)malloc(outlen + 1);
    
    int mode = (data[table_len + 4 + inlen * sizeof(widechar)] % 4);
    
    lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen,
                               typeform, spacing, outputPos, inputPos,
                               &cursorPos, inputHyphens, outputHyphens, mode);
    
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(inputHyphens);
    free(outputHyphens);
}

static void fuzz_lou_translateString(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t table_len = data[0] % 32 + 1;
    if (table_len + 1 > size) return;
    
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return;
    
    memcpy(tableList, data + 1, table_len);
    tableList[table_len] = '\0';
    
    size_t remaining = size - table_len - 1;
    if (remaining < 4) {
        free(tableList);
        return;
    }
    
    int inlen = (data[table_len + 1] % 128) + 1;
    int outlen = (data[table_len + 2] % 256) + 1;
    
    if (inlen * sizeof(widechar) > remaining - 2) {
        free(tableList);
        return;
    }
    
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return;
    }
    
    memcpy(inbuf, data + table_len + 3, inlen * sizeof(widechar));
    
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(tableList);
        free(inbuf);
        return;
    }
    
    formtype *typeform = (formtype *)malloc(inlen * sizeof(formtype));
    char *spacing = (char *)malloc((inlen + 1) * sizeof(char));
    int mode = (data[table_len + 3 + inlen * sizeof(widechar)] % 4);
    
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen,
                        typeform, spacing, mode);
    
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_lou_readCharFromFile(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_lou_hyphenate(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_lou_translate(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_lou_charSize();
            break;
        case 4:
            fuzz_lou_translatePrehyphenated(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_lou_translateString(Data + 1, Size - 1);
            break;
    }
    
    return 0;
}
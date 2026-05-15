// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void fuzz_lou_backTranslateString(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int mode = data[0] & 0x0F;
    size_t input_size = size - 1;
    if (input_size > 1024) input_size = 1024;
    
    int inlen = (int)input_size;
    int outlen = inlen * 4;
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    char *spacing = (char *)malloc(outlen * sizeof(char));
    
    if (!inbuf || !outbuf || !typeform || !spacing) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        return;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        inbuf[i] = (widechar)data[i + 1];
    }
    memset(typeform, 0, outlen * sizeof(formtype));
    memset(spacing, 0, outlen * sizeof(char));
    
    const char *tableList = "en-us-g2.ctb";
    
    lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                           typeform, spacing, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing);
}

static void fuzz_lou_readCharFromFile(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    int mode = 1;
    while (1) {
        int ch = lou_readCharFromFile("./dummy_file", &mode);
        if (ch == EOF) break;
    }
    
    remove("./dummy_file");
}

static void fuzz_lou_translatePrehyphenated(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int mode = data[0] & 0x0F;
    size_t input_size = size - 1;
    if (input_size > 1024) input_size = 1024;
    
    int inlen = (int)input_size;
    int outlen = inlen * 4;
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    char *spacing = (char *)malloc(outlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;
    char *inputHyphens = (char *)malloc((inlen + 1) * sizeof(char));
    char *outputHyphens = (char *)malloc((outlen + 1) * sizeof(char));
    
    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos || 
        !inputHyphens || !outputHyphens) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        free(outputPos); free(inputPos); free(inputHyphens); free(outputHyphens);
        return;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        inbuf[i] = (widechar)data[i + 1];
    }
    memset(typeform, 0, outlen * sizeof(formtype));
    memset(spacing, 0, outlen * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));
    memset(inputHyphens, 0, (inlen + 1) * sizeof(char));
    memset(outputHyphens, 0, (outlen + 1) * sizeof(char));
    
    const char *tableList = "en-us-g2.ctb";
    
    lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen, 
                              typeform, spacing, outputPos, inputPos, &cursorPos,
                              inputHyphens, outputHyphens, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing);
    free(outputPos); free(inputPos); free(inputHyphens); free(outputHyphens);
}

static void fuzz_lou_translateString(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int mode = data[0] & 0x0F;
    size_t input_size = size - 1;
    if (input_size > 1024) input_size = 1024;
    
    int inlen = (int)input_size;
    int outlen = inlen * 4;
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(inlen * sizeof(formtype));
    char *spacing = (char *)malloc((inlen + 1) * sizeof(char));
    
    if (!inbuf || !outbuf || !typeform || !spacing) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        return;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        inbuf[i] = (widechar)data[i + 1];
    }
    memset(typeform, 0, inlen * sizeof(formtype));
    memset(spacing, 0, (inlen + 1) * sizeof(char));
    
    const char *tableList = "en-us-g2.ctb";
    
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                       typeform, spacing, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing);
}

static void fuzz_lou_translate(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int mode = data[0] & 0x0F;
    size_t input_size = size - 1;
    if (input_size > 1024) input_size = 1024;
    
    int inlen = (int)input_size;
    int outlen = inlen * 4;
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
    char *spacing = (char *)malloc(outlen * sizeof(char));
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int cursorPos = 0;
    
    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform); free(spacing);
        free(outputPos); free(inputPos);
        return;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        inbuf[i] = (widechar)data[i + 1];
    }
    memset(typeform, 0, outlen * sizeof(formtype));
    memset(spacing, 0, outlen * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));
    
    const char *tableList = "en-us-g2.ctb";
    
    lou_translate(tableList, inbuf, &inlen, outbuf, &outlen, 
                 typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing);
    free(outputPos); free(inputPos);
}

static void fuzz_lou_hyphenate(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    int mode = data[0] & 0x0F;
    size_t input_size = size - 1;
    if (input_size > 1024) input_size = 1024;
    
    int inlen = (int)input_size;
    widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
    char *hyphens = (char *)malloc((inlen + 1) * sizeof(char));
    
    if (!inbuf || !hyphens) {
        free(inbuf); free(hyphens);
        return;
    }
    
    for (size_t i = 0; i < input_size; i++) {
        inbuf[i] = (widechar)data[i + 1];
    }
    memset(hyphens, 0, (inlen + 1) * sizeof(char));
    
    const char *tableList = "en-us-g2.ctb";
    
    lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
    
    free(inbuf); free(hyphens);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    unsigned char selector = data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_lou_backTranslateString(data, size);
            break;
        case 1:
            fuzz_lou_readCharFromFile(data, size);
            break;
        case 2:
            fuzz_lou_translatePrehyphenated(data, size);
            break;
        case 3:
            fuzz_lou_translateString(data, size);
            break;
        case 4:
            fuzz_lou_translate(data, size);
            break;
        case 5:
            fuzz_lou_hyphenate(data, size);
            break;
    }
    
    return 0;
}
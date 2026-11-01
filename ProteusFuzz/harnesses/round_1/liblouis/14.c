// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
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

#define MAX_SIZE 4096

static int fuzz_dotsToChar(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    int length = data[0] % 100 + 1;
    if (length > MAX_SIZE) length = MAX_SIZE;
    
    widechar *inbuf = malloc(length * sizeof(widechar));
    widechar *outbuf = malloc(length * sizeof(widechar));
    
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        return 0;
    }
    
    for (int i = 0; i < length; i++) {
        inbuf[i] = (i < size - 1) ? data[i + 1] : 0;
        outbuf[i] = 0;
    }
    
    int mode = data[size - 1] % 4;
    int result = lou_dotsToChar("en-ga-8", inbuf, outbuf, length, mode);
    
    free(inbuf);
    free(outbuf);
    return result;
}

static int fuzz_charToDots(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    int length = data[0] % 100 + 1;
    if (length > MAX_SIZE) length = MAX_SIZE;
    
    widechar *inbuf = malloc(length * sizeof(widechar));
    widechar *outbuf = malloc(length * sizeof(widechar));
    
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        return 0;
    }
    
    for (int i = 0; i < length; i++) {
        inbuf[i] = (i < size - 1) ? data[i + 1] : 0;
        outbuf[i] = 0;
    }
    
    int mode = data[size - 1] % 4;
    int result = lou_charToDots("en-ga-8", inbuf, outbuf, length, mode);
    
    free(inbuf);
    free(outbuf);
    return result;
}

static int fuzz_translatePrehyphenated(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    int inlen = data[0] % 100 + 1;
    int outlen = data[1] % 100 + 1;
    if (inlen > MAX_SIZE) inlen = MAX_SIZE;
    if (outlen > MAX_SIZE) outlen = MAX_SIZE;
    
    widechar *inbuf = malloc(inlen * sizeof(widechar));
    widechar *outbuf = malloc(outlen * sizeof(widechar));
    formtype *typeform = malloc(inlen * sizeof(formtype));
    char *spacing = malloc(inlen * sizeof(char));
    int *outputPos = malloc(inlen * sizeof(int));
    int *inputPos = malloc(outlen * sizeof(int));
    char *inputHyphens = malloc(inlen * sizeof(char));
    char *outputHyphens = malloc(outlen * sizeof(char));
    
    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos || 
        !inputHyphens || !outputHyphens) {
        free(inbuf); free(outbuf); free(typeform); free(spacing); 
        free(outputPos); free(inputPos); free(inputHyphens); free(outputHyphens);
        return 0;
    }
    
    memset(inbuf, 0, inlen * sizeof(widechar));
    memset(outbuf, 0, outlen * sizeof(widechar));
    memset(typeform, 0, inlen * sizeof(formtype));
    memset(spacing, 0, inlen * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));
    memset(inputHyphens, 0, inlen * sizeof(char));
    memset(outputHyphens, 0, outlen * sizeof(char));
    
    for (int i = 0; i < inlen && i < size - 3; i++) {
        inbuf[i] = data[i + 2];
    }
    
    int cursorPos = data[size - 1] % inlen;
    int mode = data[size - 2] % 4;
    
    int result = lou_translatePrehyphenated("en-ga-8", inbuf, &inlen, outbuf, &outlen, 
                                            typeform, spacing, outputPos, inputPos, 
                                            &cursorPos, inputHyphens, outputHyphens, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing); 
    free(outputPos); free(inputPos); free(inputHyphens); free(outputHyphens);
    return result;
}

static int fuzz_backTranslate(const uint8_t *data, size_t size) {
    if (size < 3) return 0;
    
    int inlen = data[0] % 100 + 1;
    int outlen = data[1] % 100 + 1;
    if (inlen > MAX_SIZE) inlen = MAX_SIZE;
    if (outlen > MAX_SIZE) outlen = MAX_SIZE;
    
    widechar *inbuf = malloc(inlen * sizeof(widechar));
    widechar *outbuf = malloc(outlen * sizeof(widechar));
    formtype *typeform = malloc(inlen * sizeof(formtype));
    char *spacing = malloc(inlen * sizeof(char));
    int *outputPos = malloc(inlen * sizeof(int));
    int *inputPos = malloc(outlen * sizeof(int));
    
    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform); free(spacing); 
        free(outputPos); free(inputPos);
        return 0;
    }
    
    memset(inbuf, 0, inlen * sizeof(widechar));
    memset(outbuf, 0, outlen * sizeof(widechar));
    memset(typeform, 0, inlen * sizeof(formtype));
    memset(spacing, 0, inlen * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));
    
    for (int i = 0; i < inlen && i < size - 2; i++) {
        inbuf[i] = data[i + 2];
    }
    
    int cursorPos = data[size - 1] % inlen;
    int mode = data[size - 2] % 4;
    
    int result = lou_backTranslate("en-ga-8", inbuf, &inlen, outbuf, &outlen, 
                                   typeform, spacing, outputPos, inputPos, 
                                   &cursorPos, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing); 
    free(outputPos); free(inputPos);
    return result;
}

static int fuzz_translate(const uint8_t *data, size_t size) {
    if (size < 3) return 0;
    
    int inlen = data[0] % 100 + 1;
    int outlen = data[1] % 100 + 1;
    if (inlen > MAX_SIZE) inlen = MAX_SIZE;
    if (outlen > MAX_SIZE) outlen = MAX_SIZE;
    
    widechar *inbuf = malloc(inlen * sizeof(widechar));
    widechar *outbuf = malloc(outlen * sizeof(widechar));
    formtype *typeform = malloc(inlen * sizeof(formtype));
    char *spacing = malloc(inlen * sizeof(char));
    int *outputPos = malloc(inlen * sizeof(int));
    int *inputPos = malloc(outlen * sizeof(int));
    
    if (!inbuf || !outbuf || !typeform || !spacing || !outputPos || !inputPos) {
        free(inbuf); free(outbuf); free(typeform); free(spacing); 
        free(outputPos); free(inputPos);
        return 0;
    }
    
    memset(inbuf, 0, inlen * sizeof(widechar));
    memset(outbuf, 0, outlen * sizeof(widechar));
    memset(typeform, 0, inlen * sizeof(formtype));
    memset(spacing, 0, inlen * sizeof(char));
    memset(outputPos, 0, inlen * sizeof(int));
    memset(inputPos, 0, outlen * sizeof(int));
    
    for (int i = 0; i < inlen && i < size - 2; i++) {
        inbuf[i] = data[i + 2];
    }
    
    int cursorPos = data[size - 1] % inlen;
    int mode = data[size - 2] % 4;
    
    int result = lou_translate("en-ga-8", inbuf, &inlen, outbuf, &outlen, 
                               typeform, spacing, outputPos, inputPos, 
                               &cursorPos, mode);
    
    free(inbuf); free(outbuf); free(typeform); free(spacing); 
    free(outputPos); free(inputPos);
    return result;
}

static int fuzz_hyphenate(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    int inlen = data[0] % 100 + 1;
    if (inlen > MAX_SIZE) inlen = MAX_SIZE;
    
    widechar *inbuf = malloc(inlen * sizeof(widechar));
    char *hyphens = malloc(inlen * sizeof(char));
    
    if (!inbuf || !hyphens) {
        free(inbuf);
        free(hyphens);
        return 0;
    }
    
    memset(inbuf, 0, inlen * sizeof(widechar));
    memset(hyphens, 0, inlen * sizeof(char));
    
    for (int i = 0; i < inlen && i < size - 1; i++) {
        inbuf[i] = data[i + 1];
    }
    
    int mode = data[size - 1] % 2;
    int result = lou_hyphenate("en-ga-8", inbuf, inlen, hyphens, mode);
    
    free(inbuf);
    free(hyphens);
    return result;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    int func = Data[0] % 6;
    
    switch (func) {
        case 0:
            fuzz_dotsToChar(Data, Size);
            break;
        case 1:
            fuzz_charToDots(Data, Size);
            break;
        case 2:
            fuzz_translatePrehyphenated(Data, Size);
            break;
        case 3:
            fuzz_backTranslate(Data, Size);
            break;
        case 4:
            fuzz_translate(Data, Size);
            break;
        case 5:
            fuzz_hyphenate(Data, Size);
            break;
    }
    
    return 0;
}
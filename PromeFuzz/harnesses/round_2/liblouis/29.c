// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Use first byte to select function
    uint8_t function_selector = Data[0] % 6;
    // Use second byte for mode
    int mode = Data[1];
    
    // Fixed table list for simplicity
    const char *tableList = "en-us-g2.ctb";
    
    // Calculate input length (widechar = 2 bytes)
    size_t widechar_count = (Size - 2) / 2;
    if (widechar_count > MAX_INPUT_SIZE) {
        widechar_count = MAX_INPUT_SIZE;
    }
    
    // Allocate and initialize input buffer
    widechar *inbuf = (widechar *)malloc(MAX_INPUT_SIZE * sizeof(widechar));
    if (!inbuf) return 0;
    
    // Convert data to widechar (simple copy)
    for (size_t i = 0; i < widechar_count; i++) {
        size_t data_idx = 2 + i * 2;
        if (data_idx + 1 < Size) {
            inbuf[i] = (Data[data_idx] << 8) | Data[data_idx + 1];
        } else {
            inbuf[i] = 0;
        }
    }
    
    int inlen = (int)widechar_count;
    int outlen = MAX_OUTPUT_SIZE;
    widechar *outbuf = (widechar *)malloc(MAX_OUTPUT_SIZE * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }
    
    // Common parameters
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int *cursorPos = NULL;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;
    char *hyphens = NULL;
    
    // Allocate optional arrays based on selector bits
    uint8_t alloc_flags = Data[0];
    
    switch (function_selector) {
        case 0: { // lou_translateString
            if (alloc_flags & 0x01) {
                typeform = (formtype *)malloc(inlen * sizeof(formtype));
                if (typeform) memset(typeform, 0, inlen * sizeof(formtype));
            }
            if (alloc_flags & 0x02) {
                spacing = (char *)malloc((inlen + 1) * sizeof(char));
                if (spacing) memset(spacing, 0, (inlen + 1) * sizeof(char));
            }
            lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                              typeform, spacing, mode);
            break;
        }
        
        case 1: { // lou_translatePrehyphenated
            if (alloc_flags & 0x01) {
                typeform = (formtype *)malloc(inlen * sizeof(formtype));
                if (typeform) memset(typeform, 0, inlen * sizeof(formtype));
            }
            if (alloc_flags & 0x02) {
                spacing = (char *)malloc(inlen * sizeof(char));
                if (spacing) memset(spacing, 0, inlen * sizeof(char));
            }
            if (alloc_flags & 0x04) {
                outputPos = (int *)malloc(inlen * sizeof(int));
                inputPos = (int *)malloc(outlen * sizeof(int));
                cursorPos = (int *)malloc(sizeof(int));
                if (cursorPos) *cursorPos = 0;
            }
            if (alloc_flags & 0x08) {
                inputHyphens = (char *)malloc(inlen * sizeof(char));
                outputHyphens = (char *)malloc(outlen * sizeof(char));
                if (inputHyphens && outputHyphens) {
                    memset(inputHyphens, 0, inlen * sizeof(char));
                    memset(outputHyphens, 0, outlen * sizeof(char));
                }
            }
            lou_translatePrehyphenated(tableList, inbuf, &inlen, outbuf, &outlen,
                                     typeform, spacing, outputPos, inputPos,
                                     cursorPos, inputHyphens, outputHyphens, mode);
            break;
        }
        
        case 2: { // lou_translate
            if (alloc_flags & 0x01) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                if (typeform) memset(typeform, 0, outlen * sizeof(formtype));
            }
            if (alloc_flags & 0x02) {
                spacing = (char *)malloc(inlen * sizeof(char));
                if (spacing) memset(spacing, 0, inlen * sizeof(char));
            }
            if (alloc_flags & 0x04) {
                outputPos = (int *)malloc(inlen * sizeof(int));
                inputPos = (int *)malloc(outlen * sizeof(int));
                cursorPos = (int *)malloc(sizeof(int));
                if (cursorPos) *cursorPos = 0;
            }
            lou_translate(tableList, inbuf, &inlen, outbuf, &outlen,
                         typeform, spacing, outputPos, inputPos, cursorPos, mode);
            break;
        }
        
        case 3: { // lou_hyphenate
            hyphens = (char *)malloc(inlen * sizeof(char));
            if (hyphens) {
                memset(hyphens, 0, inlen * sizeof(char));
                lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
            }
            break;
        }
        
        case 4: { // lou_getTable
            lou_getTable(tableList);
            break;
        }
        
        case 5: { // lou_backTranslateString
            if (alloc_flags & 0x01) {
                typeform = (formtype *)malloc(outlen * sizeof(formtype));
                if (typeform) memset(typeform, 0, outlen * sizeof(formtype));
            }
            if (alloc_flags & 0x02) {
                spacing = (char *)malloc(outlen * sizeof(char));
                if (spacing) memset(spacing, 0, outlen * sizeof(char));
            }
            lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                  typeform, spacing, mode);
            break;
        }
    }
    
    // Cleanup
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(inputHyphens);
    free(outputHyphens);
    free(hyphens);
    
    return 0;
}
// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_hyphenate at lou_translateString.c:4066:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract table list string (null-terminated)
    size_t table_len = Size > 100 ? 100 : Size;
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return 0;
    memcpy(tableList, Data, table_len);
    tableList[table_len] = 0;
    
    // Use remaining data for input buffer (as widechar)
    size_t widechar_count = (Size - table_len) / sizeof(widechar);
    if (widechar_count > 1000) widechar_count = 1000;
    widechar *inbuf = (widechar *)malloc((widechar_count + 1) * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return 0;
    }
    if (widechar_count > 0) {
        memcpy(inbuf, Data + table_len, widechar_count * sizeof(widechar));
    }
    inbuf[widechar_count] = 0;
    
    int inlen = (int)widechar_count;
    int outlen = inlen * 4 + 10; // Reasonable output buffer size
    if (outlen < 10) outlen = 10;
    
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    formtype *typeform = NULL;
    char *spacing = NULL;
    int *outputPos = NULL;
    int *inputPos = NULL;
    int cursorPos = 0;
    char *inputHyphens = NULL;
    char *outputHyphens = NULL;
    
    // Mode from first byte
    int mode = Data[0] & 0xFF;
    
    // Test lou_translateString
    {
        int inlen_copy = inlen;
        int outlen_copy = outlen;
        
        // Sometimes allocate typeform and spacing
        if (Data[1] & 0x01) {
            typeform = (formtype *)malloc(inlen * sizeof(formtype));
            if (typeform) {
                memset(typeform, 0, inlen * sizeof(formtype));
            }
        }
        if (Data[1] & 0x02) {
            spacing = (char *)malloc((inlen + 1) * sizeof(char));
            if (spacing) {
                memset(spacing, 0, (inlen + 1) * sizeof(char));
            }
        }
        
        lou_translateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                           typeform, spacing, mode);
        
        if (typeform) {
            free(typeform);
            typeform = NULL;
        }
        if (spacing) {
            free(spacing);
            spacing = NULL;
        }
    }
    
    // Test lou_hyphenate
    {
        char *hyphens = (char *)malloc((inlen + 1) * sizeof(char));
        if (hyphens) {
            lou_hyphenate(tableList, inbuf, inlen, hyphens, mode);
            free(hyphens);
        }
    }
    
    // Test lou_backTranslateString
    {
        int inlen_copy = inlen;
        int outlen_copy = outlen;
        
        // For back translation, typeform and spacing should be sized for output
        if (Data[1] & 0x04) {
            typeform = (formtype *)malloc(outlen * sizeof(formtype));
            if (typeform) {
                memset(typeform, 0, outlen * sizeof(formtype));
            }
        }
        if (Data[1] & 0x08) {
            spacing = (char *)malloc(outlen * sizeof(char));
            if (spacing) {
                memset(spacing, 0, outlen * sizeof(char));
            }
        }
        
        lou_backTranslateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                               typeform, spacing, mode);
        
        if (typeform) {
            free(typeform);
            typeform = NULL;
        }
        if (spacing) {
            free(spacing);
            spacing = NULL;
        }
    }
    
    // Test lou_readCharFromFile
    {
        write_dummy_file(Data, Size > 1024 ? 1024 : Size);
        int file_mode = 1;
        while (lou_readCharFromFile("./dummy_file", &file_mode) != EOF) {
            // Continue reading until EOF
        }
        remove_dummy_file();
    }
    
    // Test lou_translate
    {
        int inlen_copy = inlen;
        int outlen_copy = outlen;
        
        // Allocate arrays for lou_translate
        if (Data[1] & 0x10) {
            typeform = (formtype *)malloc(outlen * sizeof(formtype));
            if (typeform) {
                memset(typeform, 0, outlen * sizeof(formtype));
            }
        }
        if (Data[1] & 0x20) {
            spacing = (char *)malloc(inlen * sizeof(char));
            if (spacing) {
                memset(spacing, 0, inlen * sizeof(char));
            }
        }
        
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
        cursorPos = inlen > 0 ? (Data[1] % inlen) : 0;
        
        if (outputPos && inputPos) {
            lou_translate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
        }
        
        if (typeform) {
            free(typeform);
            typeform = NULL;
        }
        if (spacing) {
            free(spacing);
            spacing = NULL;
        }
        if (outputPos) {
            free(outputPos);
            outputPos = NULL;
        }
        if (inputPos) {
            free(inputPos);
            inputPos = NULL;
        }
    }
    
    // Test lou_translatePrehyphenated
    {
        int inlen_copy = inlen;
        int outlen_copy = outlen;
        
        // Allocate arrays for lou_translatePrehyphenated
        if (Data[1] & 0x40) {
            typeform = (formtype *)malloc(outlen * sizeof(formtype));
            if (typeform) {
                memset(typeform, 0, outlen * sizeof(formtype));
            }
        }
        if (Data[1] & 0x80) {
            spacing = (char *)malloc(inlen * sizeof(char));
            if (spacing) {
                memset(spacing, 0, inlen * sizeof(char));
            }
        }
        
        outputPos = (int *)malloc(inlen * sizeof(int));
        inputPos = (int *)malloc(outlen * sizeof(int));
        cursorPos = inlen > 0 ? (Data[1] % inlen) : 0;
        
        inputHyphens = (char *)malloc((inlen + 1) * sizeof(char));
        outputHyphens = (char *)malloc((outlen + 1) * sizeof(char));
        
        if (outputPos && inputPos && inputHyphens && outputHyphens) {
            // Initialize hyphen arrays
            memset(inputHyphens, 0, (inlen + 1) * sizeof(char));
            memset(outputHyphens, 0, (outlen + 1) * sizeof(char));
            
            lou_translatePrehyphenated(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                                      typeform, spacing, outputPos, inputPos, &cursorPos,
                                      inputHyphens, outputHyphens, mode);
        }
        
        if (typeform) free(typeform);
        if (spacing) free(spacing);
        if (outputPos) free(outputPos);
        if (inputPos) free(inputPos);
        if (inputHyphens) free(inputHyphens);
        if (outputHyphens) free(outputHyphens);
    }
    
    // Cleanup
    free(tableList);
    free(inbuf);
    free(outbuf);
    
    return 0;
}
// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslate at lou_backTranslateString.c:159:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    /* Split input for different parameters */
    size_t split1 = Size / 4;
    size_t split2 = Size / 2;
    size_t split3 = Size * 3 / 4;

    /* Prepare table list string - ensure null termination */
    char tableList[256];
    size_t tableLen = (split1 < 255) ? split1 : 255;
    memcpy(tableList, Data, tableLen);
    tableList[tableLen] = '\0';

    /* Prepare input buffer for translation functions */
    int inlen = (split2 - split1) / sizeof(widechar);
    if (inlen <= 0) inlen = 1;
    widechar *inbuf = malloc(inlen * sizeof(widechar));
    if (!inbuf) return 0;
    
    size_t copySize = (split2 - split1 < inlen * sizeof(widechar)) ? 
                      split2 - split1 : inlen * sizeof(widechar);
    memcpy(inbuf, Data + split1, copySize);
    
    /* Prepare output buffer */
    int outlen = inlen * 2; /* Reasonable guess for output size */
    widechar *outbuf = malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        return 0;
    }

    /* Prepare typeform and spacing arrays */
    formtype *typeform = NULL;
    char *spacing = NULL;
    
    /* Randomly decide whether to use typeform and spacing */
    if (Data[0] & 0x01) {
        typeform = malloc(inlen * sizeof(formtype));
        if (typeform) {
            size_t tfSize = (split3 - split2 < inlen * sizeof(formtype)) ?
                           split3 - split2 : inlen * sizeof(formtype);
            memcpy(typeform, Data + split2, tfSize);
        }
    }
    
    if (Data[0] & 0x02) {
        spacing = malloc((inlen + 1) * sizeof(char));
        if (spacing) {
            size_t spSize = (Size - split3 < (inlen + 1)) ?
                           Size - split3 : (inlen + 1);
            memcpy(spacing, Data + split3, spSize);
        }
    }

    /* Mode parameter */
    int mode = Data[1];

    /* Test lou_translateString */
    int inlen_copy = inlen;
    int outlen_copy = outlen;
    lou_translateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                       typeform, spacing, mode);

    /* Test lou_backTranslate with additional position arrays */
    int *outputPos = malloc(inlen * sizeof(int));
    int *inputPos = malloc(outlen * sizeof(int));
    int cursorPos = Data[2];
    
    if (outputPos && inputPos) {
        inlen_copy = inlen;
        outlen_copy = outlen;
        lou_backTranslate(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                         typeform, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    free(outputPos);
    free(inputPos);

    /* Test lou_getTypeformForEmphClass */
    char emphClass[256];
    size_t emphLen = (split1 < 255) ? split1 : 255;
    memcpy(emphClass, Data, emphLen);
    emphClass[emphLen] = '\0';
    lou_getTypeformForEmphClass(tableList, emphClass);

    /* Test lou_readCharFromFile */
    write_dummy_file(Data, Size);
    int filemode = 1;
    lou_readCharFromFile("./dummy_file", &filemode);
    cleanup_dummy_file();

    /* Test lou_compileString */
    char compileString[256];
    size_t compileLen = (split2 - split1 < 255) ? split2 - split1 : 255;
    memcpy(compileString, Data + split1, compileLen);
    compileString[compileLen] = '\0';
    lou_compileString(tableList, compileString);

    /* Test lou_backTranslateString */
    inlen_copy = inlen;
    outlen_copy = outlen;
    lou_backTranslateString(tableList, inbuf, &inlen_copy, outbuf, &outlen_copy,
                           typeform, spacing, mode);

    /* Cleanup */
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    
    return 0;
}
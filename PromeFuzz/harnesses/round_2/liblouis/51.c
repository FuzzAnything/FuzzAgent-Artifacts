// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

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

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    unsigned int seed = Data[0];
    size_t offset = 1;
    
    size_t max_len = 256;
    if (Size - offset < max_len) max_len = Size - offset;
    
    size_t table_len = max_len > 0 ? max_len : 1;
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return 0;
    
    if (max_len > 0) {
        memcpy(tableList, Data + offset, table_len);
        offset += table_len;
    } else {
        tableList[0] = 'e';
    }
    tableList[table_len] = '\0';
    
    size_t inbuf_len = max_len / sizeof(widechar);
    if (inbuf_len > 64) inbuf_len = 64;
    
    widechar *inbuf = (widechar *)malloc(inbuf_len * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return 0;
    }
    
    if (Size - offset >= inbuf_len * sizeof(widechar)) {
        memcpy(inbuf, Data + offset, inbuf_len * sizeof(widechar));
        offset += inbuf_len * sizeof(widechar);
    } else {
        for (size_t i = 0; i < inbuf_len; i++) {
            inbuf[i] = (widechar)(seed + i);
        }
    }
    
    lou_checkTable(tableList);
    
    lou_charSize();
    
    lou_getTable(tableList);
    
    if (inbuf_len > 0) {
        widechar *outbuf_dots = (widechar *)malloc(inbuf_len * sizeof(widechar));
        if (outbuf_dots) {
            int mode_dots = seed % 4;
            lou_charToDots(tableList, inbuf, outbuf_dots, (int)inbuf_len, mode_dots);
            free(outbuf_dots);
        }
    }
    
    if (inbuf_len > 0) {
        int inlen = (int)inbuf_len;
        int outlen = inlen * 2;
        widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        formtype *typeform = NULL;
        char *spacing = NULL;
        
        if (outbuf) {
            if (seed % 2 == 0) {
                typeform = (formtype *)malloc(inlen * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < inlen; i++) {
                        typeform[i] = (formtype)((seed + i) % 8);
                    }
                }
            }
            
            if (seed % 3 == 0) {
                spacing = (char *)malloc((inlen + 1) * sizeof(char));
                if (spacing) {
                    memset(spacing, 0, (inlen + 1) * sizeof(char));
                }
            }
            
            int mode = seed % 8;
            lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                              typeform, spacing, mode);
            
            free(typeform);
            free(spacing);
            free(outbuf);
        }
    }
    
    if (Size > offset) {
        write_dummy_file(Data + offset, Size - offset);
        int mode = 1;
        while (lou_readCharFromFile("./dummy_file", &mode) != EOF) {
            if (mode == 0) break;
        }
        remove_dummy_file();
    }
    
    free(inbuf);
    free(tableList);
    
    return 0;
}
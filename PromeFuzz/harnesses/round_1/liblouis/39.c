// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_charSize at compileTranslationTable.c:5425:1 in liblouis.h
// lou_readCharFromFile at compileTranslationTable.c:4352:1 in liblouis.h
#include "config.h"
#include "liblouis.h"

#include <stdint.h>
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

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    uint8_t func_selector = Data[0] % 6;
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;

    switch (func_selector) {
        case 0: {
            if (func_size < 4) break;
            
            size_t table_len = func_data[0] % 32;
            if (table_len > func_size - 1) table_len = func_size - 1;
            
            char *tableList = (char *)malloc(table_len + 1);
            if (!tableList) break;
            memcpy(tableList, func_data + 1, table_len);
            tableList[table_len] = '\0';
            
            size_t remaining = func_size - 1 - table_len;
            if (remaining < 2) {
                free(tableList);
                break;
            }
            
            int inlen = (func_data[1 + table_len] % 128) + 1;
            if (inlen > (remaining - 1) / 2) {
                inlen = (remaining - 1) / 2;
            }
            if (inlen <= 0) {
                free(tableList);
                break;
            }
            
            widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
            if (!inbuf) {
                free(tableList);
                break;
            }
            for (int i = 0; i < inlen; i++) {
                inbuf[i] = (func_data[2 + table_len + i * 2] << 8) | 
                          func_data[2 + table_len + i * 2 + 1];
            }
            
            int outlen = inlen * 4;
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            char *spacing = (char *)malloc(outlen * sizeof(char));
            
            if (outbuf && typeform && spacing) {
                int mode = func_data[func_size - 1] % 8;
                lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                                   typeform, spacing, mode);
            }
            
            free(tableList);
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
        
        case 1: {
            lou_charSize();
            break;
        }
        
        case 2: {
            write_dummy_file(func_data, func_size);
            int mode = 1;
            while (lou_readCharFromFile("./dummy_file", &mode) != EOF) {
            }
            remove_dummy_file();
            break;
        }
        
        case 3: {
            if (func_size < 1) break;
            
            size_t table_len = func_data[0] % 64;
            if (table_len > func_size - 1) table_len = func_size - 1;
            
            char *tableList = (char *)malloc(table_len + 1);
            if (!tableList) break;
            memcpy(tableList, func_data + 1, table_len);
            tableList[table_len] = '\0';
            
            lou_checkTable(tableList);
            free(tableList);
            break;
        }
        
        case 4: {
            if (func_size < 4) break;
            
            size_t table_len = func_data[0] % 32;
            if (table_len > func_size - 1) table_len = func_size - 1;
            
            char *tableList = (char *)malloc(table_len + 1);
            if (!tableList) break;
            memcpy(tableList, func_data + 1, table_len);
            tableList[table_len] = '\0';
            
            size_t remaining = func_size - 1 - table_len;
            if (remaining < 2) {
                free(tableList);
                break;
            }
            
            int length = (func_data[1 + table_len] % 64) + 1;
            if (length > (remaining - 1) / 2) {
                length = (remaining - 1) / 2;
            }
            if (length <= 0) {
                free(tableList);
                break;
            }
            
            widechar *inbuf = (widechar *)malloc(length * sizeof(widechar));
            widechar *outbuf = (widechar *)malloc(length * sizeof(widechar));
            
            if (inbuf && outbuf) {
                for (int i = 0; i < length; i++) {
                    inbuf[i] = (func_data[2 + table_len + i * 2] << 8) | 
                              func_data[2 + table_len + i * 2 + 1];
                }
                int mode = func_data[func_size - 1] % 4;
                lou_charToDots(tableList, inbuf, outbuf, length, mode);
            }
            
            free(tableList);
            free(inbuf);
            free(outbuf);
            break;
        }
        
        case 5: {
            if (func_size < 4) break;
            
            size_t table_len = func_data[0] % 32;
            if (table_len > func_size - 1) table_len = func_size - 1;
            
            char *tableList = (char *)malloc(table_len + 1);
            if (!tableList) break;
            memcpy(tableList, func_data + 1, table_len);
            tableList[table_len] = '\0';
            
            size_t remaining = func_size - 1 - table_len;
            if (remaining < 2) {
                free(tableList);
                break;
            }
            
            int inlen = (func_data[1 + table_len] % 128) + 1;
            if (inlen > (remaining - 1) / 2) {
                inlen = (remaining - 1) / 2;
            }
            if (inlen <= 0) {
                free(tableList);
                break;
            }
            
            widechar *inbuf = (widechar *)malloc(inlen * sizeof(widechar));
            if (!inbuf) {
                free(tableList);
                break;
            }
            for (int i = 0; i < inlen; i++) {
                inbuf[i] = (func_data[2 + table_len + i * 2] << 8) | 
                          func_data[2 + table_len + i * 2 + 1];
            }
            
            int outlen = inlen * 4;
            widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            formtype *typeform = (formtype *)malloc(outlen * sizeof(formtype));
            char *spacing = (char *)malloc(outlen * sizeof(char));
            
            if (outbuf && typeform && spacing) {
                int mode = func_data[func_size - 1] % 8;
                lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                       typeform, spacing, mode);
            }
            
            free(tableList);
            free(inbuf);
            free(outbuf);
            free(typeform);
            free(spacing);
            break;
        }
    }
    
    return 0;
}
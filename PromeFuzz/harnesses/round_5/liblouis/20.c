// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_findTable at metadata.c:1063:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
// lou_freeTableInfo at metadata.c:1167:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "liblouis.h"

static char *read_string(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        return NULL;
    }
    
    size_t len = 0;
    while (len < *size && (*data)[len] != '\0') {
        len++;
    }
    
    char *str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    
    if (len > 0) {
        memcpy(str, *data, len);
    }
    str[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return str;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    uint8_t function_selector = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (function_selector) {
        case 0: {  // lou_freeTableInfo
            char *table = read_string(&remaining_data, &remaining_size);
            char *key = read_string(&remaining_data, &remaining_size);
            
            if (table && key) {
                char *info = lou_getTableInfo(table, key);
                if (info) {
                    lou_freeTableInfo(info);
                }
            }
            
            free(table);
            free(key);
            break;
        }
        
        case 1: {  // lou_getTableInfo
            char *table = read_string(&remaining_data, &remaining_size);
            char *key = read_string(&remaining_data, &remaining_size);
            
            if (table && key) {
                char *info = lou_getTableInfo(table, key);
                if (info) {
                    lou_freeTableInfo(info);
                }
            }
            
            free(table);
            free(key);
            break;
        }
        
        case 2: {  // lou_getTable
            char *table_list = read_string(&remaining_data, &remaining_size);
            
            if (table_list) {
                const void *table = lou_getTable(table_list);
                (void)table;
            }
            
            free(table_list);
            break;
        }
        
        case 3: {  // lou_getEmphClasses
            char *table_list = read_string(&remaining_data, &remaining_size);
            
            if (table_list) {
                char const **classes = lou_getEmphClasses(table_list);
                if (classes) {
                    for (int i = 0; classes[i] != NULL; i++) {
                        // Just access the strings
                    }
                    free(classes);
                }
            }
            
            free(table_list);
            break;
        }
        
        case 4: {  // lou_dotsToChar
            char *table_list = read_string(&remaining_data, &remaining_size);
            
            if (table_list && remaining_size >= sizeof(int) * 2) {
                int length = *(int *)remaining_data;
                remaining_data += sizeof(int);
                remaining_size -= sizeof(int);
                
                int mode = *(int *)remaining_data;
                remaining_data += sizeof(int);
                remaining_size -= sizeof(int);
                
                if (length > 0 && length <= 1024 && remaining_size >= sizeof(widechar) * length) {
                    widechar *inbuf = malloc(sizeof(widechar) * length);
                    widechar *outbuf = malloc(sizeof(widechar) * length);
                    
                    if (inbuf && outbuf) {
                        memcpy(inbuf, remaining_data, sizeof(widechar) * length);
                        int result = lou_dotsToChar(table_list, inbuf, outbuf, length, mode);
                        (void)result;
                    }
                    
                    free(inbuf);
                    free(outbuf);
                }
            }
            
            free(table_list);
            break;
        }
        
        case 5: {  // lou_findTable
            FILE *fp = fopen("./dummy_file", "w");
            if (fp) {
                fwrite(Data, 1, Size, fp);
                fclose(fp);
            }
            
            char *query = read_string(&remaining_data, &remaining_size);
            
            if (query) {
                char *table = lou_findTable(query);
                if (table) {
                    free(table);
                }
            }
            
            free(query);
            break;
        }
    }
    
    return 0;
}
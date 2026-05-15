// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTypeformForEmphClass at compileTranslationTable.c:5244:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_listTables at metadata.c:1172:1 in liblouis.h
// lou_getTableInfo at metadata.c:1142:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "liblouis.h"

static void free_string_array(char **array) {
    if (array) {
        for (int i = 0; array[i] != NULL; i++) {
            free(array[i]);
        }
        free(array);
    }
}

static void free_const_string_array(char const **array) {
    if (array) {
        for (int i = 0; array[i] != NULL; i++) {
            free((void *)array[i]);
        }
        free((void *)array);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    // Prepare buffers for string parameters
    char table_buf[256] = {0};
    char key_buf[256] = {0};
    char emph_buf[256] = {0};
    
    // Extract strings from fuzz data
    size_t table_len = fuzz_size > 0 ? (fuzz_data[0] % 255) + 1 : 0;
    if (table_len > 255) table_len = 255;
    if (table_len > fuzz_size) table_len = fuzz_size;
    if (table_len > 0) {
        memcpy(table_buf, fuzz_data, table_len);
        table_buf[table_len] = '\0';
    } else {
        table_buf[0] = '\0';
    }

    size_t key_len = fuzz_size > table_len ? ((fuzz_data[table_len] % 255) + 1) : 0;
    if (key_len > 255) key_len = 255;
    if (key_len > fuzz_size - table_len) key_len = fuzz_size - table_len;
    if (key_len > 0) {
        memcpy(key_buf, fuzz_data + table_len, key_len);
        key_buf[key_len] = '\0';
    } else {
        key_buf[0] = '\0';
    }

    size_t emph_len = fuzz_size > table_len + key_len ? 
                     ((fuzz_data[table_len + key_len] % 255) + 1) : 0;
    if (emph_len > 255) emph_len = 255;
    if (emph_len > fuzz_size - table_len - key_len) 
        emph_len = fuzz_size - table_len - key_len;
    if (emph_len > 0) {
        memcpy(emph_buf, fuzz_data + table_len + key_len, emph_len);
        emph_buf[emph_len] = '\0';
    } else {
        emph_buf[0] = '\0';
    }

    switch (selector) {
        case 0: {
            // Test lou_listTables
            char **tables = lou_listTables();
            if (tables) {
                free_string_array(tables);
            }
            break;
        }
        
        case 1: {
            // Test lou_getTableInfo
            char *info = lou_getTableInfo(table_buf, key_buf);
            if (info) {
                free(info);
            }
            break;
        }
        
        case 2: {
            // Test lou_dotsToChar
            if (fuzz_size > 0) {
                int length = (fuzz_data[0] % 16) + 1;
                if (length * sizeof(widechar) > fuzz_size - 1) {
                    length = (fuzz_size - 1) / sizeof(widechar);
                }
                if (length > 0) {
                    widechar *inbuf = malloc(length * sizeof(widechar));
                    widechar *outbuf = malloc(length * sizeof(widechar));
                    if (inbuf && outbuf) {
                        // Fill inbuf with fuzz data
                        size_t copy_size = length * sizeof(widechar);
                        if (copy_size > fuzz_size - 1) 
                            copy_size = fuzz_size - 1;
                        memcpy(inbuf, fuzz_data + 1, copy_size);
                        
                        int mode = 0; // deprecated
                        lou_dotsToChar(table_buf, inbuf, outbuf, length, mode);
                        
                        free(inbuf);
                        free(outbuf);
                    }
                    if (inbuf) free(inbuf);
                    if (outbuf) free(outbuf);
                }
            }
            break;
        }
        
        case 3: {
            // Test lou_getTable
            const void *table = lou_getTable(table_buf);
            (void)table; // Result not used
            break;
        }
        
        case 4: {
            // Test lou_getTypeformForEmphClass
            formtype typeform = lou_getTypeformForEmphClass(table_buf, emph_buf);
            (void)typeform; // Result not used
            break;
        }
        
        case 5: {
            // Test lou_getEmphClasses
            char const **classes = lou_getEmphClasses(table_buf);
            if (classes) {
                free_const_string_array(classes);
            }
            break;
        }
    }

    return 0;
}
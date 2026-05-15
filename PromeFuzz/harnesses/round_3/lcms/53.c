// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cmsHANDLE create_test_it8(void) {
    return cmsIT8Alloc(NULL);
}

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

static char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) {
        return NULL;
    }
    
    size_t max_len = *size;
    size_t len = 0;
    
    while (len < max_len && (*data)[len] != 0) {
        len++;
    }
    
    char* str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    
    if (len > 0) {
        memcpy(str, *data, len);
    }
    str[len] = '\0';
    
    *data += len;
    *size -= len;
    
    if (*size > 0 && (*data)[0] == 0) {
        (*data)++;
        (*size)--;
    }
    
    return str;
}

static int extract_int(const uint8_t** data, size_t* size) {
    if (*size < sizeof(int)) {
        return 0;
    }
    
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    
    return value;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsHANDLE hIT8 = create_test_it8();
    if (!hIT8) {
        return 0;
    }
    
    const uint8_t* current_data = Data;
    size_t remaining_size = Size;
    
    while (remaining_size > 0) {
        if (remaining_size < 1) break;
        
        uint8_t func_selector = current_data[0] % 6;
        current_data++;
        remaining_size--;
        
        switch (func_selector) {
            case 0: {
                char* key = extract_string(&current_data, &remaining_size);
                char* buffer = extract_string(&current_data, &remaining_size);
                
                if (key && buffer) {
                    cmsIT8SetPropertyUncooked(hIT8, key, buffer);
                }
                
                free(key);
                free(buffer);
                break;
            }
            
            case 1: {
                char* patch = extract_string(&current_data, &remaining_size);
                char* sample = extract_string(&current_data, &remaining_size);
                char* val = extract_string(&current_data, &remaining_size);
                
                if (patch && sample && val) {
                    cmsIT8SetData(hIT8, patch, sample, val);
                }
                
                free(patch);
                free(sample);
                free(val);
                break;
            }
            
            case 2: {
                char* prop = extract_string(&current_data, &remaining_size);
                char* str = extract_string(&current_data, &remaining_size);
                
                if (prop && str) {
                    cmsIT8SetPropertyStr(hIT8, prop, str);
                }
                
                free(prop);
                free(str);
                break;
            }
            
            case 3: {
                char* key = extract_string(&current_data, &remaining_size);
                char* subkey = extract_string(&current_data, &remaining_size);
                char* buffer = extract_string(&current_data, &remaining_size);
                
                if (key && subkey && buffer) {
                    cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
                }
                
                free(key);
                free(subkey);
                free(buffer);
                break;
            }
            
            case 4: {
                char* comment = extract_string(&current_data, &remaining_size);
                
                if (comment) {
                    cmsIT8SetComment(hIT8, comment);
                }
                
                free(comment);
                break;
            }
            
            case 5: {
                int row = extract_int(&current_data, &remaining_size);
                int col = extract_int(&current_data, &remaining_size);
                char* val = extract_string(&current_data, &remaining_size);
                
                if (val) {
                    cmsIT8SetDataRowCol(hIT8, row, col, val);
                }
                
                free(val);
                break;
            }
        }
    }
    
    cleanup_it8(hIT8);
    return 0;
}
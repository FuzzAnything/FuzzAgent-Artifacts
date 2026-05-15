// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_it8(cmsHANDLE hIT8, cmsContext ctx) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHANDLE create_it8_handle(cmsContext *ctx) {
    *ctx = cmsCreateContext(NULL, NULL);
    if (!*ctx) {
        return NULL;
    }
    cmsHANDLE hIT8 = cmsIT8Alloc(*ctx);
    return hIT8;
}

static char* extract_string(const uint8_t **data, size_t *size, size_t max_len) {
    if (*size == 0) {
        return NULL;
    }
    
    size_t len = (*data)[0] % (max_len + 1);
    if (len == 0 || len > *size - 1) {
        len = (*size > 1) ? (*size - 1) : 0;
    }
    
    if (len == 0) {
        (*data)++;
        (*size)--;
        return NULL;
    }
    
    char *str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    
    memcpy(str, *data + 1, len);
    str[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return str;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext ctx = NULL;
    cmsHANDLE hIT8 = create_it8_handle(&ctx);
    if (!hIT8) {
        cleanup_it8(NULL, ctx);
        return 0;
    }
    
    const uint8_t *current_data = Data;
    size_t remaining_size = Size;
    
    while (remaining_size > 4) {
        uint8_t op = current_data[0] % 6;
        current_data++;
        remaining_size--;
        
        switch (op) {
            case 0: {
                int row = (int)(current_data[0] % 100);
                int col = (int)(current_data[1] % 100);
                current_data += 2;
                remaining_size -= 2;
                
                char *val = extract_string(&current_data, &remaining_size, 50);
                if (val) {
                    cmsIT8SetDataRowCol(hIT8, row, col, val);
                    free(val);
                }
                break;
            }
            
            case 1: {
                char *patch = extract_string(&current_data, &remaining_size, 30);
                char *sample = extract_string(&current_data, &remaining_size, 30);
                char *val = extract_string(&current_data, &remaining_size, 50);
                
                if (patch && sample && val) {
                    cmsIT8SetData(hIT8, patch, sample, val);
                }
                
                free(patch);
                free(sample);
                free(val);
                break;
            }
            
            case 2: {
                char **sample_names = NULL;
                int count = cmsIT8EnumDataFormat(hIT8, &sample_names);
                (void)count;
                break;
            }
            
            case 3: {
                char *sample = extract_string(&current_data, &remaining_size, 30);
                if (sample) {
                    cmsIT8SetIndexColumn(hIT8, sample);
                    free(sample);
                }
                break;
            }
            
            case 4: {
                char *patch = extract_string(&current_data, &remaining_size, 30);
                if (patch) {
                    int idx = cmsIT8GetPatchByName(hIT8, patch);
                    (void)idx;
                    free(patch);
                }
                break;
            }
            
            case 5: {
                char *sample = extract_string(&current_data, &remaining_size, 30);
                if (sample) {
                    int idx = cmsIT8FindDataFormat(hIT8, sample);
                    (void)idx;
                    free(sample);
                }
                break;
            }
        }
    }
    
    cleanup_it8(hIT8, ctx);
    return 0;
}
// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size > max_len ? max_len : size;
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static double extract_double(const uint8_t* data, size_t size) {
    if (size >= sizeof(double)) {
        double value;
        memcpy(&value, data, sizeof(double));
        return value;
    }
    
    static double fallback_values[] = {0.0, 1.0, -1.0, 3.14159, 2.71828, 100.0};
    size_t index = size % (sizeof(fallback_values) / sizeof(fallback_values[0]));
    return fallback_values[index];
}

int LLVMFuzzerTestOneInput_59(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    size_t consumed = 0;
    
    while (consumed < Size) {
        uint8_t op = Data[consumed] % 6;
        consumed++;
        
        size_t remaining = Size - consumed;
        if (remaining < 1) break;
        
        size_t str_len = Data[consumed] % 64 + 1;
        if (str_len > remaining - 1) str_len = remaining - 1;
        
        const uint8_t* str_data = Data + consumed + 1;
        size_t str_data_size = str_len;
        
        consumed += 1 + str_len;
        
        char* str1 = create_null_terminated_string(str_data, str_data_size, str_len);
        if (!str1) continue;
        
        remaining = Size - consumed;
        if (remaining > 0) {
            size_t str2_len = Data[consumed] % 32 + 1;
            if (str2_len > remaining - 1) str2_len = remaining - 1;
            
            const uint8_t* str2_data = Data + consumed + 1;
            size_t str2_data_size = str2_len;
            
            consumed += 1 + str2_len;
            
            char* str2 = create_null_terminated_string(str2_data, str2_data_size, str2_len);
            
            remaining = Size - consumed;
            if (remaining > 0 && str2) {
                size_t str3_len = Data[consumed] % 16 + 1;
                if (str3_len > remaining - 1) str3_len = remaining - 1;
                
                const uint8_t* str3_data = Data + consumed + 1;
                size_t str3_data_size = str3_len;
                
                consumed += 1 + str3_len;
                
                char* str3 = create_null_terminated_string(str3_data, str3_data_size, str3_len);
                
                double dbl_val = extract_double(Data + (consumed % Size), 
                                               Size - (consumed % Size));
                
                switch (op) {
                    case 0:
                        cmsIT8SetTableByLabel(hIT8, str1, str2, str3);
                        break;
                    case 1:
                        cmsIT8GetPatchByName(hIT8, str1);
                        break;
                    case 2:
                        if (str2 && str3) {
                            cmsIT8SetDataDbl(hIT8, str1, str2, dbl_val);
                        }
                        break;
                    case 3: {
                        char** sampleNames = NULL;
                        cmsIT8EnumDataFormat(hIT8, &sampleNames);
                        break;
                    }
                    case 4:
                        cmsIT8SetPropertyDbl(hIT8, str1, dbl_val);
                        break;
                    case 5:
                        cmsIT8SetComment(hIT8, str1);
                        break;
                }
                
                if (str3) free(str3);
            }
            
            if (str2) free(str2);
        }
        
        free(str1);
        
        if (consumed >= Size) break;
    }
    
    cmsIT8Free(hIT8);
    return 0;
}
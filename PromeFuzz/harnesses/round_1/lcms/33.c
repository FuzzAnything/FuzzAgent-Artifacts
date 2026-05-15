// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
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

static char *create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) {
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static void free_string_array(char **array, int count) {
    if (!array) return;
    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
    free(array);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Split input data for different parameters
    size_t chunk_size = Size / 7;
    if (chunk_size == 0) chunk_size = 1;
    
    // Prepare strings from input data
    char *type_str = create_null_terminated_string(Data, chunk_size, 100);
    char *key_str = create_null_terminated_string(Data + chunk_size, chunk_size, 100);
    char *subkey_str = create_null_terminated_string(Data + chunk_size * 2, chunk_size, 100);
    char *buffer_str = create_null_terminated_string(Data + chunk_size * 3, chunk_size, 100);
    char *prop_str = create_null_terminated_string(Data + chunk_size * 4, chunk_size, 100);
    char *value_str = create_null_terminated_string(Data + chunk_size * 5, chunk_size, 100);
    
    // Test cmsIT8SetSheetType
    if (type_str) {
        cmsIT8SetSheetType(hIT8, type_str);
    }
    
    // Test cmsIT8SetPropertyMulti
    if (key_str && subkey_str && buffer_str) {
        cmsIT8SetPropertyMulti(hIT8, key_str, subkey_str, buffer_str);
    }
    
    // Test cmsIT8SetPropertyStr
    if (prop_str && value_str) {
        cmsIT8SetPropertyStr(hIT8, prop_str, value_str);
    }
    
    // Test cmsIT8EnumDataFormat
    char **sample_names = NULL;
    int format_count = cmsIT8EnumDataFormat(hIT8, &sample_names);
    if (sample_names && format_count > 0) {
        // Clean up the sample names array if needed
        // Note: According to lcms documentation, the caller should free the array
        // but not the individual strings which are owned by the IT8 handle
        free(sample_names);
    }
    
    // Test cmsIT8GetDataRowCol
    // Use some bytes from input to determine row and col
    int row = (Data[0] << 8) | Data[1];
    int col = (Data[2] << 8) | Data[3];
    const char *cell_data = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)cell_data; // Use the result to avoid unused variable warning
    
    // Test cmsIT8SaveToFile
    // Write some data to dummy file first
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        // Write remaining input data to file
        size_t write_size = Size > chunk_size * 6 ? Size - chunk_size * 6 : 0;
        if (write_size > 0) {
            fwrite(Data + chunk_size * 6, 1, write_size, fp);
        }
        fclose(fp);
        
        // Now save IT8 data to the file
        cmsIT8SaveToFile(hIT8, filename);
        
        // Clean up the dummy file
        remove(filename);
    }
    
    // Clean up
    free(type_str);
    free(key_str);
    free(subkey_str);
    free(buffer_str);
    free(prop_str);
    free(value_str);
    
    cmsIT8Free(hIT8);
    
    return 0;
}
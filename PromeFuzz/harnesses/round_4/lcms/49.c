// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static char *create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    size_t len = size > max_len ? max_len : size;
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;

    // Extract parameters from fuzz data
    size_t offset = 0;
    
    // For string parameters, create null-terminated strings from fuzz data
    size_t str1_len = Size / 4;
    if (str1_len > 100) str1_len = 100;
    char *str1 = create_null_terminated_string(Data + offset, Size - offset, str1_len);
    offset += str1_len;
    
    size_t str2_len = Size / 8;
    if (str2_len > 50) str2_len = 50;
    char *str2 = create_null_terminated_string(Data + offset, Size - offset, str2_len);
    offset += str2_len;
    
    size_t str3_len = Size / 16;
    if (str3_len > 30) str3_len = 30;
    char *str3 = create_null_terminated_string(Data + offset, Size - offset, str3_len);
    
    // Extract numeric parameters
    cmsUInt32Number num1 = 0;
    cmsUInt32Number num2 = 0;
    if (Size >= offset + 8) {
        num1 = *(cmsUInt32Number *)(Data + offset);
        num2 = *(cmsUInt32Number *)(Data + offset + 4);
    }

    // Test cmsIT8SetComment
    if (str1 && str1[0] != '\0') {
        cmsIT8SetComment(hIT8, str1);
    }

    // Test cmsIT8SetTable
    cmsIT8SetTable(hIT8, num1);
    cmsIT8SetTable(hIT8, num1 + 1);
    cmsIT8SetTable(hIT8, 0);

    // Test cmsIT8SetPropertyStr
    if (str2 && str2[0] != '\0' && str3 && str3[0] != '\0') {
        cmsIT8SetPropertyStr(hIT8, str2, str3);
    }

    // Test cmsIT8SetPropertyHex
    cmsIT8SetPropertyHex(hIT8, "TestHex", num2);
    if (str2 && str2[0] != '\0') {
        cmsIT8SetPropertyHex(hIT8, str2, num1);
    }

    // Test cmsIT8SaveToMem
    cmsUInt32Number bytes_needed = 0;
    cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) {
        void *buffer = malloc(bytes_needed);
        if (buffer) {
            cmsIT8SaveToMem(hIT8, buffer, &bytes_needed);
            free(buffer);
        }
    }

    // Test cmsIT8SaveToFile
    write_dummy_file(Data, Size);
    cmsIT8SaveToFile(hIT8, "./dummy_file");

    // Cleanup
    if (str1) free(str1);
    if (str2) free(str2);
    if (str3) free(str3);
    cmsIT8Free(hIT8);
    
    return 0;
}
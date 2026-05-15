// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static const char* get_string_from_data(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) return NULL;
    size_t len = size - *offset;
    if (len > max_len) len = max_len;
    static char buf[256];
    if (len > 255) len = 255;
    memcpy(buf, data + *offset, len);
    buf[len] = '\0';
    *offset += len;
    return buf;
}

static cmsUInt32Number get_uint32_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) return 0;
    cmsUInt32Number val;
    memcpy(&val, data + *offset, 4);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    size_t offset = 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Call functions in various orders based on input
    for (size_t i = 0; i < 10 && offset < Size; i++) {
        uint8_t op = Data[offset++] % 6;
        
        switch (op) {
            case 0: {
                // cmsIT8SaveToFile
                write_dummy_file(Data, Size);
                cmsIT8SaveToFile(hIT8, "./dummy_file");
                break;
            }
            case 1: {
                // cmsIT8SetComment
                const char* comment = get_string_from_data(Data, Size, &offset, 100);
                if (comment && comment[0] != '\0') {
                    cmsIT8SetComment(hIT8, comment);
                }
                break;
            }
            case 2: {
                // cmsIT8SetSheetType
                const char* sheet_type = get_string_from_data(Data, Size, &offset, 50);
                if (sheet_type) {
                    cmsIT8SetSheetType(hIT8, sheet_type);
                }
                break;
            }
            case 3: {
                // cmsIT8SaveToMem
                cmsUInt32Number bytes_needed = 0;
                cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
                if (bytes_needed > 0 && bytes_needed < 1024*1024) {
                    void* buffer = malloc(bytes_needed);
                    if (buffer) {
                        cmsIT8SaveToMem(hIT8, buffer, &bytes_needed);
                        free(buffer);
                    }
                }
                break;
            }
            case 4: {
                // cmsIT8SetTable
                cmsUInt32Number table_num = get_uint32_from_data(Data, Size, &offset) % 10;
                cmsIT8SetTable(hIT8, table_num);
                break;
            }
            case 5: {
                // cmsIT8SetPropertyHex
                const char* prop_name = get_string_from_data(Data, Size, &offset, 50);
                cmsUInt32Number prop_val = get_uint32_from_data(Data, Size, &offset);
                if (prop_name) {
                    cmsIT8SetPropertyHex(hIT8, prop_name, prop_val);
                }
                break;
            }
        }
    }
    
    // Clean up
    cmsIT8Free(hIT8);
    
    return 0;
}
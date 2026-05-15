// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static sqlite3_str* create_test_string(const uint8_t *data, size_t size) {
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str && size > 0) {
        sqlite3_str_append(str, (const char*)data, (int)(size % 256));
    }
    return str;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3_str *str = create_test_string(Data, Size);
    if (!str) return 0;

    uint8_t op = Data[0];
    size_t data_index = 1;

    switch (op % 6) {
        case 0: {
            int err = sqlite3_str_errcode(str);
            (void)err;
            break;
        }
        case 1: {
            char *value = sqlite3_str_value(str);
            (void)value;
            break;
        }
        case 2: {
            int len = sqlite3_str_length(str);
            (void)len;
            break;
        }
        case 3: {
            int n = (data_index < Size) ? (int)Data[data_index] % 256 : 0;
            sqlite3_str_truncate(str, n);
            break;
        }
        case 4: {
            if (data_index + 1 < Size) {
                int n = (int)Data[data_index] % 256;
                char c = (char)Data[data_index + 1];
                sqlite3_str_appendchar(str, n, c);
            }
            break;
        }
        case 5: {
            if (data_index < Size) {
                int n = (int)Data[data_index] % 256;
                size_t remaining = Size - data_index - 1;
                if (remaining > 0) {
                    const char *z = (const char*)(Data + data_index + 1);
                    sqlite3_str_append(str, z, (int)(n % remaining));
                }
            }
            break;
        }
    }

    sqlite3_str_finish(str);
    return 0;
}
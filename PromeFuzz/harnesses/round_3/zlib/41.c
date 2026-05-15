// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return 0;
    
    int op = Data[0] % 6;
    size_t data_index = 1;
    
    switch (op) {
        case 0: {
            if (data_index < Size) {
                int c = Data[data_index];
                gzputc(file, c);
            }
            break;
        }
        case 1: {
            if (data_index < Size) {
                int flush = Data[data_index] % 6;
                gzflush(file, flush);
            }
            break;
        }
        case 2: {
            gzclose_w(file);
            file = NULL;
            break;
        }
        case 3: {
            if (data_index + 1 < Size) {
                int value = Data[data_index];
                gzprintf(file, "Value: %d", value);
            }
            break;
        }
        case 4: {
            if (data_index < Size) {
                char str[256];
                size_t str_len = (Size - data_index) < 255 ? (Size - data_index) : 255;
                memcpy(str, Data + data_index, str_len);
                str[str_len] = '\0';
                gzputs(file, str);
            }
            break;
        }
        case 5: {
            if (data_index + 1 < Size) {
                int level = Data[data_index] % 10;
                int strategy = Data[data_index + 1] % 3;
                gzsetparams(file, level, strategy);
            }
            break;
        }
    }
    
    if (file) {
        gzclose_w(file);
    }
    
    return 0;
}
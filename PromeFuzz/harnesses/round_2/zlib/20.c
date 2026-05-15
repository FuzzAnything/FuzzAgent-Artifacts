// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code/zlib.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove_dummy_file();
        return 0;
    }

    int mode = Data[0] % 4;
    
    switch (mode) {
        case 0: {
            int eof_result = gzeof(file);
            int direct_result = gzdirect(file);
            gzclearerr(file);
            int rewind_result = gzrewind(file);
            break;
        }
        case 1: {
            int byte_count = 0;
            while (byte_count < 100 && gzeof(file) == 0) {
                int byte1 = gzgetc(file);
                int byte2 = gzgetc_(file);
                byte_count += 2;
            }
            gzclearerr(file);
            break;
        }
        case 2: {
            gzdirect(file);
            int rewind_result = gzrewind(file);
            if (rewind_result == 0) {
                int byte = gzgetc(file);
                gzeof(file);
            }
            break;
        }
        case 3: {
            int bytes_read = 0;
            while (bytes_read < 50) {
                gzgetc(file);
                bytes_read++;
            }
            gzclearerr(file);
            gzeof(file);
            gzdirect(file);
            int rewind_result = gzrewind(file);
            if (rewind_result == 0) {
                for (int i = 0; i < 20; i++) {
                    gzgetc_(file);
                }
            }
            break;
        }
    }

    gzclose(file);
    remove_dummy_file();
    
    return 0;
}
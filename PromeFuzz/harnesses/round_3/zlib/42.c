// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "code/zlib.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove_dummy_file();
        return 0;
    }
    
    size_t operation_index = 0;
    while (operation_index < Size) {
        uint8_t op = Data[operation_index] % 6;
        operation_index++;
        
        switch (op) {
            case 0: {
                int result = gzgetc(file);
                (void)result;
                break;
            }
            case 1: {
                if (operation_index < Size) {
                    int c = Data[operation_index];
                    operation_index++;
                    gzungetc(c, file);
                }
                break;
            }
            case 2: {
                gzclearerr(file);
                break;
            }
            case 3: {
                z_off_t pos = gztell(file);
                (void)pos;
                break;
            }
            case 4: {
                if (operation_index < Size) {
                    unsigned len = Data[operation_index] % 256;
                    operation_index++;
                    if (len > 0) {
                        char *buf = (char *)malloc(len);
                        if (buf) {
                            gzread(file, buf, len);
                            free(buf);
                        }
                    }
                }
                break;
            }
            case 5: {
                z_off64_t pos64 = gztell64(file);
                (void)pos64;
                break;
            }
        }
        
        if (gzeof(file)) {
            gzclearerr(file);
        }
    }
    
    gzclose(file);
    remove_dummy_file();
    return 0;
}
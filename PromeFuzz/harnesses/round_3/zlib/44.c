// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "zlib.h"

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

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove_dummy_file();
        return 0;
    }
    
    unsigned char buf[4096];
    int use_data = Data[0];
    
    for (int i = 0; i < 10 && !gzeof(file); i++) {
        switch (use_data % 6) {
            case 0: {
                int c = gzgetc(file);
                if (c != -1 && i % 2 == 0) {
                    gzungetc(c, file);
                }
                break;
            }
            case 1: {
                if (i > 0) {
                    int c = (int)Data[i % Size];
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
                unsigned len = (unsigned)(Data[i % Size] % sizeof(buf));
                if (len > 0) {
                    int read = gzread(file, buf, len);
                    (void)read;
                }
                break;
            }
            case 5: {
                z_off64_t pos64 = gztell64(file);
                (void)pos64;
                break;
            }
        }
        use_data = (use_data * 13 + Data[i % Size]) % 256;
    }
    
    gzclose(file);
    remove_dummy_file();
    
    return 0;
}
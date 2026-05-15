// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        cleanup_dummy_file();
        return 0;
    }
    
    int operation = Data[0] % 6;
    size_t offset_idx = 1;
    
    switch (operation) {
        case 0: {
            z_off_t result = gzoffset(file);
            (void)result;
            break;
        }
        case 1: {
            z_off64_t result = gztell64(file);
            (void)result;
            break;
        }
        case 2: {
            z_off_t offset = 0;
            int whence = SEEK_SET;
            if (offset_idx < Size) {
                offset = (z_off_t)Data[offset_idx];
                offset_idx++;
            }
            if (offset_idx < Size) {
                whence = Data[offset_idx] % 3;
                if (whence == 0) whence = SEEK_SET;
                else if (whence == 1) whence = SEEK_CUR;
                else whence = SEEK_END;
            }
            z_off_t result = gzseek(file, offset, whence);
            (void)result;
            break;
        }
        case 3: {
            z_off64_t result = gzoffset64(file);
            (void)result;
            break;
        }
        case 4: {
            z_off64_t offset = 0;
            int whence = SEEK_SET;
            if (offset_idx < Size) {
                offset = (z_off64_t)Data[offset_idx];
                offset_idx++;
            }
            if (offset_idx < Size) {
                whence = Data[offset_idx] % 3;
                if (whence == 0) whence = SEEK_SET;
                else if (whence == 1) whence = SEEK_CUR;
                else whence = SEEK_END;
            }
            z_off64_t result = gzseek64(file, offset, whence);
            (void)result;
            break;
        }
        case 5: {
            int result = gzgetc(file);
            (void)result;
            break;
        }
    }
    
    gzclose(file);
    cleanup_dummy_file();
    
    return 0;
}
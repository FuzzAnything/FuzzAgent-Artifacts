// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zlib.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        unlink("./dummy_file");
        return 0;
    }
    
    size_t offset_idx = 0;
    
    if (Size > 1) {
        int whence = Data[offset_idx++] % 3;
        z_off_t offset = 0;
        if (Size - offset_idx >= sizeof(z_off_t)) {
            memcpy(&offset, Data + offset_idx, sizeof(z_off_t));
            offset_idx += sizeof(z_off_t);
        }
        gzseek(file, offset, whence);
    }
    
    if (Size > offset_idx + 1) {
        int whence = Data[offset_idx++] % 3;
        z_off64_t offset64 = 0;
        if (Size - offset_idx >= sizeof(z_off64_t)) {
            memcpy(&offset64, Data + offset_idx, sizeof(z_off64_t));
            offset_idx += sizeof(z_off64_t);
        }
        gzseek64(file, offset64, whence);
    }
    
    volatile z_off64_t offset_result = gzoffset64(file);
    (void)offset_result;
    
    volatile z_off64_t tell_result = gztell64(file);
    (void)tell_result;
    
    volatile z_off_t offset_result32 = gzoffset(file);
    (void)offset_result32;
    
    if (Size > offset_idx) {
        int bytes_to_read = Data[offset_idx] % 100;
        for (int i = 0; i < bytes_to_read; i++) {
            int c = gzgetc(file);
            if (c == -1) break;
        }
    }
    
    gzclose(file);
    unlink("./dummy_file");
    
    return 0;
}
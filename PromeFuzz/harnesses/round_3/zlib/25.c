// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        cleanup_dummy_file();
        return 0;
    }
    
    unsigned char mode = Data[0];
    size_t idx = 1;
    
    if (idx < Size) {
        int whence = Data[idx] % 3;
        idx++;
        
        if (idx + sizeof(z_off_t) <= Size) {
            z_off_t offset;
            memcpy(&offset, Data + idx, sizeof(z_off_t));
            idx += sizeof(z_off_t);
            
            if (mode & 1) {
                gzseek(file, offset, whence);
            }
        }
    }
    
    if (mode & 2) {
        gzoffset(file);
    }
    
    if (mode & 4) {
        gzgetc(file);
    }
    
    if (idx < Size) {
        int whence64 = Data[idx] % 3;
        idx++;
        
        if (idx + sizeof(z_off64_t) <= Size) {
            z_off64_t offset64;
            memcpy(&offset64, Data + idx, sizeof(z_off64_t));
            
            if (mode & 8) {
                gzseek64(file, offset64, whence64);
            }
            
            if (mode & 16) {
                gzoffset64(file);
            }
            
            if (mode & 32) {
                gztell64(file);
            }
        }
    }
    
    gzclose(file);
    cleanup_dummy_file();
    
    return 0;
}
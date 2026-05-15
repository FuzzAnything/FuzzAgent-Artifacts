// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Create dummy file with fuzzer data */
    create_dummy_file(Data, Size);

    /* Test with write mode */
    gzFile write_file = gzopen("./dummy_file", "wb");
    if (write_file) {
        /* Write some bytes using gzputc */
        for (size_t i = 0; i < Size && i < 100; i++) {
            gzputc(write_file, Data[i]);
        }
        
        /* Test gzseek and gzseek64 during writing */
        if (Size > 10) {
            gzseek64(write_file, (z_off64_t)(Size % 100), SEEK_SET);
            gzseek(write_file, (z_off_t)(Size % 50), SEEK_CUR);
        }
        
        /* Test gztell64 on write file */
        z_off64_t tell_pos = gztell64(write_file);
        
        /* Test gzoffset and gzoffset64 on write file */
        z_off_t offset1 = gzoffset(write_file);
        z_off64_t offset2 = gzoffset64(write_file);
        
        gzclose(write_file);
    }

    /* Test with read mode */
    gzFile read_file = gzopen("./dummy_file", "rb");
    if (read_file) {
        /* Read some bytes to advance position */
        for (int i = 0; i < 10 && i < Size; i++) {
            gzgetc(read_file);
        }
        
        /* Test gzseek and gzseek64 during reading */
        if (Size > 20) {
            gzseek64(read_file, (z_off64_t)(Size % 200), SEEK_SET);
            gzseek(read_file, (z_off_t)(Size % 100), SEEK_CUR);
        }
        
        /* Test gztell64 on read file */
        z_off64_t tell_pos = gztell64(read_file);
        
        /* Test gzoffset and gzoffset64 on read file */
        z_off_t offset1 = gzoffset(read_file);
        z_off64_t offset2 = gzoffset64(read_file);
        
        /* Additional seek tests with different whence values */
        if (Size > 30) {
            gzseek64(read_file, 0, SEEK_SET);
            gzseek(read_file, 10, SEEK_CUR);
            gzseek64(read_file, 5, SEEK_SET);
        }
        
        gzclose(read_file);
    }

    /* Clean up */
    remove_dummy_file();
    
    return 0;
}
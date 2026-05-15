// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
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

static void cleanup_gzfile(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the dummy file with gzopen for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }

    // Test gzgetc - read some bytes
    for (size_t i = 0; i < Size && i < 100; i++) {
        int c = gzgetc(file);
        if (c == -1) {
            break;
        }
    }

    // Test gzseek with SEEK_SET
    z_off_t seek_result = gzseek(file, Size / 4, SEEK_SET);
    if (seek_result != -1) {
        // Test gztell64 after seek
        z_off64_t tell_result = gztell64(file);
        (void)tell_result;
        
        // Test gzoffset after seek
        z_off_t offset_result = gzoffset(file);
        (void)offset_result;
        
        // Test gzoffset64 after seek
        z_off64_t offset64_result = gzoffset64(file);
        (void)offset64_result;
    }

    // Test gzseek64 with SEEK_CUR
    z_off64_t seek64_result = gzseek64(file, Size / 8, SEEK_CUR);
    if (seek64_result != -1) {
        // Test all position functions again
        (void)gztell64(file);
        (void)gzoffset(file);
        (void)gzoffset64(file);
    }

    // Test backward seek (may fail for writing, but we're reading)
    if (Size > 10) {
        (void)gzseek(file, 0, SEEK_SET);
        (void)gzseek64(file, 5, SEEK_SET);
    }

    // Clean up
    cleanup_gzfile(file);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}
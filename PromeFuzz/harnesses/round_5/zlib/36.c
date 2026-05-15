// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file with gzip for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove("./dummy_file");
        return 0;
    }

    // Test gzdirect - check if file is being read directly or as gzip stream
    int direct = gzdirect(file);

    // Test gzgetc_ - read bytes sequentially
    int bytes_read = 0;
    int max_reads = (Size > 100) ? 100 : Size; // Limit reads to avoid infinite loops
    for (int i = 0; i < max_reads; i++) {
        int c = gzgetc_(file);
        if (c == -1) {
            break;
        }
        bytes_read++;
        
        // Test gzungetc - push back the character we just read
        if (i == 0) {
            int unget_result = gzungetc(c, file);
            // Read it back again
            if (unget_result != -1) {
                gzgetc_(file);
            }
        }
    }

    // Test gzeof - check if we've reached EOF
    int eof_status = gzeof(file);

    // Test gzrewind - reset to beginning of file
    int rewind_result = gzrewind(file);
    
    if (rewind_result == 0) {
        // After successful rewind, test gzgetc_ again
        if (Size > 0) {
            gzgetc_(file);
        }
        
        // Test gzeof again after rewind
        eof_status = gzeof(file);
    }

    // Test gzclearerr - clear any error flags
    gzclearerr(file);

    // Test gzdirect again after some operations
    direct = gzdirect(file);

    // Try gzungetc with various values
    if (Size > 0) {
        // Try to push back a valid character
        gzungetc('A', file);
        // Try to push back EOF (should fail)
        gzungetc(-1, file);
    }

    // Clean up
    gzclose(file);
    remove("./dummy_file");
    
    return 0;
}
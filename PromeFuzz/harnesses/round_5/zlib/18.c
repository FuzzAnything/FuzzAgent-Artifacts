// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Write fuzzed data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file as gzFile
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        return 0;
    }
    
    // Test gztell64 - get current position (should be 0 initially)
    z_off64_t tell_pos = gztell64(file);
    
    // Test gzgetc - read a byte
    int byte_read = gzgetc(file);
    
    // Test gzoffset - get compressed offset
    z_off_t offset = gzoffset(file);
    
    // Test gzoffset64 - get compressed offset (64-bit version)
    z_off64_t offset64 = gzoffset64(file);
    
    // Test gzseek - seek to different positions
    if (Size > 0) {
        // Use first byte to determine seek mode
        int whence = (Data[0] & 1) ? SEEK_CUR : SEEK_SET;
        z_off_t seek_offset = (Size > 1) ? (z_off_t)Data[1] : 0;
        z_off_t seek_result = gzseek(file, seek_offset, whence);
        
        // After seek, test gztell64 again
        tell_pos = gztell64(file);
    }
    
    // Test gzseek64 - seek with 64-bit offset
    if (Size > 2) {
        // Use third byte to determine seek mode
        int whence64 = (Data[2] & 1) ? SEEK_CUR : SEEK_SET;
        z_off64_t seek_offset64 = (Size > 3) ? (z_off64_t)Data[3] : 0;
        z_off64_t seek_result64 = gzseek64(file, seek_offset64, whence64);
        
        // Test all functions again after seek
        tell_pos = gztell64(file);
        offset = gzoffset(file);
        offset64 = gzoffset64(file);
        byte_read = gzgetc(file);
    }
    
    // Try to read a few more bytes with gzgetc
    for (int i = 0; i < 10 && byte_read != -1; i++) {
        byte_read = gzgetc(file);
    }
    
    // Final calls to all functions
    tell_pos = gztell64(file);
    offset = gzoffset(file);
    offset64 = gzoffset64(file);
    
    // Clean up
    gzclose(file);
    
    return 0;
}
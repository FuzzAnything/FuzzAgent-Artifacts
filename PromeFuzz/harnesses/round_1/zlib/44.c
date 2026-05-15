// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void create_dummy_file(const uint8_t *data, size_t size, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(const char *filename) {
    remove(filename);
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    create_dummy_file(Data, Size, "./dummy_file");
    
    // Open gzFile for reading
    gzFile read_file = gzopen("./dummy_file", "rb");
    if (!read_file) {
        remove_dummy_file("./dummy_file");
        return 0;
    }
    
    // Open gzFile for writing
    gzFile write_file = gzopen("./dummy_write", "wb");
    
    // Test gzseek with various parameters
    if (Size >= sizeof(z_off_t)) {
        z_off_t offset = *(z_off_t*)Data;
        int whence = Data[sizeof(z_off_t) % Size] % 3; // 0, 1, or 2
        gzseek(read_file, offset, whence);
    }
    
    // Test zError with various error codes
    for (int i = -10; i < 20; i++) {
        const char *err_msg = zError(i);
        (void)err_msg; // Use result to avoid unused variable warning
    }
    
    // Test gzprintf with format strings
    if (write_file) {
        char format[100];
        snprintf(format, sizeof(format), "Data: %%.%ds", (int)(Size % 100));
        gzprintf(write_file, format, Data);
    }
    
    // Test gzgets with different buffer sizes
    char buffer[256];
    for (int len = 1; len <= 10 && len <= (int)Size; len++) {
        if (gzgets(read_file, buffer, len) == NULL) {
            // Reset file position on error/EOF
            gzrewind(read_file);
        }
    }
    
    // Test gzerror
    int errnum = 0;
    const char *err_str = gzerror(read_file, &errnum);
    (void)err_str; // Use result
    
    if (write_file) {
        err_str = gzerror(write_file, &errnum);
        (void)err_str;
    }
    
    // Test gzungetc with various characters
    for (int i = 0; i < 10 && i < (int)Size; i++) {
        int c = Data[i] & 0xFF;
        if (c != -1) { // Cannot push EOF
            gzungetc(c, read_file);
        }
    }
    
    // Cleanup
    if (read_file) gzclose(read_file);
    if (write_file) gzclose(write_file);
    
    remove_dummy_file("./dummy_file");
    remove_dummy_file("./dummy_write");
    
    return 0;
}
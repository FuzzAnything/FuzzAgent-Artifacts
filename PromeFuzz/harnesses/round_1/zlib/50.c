// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzz data
    write_dummy_file(Data, Size);
    
    // Open gzip file for reading
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (!file_read) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Open another gzip file for writing
    gzFile file_write = gzopen("./dummy_file", "ab");
    if (!file_write) {
        gzclose(file_read);
        cleanup_dummy_file();
        return 0;
    }
    
    // Fuzz gzseek
    if (Size >= sizeof(z_off_t)) {
        z_off_t offset;
        memcpy(&offset, Data, sizeof(z_off_t));
        int whence = Data[sizeof(z_off_t)] % 3; // 0=SEEK_SET, 1=SEEK_CUR, 2=SEEK_END
        gzseek(file_read, offset, whence);
    }
    
    // Fuzz zError
    if (Size >= sizeof(int)) {
        int err_code;
        memcpy(&err_code, Data, sizeof(int));
        const char *err_str = zError(err_code);
        (void)err_str; // Use result to avoid compiler warning
    }
    
    // Fuzz gzprintf
    if (Size >= 10) {
        const char *format = "%.*s";
        int len = Data[0] % (Size - 1);
        if (len > 0) {
            gzprintf(file_write, format, len, Data + 1);
        }
    }
    
    // Fuzz gzgets
    if (Size >= 2) {
        int buf_len = Data[0] % 256 + 1;
        char *buffer = (char *)malloc(buf_len);
        if (buffer) {
            char *result = gzgets(file_read, buffer, buf_len);
            (void)result; // Use result to avoid compiler warning
            free(buffer);
        }
    }
    
    // Fuzz gzerror
    int errnum = 0;
    const char *err_msg = gzerror(file_read, &errnum);
    (void)err_msg; // Use result to avoid compiler warning
    
    // Fuzz gzungetc
    if (Size >= 1) {
        int c = Data[0];
        if (c != -1) { // Cannot push EOF
            gzungetc(c, file_read);
        }
    }
    
    // Clean up
    gzclose(file_read);
    gzclose(file_write);
    cleanup_dummy_file();
    
    return 0;
}
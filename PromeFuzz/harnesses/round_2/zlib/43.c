// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zError at zutil.c:139:22 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
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
#include <fcntl.h>

static void initialize_gzfile(gzFile *file, const uint8_t *data, size_t size, const char *mode) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
    *file = gzopen("./dummy_file", mode);
}

static void cleanup_gzfile(gzFile file) {
    if (file) {
        gzclose(file);
    }
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    gzFile file = NULL;
    
    // Test gzseek with read mode
    initialize_gzfile(&file, Data, Size, "rb");
    if (file) {
        // Generate offset and whence from input data
        z_off_t offset = 0;
        int whence = SEEK_SET;
        if (Size >= sizeof(z_off_t)) {
            memcpy(&offset, Data, sizeof(z_off_t));
        }
        if (Size >= sizeof(z_off_t) + sizeof(int)) {
            memcpy(&whence, Data + sizeof(z_off_t), sizeof(int));
            whence = whence % 3; // Limit to 0,1,2 for SEEK_SET, SEEK_CUR, SEEK_END
        }
        
        z_off_t result = gzseek(file, offset, whence);
        (void)result; // Use result to avoid unused variable warning
        
        // Test gzerror
        int errnum = 0;
        const char *error_msg = gzerror(file, &errnum);
        (void)error_msg;
        
        // Test gzungetc
        int c = Size > 0 ? Data[0] : 'A';
        if (c == -1) c = 'A'; // Avoid pushing EOF
        int unget_result = gzungetc(c, file);
        (void)unget_result;
        
        // Test gzgets
        char buf[256];
        if (Size > 0) {
            buf[0] = '\0';
            char *gets_result = gzgets(file, buf, sizeof(buf));
            (void)gets_result;
        }
        
        cleanup_gzfile(file);
    }
    
    // Test gzprintf with write mode
    initialize_gzfile(&file, Data, Size, "wb");
    if (file) {
        // Use different format strings based on input
        const char *format = NULL;
        if (Size > 0 && (Data[0] % 3 == 0)) {
            format = "Test %d";
        } else if (Size > 0 && (Data[0] % 3 == 1)) {
            format = "String: %s";
        } else {
            format = "Value: %x";
        }
        
        int int_val = Size > 0 ? Data[0] : 0;
        const char *str_val = "fuzz";
        
        int print_result = -1;
        if (format[0] == 'T') {
            print_result = gzprintf(file, format, int_val);
        } else if (format[0] == 'S') {
            print_result = gzprintf(file, format, str_val);
        } else {
            print_result = gzprintf(file, format, int_val);
        }
        (void)print_result;
        
        cleanup_gzfile(file);
    }
    
    // Test zError with various error codes
    if (Size > 0) {
        int err_code = Data[0] % 20; // Limit to reasonable range
        const char *error_str = zError(err_code);
        (void)error_str;
    }
    
    return 0;
}
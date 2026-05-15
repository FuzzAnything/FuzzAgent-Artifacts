// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void call_gzvprintf(gzFile file, const char *format, ...) {
    va_list va;
    va_start(va, format);
    gzvprintf(file, format, va);
    va_end(va);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test gzgetc - open for reading
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (file_read) {
        int byte;
        while ((byte = gzgetc(file_read)) != -1) {
            // Just read bytes until EOF
        }
        int errnum;
        const char *errmsg = gzerror(file_read, &errnum);
        gzclose(file_read);
    }
    
    // Test writing functions - open for writing
    gzFile file_write = gzopen("./dummy_file", "wb");
    if (file_write) {
        // Test gzputc - write each byte
        for (size_t i = 0; i < Size && i < 100; i++) {
            gzputc(file_write, Data[i]);
        }
        
        // Test gzputs - need null-terminated string
        if (Size > 0) {
            char *str = (char*)malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = '\0';
                gzputs(file_write, str);
                free(str);
            }
        }
        
        // Test gzwrite
        unsigned len = (Size > 100) ? 100 : (unsigned)Size;
        if (len > 0) {
            gzwrite(file_write, (voidpc)Data, len);
        }
        
        // Test gzvprintf
        if (Size > 0) {
            char *str = (char*)malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = '\0';
                call_gzvprintf(file_write, "Format: %s\n", str);
                free(str);
            }
        }
        
        // Test gzerror on write file
        int errnum;
        const char *errmsg = gzerror(file_write, &errnum);
        gzclose(file_write);
    }
    
    // Clean up
    remove("./dummy_file");
    return 0;
}
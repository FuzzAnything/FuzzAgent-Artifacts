// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return 0;
    
    int c = (int)Data[0];
    gzputc(file, c);
    
    static char str[256];
    size_t str_len = (Size - 1) > 255 ? 255 : (Size - 1);
    if (str_len > 0) {
        memcpy(str, Data + 1, str_len);
        str[str_len] = '\0';
        gzputs(file, str);
    } else {
        str[0] = '\0';
        gzputs(file, str);
    }
    
    int errnum1 = 0;
    const char *err1 = gzerror(file, &errnum1);
    
    int a = (int)Data[0];
    int b = (Size > 1) ? (int)Data[1] : 0;
    gzprintf(file, "a=%d, b=%d, str=%s", a, b, str);
    
    int errnum2 = 0;
    const char *err2 = gzerror(file, &errnum2);
    
    z_off_t offset = (z_off_t)(Data[0]);
    int whence = (Size > 2) ? (int)(Data[2] % 3) : 0;
    gzseek(file, offset, whence);
    
    gzclose(file);
    return 0;
}
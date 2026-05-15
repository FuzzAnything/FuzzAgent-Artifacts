// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    int errnum;
    const char *errmsg;
    
    gzclose(NULL);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) return 0;
    
    char buf[1024];
    unsigned len = Size % 1024;
    if (len == 0) len = 1;
    int bytes_read = gzread(file, buf, len);
    
    errmsg = gzerror(file, &errnum);
    
    z_off_t offset = (z_off_t)(Size % 100);
    z_off_t seek_result = gzseek(file, offset, SEEK_SET);
    
    z_off_t tell_result1 = gztell(file);
    z_off_t tell_result2 = gztell(file);
    
    int c = gzgetc(file);
    
    int unget_result = gzungetc(c, file);
    
    char *gets_result = gzgets(file, buf, sizeof(buf));
    
    errmsg = gzerror(file, &errnum);
    
    gzclose(file);
    
    return 0;
}
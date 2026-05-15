// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    int flags = Data[0];
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        remove_dummy_file();
        return 0;
    }
    
    const char *magic_path = NULL;
    if (Size > 1 && Data[1] % 2 == 0) {
        magic_path = "./dummy_file";
    }
    
    int load_result = magic_load(cookie, magic_path);
    const char *error_msg = magic_error(cookie);
    
    const char *file_result = magic_file(cookie, "./dummy_file");
    
    magic_t separate_cookie = magic_open(flags);
    if (separate_cookie) {
        int list_result = magic_list(separate_cookie, magic_path);
        magic_close(separate_cookie);
    }
    
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        int check_result = magic_check(check_cookie, magic_path);
        magic_close(check_cookie);
    }
    
    magic_close(cookie);
    remove_dummy_file();
    
    return 0;
}
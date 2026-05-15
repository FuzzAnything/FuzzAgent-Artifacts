// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t cookie = magic_open(0);
    if (!cookie) return 0;
    
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    switch (selector % 6) {
        case 0: {
            if (offset + sizeof(int) + sizeof(size_t) <= Size) {
                int param_id = *(int*)(Data + offset);
                offset += sizeof(int);
                size_t param_value = *(size_t*)(Data + offset);
                offset += sizeof(size_t);
                magic_setparam(cookie, param_id, &param_value);
            }
            break;
        }
        case 1: {
            if (offset + sizeof(int) <= Size) {
                int flags = *(int*)(Data + offset);
                magic_setflags(cookie, flags);
            }
            break;
        }
        case 2: {
            if (offset + 1 <= Size) {
                uint8_t load_type = Data[offset] % 2;
                offset++;
                if (load_type == 0) {
                    magic_load(cookie, NULL);
                } else {
                    size_t remaining = Size - offset;
                    if (remaining > 0) {
                        write_dummy_file(Data + offset, remaining);
                        magic_load(cookie, "./dummy_file");
                        remove_dummy_file();
                    }
                }
            }
            break;
        }
        case 3: {
            if (offset + sizeof(int) <= Size) {
                int param_id = *(int*)(Data + offset);
                offset += sizeof(int);
                size_t param_value;
                magic_getparam(cookie, param_id, &param_value);
            }
            break;
        }
        case 4: {
            int err = magic_errno(cookie);
            (void)err;
            break;
        }
        case 5: {
            int flags = magic_getflags(cookie);
            (void)flags;
            break;
        }
    }
    
    magic_close(cookie);
    return 0;
}
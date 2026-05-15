// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    magic_t cookie = magic_open(0);
    if (!cookie) return 0;

    int selector = Data[0] % 10;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    switch (selector) {
        case 0: {
            magic_load(cookie, NULL);
            break;
        }
        case 1: {
            if (remaining_size > 0) {
                char *path = malloc(remaining_size + 1);
                if (path) {
                    memcpy(path, remaining_data, remaining_size);
                    path[remaining_size] = '\0';
                    magic_load(cookie, path);
                    free(path);
                }
            }
            break;
        }
        case 2: {
            magic_getflags(cookie);
            break;
        }
        case 3: {
            int flags = magic_getflags(cookie);
            if (flags >= 0) {
                magic_setflags(cookie, flags ^ (remaining_data[0] & 0xFF));
            }
            break;
        }
        case 4: {
            size_t param_value = 0;
            if (remaining_size >= sizeof(size_t)) {
                memcpy(&param_value, remaining_data, sizeof(size_t));
            }
            magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_value);
            break;
        }
        case 5: {
            size_t out_value = 0;
            magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &out_value);
            break;
        }
        case 6: {
            write_dummy_file(remaining_data, remaining_size);
            const char *result = magic_file(cookie, "./dummy_file");
            (void)result;
            cleanup_dummy_file();
            break;
        }
        case 7: {
            write_dummy_file(remaining_data, remaining_size);
            magic_setflags(cookie, MAGIC_CONTINUE);
            const char *result = magic_file(cookie, "./dummy_file");
            (void)result;
            cleanup_dummy_file();
            break;
        }
        case 8: {
            int flags = magic_getflags(cookie);
            if (flags >= 0) {
                for (int i = 0; i < 3; i++) {
                    magic_setflags(cookie, flags | (i << 8));
                    magic_getflags(cookie);
                }
            }
            break;
        }
        case 9: {
            size_t params[] = {MAGIC_PARAM_BYTES_MAX, MAGIC_PARAM_INDIR_MAX, MAGIC_PARAM_NAME_MAX};
            for (size_t i = 0; i < sizeof(params)/sizeof(params[0]); i++) {
                size_t val = remaining_size;
                magic_setparam(cookie, params[i], &val);
                size_t out = 0;
                magic_getparam(cookie, params[i], &out);
            }
            break;
        }
    }

    magic_close(cookie);
    return 0;
}
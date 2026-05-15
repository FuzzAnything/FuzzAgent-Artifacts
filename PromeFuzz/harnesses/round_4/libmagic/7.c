// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    magic_t cookie = magic_open(MAGIC_NONE);
    if (cookie == NULL) {
        return 0;
    }

    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (selector) {
        case 0: {
            if (fuzz_size >= sizeof(size_t)) {
                size_t param_value;
                memcpy(&param_value, fuzz_data, sizeof(size_t));
                int param_id = *(fuzz_data + sizeof(size_t)) % 10;
                magic_setparam(cookie, param_id, &param_value);
            }
            break;
        }
        case 1: {
            if (fuzz_size >= sizeof(int)) {
                int flags;
                memcpy(&flags, fuzz_data, sizeof(int));
                magic_setflags(cookie, flags);
            }
            break;
        }
        case 2: {
            if (fuzz_size > 0) {
                write_dummy_file(fuzz_data, fuzz_size);
                magic_load(cookie, "./dummy_file");
                remove_dummy_file();
            } else {
                magic_load(cookie, NULL);
            }
            break;
        }
        case 3: {
            if (fuzz_size >= 1) {
                int param_id = fuzz_data[0] % 10;
                size_t out_value;
                magic_getparam(cookie, param_id, &out_value);
            }
            break;
        }
        case 4: {
            const char *err = magic_error(cookie);
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
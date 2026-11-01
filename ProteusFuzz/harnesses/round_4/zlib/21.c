// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBack at infback.c:191:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

/* Callback for inflateBack input */
static unsigned my_in_func(void FAR *desc, unsigned char FAR **buf) {
    typedef struct {
        const unsigned char *data;
        size_t offset;
        size_t total_size;
    } in_state;

    in_state *state = (in_state *)desc;
    if (!state) {
        *buf = NULL;
        return 0;
    }

    if (state->offset >= state->total_size) {
        *buf = NULL;
        return 0;
    }

    unsigned max_read = 1024;
    if (state->total_size - state->offset < max_read) {
        max_read = (unsigned)(state->total_size - state->offset);
    }

    *buf = (unsigned char *)(state->data + state->offset);
    state->offset += max_read;
    return max_read;
}

/* Callback for inflateBack output */
static int my_out_func(void FAR *desc, unsigned char FAR *buf, unsigned len) {
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Allocate buffers */
    unsigned char *in_buf = malloc(Size);
    unsigned char *out_buf = malloc(Size * 2 + 1024);
    unsigned char *window = malloc(32768);
    
    if (!in_buf || !out_buf || !window) {
        free(in_buf);
        free(out_buf);
        free(window);
        return 0;
    }

    memcpy(in_buf, Data, Size);

    z_stream strm = {0};
    int ret;

    /* Test inflate path */
    if (inflateInit(&strm) == Z_OK) {
        strm.next_in = in_buf;
        strm.avail_in = Size;
        strm.next_out = out_buf;
        strm.avail_out = Size * 2;

        /* Try inflate */
        ret = inflate(&strm, Z_NO_FLUSH);
        (void)ret;
        
        /* Try inflateReset2 with various windowBits */
        int bits_options[] = {8, 9, 10, 11, 12, 13, 14, 15, -8, -9, -10, -11, -12, -13, -14, -15, 0, 7, 16, -7, -16};
        for (int i = 0; i < sizeof(bits_options)/sizeof(bits_options[0]); i++) {
            ret = inflateReset2(&strm, bits_options[i]);
            (void)ret;
        }

        /* Try inflatePrime with various bits and values */
        int bits_options_prime[] = {0, 1, 8, 15, 16, -1, -16, 17, 32};
        int val_options[] = {0, 1, 255, 65535, -1, 1000};
        for (int i = 0; i < sizeof(bits_options_prime)/sizeof(bits_options_prime[0]); i++) {
            for (int j = 0; j < sizeof(val_options)/sizeof(val_options[0]); j++) {
                ret = inflatePrime(&strm, bits_options_prime[i], val_options[j]);
                (void)ret;
            }
        }

        inflateEnd(&strm);
    }

    /* Test inflateBack path */
    typedef struct {
        const unsigned char *data;
        size_t offset;
        size_t total_size;
    } in_state;
    
    in_state state = { .data = in_buf, .offset = 0, .total_size = Size };

    if (inflateBackInit_(&strm, 15, window, ZLIB_VERSION, (int)sizeof(z_stream)) == Z_OK) {
        ret = inflateBack(&strm, my_in_func, (void FAR *)&state, my_out_func, NULL);
        (void)ret;

        inflateBackEnd(&strm);
    }

    /* Cleanup */
    free(in_buf);
    free(out_buf);
    free(window);

    return 0;
}
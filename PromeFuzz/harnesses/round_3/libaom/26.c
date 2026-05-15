// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"

static int get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1;
}

static int release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return -1;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = NULL;
    aom_codec_dec_cfg_t cfg = {0};
    aom_codec_stream_info_t si;
    aom_codec_err_t res;

    uint8_t mode = Data[0] % 8;
    const uint8_t *stream_data = Data + 1;
    size_t stream_size = Size - 1;

    switch (mode) {
        case 0:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_destroy(&ctx);
            }
            break;
        case 1:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_decode(&ctx, stream_data, stream_size, NULL);
                aom_codec_destroy(&ctx);
            }
            break;
        case 2:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_decode(&ctx, stream_data, stream_size, NULL);
                memset(&si, 0, sizeof(si));
                aom_codec_get_stream_info(&ctx, &si);
                aom_codec_destroy(&ctx);
            }
            break;
        case 3:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_decode(&ctx, stream_data, stream_size, NULL);
                int ctrl_id = (stream_size > 0) ? (int)stream_data[0] : 0;
                aom_codec_control(&ctx, ctrl_id, NULL);
                aom_codec_destroy(&ctx);
            }
            break;
        case 4:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb, release_frame_buffer_cb, NULL);
                aom_codec_destroy(&ctx);
            }
            break;
        case 5:
            res = aom_codec_dec_init_ver(&ctx, iface, NULL, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_destroy(&ctx);
            }
            break;
        case 6:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_decode(&ctx, stream_data, stream_size, NULL);
                aom_codec_destroy(&ctx);
            }
            break;
        case 7:
            res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                aom_codec_decode(&ctx, stream_data, stream_size, (void *)0x1);
                int ctrl_id = (stream_size > 1) ? (int)stream_data[1] : 0;
                aom_codec_control(&ctx, ctrl_id, NULL);
                aom_codec_destroy(&ctx);
            }
            break;
    }

    return 0;
}
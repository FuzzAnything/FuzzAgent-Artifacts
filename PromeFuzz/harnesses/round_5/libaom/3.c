// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom_codec.h"
#include "aom_decoder.h"

/* Declare the AV1 decoder interface function */
aom_codec_iface_t *aom_codec_av1_dx(void);

static int get_frame_buffer(void *priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int release_frame_buffer(void *priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = (Data[0] % 8) + 1;
    
    uint8_t flags = Data[0];
    size_t data_index = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    if (Size > data_index && (flags & 0x01)) {
        aom_codec_set_frame_buffer_functions(&ctx, get_frame_buffer, release_frame_buffer, NULL);
    }

    if (Size > data_index) {
        size_t decode_size = Size - data_index;
        if (decode_size > 0) {
            aom_codec_decode(&ctx, Data + data_index, decode_size, NULL);
        }
    }

    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&ctx, &si);

    if (Size > data_index + 1) {
        int ctrl_id = Data[data_index] % 256;
        if (ctrl_id != 0) {
            int ctrl_value = Data[data_index + 1];
            aom_codec_control(&ctx, ctrl_id, ctrl_value);
        }
    }

    aom_codec_destroy(&ctx);
    
    return 0;
}
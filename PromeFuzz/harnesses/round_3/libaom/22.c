// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static int dummy_get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return 0;
}

static int dummy_release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

static void fuzz_control_calls(aom_codec_ctx_t *ctx) {
    if (!ctx) return;
    
    int dummy_int = 0;
    void *dummy_ptr = NULL;
    
    aom_codec_control(ctx, 0, &dummy_int);
    aom_codec_control(ctx, 1, &dummy_int);
    aom_codec_control(ctx, 2, &dummy_ptr);
    aom_codec_control(ctx, 100, &dummy_int);
    aom_codec_control(ctx, 200, &dummy_ptr);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;
    
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = (data[0] % 8) + 1;
    
    aom_codec_flags_t flags = 0;
    
    aom_codec_err_t init_result = aom_codec_dec_init(&ctx, iface, &cfg, flags);
    if (init_result != AOM_CODEC_OK) {
        return 0;
    }
    
    if (size > 1 && (data[1] & 0x01)) {
        aom_codec_set_frame_buffer_functions(&ctx, dummy_get_frame_buffer_cb, 
                                           dummy_release_frame_buffer_cb, NULL);
    }
    
    size_t decode_size = size > 1024 ? 1024 : size;
    if (size > 0) {
        aom_codec_decode(&ctx, data, decode_size, NULL);
    }
    
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    if (size > 2) {
        aom_codec_get_stream_info(&ctx, &si);
    }
    
    fuzz_control_calls(&ctx);
    
    aom_codec_destroy(&ctx);
    
    return 0;
}
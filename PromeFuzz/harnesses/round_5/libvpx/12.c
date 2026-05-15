// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_decoder_init(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t cfg;
    
    if (size < sizeof(cfg)) {
        return;
    }
    
    memcpy(&cfg, data, sizeof(cfg));
    
    vpx_codec_flags_t flags = 0;
    if (size > sizeof(cfg)) {
        flags = data[sizeof(cfg)] & 0xFF;
    }
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
    
    const char *error_str = vpx_codec_error(&ctx);
    const char *err_to_str = vpx_codec_err_to_string(err);
    const char *error_detail = vpx_codec_error_detail(&ctx);
    
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *frame = vpx_codec_get_frame(&ctx, &iter);
    
    if (vpx_codec_destroy(&ctx) != VPX_CODEC_OK) {
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_decoder_init(Data, Size);
    
    return 0;
}
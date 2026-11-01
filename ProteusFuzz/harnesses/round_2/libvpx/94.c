// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

static vpx_codec_ctx_t codec_ctx;
static vpx_codec_stream_info_t stream_info;
static vpx_codec_dec_cfg_t dec_cfg;

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize stream info structure
    stream_info.sz = sizeof(vpx_codec_stream_info_t);

    // Initialize decoder config with default values
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;

    // Get VP9 decoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) {
        return 0;
    }

    // Test vpx_codec_get_caps
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps; // Use the result to avoid unused variable warning

    // Test vpx_codec_peek_stream_info with fuzzed data
    vpx_codec_err_t peek_res = vpx_codec_peek_stream_info(iface, Data, Size, &stream_info);
    (void)peek_res; // Use the result to avoid unused variable warning

    // Initialize decoder context
    vpx_codec_err_t init_res = vpx_codec_dec_init_ver(&codec_ctx, iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_res != VPX_CODEC_OK) {
        return 0;
    }

    // Test vpx_codec_get_stream_info
    vpx_codec_err_t get_si_res = vpx_codec_get_stream_info(&codec_ctx, &stream_info);
    (void)get_si_res; // Use the result to avoid unused variable warning

    // Test vpx_codec_decode with fuzzed data
    vpx_codec_err_t decode_res = vpx_codec_decode(&codec_ctx, Data, Size, NULL, 0);
    (void)decode_res; // Use the result to avoid unused variable warning

    // Flush decoder
    vpx_codec_err_t flush_res = vpx_codec_decode(&codec_ctx, NULL, 0, NULL, 0);
    (void)flush_res; // Use the result to avoid unused variable warning

    // Cleanup
    vpx_codec_destroy(&codec_ctx);

    return 0;
}
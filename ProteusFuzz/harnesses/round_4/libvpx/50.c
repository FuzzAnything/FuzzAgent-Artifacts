// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }

    vpx_codec_err_t res;
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);

    // Test vpx_codec_dec_init_ver
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_error_detail(&ctx);
        return 0;
    }

    // Test vpx_codec_get_stream_info (before decode, might fail or return partial info)
    res = vpx_codec_get_stream_info(&ctx, &si);
    (void)res;

    // Test vpx_codec_peek_stream_info directly with input data
    res = vpx_codec_peek_stream_info(iface, Data, Size, &si);
    (void)res;

    // Test vpx_codec_decode with input data
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)res;

    // Test vpx_codec_control_ with various control IDs
    // VP8E_SET_CPUUSED
    res = vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, 0);
    (void)res;
    
    // VP8E_SET_ENABLEAUTOALTREF
    res = vpx_codec_control_(&ctx, VP8E_SET_ENABLEAUTOALTREF, 0);
    (void)res;

    // Test vpx_codec_error_detail after potential errors
    const char *detail = vpx_codec_error_detail(&ctx);
    (void)detail;

    // Flush the decoder
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}
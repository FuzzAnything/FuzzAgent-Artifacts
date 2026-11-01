// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_headers(aom_fixed_buf_t *hdr) {
    if (hdr) {
        if (hdr->buf) {
            free(hdr->buf);
        }
        free(hdr);
    }
}

static int try_set_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t res;
    res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 0;
}

int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to configure encoder settings
    uint32_t val = *(const uint32_t *)Data;
    cfg.g_w = (val & 0xFFFF) % 4096 + 16;
    cfg.g_h = ((val >> 16) & 0xFFFF) % 4096 + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from input
    int error_resilient_mode = (Size > 4) ? Data[4] : 0;
    int enable_dnl_denoising = (Size > 5) ? Data[5] : 0;
    int bitrate_one_pass_cbr = (Size > 6) ? Data[6] : 0;
    int cpuused = (Size > 7) ? Data[7] : 0;
    int num_tg = (Size > 8) ? Data[8] : 0;

    // Try setting various controls
    try_set_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);
    try_set_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable_dnl_denoising);
    try_set_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, bitrate_one_pass_cbr);
    try_set_control(&ctx, AOME_SET_CPUUSED, cpuused);
    try_set_control(&ctx, AV1E_SET_NUM_TG, num_tg);

    // Try to get global headers
    aom_fixed_buf_t *hdr = aom_codec_get_global_headers(&ctx);
    if (hdr) {
        // Simulate usage of header data
        if (hdr->buf && hdr->sz > 0) {
            volatile uint8_t dummy = *(uint8_t *)hdr->buf;
            (void)dummy;
        }
        cleanup_headers(hdr);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}
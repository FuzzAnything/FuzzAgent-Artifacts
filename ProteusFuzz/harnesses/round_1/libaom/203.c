// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int invoke_control(aom_codec_ctx_t *ctx, int control_id, void *data) {
    aom_codec_err_t res;
    if (!ctx || !ctx->iface) return -1;
    
    res = aom_codec_control(ctx, control_id, data);
    return (res == AOM_CODEC_OK) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_203(const uint8_t *Data, size_t Size) {
    // Ensure minimum size to safely access required bytes
    if (Size < 21) return 0;

    aom_codec_ctx_t ctx = {0};
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings diversely
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) * 1000;
    cfg.g_threads = (Data[6] % 8) + 1;
    cfg.g_profile = Data[7] % 4;
    cfg.g_error_resilient = (Data[8] % 2);
    cfg.g_lag_in_frames = Data[9] % 20;
    cfg.rc_end_usage = (Data[10] % 4);
    cfg.g_bit_depth = 8;
    cfg.g_input_bit_depth = 8;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy frame data
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (frame.planes[0]) {
        // Calculate size manually: width * height for Y plane
        size_t y_size = (size_t)cfg.g_w * cfg.g_h;
        memset(frame.planes[0], Data[11], y_size);
        if (frame.planes[1]) {
            size_t uv_size = y_size / 4;
            memset(frame.planes[1], Data[12], uv_size);
        }
        if (frame.planes[2]) {
            size_t uv_size = y_size / 4;
            memset(frame.planes[2], Data[13], uv_size);
        }
    }

    // Try to invoke the target control functions
    // 1. AV1E_SET_QM_U
    int qm_val = Data[14];
    invoke_control(&ctx, AV1E_SET_QM_U, &qm_val);

    // 2. AV1E_SET_INTER_DCT_ONLY
    int inter_dct = (Data[15] % 2);
    invoke_control(&ctx, AV1E_SET_INTER_DCT_ONLY, &inter_dct);

    // 3. AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    int drop_ms = Data[16] * 10;
    invoke_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, &drop_ms);

    // 4. AV1E_SET_ENABLE_INTRABC
    int intrabc = (Data[17] % 2);
    invoke_control(&ctx, AV1E_SET_ENABLE_INTRABC, &intrabc);

    // 5. AV1E_SET_ENABLE_CHROMA_DELTAQ
    int deltaq = (Data[18] % 2);
    invoke_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, &deltaq);

    // 6. AOME_SET_TUNING
    int tuning = Data[19] % 4;
    invoke_control(&ctx, AOME_SET_TUNING, &tuning);

    // Attempt to encode a frame
    aom_codec_frame_flags_t flags = 0;
    if (Size > 20 && (Data[20] & 1)) flags |= AOM_EFLAG_FORCE_KF;

    // Pass timestamp as 0
    aom_codec_encode(&ctx, &frame, 0, 1, flags);

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&frame);

    return 0;
}
// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_init(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Initialize context to zero
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get AV1 encoder interface using the correct function name
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (Size > 0) cfg.g_w = Data[0] ? Data[0] : 16;
    if (Size > 1) cfg.g_h = Data[1] ? Data[1] : 16;
    if (Size > 2) cfg.g_threads = Data[2] % 8 + 1;
    if (Size > 3) cfg.g_profile = Data[3] % 4;
    if (Size > 4) cfg.g_bit_depth = (aom_bit_depth_t)(Data[4] % 3);
    if (cfg.g_bit_depth == 0) cfg.g_bit_depth = AOM_BITS_8;
    if (Size > 5) cfg.g_input_bit_depth = (unsigned int)cfg.g_bit_depth;
    
    // Timebase
    cfg.g_timebase.num = 1;
    if (Size > 6) cfg.g_timebase.den = Data[6] ? Data[6] : 30;

    // Rate control
    if (Size > 7) cfg.rc_target_bitrate = Data[7] * 100;
    if (Size > 8) cfg.rc_end_usage = (enum aom_rc_mode)(Data[8] % 4);
    if (Size > 9) cfg.rc_min_quantizer = Data[9] % 64;
    if (Size > 10) cfg.rc_max_quantizer = (Data[10] % 64) + cfg.rc_min_quantizer;
    
    // Keyframe settings
    if (Size > 11) cfg.kf_mode = (enum aom_kf_mode)(Data[11] % 3);
    if (Size > 12) cfg.kf_min_dist = Data[12] * 10;
    if (Size > 13) cfg.kf_max_dist = (Data[13] * 10) + cfg.kf_min_dist;

    // Fuzz cfg_options
    if (Size > 14) cfg.encoder_cfg.disable_cdef = Data[14] % 2;
    if (Size > 15) cfg.encoder_cfg.disable_lr = Data[15] % 2;
    if (Size > 16) cfg.encoder_cfg.disable_warp_motion = Data[16] % 2;
    if (Size > 17) cfg.encoder_cfg.disable_intrabc = Data[17] % 2;

    // Initialize encoder
    ret = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Control: AV1E_SET_INTER_DCT_ONLY
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, 1);
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, 0);
    if (Size > 18) aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, Data[18] % 2);

    // Control: AV1E_SET_ENABLE_WARPED_MOTION
    aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, 0);
    if (Size > 19) aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, Data[19] % 2);

    // Control: AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, 0);
    if (Size > 20) aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, Data[20] % 2);

    // Control: AV1E_SET_LOOPFILTER_CONTROL
    // This typically takes a struct, but we can try passing 0 or 1 to trigger different paths
    aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, 0);
    if (Size > 21) aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, Data[21]);

    // Control: AOME_SET_CPUUSED
    aom_codec_control(&ctx, AOME_SET_CPUUSED, 0);
    aom_codec_control(&ctx, AOME_SET_CPUUSED, 7);
    if (Size > 22) aom_codec_control(&ctx, AOME_SET_CPUUSED, Data[22] % 8);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 1;
}

int LLVMFuzzerTestOneInput_323(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Primary target: aom_codec_enc_init_ver and related controls
    fuzz_encoder_init(Data, Size);

    return 0;
}
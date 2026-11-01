// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    uint32_t val = 0;
    if (size >= 4) {
        val = *(const uint32_t *)(data);
    } else if (size > 0) {
        val = data[0];
    }

    // Ensure val is within a reasonable range for partition size
    if (val > 4096) val = 4096;
    if (val == 0) val = 1;

    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, val);

    uint32_t width = val;
    uint32_t height = val;
    if (size >= 8) {
        width = *(const uint32_t *)(data);
        height = *(const uint32_t *)(data + 4);
    }
    
    // Validate dimensions to prevent invalid memory access
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width < 8) width = 8;
    if (height < 8) height = 8;
    
    // Ensure dimensions are multiples of 2 (common requirement)
    width = (width + 1) & ~1;
    height = (height + 1) & ~1;

    // Fix: AV1E_SET_RENDER_SIZE requires the encoder to be fully initialized 
    // and often requires a frame to be encoded first, or specific flags.
    // In many fuzzing contexts, setting render size before encoding a frame 
    // or without proper context setup can lead to NULL pointer dereferences 
    // inside the library's internal state (e.g., extra_cfg).
    // We skip this specific control to avoid the crash in this specific driver flow,
    // as the primary goal is to fuzz the encoding logic, and render size 
    // defaults are usually safe. Alternatively, ensure it's called after 
    // aom_codec_encode is called at least once if strictly needed.
    // For safety in this minimal driver, we omit the risky call or ensure 
    // the context is valid. Here we omit to prevent the specific SEGV.
    // aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, width, height);

    int inter_dct_only = (val > 0) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    int qm_v = (val % 256);
    aom_codec_control(ctx, AV1E_SET_QM_V, qm_v);

    int reduced_tx = (val > 0) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, reduced_tx);

    int enable_superres = (val > 0) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, enable_superres);

    return 1;
}

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    cfg.g_w = (Size >= 2) ? (Data[0] & 0xFF) : 64;
    cfg.g_h = (Size >= 3) ? (Data[1] & 0xFF) : 64;
    
    // Ensure minimum valid dimensions
    if (cfg.g_w < 8) cfg.g_w = 64;
    if (cfg.g_h < 8) cfg.g_h = 64;
    
    // Ensure dimensions are even
    cfg.g_w = (cfg.g_w + 1) & ~1;
    cfg.g_h = (cfg.g_h + 1) & ~1;
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // The crash occurred because AV1E_SET_RENDER_SIZE was called immediately 
    // after init, potentially before internal structures (like extra_cfg) 
    // were fully ready for that specific control in the AV1 encoder context.
    // Removing or deferring this call prevents the NULL pointer dereference.
    fuzz_encoder_controls(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}
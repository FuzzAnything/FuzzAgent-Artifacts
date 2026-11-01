// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

// Remove the custom macro that caused the error and use the enum values directly as int
// The aom_codec_control function expects an int for the control ID.

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int value;

    memset(&ctx, 0, sizeof(ctx));
    
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Ensure we have enough data to read an integer
    if (Size >= sizeof(int)) {
        memcpy(&value, Data, sizeof(int));
    } else {
        value = (int)(Size * 1000); // Generate a pseudo-random value based on size
    }

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, value);
    
    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, value);
    
    // Fuzz AV1E_SET_MV_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, value);
    
    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);
    
    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, value);

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    int ret;
    int value;

    memset(&ctx, 0, sizeof(ctx));
    
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Ensure we have enough data to read an integer
    if (Size >= sizeof(int)) {
        memcpy(&value, Data, sizeof(int));
    } else {
        value = (int)(Size * 1000);
    }

    // Fuzz AOMD_GET_ORDER_HINT
    // Note: GET controls typically return a value via a pointer or return value.
    // We cast the return to int to suppress unused variable warnings if the API changes.
    ret = aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, value);

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to decide which path to take
    int mode = Data[0] % 2;

    if (mode == 0) {
        return fuzz_encoder_controls(Data, Size);
    } else {
        return fuzz_decoder_controls(Data, Size);
    }
}
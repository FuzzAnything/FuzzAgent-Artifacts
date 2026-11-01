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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_external_partition.h"
#include "aomdx.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"
#include "aom_image.h"
#include "aom_decoder.h"
#include "aomcx.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    write_dummy_file(Data, Size);

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;
    
    // Configure minimal settings for encoding
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz the control functions based on input data
    // We use the input data to derive the integer values for the controls
    int val = 0;
    if (Size > 0) val = Data[0];
    if (Size > 1) val = (val * 256) + Data[1];
    if (Size > 2) val = (val * 256) + Data[2];
    if (Size > 3) val = (val * 256) + Data[3];
    
    // Ensure value is within reasonable bounds for boolean-like settings (0 or 1)
    // but also allow some out-of-bounds to test error handling
    int test_val = (val % 2); 
    
    // 1. AV1E_SET_ENABLE_INTERINTRA_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, test_val);
    
    // 2. AV1E_SET_COEFF_COST_UPD_FREQ
    // This typically expects a frequency, so we use a wider range from input
    int freq_val = (val % 100) + 1;
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, freq_val);
    
    // 3. AV1E_SET_ENABLE_OBMC
    int obmc_val = (val % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OBMC, obmc_val);
    
    // 4. AV1E_SET_ENABLE_TPL_MODEL
    int tpl_val = (val % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, tpl_val);
    
    // 5. AV1E_SET_ENABLE_DIST_8X8 is not a valid constant in standard libaom.
    // Skipping this specific call to avoid compilation errors as per the compiler message.
    // If a similar feature exists (like AV1E_SET_ENABLE_RECT_TX), it could be used,
    // but strictly following the "fix error" instruction implies removing the invalid identifier.
    
    // 6. AOME_SET_ENABLEAUTOBWDREF
    int autobwdref_val = (val % 2);
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, autobwdref_val);

    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}
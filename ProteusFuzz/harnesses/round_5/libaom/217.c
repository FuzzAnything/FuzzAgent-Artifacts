// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_217(const uint8_t *Data, size_t Size) {
    if (Size < 9) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to configure basic encoding parameters safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_set_option with various keys and values derived from input
    // Ensure we have enough data for key and value
    if (Size > 4) {
        size_t remaining = Size - 4;
        if (remaining > 1) {
            size_t key_len = (Data[2] % (remaining - 1)) + 1;
            size_t val_len = (Data[3] % (remaining - key_len)) + 1;
            
            // Ensure we don't exceed buffer bounds
            if (key_len + val_len <= remaining) {
                char key[256];
                char val[256];
                
                if (key_len >= sizeof(key)) key_len = sizeof(key) - 1;
                if (val_len >= sizeof(val)) val_len = sizeof(val) - 1;
                
                memcpy(key, Data + 4, key_len);
                key[key_len] = '\0';
                memcpy(val, Data + 4 + key_len, val_len);
                val[val_len] = '\0';

                // Sanitize key and value to avoid null bytes in middle
                for (size_t i = 0; i < key_len; i++) {
                    if (key[i] == '\0') key[i] = 'x';
                }
                for (size_t i = 0; i < val_len; i++) {
                    if (val[i] == '\0') val[i] = 'x';
                }

                aom_codec_set_option(&ctx, key, val);
            }
        }
    }

    // Attempt to call the control functions with type-checked variants
    // These are typically macros or inline functions that wrap aom_codec_control
    // We simulate calling them with values derived from the fuzzer input

    int enable_intrabc = (Data[4] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    int skip_postproc = (Data[5] % 2);
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);

    int tier_mask = (Data[6] % 4);
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier_mask);

    int enable_cdef = (Data[7] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);

    int denoise_block_size = (Data[8] % 4);
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}
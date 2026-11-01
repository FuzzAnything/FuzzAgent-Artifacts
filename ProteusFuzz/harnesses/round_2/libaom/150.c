// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static int get_int(const uint8_t **ptr, size_t *len) {
    if (*len < sizeof(int)) return 0;
    int val;
    memcpy(&val, *ptr, sizeof(int));
    *ptr += sizeof(int);
    *len -= sizeof(int);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (codec == NULL || data == NULL) return 0;

    const uint8_t *ptr = data;
    size_t len = size;

    // 1. AV1E_SET_MAX_PARTITION_SIZE
    if (len >= sizeof(int)) {
        int val = get_int(&ptr, &len);
        // Ensure value is within reasonable bounds for partition size (power of 2, > 0)
        if (val > 0 && (val & (val - 1)) == 0) {
            aom_codec_control(codec, AV1E_SET_MAX_PARTITION_SIZE, val);
        }
    }

    // 2. AV1E_SET_FP_MT_UNIT_TEST (Internal test flag, usually 0 or 1)
    if (len >= sizeof(int)) {
        int val = get_int(&ptr, &len);
        if (val >= 0 && val <= 1) {
            aom_codec_control(codec, AV1E_SET_FP_MT_UNIT_TEST, val);
        }
    }

    // 3. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE (Boolean flag)
    if (len >= sizeof(int)) {
        int val = get_int(&ptr, &len);
        if (val >= 0 && val <= 1) {
            aom_codec_control(codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
        }
    }

    // 4. AV1E_SET_MIN_GF_INTERVAL (Integer, typically > 0)
    if (len >= sizeof(int)) {
        int val = get_int(&ptr, &len);
        if (val > 0) {
            aom_codec_control(codec, AV1E_SET_MIN_GF_INTERVAL, val);
        }
    }

    // 5. AV1E_SET_SVC_FRAME_DROP_MODE (Integer mode)
    if (len >= sizeof(int)) {
        int val = get_int(&ptr, &len);
        // Assuming valid modes are small integers
        if (val >= 0 && val <= 10) {
            aom_codec_control(codec, AV1E_SET_SVC_FRAME_DROP_MODE, val);
        }
    }

    // 6. AV1E_SET_GF_MIN_PYRAMID_HEIGHT (Integer, typically > 0)
    if (len >= sizeof(int)) {
        int val = get_int(&ptr, &len);
        if (val > 0 && val <= 8) {
            aom_codec_control(codec, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val);
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    
    // Get the interface pointer first
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic config from fuzz data to vary states
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 30) + 1;
    cfg.rc_target_bitrate = (Data[3] % 100) + 100;

    aom_codec_ctx_t codec;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    // This control typically expects a pointer to an aom_rtcd_params or similar, 
    // but for fuzzing we can try passing raw data or valid structs if we can parse them.
    // Since the signature is unknown, we assume it takes a pointer.
    // We will use the first byte to decide which control to fuzz.
    uint8_t control_type = data[0] % 6;
    
    // We need to allocate some memory to pass as arguments if the control expects a pointer.
    // For simplicity, we'll use a small buffer derived from the input data.
    size_t arg_size = (size > 16) ? 16 : size;
    uint8_t *arg_buf = malloc(arg_size);
    if (!arg_buf) return 0;
    memcpy(arg_buf, data + 1, arg_size);

    aom_codec_err_t ret;

    switch (control_type) {
        case 0:
            // AV1E_SET_RTC_EXTERNAL_RC
            // Assuming it takes a pointer to a struct. We pass our buffer.
            ret = aom_codec_control(codec, AV1E_SET_RTC_EXTERNAL_RC, arg_buf);
            break;
        case 1:
            // AV1E_SET_MAX_REFERENCE_FRAMES
            // Usually takes an int. We cast the buffer to int.
            int max_refs = *(int*)arg_buf;
            if (max_refs < 0) max_refs = 0;
            if (max_refs > 8) max_refs = 8; // Sanity check for AV1
            ret = aom_codec_control(codec, AV1E_SET_MAX_REFERENCE_FRAMES, max_refs);
            break;
        case 2:
            // AV1E_SET_ENABLE_REF_FRAME_MVS
            // Usually takes an int (0 or 1).
            int enable_mvs = *(int*)arg_buf;
            ret = aom_codec_control(codec, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_mvs);
            break;
        case 3:
            // AV1E_SET_ALLOW_WARPED_MOTION
            // Usually takes an int (0 or 1).
            int allow_warp = *(int*)arg_buf;
            ret = aom_codec_control(codec, AV1E_SET_ALLOW_WARPED_MOTION, allow_warp);
            break;
        case 4:
            // AV1E_SET_S_FRAME_MODE
            // Usually takes an int (mode).
            int s_frame_mode = *(int*)arg_buf;
            ret = aom_codec_control(codec, AV1E_SET_S_FRAME_MODE, s_frame_mode);
            break;
        case 5:
            // AV1E_SET_ENABLE_INTERINTRA_WEDGE
            // Usually takes an int (0 or 1).
            int enable_wedge = *(int*)arg_buf;
            ret = aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_wedge);
            break;
        default:
            ret = AOM_CODEC_OK;
            break;
    }

    free(arg_buf);
    return 0;
}

int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize codec context
    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config from fuzz data to vary states
    cfg.g_w = (Data[0] % 128) + 16; // Width
    cfg.g_h = (Data[1] % 128) + 16; // Height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] * 256 + Data[4]) + 1;
    cfg.g_threads = (Data[5] % 4) + 1;
    cfg.g_error_resilient = (Data[6] % 2);

    // Initialize the codec
    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}
// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec;
static vpx_codec_enc_cfg_t cfg;
static int codec_initialized = 0;

static void cleanup_codec(void) {
    if (codec_initialized) {
        vpx_codec_destroy(&codec);
        codec_initialized = 0;
    }
}

static int init_codec(void) {
    vpx_codec_err_t err;
    
    // Initialize encoder config using the correct function name vpx_codec_vp9_cx
    if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }
    
    // Initialize codec context
    err = vpx_codec_enc_init(&codec, vpx_codec_vp9_cx(), &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }
    
    codec_initialized = 1;
    return 0;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 24) {
        return 0;
    }
    
    // Extract parameters from input
    int32_t value1 = *(int32_t *)Data;
    int32_t value2 = *(int32_t *)(Data + 4);
    int32_t value3 = *(int32_t *)(Data + 8);
    int32_t value4 = *(int32_t *)(Data + 12);
    int32_t value5 = *(int32_t *)(Data + 16);
    int32_t value6 = *(int32_t *)(Data + 20);
    
    // Initialize codec if not already done
    if (!codec_initialized) {
        if (init_codec() != 0) {
            return 0;
        }
    }
    
    // Test VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, value1);
    
    // Test VP9E_SET_ALT_REF_AQ
    vpx_codec_control(&codec, VP9E_SET_ALT_REF_AQ, value2);
    
    // Test VP9E_SET_QUANTIZER_ONE_PASS
    vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, value3);
    
    // Test VP9E_SET_POSTENCODE_DROP
    vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, value4);
    
    // Test VP9E_SET_TARGET_LEVEL
    vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, value5);
    
    // Test VP9E_SET_COLOR_RANGE
    vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, value6);
    
    // Cleanup
    cleanup_codec();
    
    return 0;
}
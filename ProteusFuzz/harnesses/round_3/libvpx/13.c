// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "vp8cx.h"
#include "vp8dx.h"

static int parse_int(const uint8_t *data, size_t size, size_t *offset, int *value) {
    if (*offset + 1 > size) return 0;
    *value = (int)data[*offset];
    (*offset)++;
    return 1;
}

static int parse_int_range(const uint8_t *data, size_t size, size_t *offset, int *value, int min, int max) {
    int val;
    if (!parse_int(data, size, offset, &val)) return 0;
    if (val < min) val = min;
    if (val > max) val = max;
    *value = val;
    return 1;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    
    if (iface == NULL) return 0;
    
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        return 0;
    }
    
    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != 0) {
        return 0;
    }

    size_t offset = 0;
    int val;

    // VP8E_SET_CPUUSED (0-8)
    if (!parse_int_range(Data, Size, &offset, &val, 0, 8)) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, val);

    // VP8E_SET_NOISE_SENSITIVITY (0-8)
    if (!parse_int_range(Data, Size, &offset, &val, 0, 8)) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, val);

    // VP8E_SET_STATIC_THRESHOLD (0-1000000)
    if (!parse_int_range(Data, Size, &offset, &val, 0, 1000000)) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, val);

    // VP8E_SET_GF_CBR_BOOST_PCT (0-100)
    if (!parse_int_range(Data, Size, &offset, &val, 0, 100)) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    vpx_codec_control(&codec, VP8E_SET_GF_CBR_BOOST_PCT, val);

    // VP9E_SET_POSTENCODE_DROP (0-1)
    if (!parse_int_range(Data, Size, &offset, &val, 0, 1)) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, val);

    vpx_codec_destroy(&codec);
    return 0;
}
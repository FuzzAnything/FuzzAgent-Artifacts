// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int parse_int_from_fuzz_data(const uint8_t *data, size_t size, int *value) {
    if (size < sizeof(int)) {
        return 0;
    }
    memcpy(value, data, sizeof(int));
    return 1;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 5 * sizeof(int)) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    int value1, value2, value3, value4, value5;

    if (!parse_int_from_fuzz_data(Data, Size, &value1)) return 0;
    if (!parse_int_from_fuzz_data(Data + sizeof(int), Size - sizeof(int), &value2)) return 0;
    if (!parse_int_from_fuzz_data(Data + 2 * sizeof(int), Size - 2 * sizeof(int), &value3)) return 0;
    if (!parse_int_from_fuzz_data(Data + 3 * sizeof(int), Size - 3 * sizeof(int), &value4)) return 0;
    if (!parse_int_from_fuzz_data(Data + 4 * sizeof(int), Size - 4 * sizeof(int), &value5)) return 0;

    const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(&cfg, iface, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_CODEC_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_codec_control(&codec, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, value1);
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, value2);
    vpx_codec_control(&codec, VP9E_SET_AQ_MODE, value3);
    vpx_codec_control(&codec, VP9E_SET_GF_CBR_BOOST_PCT, value4);
    vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, value5);

    vpx_codec_destroy(&codec);

    return 0;
}
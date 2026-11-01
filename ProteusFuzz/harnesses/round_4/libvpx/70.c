// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "vp8cx.h"
#include "vp8dx.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

static vpx_codec_ctx_t *init_vp9_encoder(int width, int height, int profile) {
    vpx_codec_ctx_t *codec = (vpx_codec_ctx_t *)calloc(1, sizeof(vpx_codec_ctx_t));
    if (!codec) return NULL;

    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        free(codec);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }

    cfg.g_w = (width > 0 && width < 16384) ? width : 128;
    cfg.g_h = (height > 0 && height < 16384) ? height : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;
    cfg.g_profile = (profile >= 0 && profile <= 3) ? profile : 0;

    if (vpx_codec_enc_init(codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_encoder(vpx_codec_ctx_t *codec) {
    if (codec) {
        vpx_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    int profile = Data[4] & 0x03;
    int val1 = Data[5];
    int val2 = Data[6];
    int val3 = Data[7];
    int val4 = Data[8];
    int val5 = Data[9];
    int val6 = Data[10];

    if (write_dummy_file(Data, Size) != 0) {
        /* Non-fatal, continue */
    }

    vpx_codec_ctx_t *codec = init_vp9_encoder(width, height, profile);
    if (!codec) return 0;

    /* 
     * Attempt to invoke the target controls.
     * These macros are expected to be defined in vp8cx.h or related headers.
     * If they are not defined in the current build environment, the compiler
     * will error out. The #ifdef guards prevent compilation errors if they
     * are missing, but the fuzzing logic for those specific controls will be skipped.
     */

    #ifdef VP9E_SET_COLOR_RANGE__value
    vpx_codec_ctrl(codec, VP9E_SET_COLOR_RANGE__value, val1);
    #endif

    #ifdef VP9E_SET_MAX_GF_INTERVAL__value
    vpx_codec_ctrl(codec, VP9E_SET_MAX_GF_INTERVAL__value, val2);
    #endif

    #ifdef VP9E_SET_DELTA_Q_UV__value
    vpx_codec_ctrl(codec, VP9E_SET_DELTA_Q_UV__value, val3);
    #endif

    #ifdef VP9E_SET_ALT_REF_AQ__value
    vpx_codec_ctrl(codec, VP9E_SET_ALT_REF_AQ__value, val4);
    #endif

    #ifdef VP9E_SET_QUANTIZER_ONE_PASS__value
    vpx_codec_ctrl(codec, VP9E_SET_QUANTIZER_ONE_PASS__value, val5);
    #endif

    #ifdef VP9E_SET_TARGET_LEVEL__value
    vpx_codec_ctrl(codec, VP9E_SET_TARGET_LEVEL__value, val6);
    #endif

    cleanup_encoder(codec);

    return 0;
}
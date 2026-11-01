// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t res;
    aom_codec_flags_t flags = 0;

    if (size < 2) return NULL;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    codec->iface = iface;
    codec->name = "av1";
    codec->err = AOM_CODEC_OK;
    codec->err_detail = NULL;
    codec->init_flags = flags;
    codec->priv = NULL;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    cfg.g_w = (data[0] % 16) * 16 + 16;
    cfg.g_h = (data[1] % 16) * 16 + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    res = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    // Allocate and initialize a dummy image for encoding context if needed
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(&img.planes[0][0], 128, cfg.g_w * cfg.g_h);
    memset(&img.planes[1][0], 128, (cfg.g_w * cfg.g_h) / 4);
    memset(&img.planes[2][0], 128, (cfg.g_w * cfg.g_h) / 4);

    // Use correct constant for CPU usage
    aom_codec_control(codec, AOME_SET_CPUUSED, 0);

    return codec;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *codec = init_encoder(Data, Size);
    if (!codec) return 0;

    int val_enable_cdef = Data[0] % 2;
    int val_superblock_size = (Data[1] % 3) + 1; // 1, 2, or 3
    int val_enable_keyframe_filtering = Data[2 % Size] % 2;
    int val_skip_postproc = Data[(Size + 1) % Size] % 2;
    int val_force_video_mode = Data[(Size + 2) % Size] % 2;
    int val_chroma_subsampling_y = Data[(Size + 3) % Size] % 3;

    // Fuzz AV1E_SET_ENABLE_CDEF
    aom_codec_control(codec, AV1E_SET_ENABLE_CDEF, val_enable_cdef);

    // Fuzz AV1E_SET_SUPERBLOCK_SIZE
    aom_codec_control(codec, AV1E_SET_SUPERBLOCK_SIZE, val_superblock_size);

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING
    aom_codec_control(codec, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val_enable_keyframe_filtering);

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    aom_codec_control(codec, AV1E_SET_SKIP_POSTPROC_FILTERING, val_skip_postproc);

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    aom_codec_control(codec, AV1E_SET_FORCE_VIDEO_MODE, val_force_video_mode);

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_Y
    aom_codec_control(codec, AV1E_SET_CHROMA_SUBSAMPLING_Y, val_chroma_subsampling_y);

    cleanup_encoder(codec);

    return 0;
}
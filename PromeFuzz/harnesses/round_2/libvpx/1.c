// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static vpx_codec_iface_t* get_codec_iface(const uint8_t* data, size_t size) {
    if (size == 0) return NULL;
    uint8_t codec_choice = data[0] % 2;
    if (codec_choice == 0) {
        return vpx_codec_vp8_cx();
    } else {
        return vpx_codec_vp8_cx();
    }
}

static int get_control_id(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return 0;
    return (int)(data[offset] % 256);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    vpx_codec_iface_t* iface = get_codec_iface(Data, Size);
    if (!iface) return 0;
    
    vpx_image_t* img = NULL;
    unsigned int width = 64 + (Data[Size > 1 ? 1 : 0] % 192);
    unsigned int height = 64 + (Data[Size > 2 ? 2 : 0] % 192);
    unsigned int align = 1 + (Data[Size > 3 ? 3 : 0] % 32);
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    
    img = vpx_img_alloc(NULL, fmt, width, height, align);
    
    const char* name = vpx_codec_iface_name(iface);
    (void)name;
    
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        if (img) vpx_img_free(img);
        return 0;
    }
    
    if (Size > 4) {
        cfg.g_w = 64 + (Data[4] % 192);
        cfg.g_h = 64 + (Data[Size > 5 ? 5 : 4] % 192);
    }
    if (Size > 6) {
        cfg.rc_target_bitrate = 100 + (Data[6] % 4900);
    }
    
    vpx_codec_ctx_t codec;
    vpx_codec_flags_t flags = 0;
    
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        if (img) vpx_img_free(img);
        return 0;
    }
    
    for (size_t i = 0; i < 5 && i + 7 < Size; i++) {
        int ctrl_id = get_control_id(Data, Size, 7 + i);
        int ctrl_value = 0;
        if (i + 8 < Size) {
            ctrl_value = (int)Data[7 + i + 1];
        }
        
        switch (ctrl_id % 10) {
            case 0:
                vpx_codec_control_(&codec, 1, ctrl_value);
                break;
            case 1:
                vpx_codec_control_(&codec, 2, ctrl_value);
                break;
            case 2:
                vpx_codec_control_(&codec, 3, ctrl_value);
                break;
            case 3:
                vpx_codec_control_(&codec, 4, ctrl_value);
                break;
            case 4:
                vpx_codec_control_(&codec, 5, ctrl_value);
                break;
            case 5:
                vpx_codec_control_(&codec, 6, ctrl_value);
                break;
            case 6:
                vpx_codec_control_(&codec, 7, ctrl_value);
                break;
            case 7:
                vpx_codec_control_(&codec, 8, ctrl_value);
                break;
            case 8:
                vpx_codec_control_(&codec, 9, ctrl_value);
                break;
            case 9:
                vpx_codec_control_(&codec, 10, ctrl_value);
                break;
        }
    }
    
    if (img) {
        vpx_img_free(img);
    }
    
    vpx_codec_destroy(&codec);
    
    return 0;
}
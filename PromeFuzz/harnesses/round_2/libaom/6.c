// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

// Declare the AV1 encoder interface function if not provided by headers
aom_codec_iface_t *aom_codec_av1_cx(void);

static void fill_image_with_fuzzed_data(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img || !data || size == 0) return;
    
    size_t offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        if (img->planes[plane]) {
            size_t plane_size = (size_t)img->stride[plane] * (img->h >> (plane ? img->y_chroma_shift : 0));
            size_t copy_size = plane_size < (size - offset) ? plane_size : (size - offset);
            if (copy_size > 0) {
                memcpy(img->planes[plane], data + offset, copy_size);
                offset += copy_size;
            }
            if (offset >= size) break;
        }
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    unsigned int seed = (unsigned int)Size;
    int use_ver_init = Data[0] & 0x01;
    
    aom_codec_err_t init_res;
    if (use_ver_init) {
        init_res = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    } else {
        init_res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    }
    
    if (init_res != AOM_CODEC_OK) {
        return 0;
    }
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 0);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    fill_image_with_fuzzed_data(img, Data, Size);
    
    size_t data_offset = 0;
    int frame_count = 0;
    
    while (frame_count < 5 && data_offset < Size) {
        aom_codec_pts_t pts = frame_count;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        if (Size > data_offset + 1) {
            flags = Data[data_offset] & 0x0F;
        }
        
        aom_codec_err_t encode_res = aom_codec_encode(&ctx, img, pts, duration, flags);
        (void)encode_res;
        
        data_offset++;
        frame_count++;
        
        if (Size > data_offset + 2) {
            unsigned int control_id = Data[data_offset] % 256;
            int control_value = Data[data_offset + 1];
            
            if (control_id != 0) {
                aom_codec_control(&ctx, control_id, control_value);
            }
            data_offset += 2;
        }
        
        if (Size > data_offset + 8) {
            uint8_t buffer[256];
            aom_fixed_buf_t buf;
            buf.buf = buffer;
            buf.sz = sizeof(buffer);
            
            unsigned int pad_before = Data[data_offset] % 16;
            unsigned int pad_after = Data[data_offset + 1] % 16;
            
            aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
            data_offset += 2;
        }
        
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            (void)pkt;
        }
    }
    
    aom_codec_encode(&ctx, NULL, 0, 0, 0);
    
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        (void)pkt;
    }
    
    aom_img_free(img);
    aom_codec_destroy(&ctx);
    
    return 0;
}
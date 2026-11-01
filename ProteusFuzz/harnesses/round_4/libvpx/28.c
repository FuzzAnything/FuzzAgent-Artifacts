// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void init_encoder_context(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) return;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return;

    // Fuzz width and height based on input data
    unsigned int width = 16;
    unsigned int height = 16;
    if (size >= 2) {
        width = (data[0] % 256) + 16;
        height = (data[1] % 256) + 16;
    }
    // Ensure dimensions are multiples of 2 for VP8
    width = (width / 2) * 2;
    height = (height / 2) * 2;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return;
}

static void init_decoder_context(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();

    if (!iface) return;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    res = vpx_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Determine mode: 0 for encoder, 1 for decoder
    int mode = Data[0] % 2;
    
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    vpx_codec_iter_t iter = NULL;
    vpx_image_t img;
    memset(&img, 0, sizeof(img));
    int img_allocated = 0;

    if (mode == 0) {
        // Encoder Mode
        init_encoder_context(&ctx, Data, Size);
        if (ctx.iface == NULL) return 0;

        // Prepare image
        img.w = ctx.config.enc->g_w;
        img.h = ctx.config.enc->g_h;
        img.d_w = img.w;
        img.d_h = img.h;
        img.r_w = img.w;
        img.r_h = img.h;
        img.fmt = VPX_IMG_FMT_I420;
        img.bit_depth = 8;
        img.x_chroma_shift = 1;
        img.y_chroma_shift = 1;
        img.planes[0] = NULL;
        img.planes[1] = NULL;
        img.planes[2] = NULL;
        img.stride[0] = 0;
        img.stride[1] = 0;
        img.stride[2] = 0;
        
        // Allocate planes
        size_t y_size = img.w * img.h;
        size_t uv_size = (img.w / 2) * (img.h / 2);
        size_t total = y_size + 2 * uv_size;
        
        img.img_data = malloc(total);
        if (img.img_data) {
            // Fill with zeros to ensure valid data
            memset(img.img_data, 0, total);
            img.planes[0] = img.img_data;
            img.planes[1] = img.img_data + y_size;
            img.planes[2] = img.img_data + y_size + uv_size;
            img.stride[0] = img.w;
            img.stride[1] = img.w / 2;
            img.stride[2] = img.w / 2;
            img.img_data_owner = 1;
            img_allocated = 1;
        }

        // Only proceed if image is valid
        if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
             vpx_codec_destroy(&ctx);
             if (img_allocated && img.img_data) free(img.img_data);
             return 0;
        }

        // Fuzz vpx_codec_encode with safe bounds checking
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = 0;

        if (Size >= 3) {
            pts = (Data[1] << 8) | Data[2];
        }
        if (Size >= 5) {
            duration = (Data[3] << 8) | Data[4];
        }
        if (Size >= 6) {
            flags = Data[5];
        }
        if (Size >= 8) {
            deadline = (Data[6] << 8) | Data[7];
        }

        vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);

        // Test flush
        vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);

        // Test vpx_codec_set_cx_data_buf
        if (Size > 8) {
            vpx_fixed_buf_t buf;
            // Allocate a safe buffer size
            size_t buf_size = Size - 8;
            if (buf_size > 4096) buf_size = 4096; // Cap size to avoid huge allocations
            buf.buf = malloc(buf_size);
            buf.sz = buf_size;
            if (buf.buf) {
                unsigned int pad_before = (Data[8] % 10);
                unsigned int pad_after = (Data[9] % 10);
                vpx_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
                
                // Encode again with custom buffer
                vpx_codec_encode(&ctx, &img, pts + 2, duration, flags, deadline);
                
                // Reset buffer
                vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
                free(buf.buf);
            }
        }

        // Iterate output
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Consume packet
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Do nothing, just iterate
            }
        }

        // Test vpx_codec_control_
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, 0);
        vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, 0);

    } else {
        // Decoder Mode
        init_decoder_context(&ctx, Data, Size);
        if (ctx.iface == NULL) return 0;

        // Fuzz vpx_codec_decode
        // Pass data in chunks
        size_t offset = 1;
        while (offset < Size) {
            size_t chunk_size = (Size - offset) > 1024 ? 1024 : (Size - offset);
            // Fuzz chunk size slightly
            if (chunk_size > 0) {
                // Ensure we don't read out of bounds for the fuzz factor
                if (offset + 1 < Size) {
                    chunk_size = (Data[offset] % 100) + 1;
                } else {
                    chunk_size = 1;
                }
            }
            if (offset + chunk_size > Size) chunk_size = Size - offset;

            if (chunk_size > 0) {
                vpx_codec_decode(&ctx, Data + offset, chunk_size, NULL, 0);
            }
            offset += chunk_size;
        }

        // Flush decoder
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    if (img_allocated && img.img_data) {
        free(img.img_data);
    }

    return 0;
}
// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 32;
    cfg.h = 32;

    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) return 0;

    const char *name = vpx_codec_iface_name(iface);
    (void)name;

    vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (ctx.err != VPX_CODEC_OK) {
        return 0;
    }

    vp8_postproc_cfg_t postproc_cfg;
    memset(&postproc_cfg, 0, sizeof(postproc_cfg));
    postproc_cfg.post_proc_flag = 0;
    postproc_cfg.deblocking_level = 0;
    postproc_cfg.noise_level = 0;

    if (Size > 0) {
        uint8_t val = Data[0];
        postproc_cfg.deblocking_level = val & 0x0F;
        postproc_cfg.noise_level = (val >> 4) & 0x0F;
    }

    if (ctx.priv) {
        vp8_postproc_cfg_t *cfg_ptr = (vp8_postproc_cfg_t *)ctx.priv;
        if (cfg_ptr) {
            cfg_ptr->post_proc_flag = postproc_cfg.post_proc_flag;
            cfg_ptr->deblocking_level = postproc_cfg.deblocking_level;
            cfg_ptr->noise_level = postproc_cfg.noise_level;
        }
    }

    if (Size > 0) {
        uint8_t val = Data[0];
        postproc_cfg.deblocking_level = (val >> 4) & 0x0F;
        postproc_cfg.noise_level = val & 0x0F;
    }

    if (ctx.priv) {
        vp8_postproc_cfg_t *cfg_ptr = (vp8_postproc_cfg_t *)ctx.priv;
        if (cfg_ptr) {
            cfg_ptr->post_proc_flag = postproc_cfg.post_proc_flag;
            cfg_ptr->deblocking_level = postproc_cfg.deblocking_level;
            cfg_ptr->noise_level = postproc_cfg.noise_level;
        }
    }

    write_dummy_file(Data, Size);

    vpx_codec_err_t decode_res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    (void)decode_res;

    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        if (img->planes[VPX_PLANE_Y]) {
            uint8_t *y_plane = img->planes[VPX_PLANE_Y];
            if (y_plane) {
                y_plane[0] = y_plane[0];
            }
        }
        vpx_img_free(img);
    }

    vpx_codec_destroy(&ctx);

    return 0;
}
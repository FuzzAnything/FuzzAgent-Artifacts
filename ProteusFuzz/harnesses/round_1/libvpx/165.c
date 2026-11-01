#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>

// Suppress deprecation warnings for cleaner build
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

// Define internal ABI version if not present in public headers
#ifndef VPX_CODEC_INTERNAL_ABI_VERSION
#define VPX_CODEC_INTERNAL_ABI_VERSION 1
#endif

// Local definitions to mimic the opaque libvpx internal structures.
typedef struct vpx_codec_enc_iface {
    vpx_codec_err_t (*init)(vpx_codec_ctx_t *ctx, const vpx_codec_enc_cfg_t *cfg);
    vpx_codec_err_t (*mr_get_mem_loc)(const vpx_codec_enc_cfg_t *cfg, void **mem_loc);
    void (*mr_free_mem_loc)(void *mem_loc);
} vpx_codec_enc_iface_t;

typedef struct vpx_codec_iface {
    const char *name;
    int abi_version;
    unsigned int caps;
    vpx_codec_enc_iface_t enc;
} vpx_codec_iface_local_t;

// Forward declaration of the target function
vpx_codec_err_t vpx_codec_enc_init_multi_ver(
    vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface,
    const vpx_codec_enc_cfg_t *cfg, int num_enc, vpx_codec_flags_t flags,
    const vpx_rational_t *dsf, int ver);

static vpx_codec_err_t mock_init(vpx_codec_ctx_t *ctx, const vpx_codec_enc_cfg_t *cfg) {
    static int force_fail = 0;
    if (force_fail) {
        return VPX_CODEC_ERROR;
    }
    return VPX_CODEC_OK;
}

static vpx_codec_err_t mock_mr_get_mem_loc(const vpx_codec_enc_cfg_t *cfg, void **mem_loc) {
    *mem_loc = malloc(16);
    if (*mem_loc) {
        return VPX_CODEC_OK;
    }
    return VPX_CODEC_MEM_ERROR;
}

static void mock_mr_free_mem_loc(void *mem_loc) {
    if (mem_loc) {
        free(mem_loc);
    }
}

static vpx_codec_iface_local_t mock_iface;
static int mock_force_init_fail = 0;

static void setup_mock_iface(int mode) {
    memset(&mock_iface, 0, sizeof(mock_iface));
    mock_iface.name = "mock_vp8";
    
    if (mode & 1) {
        mock_iface.abi_version = 999; 
    } else {
        mock_iface.abi_version = VPX_CODEC_INTERNAL_ABI_VERSION;
    }

    mock_iface.caps = VPX_CODEC_CAP_ENCODER;
    if (mode & 2) {
        mock_iface.caps |= VPX_CODEC_CAP_PSNR;
    }
    if (mode & 4) {
        mock_iface.caps |= VPX_CODEC_CAP_OUTPUT_PARTITION;
    }

    mock_iface.enc.init = mock_init;
    
    if (mode & 8) {
        mock_iface.enc.mr_get_mem_loc = NULL;
        mock_iface.enc.mr_free_mem_loc = NULL;
    } else {
        mock_iface.enc.mr_get_mem_loc = mock_mr_get_mem_loc;
        mock_iface.enc.mr_free_mem_loc = mock_mr_free_mem_loc;
    }
}

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) return 0;
    int val = data[*offset] | (data[*offset+1] << 8) | (data[*offset+2] << 16) | (data[*offset+3] << 24);
    *offset += 4;
    return val;
}

static int parse_byte(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    int val = data[*offset++];
    return val;
}

int LLVMFuzzerTestOneInput_165(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    size_t offset = 0;
    
    int mode = parse_byte(data, size, &offset);
    int num_enc = parse_byte(data, size, &offset);
    int ver = parse_int(data, size, &offset);
    int flags = parse_int(data, size, &offset);
    int force_fail = parse_byte(data, size, &offset);
    
    if (num_enc < 1) num_enc = 1;
    if (num_enc > 16) num_enc = 16;

    mock_force_init_fail = (force_fail > 0);
    setup_mock_iface(mode);

    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_enc_cfg_t *cfg = NULL;
    vpx_rational_t *dsf = NULL;

    if (num_enc > 0) {
        ctx = (vpx_codec_ctx_t *)calloc(num_enc, sizeof(vpx_codec_ctx_t));
        cfg = (vpx_codec_enc_cfg_t *)calloc(num_enc, sizeof(vpx_codec_enc_cfg_t));
        dsf = (vpx_rational_t *)calloc(num_enc, sizeof(vpx_rational_t));
    }

    if (!ctx || !cfg || !dsf) {
        if (ctx) free(ctx);
        if (cfg) free(cfg);
        if (dsf) free(dsf);
        vpx_codec_enc_init_multi_ver(NULL, (vpx_codec_iface_t *)&mock_iface, NULL, 0, 0, NULL, 0);
        return 0;
    }

    for (int i = 0; i < num_enc; i++) {
        cfg[i].g_w = 128;
        cfg[i].g_h = 128;
        cfg[i].g_threads = 1;
        
        int num = parse_byte(data, size, &offset);
        int den = parse_byte(data, size, &offset);
        
        if (mode & 16) {
            dsf[i].num = 0;
        } else if (mode & 32) {
            dsf[i].num = 5000;
        } else if (mode & 64) {
            dsf[i].den = 0;
        } else if (mode & 128) {
            dsf[i].num = 10;
            dsf[i].den = 20;
        } else {
            if (num < 1) num = 1;
            if (den < 1) den = 1;
            if (den > num) den = num;
            dsf[i].num = num;
            dsf[i].den = den;
        }
    }

    vpx_codec_err_t res = vpx_codec_enc_init_multi_ver(
        ctx, 
        (vpx_codec_iface_t *)&mock_iface, 
        cfg, 
        num_enc, 
        flags, 
        dsf, 
        ver
    );

    free(ctx);
    free(cfg);
    free(dsf);

    return 0;
}

#pragma clang diagnostic pop
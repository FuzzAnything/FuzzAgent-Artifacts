// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

// Helper to safely parse integers from fuzz data
static int parse_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

// Helper to safely parse unsigned ints
static unsigned int parse_uint_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) {
        return 0;
    }
    unsigned int val;
    memcpy(&val, data + *offset, sizeof(unsigned int));
    *offset += sizeof(unsigned int);
    return val;
}

// Helper to safely parse long
static long parse_long_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(long) > size) {
        return 0;
    }
    long val;
    memcpy(&val, data + *offset, sizeof(long));
    *offset += sizeof(long);
    return val;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;

    // 1. Fuzz vpx_codec_build_config
    const char *build_config = vpx_codec_build_config();
    if (build_config != NULL) {
        size_t len = strlen(build_config);
        if (Size > len) {
            (void)Data[len % Size];
        }
    }

    // 2. Fuzz vpx_codec_version_str
    const char *version_str = vpx_codec_version_str();
    if (version_str != NULL) {
        size_t len = strlen(version_str);
        if (Size > len) {
            (void)Data[len % Size];
        }
    }

    // 4. Fuzz vpx_codec_version
    int version_int = vpx_codec_version();
    if (version_int > 0) {
        if (Size > 0) {
            (void)Data[0];
        }
    }

    // 5. Fuzz vpx_codec_version_extra_str
    const char *extra_str = vpx_codec_version_extra_str();
    if (extra_str != NULL) {
        size_t len = strlen(extra_str);
        if (Size > len) {
            (void)Data[len % Size];
        }
    }

    // 6. Fuzz vpx_codec_iface_name with NULL
    const char *iface_name_null = vpx_codec_iface_name(NULL);
    if (iface_name_null != NULL) {
        (void)strlen(iface_name_null);
    }

    // 3. Fuzz vpx_codec_dec_init_ver
    vpx_codec_ctx_t ctx = {0};
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_flags_t flags = 0;
    int ver = VPX_DECODER_ABI_VERSION;

    // Parse configuration from fuzz data
    if (Size >= 1) {
        cfg.threads = Data[0] & 0x0F;
    }
    if (Size >= 3) {
        cfg.w = Data[1] | (Data[2] << 8);
    }
    if (Size >= 5) {
        cfg.h = Data[3] | (Data[4] << 8);
    }

    // Parse flags and version from data if available
    if (offset < Size) {
        flags = parse_long_from_data(Data, Size, &offset);
    }
    if (offset < Size) {
        int parsed_ver = parse_int_from_data(Data, Size, &offset);
        if (parsed_ver > 0 && parsed_ver <= 10) {
            ver = parsed_ver;
        }
    }

    // Get the VP8 decoder interface using the standard API function
    // This avoids linking errors with external symbols like vp8_decoder_iface
    iface = vpx_codec_vp8_dx();

    // Fuzz vpx_codec_iface_name with valid interface
    if (iface != NULL) {
        const char *iface_name = vpx_codec_iface_name(iface);
        if (iface_name != NULL) {
            (void)strlen(iface_name);
        }
    }

    // Initialize the decoder context
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, ver);
    
    // Handle return value
    if (err != VPX_CODEC_OK) {
        // Initialization failed
    } else {
        // Initialization succeeded
        if (ctx.iface != NULL) {
            const char *ctx_iface_name = vpx_codec_iface_name(ctx.iface);
            if (ctx_iface_name != NULL) {
                (void)strlen(ctx_iface_name);
            }
        }
        
        // Cleanup: Destroy the codec context
        vpx_codec_destroy(&ctx);
    }

    return 0;
}
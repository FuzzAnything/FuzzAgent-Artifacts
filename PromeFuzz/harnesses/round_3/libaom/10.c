// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_decoder_init(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (size < sizeof(aom_codec_flags_t)) {
        return;
    }
    
    aom_codec_flags_t flags;
    memcpy(&flags, data, sizeof(aom_codec_flags_t));
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, NULL, flags, AOM_DECODER_ABI_VERSION);
    
    if (err == AOM_CODEC_OK) {
        aom_codec_decode(&ctx, data, size, NULL);
        aom_codec_destroy(&ctx);
    }
}

static void fuzz_peek_stream_info(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_stream_info_t si;
    
    memset(&si, 0, sizeof(si));
    
    if (size > 0) {
        si.is_annexb = data[0] & 1;
    } else {
        si.is_annexb = 0;
    }
    
    aom_codec_peek_stream_info(iface, data, size, &si);
}

static void fuzz_codec_caps_and_name(void) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (iface != NULL) {
        aom_codec_caps_t caps = aom_codec_get_caps(iface);
        (void)caps;
        
        const char *name = aom_codec_iface_name(iface);
        (void)name;
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    fuzz_codec_caps_and_name();
    
    uint8_t selector = data[0];
    const uint8_t *fuzz_data = data + 1;
    size_t fuzz_size = size > 1 ? size - 1 : 0;
    
    if (selector & 1) {
        fuzz_decoder_init(fuzz_data, fuzz_size);
    }
    
    if (selector & 2) {
        fuzz_peek_stream_info(fuzz_data, fuzz_size);
    }
    
    return 0;
}
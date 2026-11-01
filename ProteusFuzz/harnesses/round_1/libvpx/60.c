// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"

static void safe_init_and_destroy(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface,
                                  const vpx_codec_dec_cfg_t *cfg,
                                  vpx_codec_flags_t flags) {
  if (ctx == NULL || iface == NULL) {
    return;
  }

  vpx_codec_err_t res = vpx_codec_dec_init_ver(ctx, iface, cfg, flags, VPX_DECODER_ABI_VERSION);
  if (res == VPX_CODEC_OK) {
    vpx_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
  if (Data == NULL || Size == 0) {
    return 0;
  }

  // 1. Test vpx_codec_version
  int version = vpx_codec_version();
  // Use bit manipulation to extract version components as macros might be missing
  // or defined differently in some builds. Standard encoding is (major << 16) | (minor << 8) | patch
  int major = (version >> 16) & 0xFF;
  int minor = (version >> 8) & 0xFF;
  int patch = version & 0xFF;
  (void)major; (void)minor; (void)patch;

  // 2. Test vpx_codec_version_str
  const char *ver_str = vpx_codec_version_str();
  if (ver_str != NULL) {
    size_t len = strlen(ver_str);
    if (len > 0) {
      volatile char c = ver_str[0];
      (void)c;
    }
  }

  // 3. Test vpx_codec_version_extra_str
  const char *extra_str = vpx_codec_version_extra_str();
  if (extra_str != NULL) {
    size_t len = strlen(extra_str);
    if (len > 0) {
      volatile char c = extra_str[0];
      (void)c;
    }
  }

  // 4. Test vpx_codec_build_config
  const char *build_cfg = vpx_codec_build_config();
  if (build_cfg != NULL) {
    size_t len = strlen(build_cfg);
    if (len > 0) {
      volatile char c = build_cfg[0];
      (void)c;
    }
  }

  // 5. Test vpx_codec_iface_name with NULL
  const char *null_iface_name = vpx_codec_iface_name(NULL);
  if (null_iface_name != NULL) {
    volatile char c = null_iface_name[0];
    (void)c;
  }

  // 6. Test vpx_codec_dec_init_ver
  vpx_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  vpx_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  
  if (Size >= 4) {
    cfg.threads = Data[0];
    cfg.w = ((Data[1] << 8) | Data[2]);
    cfg.h = Data[3];
  } else {
    cfg.threads = 1;
    cfg.w = 16;
    cfg.h = 16;
  }

  vpx_codec_flags_t flags = 0;
  if (Size >= 5) {
    flags = Data[4];
  }

  // Get VP8 decoder interface using the function pointer as declared in vp8dx.h
  vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_dx();
  safe_init_and_destroy(&ctx, iface_vp8, &cfg, flags);

  // Get VP9 decoder interface using the function pointer as declared in vp8dx.h
  vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_dx();
  safe_init_and_destroy(&ctx, iface_vp9, &cfg, flags);

  // Try with NULL interface to test error handling in init_ver
  memset(&ctx, 0, sizeof(ctx));
  vpx_codec_err_t res_null = vpx_codec_dec_init_ver(&ctx, NULL, &cfg, flags, VPX_DECODER_ABI_VERSION);
  (void)res_null;

  // Test vpx_codec_iface_name with a crafted pointer (non-NULL, non-valid)
  uint8_t fake_iface_buf[64];
  memset(fake_iface_buf, 0, sizeof(fake_iface_buf));
  vpx_codec_iface_t *fake_iface = (vpx_codec_iface_t *)fake_iface_buf;
  
  const char *fake_name = vpx_codec_iface_name(fake_iface);
  if (fake_name != NULL) {
    volatile char c = fake_name[0];
    (void)c;
  }

  return 0;
}
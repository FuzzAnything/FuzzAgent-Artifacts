// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

// Helper to safely extract integers from fuzz input
static int get_int_from_input(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + sizeof(int) > size) return 0;
  int val;
  memcpy(&val, data + *offset, sizeof(int));
  *offset += sizeof(int);
  return val;
}

static unsigned int get_uint_from_input(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + sizeof(unsigned int) > size) return 0;
  unsigned int val;
  memcpy(&val, data + *offset, sizeof(unsigned int));
  *offset += sizeof(unsigned int);
  return val;
}

// Callback for getting frame buffers
// Signature: int cb_get(void *cb_priv, unsigned long frame_size, struct aom_codec_frame_buffer *fb)
static int cb_get_frame_buffer(void *cb_priv, unsigned long frame_size, struct aom_codec_frame_buffer *fb) {
  (void)cb_priv;
  (void)frame_size;
  (void)fb;
  // Return 0 to indicate failure to provide buffer, or 1 if we had a valid one.
  // For fuzzing, returning 0 is safe to test error handling.
  return 0;
}

// Callback for releasing frame buffers
// Signature: int cb_release(void *cb_priv, struct aom_codec_frame_buffer *fb)
static int cb_release_frame_buffer(void *cb_priv, struct aom_codec_frame_buffer *fb) {
  (void)cb_priv;
  (void)fb;
  return 0;
}

int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  size_t offset = 0;
  int seed = get_int_from_input(Data, Size, &offset);

  // Test 1: aom_img_alloc
  {
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    unsigned int w = get_uint_from_input(Data, Size, &offset);
    unsigned int h = get_uint_from_input(Data, Size, &offset);
    unsigned int align = get_uint_from_input(Data, Size, &offset);
    int fmt_idx = get_int_from_input(Data, Size, &offset);
    
    // Validate bounds to prevent overflow and excessive allocation
    if (w > 0x08000000 || h > 0x08000000 || align > 65536) {
      w = 16; h = 16; align = 1;
    }
    
    aom_img_fmt_t fmt = (aom_img_fmt_t)(fmt_idx % 10); // Limit to known formats roughly
    if (fmt == AOM_IMG_FMT_NONE) fmt = AOM_IMG_FMT_I420;

    aom_image_t *allocated_img = aom_img_alloc(&img, fmt, w, h, align);
    if (allocated_img) {
      aom_img_free(allocated_img);
    }
  }

  // Test 2: aom_img_wrap
  {
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    unsigned int w = get_uint_from_input(Data, Size, &offset);
    unsigned int h = get_uint_from_input(Data, Size, &offset);
    unsigned int stride_align = get_uint_from_input(Data, Size, &offset);
    int fmt_idx = get_int_from_input(Data, Size, &offset);

    if (w > 0x08000000 || h > 0x08000000 || stride_align > 65536) {
      w = 16; h = 16; stride_align = 1;
    }

    aom_img_fmt_t fmt = (aom_img_fmt_t)(fmt_idx % 10);
    if (fmt == AOM_IMG_FMT_NONE) fmt = AOM_IMG_FMT_I420;

    // Allocate a buffer large enough for wrapping
    // Using a safe upper bound for calculation
    size_t max_size = (size_t)w * h * 4; 
    if (max_size > 1024 * 1024) max_size = 1024 * 1024; // Cap allocation
    unsigned char *img_data = (unsigned char *)malloc(max_size);
    
    if (img_data) {
      aom_image_t *wrapped_img = aom_img_wrap(&img, fmt, w, h, stride_align, img_data);
      if (wrapped_img) {
        // Test aom_img_plane_width
        int plane = get_int_from_input(Data, Size, &offset);
        if (plane >= 0 && plane < 3) {
          int width = aom_img_plane_width(wrapped_img, plane);
          (void)width; // Use width to avoid unused warning
        }
        aom_img_free(wrapped_img);
      }
      free(img_data);
    }
  }

  // Test 3: aom_codec_set_frame_buffer_functions
  {
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Initialize codec context to avoid ERROR return
    // We need a valid interface. Let's try to get AV1 decoder interface.
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface) {
      aom_codec_dec_cfg_t cfg;
      memset(&cfg, 0, sizeof(cfg));
      cfg.threads = 1;
      
      if (aom_codec_dec_init(&ctx, iface, &cfg, 0) == AOM_CODEC_OK) {
        // Test with valid callbacks
        aom_codec_err_t ret = aom_codec_set_frame_buffer_functions(
            &ctx, cb_get_frame_buffer, cb_release_frame_buffer, NULL);
        (void)ret;

        // Test with NULL callbacks (should return INVALID_PARAM)
        ret = aom_codec_set_frame_buffer_functions(&ctx, NULL, NULL, NULL);
        (void)ret;

        aom_codec_destroy(&ctx);
      }
    }
  }

  // Test 4: aom_img_free with NULL
  {
    aom_img_free(NULL);
  }

  return 0;
}
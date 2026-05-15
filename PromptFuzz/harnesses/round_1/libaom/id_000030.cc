#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_destroy","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_set_rect","aom_codec_destroy","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_get_cx_data","aom_codec_encode","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_destroy","aom_img_free"],"visited":0}
/*Here's a C++ fuzz driver that exercises the specified libaom APIs to decode and re-encode AV1 video data:

*/


extern "C" {
}

// Frame buffer callback structures
struct BufferData {
  std::vector<uint8_t> data;
  size_t used;
};

static int  id_000030_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
  BufferData *const buffer_data = static_cast<BufferData*>(cb_priv);
  
  if (min_size > buffer_data->data.size() - buffer_data->used) {
    // Need more space
    const size_t new_size = buffer_data->used + min_size;
    buffer_data->data.resize(new_size);
  }
  
  fb->data = buffer_data->data.data() + buffer_data->used;
  fb->size = min_size;
  buffer_data->used += min_size;
  
  return 0;
}

static int  id_000030_release_frame_buffer(void *cb_priv, 
                                aom_codec_frame_buffer_t *fb) {
  // Nothing to do here for this simple implementation
  (void)cb_priv;
  (void)fb;
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
	return 0;
	}
	
  
  // 1. Get version info (call aom_codec_version_extra_str)
  const char *version_extra = aom_codec_version_extra_str();
  (void)version_extra; // Use variable to avoid unused warning
  
  // 2. Create decoder and encoder interfaces
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  
  if (!decoder_iface || !encoder_iface) {
	return 0;
	}
	
  
  // 3. Create decoder context
  aom_codec_ctx_t decoder_ctx;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.w = 640;
  dec_cfg.h = 480;
  dec_cfg.allow_lowbitdepth = 1;
  
  aom_codec_err_t res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                               &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
  if (res != AOM_CODEC_OK) {
	return 0;
	}
	
  
  // 4. Set frame buffer functions for decoder
  BufferData buffer_data;
  buffer_data.data.resize(size * 4); // Allocate some initial space
  buffer_data.used = 0;
  
  res = aom_codec_set_frame_buffer_functions(&decoder_ctx,  id_000030_get_frame_buffer, 
                                             id_000030_release_frame_buffer, &buffer_data);
  if (res != AOM_CODEC_OK) {
    aom_codec_destroy(&decoder_ctx);
    return 0;
  }
  
  // 5. Peek at stream info
  aom_codec_stream_info_t stream_info = {0};
  aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
  
  // 6. Decode the input data
  res = aom_codec_decode(&decoder_ctx, data, size, nullptr);
  if (res != AOM_CODEC_OK) {
    aom_codec_destroy(&decoder_ctx);
    return 0;
  }
  
  // 7. Get stream info again after decoding
  aom_codec_get_stream_info(&decoder_ctx, &stream_info);
  
  // 8. Get decoded frames
  std::vector<aom_image_t*> decoded_frames;
  aom_codec_iter_t iter = nullptr;
  aom_image_t *img;
  
  while ((img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
    // Make a copy of the image to modify
    aom_image_t *img_copy = aom_img_alloc(nullptr, img->fmt, 
                                         img->d_w, img->d_h, 16);
    if (img_copy) {
      // Copy image data
      for (int plane = 0; plane < 3; ++plane) {
        int width = aom_img_plane_width(img, plane);
        int height = aom_img_plane_height(img, plane);
        
        if (width > 0 && height > 0) {
          uint8_t *src = img->planes[plane];
          uint8_t *dst = img_copy->planes[plane];
          int src_stride = img->stride[plane];
          int dst_stride = img_copy->stride[plane];
          
          for (int y = 0; y < height; ++y) {
            memcpy(dst, src, width);
            src += src_stride;
            dst += dst_stride;
          }
        }
      }
      
      // 9. Set a rectangle on the image (call aom_img_set_rect)
      // Use safe dimensions within image bounds
      unsigned int rect_w = img->d_w > 16 ? 16 : img->d_w;
      unsigned int rect_h = img->d_h > 16 ? 16 : img->d_h;
      unsigned int border = 0;
      
      if (rect_w > 0 && rect_h > 0) {
        aom_img_set_rect(img_copy, 0, 0, rect_w, rect_h, border);
      }
      
      decoded_frames.push_back(img_copy);
    }
  }
  
  // Destroy decoder
  aom_codec_destroy(&decoder_ctx);
  
  // 10. Create encoder context if we have decoded frames
  if (!decoded_frames.empty()) {
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res == AOM_CODEC_OK) {
      // Set some reasonable values
      enc_cfg.g_w = decoded_frames[0]->d_w;
      enc_cfg.g_h = decoded_frames[0]->d_h;
      enc_cfg.g_timebase.num = 1;
      enc_cfg.g_timebase.den = 30;
      enc_cfg.rc_target_bitrate = 1000;
      enc_cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;
      
      res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                   0, AOM_ENCODER_ABI_VERSION);
      if (res == AOM_CODEC_OK) {
        // 11. Set encoder options (call aom_codec_set_option)
        aom_codec_set_option(&encoder_ctx, "cpu-used", "5");
        aom_codec_set_option(&encoder_ctx, "end-usage", "cq");
        aom_codec_set_option(&encoder_ctx, "cq-level", "30");
        
        // 12. Encode each frame
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        for (auto frame : decoded_frames) {
          res = aom_codec_encode(&encoder_ctx, frame, pts, duration, flags);
          pts += duration;
          
          if (res != AOM_CODEC_OK) break;
          
          // 13. Get encoded data
          const aom_codec_cx_pkt_t *pkt;
          aom_codec_iter_t enc_iter = nullptr;
          
          while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
              // We got an encoded frame packet
              (void)pkt; // Use variable
            }
          }
        }
        
        // Flush encoder
        res = aom_codec_encode(&encoder_ctx, nullptr, pts, 0, flags);
        
        // Get any remaining packets
        const aom_codec_cx_pkt_t *pkt;
        aom_codec_iter_t enc_iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
          (void)pkt; // Use variable
        }
        
        // 14. Use aom_uleb_encode
        uint64_t test_value = 12345;
        uint8_t uleb_buffer[16];
        size_t coded_size = 0;
        
        aom_uleb_encode(test_value, sizeof(uleb_buffer), uleb_buffer, &coded_size);
        
        // 15. Destroy encoder
        aom_codec_destroy(&encoder_ctx);
      }
    }
  }
  
  // Cleanup decoded frames
  for (auto frame : decoded_frames) {
    aom_img_free(frame);
  }
  
  return 0;
}
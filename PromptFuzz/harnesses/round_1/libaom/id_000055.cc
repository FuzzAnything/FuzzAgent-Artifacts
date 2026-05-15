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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_remove_metadata","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_remove_metadata","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
  uint8_t* data;
  size_t size;
};

// Frame buffer get callback
int  id_000055_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
  FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
  
  // Allocate or reallocate buffer if needed
  if (fb_data->size < min_size) {
    free(fb_data->data);
    fb_data->data = static_cast<uint8_t*>(malloc(min_size));
    if (!fb_data->data) return -1;
    fb_data->size = min_size;
  }
  
  fb->data = fb_data->data;
  fb->size = fb_data->size;
  return 0;
}

// Frame buffer release callback
int  id_000055_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
  // Do nothing - we'll free the buffer in our destructor
  (void)cb_priv;
  (void)fb;
  return 0;
}

// Main fuzzing function
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 1) {
	return 0;
	}
	
  
  // 1. Get codec version string
  const char* version_str = aom_codec_version_str();
  (void)version_str; // Mark as used
  
  // 2. Get decoder interface
  aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) {
	return 0;
	}
	
  
  // 3. Peek stream info
  aom_codec_stream_info_t stream_info;
  memset(&stream_info, 0, sizeof(stream_info));
  aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
  
  // 4. Initialize decoder
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.w = stream_info.w;
  dec_cfg.h = stream_info.h;
  
  if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
    return 0;
  }
  
  // 5. Set frame buffer functions
  FrameBufferData fb_data = {nullptr, 0};
  if (aom_codec_set_frame_buffer_functions(&decoder,  id_000055_get_frame_buffer,  id_000055_release_frame_buffer, &fb_data) != AOM_CODEC_OK) {
    aom_codec_destroy(&decoder);
    if (fb_data.data) free(fb_data.data);
    return 0;
  }
  
  // 6. Decode the input data
  if (aom_codec_decode(&decoder, data, size, nullptr) != AOM_CODEC_OK) {
    aom_codec_destroy(&decoder);
    if (fb_data.data) free(fb_data.data);
    return 0;
  }
  
  // 7. Get encoder default config
  aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
  if (!encoder_iface) {
    aom_codec_destroy(&decoder);
    if (fb_data.data) free(fb_data.data);
    return 0;
  }
  
  aom_codec_enc_cfg_t enc_cfg;
  if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
    aom_codec_destroy(&decoder);
    if (fb_data.data) free(fb_data.data);
    return 0;
  }
  
  // 8. Initialize encoder
  aom_codec_ctx_t encoder;
  if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
    aom_codec_destroy(&decoder);
    if (fb_data.data) free(fb_data.data);
    return 0;
  }
  
  // 9. Get decoded frames
  aom_codec_iter_t iter = nullptr;
  aom_image_t* decoded_img = nullptr;
  
  while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
    // 10. Get plane dimensions
    int plane_width = aom_img_plane_width(decoded_img, 0);
    int plane_height = aom_img_plane_height(decoded_img, 0);
    (void)plane_width;
    (void)plane_height;
    
    // 11. Add metadata
    uint8_t metadata[] = {0x01, 0x02, 0x03};
    aom_img_add_metadata(decoded_img, 1, metadata, sizeof(metadata), AOM_MIF_ANY_FRAME);
    
    // 12. Get metadata count
    size_t metadata_count = aom_img_num_metadata(decoded_img);
    (void)metadata_count;
    
    // 13. Allocate a new image for encoding
    aom_image_t* img_to_encode = aom_img_alloc(nullptr, decoded_img->fmt, 
                                              decoded_img->d_w, decoded_img->d_h, 1);
    if (img_to_encode) {
      // Copy image data (simplified - in reality would need proper copy)
      for (int plane = 0; plane < 3; plane++) {
        if (decoded_img->planes[plane] && img_to_encode->planes[plane]) {
          int plane_w = aom_img_plane_width(decoded_img, plane);
          int plane_h = aom_img_plane_height(decoded_img, plane);
          size_t bytes_to_copy = plane_w * plane_h;
          if (bytes_to_copy > 0 && plane_w > 0 && plane_h > 0) {
            memcpy(img_to_encode->planes[plane], decoded_img->planes[plane], bytes_to_copy);
          }
        }
      }
      
      // 14. Encode the image
      aom_codec_encode(&encoder, img_to_encode, 0, 1, 0);
      
      // 15. Get encoded data
      aom_codec_iter_t enc_iter = nullptr;
      const aom_codec_cx_pkt_t* pkt = nullptr;
      while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Process packet if needed
        (void)pkt;
      }
      
      // 16. Remove metadata from original image
      aom_img_remove_metadata(decoded_img);
      
      // 17. Free the allocated image
      aom_img_free(img_to_encode);
    }
  }
  
  // 18. Clean up
  aom_codec_destroy(&encoder);
  aom_codec_destroy(&decoder);
  if (fb_data.data) free(fb_data.data);
  
  return 0;
}
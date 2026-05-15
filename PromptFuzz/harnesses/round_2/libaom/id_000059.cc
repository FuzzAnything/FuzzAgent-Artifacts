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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_img_wrap","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_destroy","aom_img_remove_metadata"],"critical_calls":["aom_uleb_decode","aom_img_wrap","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_destroy","aom_img_remove_metadata"],"visited":0}
/**/


extern "C" {
}

// Assuming libaom headers are available
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size == 0) {
	return 0;
	}
	

  // 1. Decode ULEB value from input
  uint64_t uleb_value = 0;
  size_t uleb_length = 0;
  if (aom_uleb_decode(data, size, &uleb_value, &uleb_length) != 0) {
    // If decode fails, still proceed with other APIs
    uleb_value = 0;
    uleb_length = 0;
  }

  // 2. Create image with metadata
  // Use minimal dimensions for fuzzing
  const unsigned int width = 16;
  const unsigned int height = 16;
  const unsigned int stride_align = 1;
  
  // Allocate image buffer (simplified for fuzzing)
  const size_t buffer_size = width * height * 3; // Overestimate for YUV formats
  unsigned char* img_buffer = (unsigned char*)malloc(buffer_size);
  if (!img_buffer) {
	return 0;
	}
	
  
  // Initialize with zero
  memset(img_buffer, 0, buffer_size);
  
  // Create image using aom_img_wrap
  aom_image_t img;
  aom_image_t* wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, width, height, 
                                         stride_align, img_buffer);
  
  if (wrapped_img) {
    // 3. Add metadata using decoded ULEB value
    // Use first byte of input as metadata type, limited size
    uint32_t metadata_type = size > 0 ? data[0] : 0;
    size_t metadata_size = size > 1 ? (size - 1) % 256 : 0; // Limit metadata size
    
    if (metadata_size > 0 && size > 1) {
      aom_img_add_metadata(wrapped_img, metadata_type, data + 1, 
                          metadata_size, AOM_MIF_ANY_FRAME);
    }
    
    // 4. Get metadata back (if any was added)
    size_t metadata_count = aom_img_num_metadata(wrapped_img);
    for (size_t i = 0; i < metadata_count; i++) {
      const aom_metadata_t* metadata = aom_img_get_metadata(wrapped_img, i);
      if (metadata) {
        // Copy metadata to avoid use-after-free
        aom_metadata_t* metadata_copy = aom_img_metadata_alloc(
            metadata->type, metadata->payload, metadata->sz, metadata->insert_flag);
        if (metadata_copy) {
          aom_img_metadata_free(metadata_copy);
        }
      }
    }
    
    // 5. Setup encoder with metadata
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (encoder_iface) {
      // Get interface name
      const char* iface_name = aom_codec_iface_name(encoder_iface);
      (void)iface_name; // Use variable to avoid unused warning
      
      // Get encoder capabilities
      aom_codec_caps_t caps = aom_codec_get_caps(encoder_iface);
      (void)caps; // Use variable to avoid unused warning
      
      // Create encoder context
      aom_codec_ctx_t encoder;
      aom_codec_enc_cfg_t cfg;
      
      // Initialize encoder with default config
      if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) == AOM_CODEC_OK) {
        // Set minimal dimensions for fuzzing
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_threads = 1;
        cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, 
                                   AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
          // Try to set encoder config
          aom_codec_enc_config_set(&encoder, &cfg);
          
          // 6. Decode using the same input (partial decode)
          aom_codec_ctx_t decoder;
          aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
          
          if (decoder_iface) {
            if (aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0,
                                       AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
              // Decode partial input (limit to avoid excessive processing)
              size_t decode_size = size > 4096 ? 4096 : size;
              aom_codec_decode(&decoder, data, decode_size, NULL);
              
              // Clean up decoder
              aom_codec_destroy(&decoder);
            }
          }
          
          // Clean up encoder
          aom_codec_destroy(&encoder);
        }
      }
    }
    
    // Clean up image metadata
    aom_img_remove_metadata(wrapped_img);
  }
  
  // Free allocated resources
  free(img_buffer);
  
  return 0;
}
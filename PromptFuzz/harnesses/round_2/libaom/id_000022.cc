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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_set_rect","aom_img_alloc","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_set_rect","aom_img_alloc","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Early exit if input is too small
  if (size < 100) {
	return 0;
	}
	
  
  // Use aom_codec_build_config()
  const char* build_config = aom_codec_build_config();
  (void)build_config;  // Prevent unused variable warning
  
  // Create decoder context
  aom_codec_ctx_t decoder;
  aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
  
  // Initialize decoder with default config
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 1;
  dec_cfg.w = 640;
  dec_cfg.h = 480;
  dec_cfg.allow_lowbitdepth = 1;
  
  aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                              0, AOM_DECODER_ABI_VERSION);
  
  // Use aom_codec_err_to_string() for error handling
  if (err != AOM_CODEC_OK) {
    const char* err_str = aom_codec_err_to_string(err);
    (void)err_str;
    return 0;
  }
  
  // Decode the input data
  FILE* in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
  
  // Read a chunk for decoding
  std::vector<uint8_t> decode_buffer(size);
  size_t read_size = fread(decode_buffer.data(), 1, size, in_file);
  assert_file_closed(&in_file);;
  
  if (read_size > 0) {
    err = aom_codec_decode(&decoder, decode_buffer.data(), read_size, nullptr);
    
    if (err != AOM_CODEC_OK) {
      const char* err_str = aom_codec_err_to_string(err);
      (void)err_str;
    }
    
    // Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t* decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
      // Use image plane dimensions
      int plane_width = aom_img_plane_width(decoded_img, 0);
      int plane_height = aom_img_plane_height(decoded_img, 0);
      
      // Flip the image
      aom_img_flip(decoded_img);
      
      // Set a rectangle on the image
      unsigned int x = 0, y = 0, w = 100, h = 100, border = 0;
      if (plane_width > 100 && plane_height > 100) {
        aom_img_set_rect(decoded_img, x, y, w, h, border);
      }
      
      // Create a test image for encoding
      aom_image_t* test_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                            640, 480, 1);
      if (test_img) {
        // Encode ULEB value for metadata
        uint64_t uleb_value = 12345;
        uint8_t uleb_buffer[10];
        size_t uleb_coded_size = 0;
        int uleb_result = aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer),
                                                    5, uleb_buffer, &uleb_coded_size);
        
        if (uleb_result == 0 && uleb_coded_size > 0) {
          // Add metadata to image
          uint32_t metadata_type = 1;  // Example type
          aom_img_add_metadata(test_img, metadata_type, uleb_buffer, 
                              uleb_coded_size, AOM_MIF_ANY_FRAME);
        }
        
        // Create encoder context
        aom_codec_ctx_t encoder;
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        
        // Get default encoder config
        aom_codec_enc_cfg_t enc_cfg;
        unsigned int usage = 0;  // Good quality
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
        
        if (err == AOM_CODEC_OK) {
          // Set some encoder parameters
          enc_cfg.g_w = 640;
          enc_cfg.g_h = 480;
          enc_cfg.g_threads = 1;
          enc_cfg.rc_target_bitrate = 1000;
          
          // Initialize encoder
          err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg,
                                       0, AOM_ENCODER_ABI_VERSION);
          
          if (err == AOM_CODEC_OK) {
            // Use encoder control
            aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
            
            // Encode the test image
            err = aom_codec_encode(&encoder, test_img, 0, 1, 0);
            
            if (err == AOM_CODEC_OK) {
              // Get encoded data
              aom_codec_iter_t enc_iter = nullptr;
              const aom_codec_cx_pkt_t* pkt = nullptr;
              
              while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                  // Write to output file
                  FILE* out_file = fopen("output_file", "wb");
                  if (out_file) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    assert_file_closed(&out_file);;
                  }
                  break;
                }
              }
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
          }
        }
        
        // Free test image
        aom_img_free(test_img);
      }
    }
  }
  
  // Clean up decoder
  aom_codec_destroy(&decoder);
  
  assert_file_closed(&in_file);
	return 0;
}
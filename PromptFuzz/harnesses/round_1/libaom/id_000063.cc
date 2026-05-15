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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_destroy","aom_uleb_decode","aom_img_alloc","aom_codec_destroy","aom_img_free","aom_codec_error"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_destroy","aom_uleb_decode","aom_img_alloc","aom_codec_destroy","aom_img_free","aom_codec_error"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size == 0) {
    return 0;
  }

  // 1. Get codec version
  const char *version_str = aom_codec_version_str();
  
  // 2. Decoder side: Try to decode the input data
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  const char *decoder_name = aom_codec_iface_name(decoder_iface);
  
  // 3. Peek stream info from input data
  aom_codec_stream_info_t stream_info;
  memset(&stream_info, 0, sizeof(stream_info));
  aom_codec_err_t peek_result = aom_codec_peek_stream_info(
      decoder_iface, data, size, &stream_info);
  
  // 4. Initialize decoder context
  aom_codec_ctx_t decoder_ctx;
  aom_codec_dec_cfg_t dec_cfg = {0};
  aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
      &decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
  
  if (dec_init_result == AOM_CODEC_OK) {
    // 5. Get stream info using decoder context
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // 6. Decode the input data
    aom_codec_decode(&decoder_ctx, data, size, nullptr);
    
    // 7. Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    if (decoded_img) {
      // 8. Use image plane functions
      int plane_width = aom_img_plane_width(decoded_img, 0);
      int plane_height = aom_img_plane_height(decoded_img, 0);
      
      // 9. Test metadata operations
      uint32_t metadata_type = 1;
      aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
      
      // Use a small portion of input data for metadata
      size_t metadata_size = size > 100 ? 100 : size;
      int add_meta_result = aom_img_add_metadata(
          decoded_img, metadata_type, data, metadata_size, insert_flag);
      
      if (add_meta_result == 0) {
        size_t num_metadata = aom_img_num_metadata(decoded_img);
        aom_img_remove_metadata(decoded_img);
      }
      
      // 10. Encoder side: Try to re-encode the decoded frame
      aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
      const char *encoder_name = aom_codec_iface_name(encoder_iface);
      
      aom_codec_ctx_t encoder_ctx;
      aom_codec_enc_cfg_t enc_cfg;
      
      // Get default encoder configuration
      aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
      
      // Set basic configuration
      enc_cfg.g_w = decoded_img->d_w;
      enc_cfg.g_h = decoded_img->d_h;
      enc_cfg.g_timebase.num = 1;
      enc_cfg.g_timebase.den = 30;
      enc_cfg.rc_target_bitrate = 1000;
      enc_cfg.g_error_resilient = 0;
      
      // Initialize encoder
      aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
          &encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
      
      if (enc_init_result == AOM_CODEC_OK) {
        // 11. Encode the image
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;
        
        aom_codec_encode(&encoder_ctx, decoded_img, pts, duration, flags);
        
        // 12. Get encoded data
        aom_codec_iter_t enc_iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
          // Process the packet if needed
        }
        
        // Get error if any
        const char *encoder_error = aom_codec_error(&encoder_ctx);
        
        // 13. Clean up encoder
        aom_codec_destroy(&encoder_ctx);
      }
    }
    
    // 14. Test ULEB decoding on input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t uleb_available = size > 10 ? 10 : size;
    aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    
    // 15. Test image allocation
    aom_image_t *allocated_img = nullptr;
    if (stream_info.w > 0 && stream_info.h > 0) {
      allocated_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                    stream_info.w, stream_info.h, 1);
    }
    
    // 16. Clean up decoder
    aom_codec_destroy(&decoder_ctx);
    
    // 17. Free allocated image
    if (allocated_img) {
      aom_img_free(allocated_img);
    }
  }
  
  // 18. Get error if any
  const char *decoder_error = aom_codec_error(&decoder_ctx);
  
  return 0;
}
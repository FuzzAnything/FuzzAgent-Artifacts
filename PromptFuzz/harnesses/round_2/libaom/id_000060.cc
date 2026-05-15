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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 112, nr_unique_branch: 3
//<Quality> {"density":28,"unique_branches":{"get_lr_job_info":[[782,7,782,27,0,0,4,1]],"lr_sync_write":[[565,7,565,10,0,0,4,1]],"get_cdef_row_next_job":[[1065,7,1065,31,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_height","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_destroy","aom_codec_destroy","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000060_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
  fb->data = (uint8_t *)malloc(min_size);
  if (!fb->data) return -1;
  fb->size = min_size;
  return 0;
}

static int  id_000060_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
  free(fb->data);
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  if (size < 100) {
	return 0;
	}
	  // Too small for meaningful fuzzing
  
  // 1. Initialize decoder
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  aom_codec_ctx_t decoder_ctx;
  
  // Get decoder capabilities
  aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
  (void)caps; // Use variable to prevent unused warning
  
  // Set frame buffer functions
  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = 2;
  dec_cfg.w = 640;
  dec_cfg.h = 480;
  dec_cfg.allow_lowbitdepth = 1;
  
  if (aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 
                            0, AOM_DECODER_ABI_VERSION)) {
    const char *detail = aom_codec_error_detail(&decoder_ctx);
    (void)detail; // Use variable
    return 0;
  }
  
  // Set frame buffer callbacks
  if (aom_codec_set_frame_buffer_functions(&decoder_ctx, 
                                           id_000060_get_frame_buffer,
                                           id_000060_release_frame_buffer,
                                          nullptr)) {
    aom_codec_destroy(&decoder_ctx);
    return 0;
  }
  
  // 2. Decode input data
  if (aom_codec_decode(&decoder_ctx, data, size, nullptr)) {
    const char *detail = aom_codec_error_detail(&decoder_ctx);
    (void)detail; // Use variable
    aom_codec_destroy(&decoder_ctx);
    return 0;
  }
  
  // 3. Get decoded frame
  aom_codec_iter_t iter = nullptr;
  aom_image_t *img = aom_codec_get_frame(&decoder_ctx, &iter);
  
  // 4. Process frame if available
  if (img) {
    // Get plane height
    int plane_height = aom_img_plane_height(img, 0);
    (void)plane_height; // Use variable
    
    // Create and add metadata
    const uint8_t meta_data[] = {0x01, 0x02, 0x03};
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        1234, meta_data, sizeof(meta_data), AOM_MIF_KEY_FRAME);
    
    if (metadata) {
      aom_img_add_metadata(img, metadata->type, metadata->payload,
                          metadata->sz, metadata->insert_flag);
      
      // Get number of metadata
      size_t meta_count = aom_img_num_metadata(img);
      (void)meta_count; // Use variable
      
      aom_img_metadata_free(metadata);
    }
    
    // 5. Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0)) {
      aom_codec_destroy(&decoder_ctx);
      return 0;
    }
    
    // Configure encoder
    enc_cfg.g_w = img->d_w;
    enc_cfg.g_h = img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 2;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 0;
    
    if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg,
                              0, AOM_ENCODER_ABI_VERSION)) {
      aom_codec_destroy(&decoder_ctx);
      return 0;
    }
    
    // Try to set a control (e.g., enable temporal scaling)
    aom_codec_control(&encoder_ctx, AOME_SET_CPUUSED, 6);
    aom_codec_control(&decoder_ctx, AV1D_SET_OUTPUT_ALL_LAYERS, 1);
    
    // 6. Encode the frame
    if (aom_codec_encode(&encoder_ctx, img, 0, 1, 0)) {
      aom_codec_destroy(&encoder_ctx);
      aom_codec_destroy(&decoder_ctx);
      return 0;
    }
    
    // 7. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Successfully got encoded packet
        break;
      }
    }
    
    // 8. Cleanup encoder
    aom_codec_destroy(&encoder_ctx);
  }
  
  // 9. Cleanup decoder
  aom_codec_destroy(&decoder_ctx);
  
  return 0;
}
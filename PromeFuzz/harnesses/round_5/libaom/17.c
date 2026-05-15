// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
  // Call the simple version functions that don't require any input
  const char *extra_str = aom_codec_version_extra_str();
  (void)extra_str; // Use the result to avoid compiler warnings
  
  int version = aom_codec_version();
  (void)version;
  
  const char *build_config = aom_codec_build_config();
  (void)build_config;
  
  const char *version_str = aom_codec_version_str();
  (void)version_str;
  
  // Test aom_codec_iface_name with NULL pointer
  const char *null_name = aom_codec_iface_name(NULL);
  (void)null_name;
  
  // Test aom_obu_type_to_string with various values from fuzzer input
  if (Size > 0) {
    // Use the first byte to determine an OBU_TYPE value
    // OBU_TYPE is an enum, so we cast the byte to that type
    // We'll also test some boundary values
    OBU_TYPE type = (OBU_TYPE)(Data[0] % 16); // Limit to reasonable range
    
    const char *obu_str = aom_obu_type_to_string(type);
    (void)obu_str;
    
    // Test some specific values that might be interesting
    if (Size > 1) {
      // Test with value from second byte
      OBU_TYPE type2 = (OBU_TYPE)Data[1];
      const char *obu_str2 = aom_obu_type_to_string(type2);
      (void)obu_str2;
    }
    
    // Test with extreme values
    const char *invalid_str = aom_obu_type_to_string((OBU_TYPE)255);
    (void)invalid_str;
    
    const char *zero_str = aom_obu_type_to_string((OBU_TYPE)0);
    (void)zero_str;
  }
  
  return 0;
}
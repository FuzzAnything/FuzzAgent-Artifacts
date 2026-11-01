// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"

// Helper to create a Parameter from fuzzed data by manipulating public members
static tinygltf::Parameter CreateParameterFromFuzz(const uint8_t *Data, size_t Size) {
  tinygltf::Parameter param;
  
  if (Size == 0) {
    return param;
  }

  // Use the first byte to determine the type of parameter to simulate
  uint8_t type_selector = Data[0] % 5;
  
  switch (type_selector) {
    case 0: { // Simulate a numeric factor (double)
      double val;
      if (Size >= sizeof(double)) {
        std::memcpy(&val, Data, sizeof(double));
      } else {
        val = static_cast<double>(Data[0]);
      }
      param.number_value = val;
      break;
    }
      
    case 1: { // Simulate a color array (3 or 4 floats)
      param.number_array.clear();
      size_t count = (Size >= 2) ? (Data[1] % 4) + 1 : 1; // 1 to 4 components
      for (size_t i = 0; i < count; ++i) {
        double val = 0.0;
        if (Size > i + 2) {
          val = static_cast<double>(Data[i + 2]) / 255.0;
        }
        param.number_array.push_back(val);
      }
      break;
    }
      
    case 2: { // Simulate a texture index (int)
      int idx = 0;
      if (Size >= 2) {
        idx = static_cast<int>(Data[1]);
      }
      param.number_value = static_cast<double>(idx);
      break;
    }

    case 3: { // Simulate TextureScale (double)
      double scale = 0.0;
      if (Size >= 2) {
        scale = static_cast<double>(Data[1]) / 10.0;
      }
      param.number_value = scale;
      break;
    }

    case 4: { // Simulate TextureStrength (double)
      double strength = 0.0;
      if (Size >= 2) {
        strength = static_cast<double>(Data[1]) / 10.0;
      }
      param.number_value = strength;
      break;
    }
  }
  
  // Also try to set string_value to trigger edge cases
  if (Size > 10) {
    std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
    param.string_value = str;
  }

  return param;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::Parameter param = CreateParameterFromFuzz(Data, Size);

  // Fuzz ColorFactor
  // The implementation in tiny_gltf.h accesses number_array[0], [1], [2] directly.
  // We must ensure the array has at least 3 elements before calling to avoid out-of-bounds.
  if (param.number_array.size() >= 3) {
    tinygltf::ColorValue color = param.ColorFactor();
    (void)color;
  } else {
    // If the array is too short, we can't safely call ColorFactor without modifying the param.
    // To test the "short input" handling logic safely, we can temporarily pad the array.
    // However, the library code might assume specific sizes. 
    // A safer approach for fuzzing is to only call if valid, or construct a valid one.
    // Let's construct a valid one specifically for this test if the original was short.
    tinygltf::Parameter safe_param = param;
    while (safe_param.number_array.size() < 3) {
      safe_param.number_array.push_back(0.0);
    }
    tinygltf::ColorValue color = safe_param.ColorFactor();
    (void)color;
  }

  // Fuzz TextureScale
  double scale = param.TextureScale();
  (void)scale;

  // Fuzz TextureStrength
  double strength = param.TextureStrength();
  (void)strength;

  // Fuzz TextureTexCoord
  int texCoord = param.TextureTexCoord();
  (void)texCoord;

  // Fuzz Factor
  double factor = param.Factor();
  (void)factor;

  // Fuzz TextureIndex
  int texIndex = param.TextureIndex();
  (void)texIndex;

  // Additional diverse invocations
  if (Size > 1) {
    // Test with a freshly constructed empty array parameter for ColorFactor (via padding logic above)
    tinygltf::Parameter empty_param;
    // Force it to have 3 elements to be safe for ColorFactor
    empty_param.number_array = {0.0, 0.0, 0.0};
    tinygltf::ColorValue empty_color = empty_param.ColorFactor();
    (void)empty_color;
    
    // Test Factor with 0.0
    tinygltf::Parameter zero_param;
    zero_param.number_value = 0.0;
    double zero_factor = zero_param.Factor();
    (void)zero_factor;
  }

  return 0;
}
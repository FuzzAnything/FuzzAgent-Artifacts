#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_005 {
/*
 * Fuzzing harness for tinygltf library - targeting equality comparison operators for GLTF model components
 * This harness specifically tests Camera, Accessor, Image, BufferView, Primitive, Texture, and Buffer equality operators
 * These have 200+ undiscovered branches and 0% coverage according to coverage analysis.
 * 
 * Strategy: Load GLTF models, extract components, create copies or modified versions,
 * and test equality operators with various edge cases.
 * 
 * Targets:
 * - Camera::operator== (48 undiscovered branches)
 * - Accessor::operator== (34 undiscovered branches)
 * - Image::operator== (32 undiscovered branches)
 * - BufferView::operator== (30 undiscovered branches)
 * - Primitive::operator== (24 undiscovered branches)
 * - Texture::operator== (22 undiscovered branches)
 * - Buffer::operator== (22 undiscovered branches)
 * 
 * Implementation uses FuzzedDataProvider for structured input splitting and
 * includes both loaded model components and synthetically generated components.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Simple GLTF JSON strings for testing component extraction
const char* SIMPLE_GLTF_JSON = R"({
  "asset": { "version": "2.0" },
  "cameras": [
    {
      "type": "perspective",
      "perspective": {
        "aspectRatio": 1.5,
        "yfov": 0.75,
        "zfar": 100,
        "znear": 0.1
      }
    }
  ],
  "accessors": [
    {
      "bufferView": 0,
      "componentType": 5126,
      "count": 36,
      "type": "VEC3"
    }
  ],
  "images": [
    {
      "uri": "test.png"
    }
  ],
  "bufferViews": [
    {
      "buffer": 0,
      "byteLength": 1024
    }
  ],
  "buffers": [
    {
      "byteLength": 1024,
      "uri": "data:application/octet-stream;base64,AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w=="
    }
  ],
  "meshes": [
    {
      "primitives": [
        {
          "attributes": { "POSITION": 0 },
          "indices": 0,
          "material": 0
        }
      ]
    }
  ],
  "materials": [
    {
      "pbrMetallicRoughness": {
        "baseColorFactor": [1.0, 1.0, 1.0, 1.0]
      }
    }
  ],
  "textures": [
    {
      "sampler": 0,
      "source": 0
    }
  ],
  "samplers": [
    {
      "magFilter": 9729,
      "minFilter": 9987,
      "wrapS": 10497,
      "wrapT": 10497
    }
  ]
})";

const char* MINIMAL_GLTF_JSON = R"({
  "asset": { "version": "2.0" },
  "scene": 0,
  "scenes": [{ "nodes": [] }]
})";

// Helper function to load a model from JSON string
static bool load_model_from_string(tinygltf::Model* model, const std::string& json_str, 
                                  tinygltf::TinyGLTF* loader) {
  std::string err;
  std::string warn;
  return loader->LoadASCIIFromString(model, &err, &warn, json_str.c_str(), 
                                     static_cast<unsigned int>(json_str.size()), "", 0);
}

// Helper to create synthetic components based on fuzzer input
static tinygltf::Camera create_synthetic_camera(FuzzedDataProvider& fdp) {
  tinygltf::Camera camera;
  camera.name = fdp.ConsumeRandomLengthString(50);
  
  uint8_t cam_type = fdp.ConsumeIntegral<uint8_t>() % 2;
  if (cam_type == 0) {
    camera.type = "perspective";
    camera.perspective.aspectRatio = fdp.ConsumeFloatingPoint<double>();
    camera.perspective.yfov = fdp.ConsumeFloatingPoint<double>();
    camera.perspective.zfar = fdp.ConsumeFloatingPoint<double>();
    camera.perspective.znear = fdp.ConsumeFloatingPoint<double>();
  } else {
    camera.type = "orthographic";
    camera.orthographic.xmag = fdp.ConsumeFloatingPoint<double>();
    camera.orthographic.ymag = fdp.ConsumeFloatingPoint<double>();
    camera.orthographic.zfar = fdp.ConsumeFloatingPoint<double>();
    camera.orthographic.znear = fdp.ConsumeFloatingPoint<double>();
  }
  
  return camera;
}

static tinygltf::Accessor create_synthetic_accessor(FuzzedDataProvider& fdp) {
  tinygltf::Accessor accessor;
  accessor.name = fdp.ConsumeRandomLengthString(50);
  accessor.bufferView = fdp.ConsumeIntegral<int>();
  accessor.byteOffset = fdp.ConsumeIntegral<size_t>();
  
  // Component type from glTF enum values
  int component_types[] = {
    5120, 5121, 5122, 5123, 5124, 5125, 5126, 5130
  };
  uint8_t comp_idx = fdp.ConsumeIntegral<uint8_t>() % 8;
  accessor.componentType = component_types[comp_idx];
  
  accessor.count = fdp.ConsumeIntegral<size_t>();
  accessor.normalized = fdp.ConsumeBool();
  
  // Type from glTF type enum
  int types[] = {
    0,  // TINYGLTF_TYPE_SCALAR
    1,  // TINYGLTF_TYPE_VECTOR
    2,  // TINYGLTF_TYPE_VEC2
    3,  // TINYGLTF_TYPE_VEC3
    4,  // TINYGLTF_TYPE_VEC4
    5,  // TINYGLTF_TYPE_MATRIX
    6,  // TINYGLTF_TYPE_MAT2
    7,  // TINYGLTF_TYPE_MAT3
    8   // TINYGLTF_TYPE_MAT4
  };
  uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 9;
  accessor.type = types[type_idx];
  
  // Set min/max values if count > 0
  if (accessor.count > 0) {
    size_t num_values = accessor.type == 0 ? 1 : 
                       (accessor.type == 1 ? 1 :
                       (accessor.type == 2 ? 2 :
                       (accessor.type == 3 ? 3 :
                       (accessor.type == 4 ? 4 :
                       (accessor.type == 5 ? 16 :
                       (accessor.type == 6 ? 4 :
                       (accessor.type == 7 ? 9 : 16)))))));
    
    for (size_t i = 0; i < num_values; i++) {
      accessor.minValues.push_back(fdp.ConsumeFloatingPoint<double>());
      accessor.maxValues.push_back(fdp.ConsumeFloatingPoint<double>());
    }
  }
  
  return accessor;
}

static tinygltf::Image create_synthetic_image(FuzzedDataProvider& fdp) {
  tinygltf::Image image;
  image.name = fdp.ConsumeRandomLengthString(50);
  image.uri = fdp.ConsumeRandomLengthString(100);
  image.bufferView = fdp.ConsumeIntegral<int>();
  
  image.width = fdp.ConsumeIntegral<int>();
  image.height = fdp.ConsumeIntegral<int>();
  image.component = fdp.ConsumeIntegral<int>() % 5; // 0-4 components
  image.bits = fdp.ConsumeIntegral<int>() % 32;
  image.pixel_type = fdp.ConsumeIntegral<int>() % 4;
  
  // Create some image data
  size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1000);
  if (data_size > 0) {
    std::vector<unsigned char> image_data = fdp.ConsumeBytes<unsigned char>(data_size);
    image.image = std::move(image_data);
  }
  
  return image;
}

static tinygltf::BufferView create_synthetic_buffer_view(FuzzedDataProvider& fdp) {
  tinygltf::BufferView bufferView;
  bufferView.name = fdp.ConsumeRandomLengthString(50);
  bufferView.buffer = fdp.ConsumeIntegral<int>();
  bufferView.byteOffset = fdp.ConsumeIntegral<size_t>();
  bufferView.byteLength = fdp.ConsumeIntegral<size_t>();
  bufferView.byteStride = fdp.ConsumeIntegral<size_t>();
  bufferView.target = fdp.ConsumeIntegral<int>();
  bufferView.dracoDecoded = fdp.ConsumeBool();
  
  return bufferView;
}

static tinygltf::Primitive create_synthetic_primitive(FuzzedDataProvider& fdp) {
  tinygltf::Primitive primitive;
  primitive.mode = fdp.ConsumeIntegral<int>() % 7; // 0-6: TINYGLTF_MODE_* values
  
  // Add some attributes
  int num_attrs = fdp.ConsumeIntegralInRange<int>(0, 5);
  for (int i = 0; i < num_attrs; i++) {
    std::string attr_name = "ATTR_" + std::to_string(i);
    int attr_value = fdp.ConsumeIntegral<int>();
    primitive.attributes[attr_name] = attr_value;
  }
  
  primitive.indices = fdp.ConsumeIntegral<int>();
  primitive.material = fdp.ConsumeIntegral<int>();
  
  // Add some targets
  int num_targets = fdp.ConsumeIntegralInRange<int>(0, 3);
  for (int i = 0; i < num_targets; i++) {
    std::map<std::string, int> target;
    int target_attrs = fdp.ConsumeIntegralInRange<int>(0, 3);
    for (int j = 0; j < target_attrs; j++) {
      std::string target_name = "TARGET_" + std::to_string(j);
      int target_value = fdp.ConsumeIntegral<int>();
      target[target_name] = target_value;
    }
    primitive.targets.push_back(target);
  }
  
  return primitive;
}

static tinygltf::Texture create_synthetic_texture(FuzzedDataProvider& fdp) {
  tinygltf::Texture texture;
  texture.name = fdp.ConsumeRandomLengthString(50);
  texture.sampler = fdp.ConsumeIntegral<int>();
  texture.source = fdp.ConsumeIntegral<int>();
  
  return texture;
}

static tinygltf::Buffer create_synthetic_buffer(FuzzedDataProvider& fdp) {
  tinygltf::Buffer buffer;
  buffer.name = fdp.ConsumeRandomLengthString(50);
  buffer.uri = fdp.ConsumeRandomLengthString(100);
  
  size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 2000);
  if (data_size > 0) {
    std::vector<unsigned char> buffer_data = fdp.ConsumeBytes<unsigned char>(data_size);
    buffer.data = std::move(buffer_data);
  }
  
  return buffer;
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 16) {
    return 0;  // Need minimum data for meaningful testing
  }

  FuzzedDataProvider fdp(data, size);
  
  tinygltf::TinyGLTF loader;
  tinygltf::Model model1, model2;
  
  // Try to load models from embedded JSON strings
  bool loaded1 = load_model_from_string(&model1, SIMPLE_GLTF_JSON, &loader);
  bool loaded2 = load_model_from_string(&model2, MINIMAL_GLTF_JSON, &loader);
  
  // We don't care if loading succeeds - we'll work with whatever we have
  (void)loaded1;
  (void)loaded2;
  
  // Consume selector to determine which equality tests to run
  uint8_t test_selector = fdp.ConsumeIntegral<uint8_t>();
  uint8_t component_type = test_selector % 7;  // 7 component types
  
  bool equality_result = false;
  
  switch (component_type) {
    case 0: {  // Camera equality
      tinygltf::Camera cam1, cam2;
      
      // Either use cameras from loaded models or create synthetic ones
      if (!model1.cameras.empty() && !model2.cameras.empty()) {
        cam1 = model1.cameras[0];
        cam2 = model2.cameras[0];
      } else {
        // Create synthetic cameras
        cam1 = create_synthetic_camera(fdp);
        cam2 = create_synthetic_camera(fdp);
        
        // Possibly make them identical based on fuzzer input
        if (fdp.ConsumeBool()) {
          cam2 = cam1;  // Copy for identical test
        }
      }
      
      // Test equality operator
      equality_result = (cam1 == cam2);
      
      // Also test self-equality
      bool self_eq = (cam1 == cam1);
      (void)self_eq;
      
      break;
    }
    
    case 1: {  // Accessor equality
      tinygltf::Accessor acc1, acc2;
      
      if (!model1.accessors.empty() && !model2.accessors.empty()) {
        acc1 = model1.accessors[0];
        acc2 = model2.accessors[0];
      } else {
        acc1 = create_synthetic_accessor(fdp);
        acc2 = create_synthetic_accessor(fdp);
        
        if (fdp.ConsumeBool()) {
          acc2 = acc1;
        }
      }
      
      equality_result = (acc1 == acc2);
      bool self_eq = (acc1 == acc1);
      (void)self_eq;
      
      break;
    }
    
    case 2: {  // Image equality
      tinygltf::Image img1, img2;
      
      if (!model1.images.empty() && !model2.images.empty()) {
        img1 = model1.images[0];
        img2 = model2.images[0];
      } else {
        img1 = create_synthetic_image(fdp);
        img2 = create_synthetic_image(fdp);
        
        if (fdp.ConsumeBool()) {
          img2 = img1;
        }
      }
      
      equality_result = (img1 == img2);
      bool self_eq = (img1 == img1);
      (void)self_eq;
      
      break;
    }
    
    case 3: {  // BufferView equality
      tinygltf::BufferView bv1, bv2;
      
      if (!model1.bufferViews.empty() && !model2.bufferViews.empty()) {
        bv1 = model1.bufferViews[0];
        bv2 = model2.bufferViews[0];
      } else {
        bv1 = create_synthetic_buffer_view(fdp);
        bv2 = create_synthetic_buffer_view(fdp);
        
        if (fdp.ConsumeBool()) {
          bv2 = bv1;
        }
      }
      
      equality_result = (bv1 == bv2);
      bool self_eq = (bv1 == bv1);
      (void)self_eq;
      
      break;
    }
    
    case 4: {  // Primitive equality
      tinygltf::Primitive prim1, prim2;
      
      if (!model1.meshes.empty() && !model2.meshes.empty() &&
          !model1.meshes[0].primitives.empty() && !model2.meshes[0].primitives.empty()) {
        prim1 = model1.meshes[0].primitives[0];
        prim2 = model2.meshes[0].primitives[0];
      } else {
        prim1 = create_synthetic_primitive(fdp);
        prim2 = create_synthetic_primitive(fdp);
        
        if (fdp.ConsumeBool()) {
          prim2 = prim1;
        }
      }
      
      equality_result = (prim1 == prim2);
      bool self_eq = (prim1 == prim1);
      (void)self_eq;
      
      break;
    }
    
    case 5: {  // Texture equality
      tinygltf::Texture tex1, tex2;
      
      if (!model1.textures.empty() && !model2.textures.empty()) {
        tex1 = model1.textures[0];
        tex2 = model2.textures[0];
      } else {
        tex1 = create_synthetic_texture(fdp);
        tex2 = create_synthetic_texture(fdp);
        
        if (fdp.ConsumeBool()) {
          tex2 = tex1;
        }
      }
      
      equality_result = (tex1 == tex2);
      bool self_eq = (tex1 == tex1);
      (void)self_eq;
      
      break;
    }
    
    case 6: {  // Buffer equality
      tinygltf::Buffer buf1, buf2;
      
      if (!model1.buffers.empty() && !model2.buffers.empty()) {
        buf1 = model1.buffers[0];
        buf2 = model2.buffers[0];
      } else {
        buf1 = create_synthetic_buffer(fdp);
        buf2 = create_synthetic_buffer(fdp);
        
        if (fdp.ConsumeBool()) {
          buf2 = buf1;
        }
      }
      
      equality_result = (buf1 == buf2);
      bool self_eq = (buf1 == buf1);
      (void)self_eq;
      
      break;
    }
    
    default:
      // Should not happen due to modulo 7
      break;
  }
  
  // Use the result to avoid compiler warnings
  (void)equality_result;
  
  // Test additional edge cases with any remaining input
  if (fdp.remaining_bytes() > 0) {
    // Create empty/default objects and test equality
    tinygltf::Camera empty_cam1, empty_cam2;
    bool empty_cam_eq = (empty_cam1 == empty_cam2);
    (void)empty_cam_eq;
    
    tinygltf::Accessor empty_acc1, empty_acc2;
    bool empty_acc_eq = (empty_acc1 == empty_acc2);
    (void)empty_acc_eq;
    
    tinygltf::Image empty_img1, empty_img2;
    bool empty_img_eq = (empty_img1 == empty_img2);
    (void)empty_img_eq;
    
    // Test Model equality if both models have some content
    bool model_eq = (model1 == model2);
    bool model1_self_eq = (model1 == model1);
    bool model2_self_eq = (model2 == model2);
    (void)model_eq;
    (void)model1_self_eq;
    (void)model2_self_eq;
  }
  
  return 0;
}

} // namespace harness_005

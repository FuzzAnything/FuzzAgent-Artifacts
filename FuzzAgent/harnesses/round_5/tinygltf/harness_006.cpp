#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_006 {
/*
 * Fuzzing harness for tinygltf library - targeting GLTF struct equality operators
 * This harness specifically tests PositionalEmitter, AnimationSampler, SpotLight, 
 * Parameter, AudioEmitter equality operators and Value class methods
 * These have 160+ undiscovered branches and 0% coverage according to coverage analysis.
 * 
 * Strategy: Load GLTF models, extract components, create duplicate or modified instances,
 * test equality operators, and exercise Value class methods (Size(), Keys(), Get()).
 * 
 * Targets:
 * - PositionalEmitter::operator== (28 undiscovered branches, 0% coverage)
 * - AnimationSampler::operator== (22 undiscovered branches, 0% coverage)
 * - SpotLight::operator== (20 undiscovered branches, 0% coverage)
 * - Parameter::operator== (18 undiscovered branches, 0% coverage)
 * - AudioEmitter::operator== (14 undiscovered branches, 0% coverage)
 * - Value::Size() (8 undiscovered branches)
 * - Value::Keys() (4 undiscovered branches)
 * - Value::Get() variants (4 undiscovered branches each)
 * 
 * Implementation uses FuzzedDataProvider for structured input splitting and
 * includes both loaded model components and synthetically generated components.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// GLTF JSON strings for testing component extraction
const char* GLTF_WITH_AUDIO = R"({
  "asset": { "version": "2.0" },
  "extensionsUsed": ["KHR_audio"],
  "extensions": {
    "KHR_audio": {
      "emitters": [
        {
          "name": "global_emitter",
          "type": "global",
          "gain": 0.5
        },
        {
          "name": "positional_emitter",
          "type": "positional",
          "gain": 1.0,
          "positional": {
            "coneInnerAngle": 0.5,
            "coneOuterAngle": 1.0,
            "coneOuterGain": 0.2,
            "maxDistance": 50.0,
            "refDistance": 1.0,
            "rolloffFactor": 2.0
          }
        }
      ],
      "sources": [
        {
          "name": "audio_source",
          "uri": "sound.mp3"
        }
      ]
    }
  },
  "scenes": [
    {
      "extensions": {
        "KHR_audio": {
          "emitters": [0, 1]
        }
      }
    }
  ]
})";

const char* GLTF_WITH_ANIMATIONS = R"({
  "asset": { "version": "2.0" },
  "animations": [
    {
      "name": "animation",
      "samplers": [
        {
          "input": 0,
          "output": 1,
          "interpolation": "LINEAR"
        },
        {
          "input": 1,
          "output": 2,
          "interpolation": "STEP"
        }
      ],
      "channels": [
        {
          "sampler": 0,
          "target": {
            "node": 0,
            "path": "translation"
          }
        }
      ]
    }
  ],
  "accessors": [
    {
      "bufferView": 0,
      "componentType": 5126,
      "count": 10,
      "type": "SCALAR"
    },
    {
      "bufferView": 1,
      "componentType": 5126,
      "count": 30,
      "type": "VEC3"
    },
    {
      "bufferView": 2,
      "componentType": 5126,
      "count": 30,
      "type": "VEC3"
    }
  ],
  "bufferViews": [
    {
      "buffer": 0,
      "byteLength": 40
    },
    {
      "buffer": 0,
      "byteLength": 120
    },
    {
      "buffer": 0,
      "byteLength": 120
    }
  ],
  "buffers": [
    {
      "byteLength": 280,
      "uri": "data:application/octet-stream;base64,AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w=="
    }
  ]
})";

const char* GLTF_WITH_LIGHTS = R"({
  "asset": { "version": "2.0" },
  "extensionsUsed": ["KHR_lights_punctual"],
  "extensions": {
    "KHR_lights_punctual": {
      "lights": [
        {
          "name": "spot_light",
          "type": "spot",
          "color": [1.0, 1.0, 1.0],
          "intensity": 1.0,
          "range": 10.0,
          "spot": {
            "innerConeAngle": 0.3,
            "outerConeAngle": 0.8
          }
        }
      ]
    }
  },
  "nodes": [
    {
      "extensions": {
        "KHR_lights_punctual": {
          "light": 0
        }
      }
    }
  ]
})";

const char* GLTF_WITH_MATERIALS = R"({
  "asset": { "version": "2.0" },
  "materials": [
    {
      "name": "material_with_params",
      "pbrMetallicRoughness": {
        "baseColorFactor": [1.0, 0.0, 0.0, 1.0],
        "metallicFactor": 0.5,
        "roughnessFactor": 0.5
      },
      "normalTexture": {
        "index": 0,
        "scale": 2.0
      },
      "occlusionTexture": {
        "index": 0,
        "strength": 0.8
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
      "minFilter": 9987
    }
  ],
  "images": [
    {
      "uri": "normal.png"
    }
  ]
})";

// Helper function to load a model from JSON string
static bool load_model_from_string(tinygltf::Model* model, const std::string& json_str, 
                                  tinygltf::TinyGLTF* loader) {
  std::string err;
  std::string warn;
  return loader->LoadASCIIFromString(model, &err, &warn, json_str.c_str(), 
                                     static_cast<unsigned int>(json_str.size()), "", 0);
}

// Helper to create synthetic PositionalEmitter based on fuzzer input
static tinygltf::PositionalEmitter create_synthetic_positional_emitter(FuzzedDataProvider& fdp) {
  tinygltf::PositionalEmitter emitter;
  
  emitter.coneInnerAngle = fdp.ConsumeFloatingPoint<double>();
  emitter.coneOuterAngle = fdp.ConsumeFloatingPoint<double>();
  emitter.coneOuterGain = fdp.ConsumeFloatingPoint<double>();
  emitter.maxDistance = fdp.ConsumeFloatingPoint<double>();
  emitter.refDistance = fdp.ConsumeFloatingPoint<double>();
  emitter.rolloffFactor = fdp.ConsumeFloatingPoint<double>();
  
  // Create extras with some data to test Value methods
  tinygltf::Value::Object extras_obj;
  int num_items = fdp.ConsumeIntegralInRange<int>(0, 5);
  for (int i = 0; i < num_items; i++) {
    std::string key = "key_" + std::to_string(i);
    double value = fdp.ConsumeFloatingPoint<double>();
    extras_obj[key] = tinygltf::Value(value);
  }
  emitter.extras = tinygltf::Value(extras_obj);
  
  return emitter;
}

// Helper to create synthetic AnimationSampler based on fuzzer input
static tinygltf::AnimationSampler create_synthetic_animation_sampler(FuzzedDataProvider& fdp) {
  tinygltf::AnimationSampler sampler;
  
  sampler.input = fdp.ConsumeIntegral<int>();
  sampler.output = fdp.ConsumeIntegral<int>();
  
  // Choose interpolation type
  const char* interpolations[] = {"LINEAR", "STEP", "CUBICSPLINE"};
  uint8_t interp_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
  sampler.interpolation = interpolations[interp_idx];
  
  // Create extras with some data to test Value methods
  tinygltf::Value::Object extras_obj;
  int num_items = fdp.ConsumeIntegralInRange<int>(0, 3);
  for (int i = 0; i < num_items; i++) {
    std::string key = "anim_key_" + std::to_string(i);
    double value = fdp.ConsumeFloatingPoint<double>();
    extras_obj[key] = tinygltf::Value(value);
  }
  sampler.extras = tinygltf::Value(extras_obj);
  
  return sampler;
}

// Helper to create synthetic SpotLight based on fuzzer input
static tinygltf::SpotLight create_synthetic_spotlight(FuzzedDataProvider& fdp) {
  tinygltf::SpotLight spotlight;
  
  spotlight.innerConeAngle = fdp.ConsumeFloatingPoint<double>();
  spotlight.outerConeAngle = fdp.ConsumeFloatingPoint<double>();
  
  // Create extras with some data to test Value methods
  tinygltf::Value::Object extras_obj;
  int num_items = fdp.ConsumeIntegralInRange<int>(0, 4);
  for (int i = 0; i < num_items; i++) {
    std::string key = "light_key_" + std::to_string(i);
    double value = fdp.ConsumeFloatingPoint<double>();
    extras_obj[key] = tinygltf::Value(value);
  }
  spotlight.extras = tinygltf::Value(extras_obj);
  return spotlight;
}

// Helper to create synthetic Parameter based on fuzzer input
static tinygltf::Parameter create_synthetic_parameter(FuzzedDataProvider& fdp) {
  tinygltf::Parameter param;
  
  param.bool_value = fdp.ConsumeBool();
  param.has_number_value = fdp.ConsumeBool();
  param.number_value = fdp.ConsumeFloatingPoint<double>();
  param.string_value = fdp.ConsumeRandomLengthString(50);
  
  // Add number array
  size_t array_size = fdp.ConsumeIntegralInRange<size_t>(0, 10);
  for (size_t i = 0; i < array_size; i++) {
    param.number_array.push_back(fdp.ConsumeFloatingPoint<double>());
  }
  
  // Add JSON double values
  size_t json_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
  for (size_t i = 0; i < json_size; i++) {
    std::string key = "json_key_" + std::to_string(i);
    double value = fdp.ConsumeFloatingPoint<double>();
    param.json_double_value[key] = value;
  }
  
  return param;
}

// Helper to create synthetic AudioEmitter based on fuzzer input
static tinygltf::AudioEmitter create_synthetic_audio_emitter(FuzzedDataProvider& fdp) {
  tinygltf::AudioEmitter emitter;
  
  emitter.name = fdp.ConsumeRandomLengthString(50);
  emitter.gain = fdp.ConsumeFloatingPoint<double>();
  emitter.loop = fdp.ConsumeBool();
  emitter.playing = fdp.ConsumeBool();
  
  // Choose type
  const char* types[] = {"global", "positional"};
  uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 2;
  emitter.type = types[type_idx];
  
  // Choose distance model
  const char* models[] = {"linear", "inverse", "exponential"};
  uint8_t model_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
  emitter.distanceModel = models[model_idx];
  
  emitter.source = fdp.ConsumeIntegral<int>();
  
  // Create positional emitter if type is positional
  if (emitter.type == "positional") {
    emitter.positional = create_synthetic_positional_emitter(fdp);
  }
  
  // Create extras with some data to test Value methods
  tinygltf::Value::Object extras_obj;
  int num_items = fdp.ConsumeIntegralInRange<int>(0, 5);
  for (int i = 0; i < num_items; i++) {
    std::string key = "audio_key_" + std::to_string(i);
    double value = fdp.ConsumeFloatingPoint<double>();
    extras_obj[key] = tinygltf::Value(value);
  }
  emitter.extras = tinygltf::Value(extras_obj);
  return emitter;
}

// Helper to create synthetic Value object for testing Value methods
static tinygltf::Value create_synthetic_value(FuzzedDataProvider& fdp) {
  uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 7;
  
  switch (value_type) {
    case 0: {  // Bool
      return tinygltf::Value(fdp.ConsumeBool());
    }
    case 1: {  // Int
      return tinygltf::Value(fdp.ConsumeIntegral<int>());
    }
    case 2: {  // Double
      return tinygltf::Value(fdp.ConsumeFloatingPoint<double>());
    }
    case 3: {  // String
      return tinygltf::Value(fdp.ConsumeRandomLengthString(50));
    }
    case 4: {  // Array
      tinygltf::Value::Array array;
      size_t array_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
      for (size_t i = 0; i < array_size; i++) {
        double value = fdp.ConsumeFloatingPoint<double>();
        array.push_back(tinygltf::Value(value));
      }
      return tinygltf::Value(array);
    }
    case 5: {  // Object
      tinygltf::Value::Object obj;
      size_t obj_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
      for (size_t i = 0; i < obj_size; i++) {
        std::string key = "obj_key_" + std::to_string(i);
        double value = fdp.ConsumeFloatingPoint<double>();
        obj[key] = tinygltf::Value(value);
      }
      return tinygltf::Value(obj);
    }
    case 6: {  // Binary (not commonly used but should be tested)
      size_t bin_size = fdp.ConsumeIntegralInRange<size_t>(0, 20);
      std::vector<unsigned char> binary(bin_size);
      for (size_t i = 0; i < bin_size; i++) {
        binary[i] = fdp.ConsumeIntegral<uint8_t>();
      }
      return tinygltf::Value(std::move(binary));
    }
    default:
      return tinygltf::Value();
  }
}

// Test Value class methods
static void test_value_methods(const tinygltf::Value& value) {
  // Test Size() method
  size_t size = value.Size();
  (void)size;
  
  // Test Keys() method - only valid for objects
  if (value.IsObject()) {
    std::vector<std::string> keys = value.Keys();
    (void)keys;
    
    // Test Get() method for each key
    for (const auto& key : keys) {
      const tinygltf::Value& retrieved = value.Get(key);
      (void)retrieved;
      
      // Test Has() method
      bool has_key = value.Has(key);
      (void)has_key;
    }
  }
  
  // Test Get() with index for arrays
  if (value.IsArray() && value.Size() > 0) {
    const tinygltf::Value& elem = value.Get(0);
    (void)elem;
  }
  
  // Test typed Get() methods
  if (value.IsBool()) {
    bool b = value.Get<bool>();
    (void)b;
  }
  if (value.IsInt()) {
    int i = value.Get<int>();
    (void)i;
  }
  if (value.IsReal()) {
    double d = value.Get<double>();
    (void)d;
  }
  if (value.IsString()) {
    std::string s = value.Get<std::string>();
    (void)s;
  }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 32) {
    return 0;  // Insufficient input for meaningful testing
  }
  
  FuzzedDataProvider fdp(data, size);
  
  // Initialize loader
  tinygltf::TinyGLTF loader;
  loader.SetStoreOriginalJSONForExtrasAndExtensions(true);
  
  // Load different models based on input
  tinygltf::Model model1, model2;
  bool model1_loaded = false;
  bool model2_loaded = false;
  
  // Choose which model to load based on input
  uint8_t model_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
  
  switch (model_choice) {
    case 0:
      model1_loaded = load_model_from_string(&model1, GLTF_WITH_AUDIO, &loader);
      model2_loaded = load_model_from_string(&model2, GLTF_WITH_ANIMATIONS, &loader);
      break;
    case 1:
      model1_loaded = load_model_from_string(&model1, GLTF_WITH_LIGHTS, &loader);
      model2_loaded = load_model_from_string(&model2, GLTF_WITH_MATERIALS, &loader);
      break;
    case 2:
      model1_loaded = load_model_from_string(&model1, GLTF_WITH_AUDIO, &loader);
      model2_loaded = load_model_from_string(&model2, GLTF_WITH_LIGHTS, &loader);
      break;
    case 3:
      model1_loaded = load_model_from_string(&model1, GLTF_WITH_ANIMATIONS, &loader);
      model2_loaded = load_model_from_string(&model2, GLTF_WITH_MATERIALS, &loader);
      break;
  }
  
  // Choose which equality operator to test based on input
  uint8_t test_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
  bool equality_result = false;
  
  switch (test_choice) {
    case 0: {  // PositionalEmitter equality
      tinygltf::PositionalEmitter emitter1, emitter2;
      
      // Try to get from loaded models if available
      if (model1_loaded && !model1.audioEmitters.empty() && 
          model1.audioEmitters[0].type == "positional") {
        emitter1 = model1.audioEmitters[0].positional;
      } else {
        emitter1 = create_synthetic_positional_emitter(fdp);
      }
      
      if (model2_loaded && !model2.audioEmitters.empty() && 
          model2.audioEmitters[0].type == "positional") {
        emitter2 = model2.audioEmitters[0].positional;
      } else {
        emitter2 = create_synthetic_positional_emitter(fdp);
      }
      
      // Make them equal sometimes
      if (fdp.ConsumeBool()) {
        emitter2 = emitter1;
      }
      
      equality_result = (emitter1 == emitter2);
      bool self_equality = (emitter1 == emitter1);
      (void)self_equality;
      
      // Test Value methods on extras
      test_value_methods(emitter1.extras);
      test_value_methods(emitter2.extras);
      
      break;
    }
    
    case 1: {  // AnimationSampler equality
      tinygltf::AnimationSampler sampler1, sampler2;
      
      // Try to get from loaded models if available
      if (model1_loaded && !model1.animations.empty() && 
          !model1.animations[0].samplers.empty()) {
        sampler1 = model1.animations[0].samplers[0];
      } else {
        sampler1 = create_synthetic_animation_sampler(fdp);
      }
      
      if (model2_loaded && !model2.animations.empty() && 
          !model2.animations[0].samplers.empty()) {
        sampler2 = model2.animations[0].samplers[0];
      } else {
        sampler2 = create_synthetic_animation_sampler(fdp);
      }
      
      // Make them equal sometimes
      if (fdp.ConsumeBool()) {
        sampler2 = sampler1;
      }
      
      equality_result = (sampler1 == sampler2);
      bool self_equality = (sampler1 == sampler1);
      (void)self_equality;
      
      // Test Value methods on extras
      test_value_methods(sampler1.extras);
      test_value_methods(sampler2.extras);
      
      break;
    }
    
    case 2: {  // SpotLight equality
      tinygltf::SpotLight light1, light2;
      
      // Try to get from loaded models if available
      if (model1_loaded && !model1.lights.empty() && 
          model1.lights[0].type == "spot") {
        light1 = model1.lights[0].spot;
      } else {
        light1 = create_synthetic_spotlight(fdp);
      }
      
      if (model2_loaded && !model2.lights.empty() && 
          model2.lights[0].type == "spot") {
        light2 = model2.lights[0].spot;
      } else {
        light2 = create_synthetic_spotlight(fdp);
      }
      
      // Make them equal sometimes
      if (fdp.ConsumeBool()) {
        light2 = light1;
      }
      
      equality_result = (light1 == light2);
      bool self_equality = (light1 == light1);
      (void)self_equality;
      
      // Test Value methods on extras
      test_value_methods(light1.extras);
      test_value_methods(light2.extras);
      
      break;
    }
    
    case 3: {  // Parameter equality
      tinygltf::Parameter param1, param2;
      
      // Parameters are typically in materials. Try to get from loaded models.
      // Note: Parameter is legacy interface, so we might not find them in models.
      param1 = create_synthetic_parameter(fdp);
      param2 = create_synthetic_parameter(fdp);
      
      // Make them equal sometimes
      if (fdp.ConsumeBool()) {
        param2 = param1;
      }
      
      equality_result = (param1 == param2);
      bool self_equality = (param1 == param1);
      (void)self_equality;
      
      break;
    }
    
    case 4: {  // AudioEmitter equality
      tinygltf::AudioEmitter emitter1, emitter2;
      
      // Try to get from loaded models if available
      if (model1_loaded && !model1.audioEmitters.empty()) {
        emitter1 = model1.audioEmitters[0];
      } else {
        emitter1 = create_synthetic_audio_emitter(fdp);
      }
      
      if (model2_loaded && !model2.audioEmitters.empty()) {
        emitter2 = model2.audioEmitters[0];
      } else {
        emitter2 = create_synthetic_audio_emitter(fdp);
      }
      
      // Make them equal sometimes
      if (fdp.ConsumeBool()) {
        emitter2 = emitter1;
      }
      
      equality_result = (emitter1 == emitter2);
      bool self_equality = (emitter1 == emitter1);
      (void)self_equality;
      
      // Test Value methods on extras
      test_value_methods(emitter1.extras);
      test_value_methods(emitter2.extras);
      
      break;
    }
    
    case 5: {  // Value equality and methods
      tinygltf::Value value1 = create_synthetic_value(fdp);
      tinygltf::Value value2 = create_synthetic_value(fdp);
      
      // Make them equal sometimes
      if (fdp.ConsumeBool()) {
        value2 = value1;
      }
      
      equality_result = (value1 == value2);
      bool self_equality = (value1 == value1);
      (void)self_equality;
      
      // Test Value methods
      test_value_methods(value1);
      test_value_methods(value2);
      
      break;
    }
    
    case 6: {  // Test Value methods on model extras
      if (model1_loaded) {
        test_value_methods(model1.extras);
        
        // Test Value methods on asset extras if present
        test_value_methods(model1.asset.extras);
        
        // Test Value methods on some component extras
        if (!model1.materials.empty()) {
          test_value_methods(model1.materials[0].extras);
        }
        if (!model1.nodes.empty()) {
          test_value_methods(model1.nodes[0].extras);
        }
      }
      
      if (model2_loaded) {
        test_value_methods(model2.extras);
        
        // Test Value methods on asset extras if present
        test_value_methods(model2.asset.extras);
      }
      
      // Also test on synthetic values
      for (int i = 0; i < 3; i++) {
        tinygltf::Value synthetic = create_synthetic_value(fdp);
        test_value_methods(synthetic);
      }
      
      break;
    }
    
    case 7: {  // Test multiple equality operators in sequence
      // Test PositionalEmitter
      tinygltf::PositionalEmitter pos1 = create_synthetic_positional_emitter(fdp);
      tinygltf::PositionalEmitter pos2 = create_synthetic_positional_emitter(fdp);
      bool pos_eq = (pos1 == pos2);
      (void)pos_eq;
      
      // Test AnimationSampler
      tinygltf::AnimationSampler anim1 = create_synthetic_animation_sampler(fdp);
      tinygltf::AnimationSampler anim2 = create_synthetic_animation_sampler(fdp);
      bool anim_eq = (anim1 == anim2);
      (void)anim_eq;
      
      // Test SpotLight
      tinygltf::SpotLight spot1 = create_synthetic_spotlight(fdp);
      tinygltf::SpotLight spot2 = create_synthetic_spotlight(fdp);
      bool spot_eq = (spot1 == spot2);
      (void)spot_eq;
      
      // Test Parameter
      tinygltf::Parameter param1 = create_synthetic_parameter(fdp);
      tinygltf::Parameter param2 = create_synthetic_parameter(fdp);
      bool param_eq = (param1 == param2);
      (void)param_eq;
      
      // Test AudioEmitter
      tinygltf::AudioEmitter audio1 = create_synthetic_audio_emitter(fdp);
      tinygltf::AudioEmitter audio2 = create_synthetic_audio_emitter(fdp);
      bool audio_eq = (audio1 == audio2);
      (void)audio_eq;
      
      // Test Value equality
      tinygltf::Value val1 = create_synthetic_value(fdp);
      tinygltf::Value val2 = create_synthetic_value(fdp);
      bool val_eq = (val1 == val2);
      (void)val_eq;
      
      // Test Value methods
      test_value_methods(val1);
      test_value_methods(val2);
      
      break;
    }
  }
  
  (void)equality_result;  // Prevent unused variable warning
  
  return 0;
}

} // namespace harness_006

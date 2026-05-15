// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

static bool custom_uri_encode(const std::string &in_uri, const std::string &base_path, std::string *out_uri, void *user_data) {
    *out_uri = in_uri;
    return true;
}

static bool custom_uri_decode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    *out_uri = in_uri;
    return true;
}

static bool null_uri_decode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine which function to test
    uint8_t selector = Data[0];
    const uint8_t* payload = Data + 1;
    size_t payload_size = Size - 1;

    switch (selector % 6) {
        case 0: {
            // Test SetURICallbacks
            tinygltf::TinyGLTF loader;
            tinygltf::URICallbacks callbacks;
            std::string err;
            
            // Test with valid callbacks
            callbacks.encode = custom_uri_encode;
            callbacks.decode = custom_uri_decode;
            callbacks.user_data = nullptr;
            bool result1 = loader.SetURICallbacks(callbacks, &err);
            
            // Test with null decode (should fail)
            tinygltf::URICallbacks callbacks2;
            callbacks2.encode = custom_uri_encode;
            callbacks2.decode = nullptr;
            callbacks2.user_data = nullptr;
            bool result2 = loader.SetURICallbacks(callbacks2, &err);
            
            // Test with null encode but valid decode
            tinygltf::URICallbacks callbacks3;
            callbacks3.encode = nullptr;
            callbacks3.decode = custom_uri_decode;
            callbacks3.user_data = nullptr;
            bool result3 = loader.SetURICallbacks(callbacks3, &err);
            break;
        }
        
        case 1: {
            // Test DecodeDataURI
            std::vector<unsigned char> out;
            std::string mime_type;
            std::string in_string(reinterpret_cast<const char*>(payload), 
                                 payload_size > 1000 ? 1000 : payload_size);
            
            // Test with various combinations
            bool result1 = tinygltf::DecodeDataURI(&out, mime_type, in_string, 0, false);
            bool result2 = tinygltf::DecodeDataURI(&out, mime_type, in_string, 100, true);
            bool result3 = tinygltf::DecodeDataURI(&out, mime_type, "data:image/png;base64,test", 4, true);
            bool result4 = tinygltf::DecodeDataURI(&out, mime_type, "", 0, false);
            break;
        }
        
        case 2: {
            // Test IsDataURI
            std::string test_string(reinterpret_cast<const char*>(payload), 
                                   payload_size > 500 ? 500 : payload_size);
            bool result1 = tinygltf::IsDataURI(test_string);
            bool result2 = tinygltf::IsDataURI("data:image/png;base64,test");
            bool result3 = tinygltf::IsDataURI("data:text/plain,hello");
            bool result4 = tinygltf::IsDataURI("");
            bool result5 = tinygltf::IsDataURI("http://example.com");
            break;
        }
        
        case 3: {
            // Test FileExists
            // Create a dummy file for testing
            std::ofstream dummy_file("./dummy_file", std::ios::binary);
            if (dummy_file.is_open() && payload_size > 0) {
                dummy_file.write(reinterpret_cast<const char*>(payload), 
                                payload_size > 1000 ? 1000 : payload_size);
                dummy_file.close();
            }
            
            bool result1 = tinygltf::FileExists("./dummy_file", nullptr);
            bool result2 = tinygltf::FileExists("/nonexistent/path", nullptr);
            bool result3 = tinygltf::FileExists("", nullptr);
            
            // Clean up
            std::remove("./dummy_file");
            break;
        }
        
        case 4: {
            // Test URIDecode
            std::string in_uri(reinterpret_cast<const char*>(payload), 
                              payload_size > 500 ? 500 : payload_size);
            std::string out_uri;
            
            bool result1 = tinygltf::URIDecode(in_uri, &out_uri, nullptr);
            bool result2 = tinygltf::URIDecode("test%20string", &out_uri, nullptr);
            bool result3 = tinygltf::URIDecode("", &out_uri, nullptr);
            bool result4 = tinygltf::URIDecode("%20%21%22", &out_uri, nullptr);
            break;
        }
        
        case 5: {
            // Test LoadASCIIFromString
            tinygltf::TinyGLTF loader;
            tinygltf::Model model;
            std::string err;
            std::string warn;
            
            std::string gltf_string(reinterpret_cast<const char*>(payload), 
                                   payload_size > 2000 ? 2000 : payload_size);
            
            // Test with various base_dir values
            bool result1 = loader.LoadASCIIFromString(&model, &err, &warn, 
                                                     gltf_string.c_str(), 
                                                     static_cast<unsigned int>(gltf_string.length()),
                                                     ".");
            
            bool result2 = loader.LoadASCIIFromString(&model, &err, &warn,
                                                     "{}",
                                                     2,
                                                     "");
            
            bool result3 = loader.LoadASCIIFromString(&model, &err, &warn,
                                                     "{\"asset\":{\"version\":\"2.0\"}}",
                                                     30,
                                                     "/tmp");
            
            // Test with empty string
            bool result4 = loader.LoadASCIIFromString(&model, &err, &warn,
                                                     "",
                                                     0,
                                                     ".");
            
            // Test with minimal valid glTF
            const char* minimal_gltf = R"({
                "asset": {
                    "version": "2.0"
                },
                "scenes": [{
                    "nodes": []
                }],
                "scene": 0
            })";
            bool result5 = loader.LoadASCIIFromString(&model, &err, &warn,
                                                     minimal_gltf,
                                                     static_cast<unsigned int>(strlen(minimal_gltf)),
                                                     ".");
            break;
        }
    }

    return 0;
}
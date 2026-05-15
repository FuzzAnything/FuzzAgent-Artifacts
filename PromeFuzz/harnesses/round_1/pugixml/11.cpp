// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

static void initialize_document(pugi::xml_document& doc) {
    // Create a root node to work with
    pugi::xml_node root = doc.append_child("root");
    // Ensure we have a valid node
    if (!root) {
        root = doc.append_child("root");
    }
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset, size_t max_len) {
    if (offset >= size) return "";
    size_t len = std::min(max_len, size - offset);
    return std::string_view(reinterpret_cast<const char*>(data + offset), len);
}

static const char* get_c_string(const uint8_t* data, size_t size, size_t offset, size_t max_len) {
    static thread_local std::string buffer;
    if (offset >= size) return "";
    size_t len = std::min(max_len, size - offset);
    buffer.assign(reinterpret_cast<const char*>(data + offset), len);
    return buffer.c_str();
}

static double get_double(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(double) > size) return 0.0;
    double value;
    std::memcpy(&value, data + offset, sizeof(double));
    return value;
}

static float get_float(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(float) > size) return 0.0f;
    float value;
    std::memcpy(&value, data + offset, sizeof(float));
    return value;
}

static bool get_bool(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return false;
    return data[offset] != 0;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* data, size_t size) {
    static const size_t MAX_STR_LEN = 256;
    static const size_t NUM_CYCLES = 6;
    
    pugi::xml_document doc;
    initialize_document(doc);
    pugi::xml_node node = doc.child("root");
    
    if (!node) {
        return 0;
    }
    
    size_t data_offset = 0;
    
    for (int i = 0; i < NUM_CYCLES; ++i) {
        // First append_attribute (string_view_t overload)
        {
            std::string_view attr_name1 = get_string_view(data, size, data_offset, MAX_STR_LEN);
            data_offset += std::min(MAX_STR_LEN, size - data_offset);
            
            if (!attr_name1.empty()) {
                std::string safe_name1(attr_name1.data(), attr_name1.length());
                pugi::xml_attribute attr1 = node.append_attribute(safe_name1.c_str());
                
                if (attr1) {
                    // First set_value (bool overload)
                    bool bool_val = get_bool(data, size, data_offset);
                    data_offset = (data_offset + 1) % size;
                    attr1.set_value(bool_val);
                }
            }
        }
        
        // Second append_attribute (const char* overload)
        {
            const char* attr_name2 = get_c_string(data, size, data_offset, MAX_STR_LEN);
            data_offset += std::min(MAX_STR_LEN, size - data_offset);
            
            if (attr_name2 && attr_name2[0] != '\0') {
                pugi::xml_attribute attr2 = node.append_attribute(attr_name2);
                
                if (attr2) {
                    // Second set_value (double overload)
                    double double_val = get_double(data, size, data_offset);
                    data_offset = (data_offset + sizeof(double)) % size;
                    attr2.set_value(double_val);
                }
            }
        }
        
        // Third append_attribute (string_view_t overload)
        {
            std::string_view attr_name3 = get_string_view(data, size, data_offset, MAX_STR_LEN);
            data_offset += std::min(MAX_STR_LEN, size - data_offset);
            
            if (!attr_name3.empty()) {
                std::string safe_name3(attr_name3.data(), attr_name3.length());
                pugi::xml_attribute attr3 = node.append_attribute(safe_name3.c_str());
                
                if (attr3) {
                    // Third set_value (float overload)
                    float float_val = get_float(data, size, data_offset);
                    data_offset = (data_offset + sizeof(float)) % size;
                    attr3.set_value(float_val);
                }
            }
        }
        
        // Fourth append_attribute (const char* overload)
        {
            const char* attr_name4 = get_c_string(data, size, data_offset, MAX_STR_LEN);
            data_offset += std::min(MAX_STR_LEN, size - data_offset);
            
            if (attr_name4 && attr_name4[0] != '\0') {
                pugi::xml_attribute attr4 = node.append_attribute(attr_name4);
                
                if (attr4) {
                    // Fourth set_value (const char* overload)
                    const char* str_val1 = get_c_string(data, size, data_offset, MAX_STR_LEN);
                    data_offset += std::min(MAX_STR_LEN, size - data_offset);
                    if (str_val1) {
                        attr4.set_value(str_val1);
                    }
                }
            }
        }
        
        // Fifth append_attribute (string_view_t overload)
        {
            std::string_view attr_name5 = get_string_view(data, size, data_offset, MAX_STR_LEN);
            data_offset += std::min(MAX_STR_LEN, size - data_offset);
            
            if (!attr_name5.empty()) {
                std::string safe_name5(attr_name5.data(), attr_name5.length());
                pugi::xml_attribute attr5 = node.append_attribute(safe_name5.c_str());
                
                if (attr5) {
                    // Fifth set_value (string_view_t overload)
                    std::string_view str_val2 = get_string_view(data, size, data_offset, MAX_STR_LEN);
                    data_offset += std::min(MAX_STR_LEN, size - data_offset);
                    attr5.set_value(str_val2);
                }
            }
        }
        
        // Sixth append_attribute (const char* overload)
        {
            const char* attr_name6 = get_c_string(data, size, data_offset, MAX_STR_LEN);
            data_offset += std::min(MAX_STR_LEN, size - data_offset);
            
            if (attr_name6 && attr_name6[0] != '\0') {
                pugi::xml_attribute attr6 = node.append_attribute(attr_name6);
                
                if (attr6) {
                    // Sixth set_value (bool overload - repeating to cover all)
                    bool bool_val2 = get_bool(data, size, data_offset);
                    data_offset = (data_offset + 1) % size;
                    attr6.set_value(bool_val2);
                }
            }
        }
        
        // Reset offset if we've consumed all data
        if (data_offset >= size) {
            data_offset = 0;
        }
    }
    
    return 0;
}
// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
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
#include <vector>
#include <string_view>

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    size_t len = size < max_len ? size : max_len;
    std::string result(reinterpret_cast<const char*>(data), len);
    if (!result.empty() && result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        pugi::xml_document doc;
        
        std::string xml_content = create_null_terminated_string(Data, Size, 1000);
        
        pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
        
        if (!result) {
            return 0;
        }
        
        pugi::xml_node root = doc.root();
        if (!root) {
            return 0;
        }
        
        std::vector<std::string> attribute_names;
        size_t offset = 0;
        
        for (int i = 0; i < 8 && offset < Size; i++) {
            size_t chunk_size = (Size - offset) / (8 - i);
            if (chunk_size > 100) chunk_size = 100;
            
            std::string name = create_null_terminated_string(Data + offset, chunk_size, 100);
            attribute_names.push_back(name);
            offset += chunk_size;
        }
        
        if (attribute_names.empty()) {
            return 0;
        }
        
        size_t name_index = 0;
        
        pugi::xml_attribute attr1 = root.attribute(std::string_view(attribute_names[name_index % attribute_names.size()]));
        name_index++;
        
        pugi::xml_node child1 = root.child(std::string_view(attribute_names[name_index % attribute_names.size()]));
        name_index++;
        
        pugi::xml_attribute attr2 = root.attribute(std::string_view(attribute_names[name_index % attribute_names.size()]));
        name_index++;
        
        pugi::xml_attribute attr3 = root.attribute(std::string_view(attribute_names[name_index % attribute_names.size()]));
        name_index++;
        
        pugi::xml_attribute attr4 = root.attribute(std::string_view(attribute_names[name_index % attribute_names.size()]));
        name_index++;
        
        pugi::xml_attribute last_attr = root.last_attribute();
        name_index++;
        
        pugi::xml_attribute attr5 = root.attribute(std::string_view(attribute_names[name_index % attribute_names.size()]));
        name_index++;
        
        pugi::xml_attribute attr6 = root.attribute(std::string_view(attribute_names[name_index % attribute_names.size()]));
        
    } catch (...) {
    }
    
    return 0;
}
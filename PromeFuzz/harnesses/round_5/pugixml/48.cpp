// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <fstream>
#include <string_view>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static class DummyWriter : public pugi::xml_writer {
public:
    virtual void write(const void* data, size_t size) override {
        // Do nothing, just consume the data
        (void)data;
        (void)size;
    }
} dummy_writer;

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential file operations
    writeDummyFile(Data, Size);
    
    // Create a document and initial structure
    pugi::xml_document doc;
    
    // Create a root node
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Create a child node to use as reference for insert_child_before
    pugi::xml_node ref_child = root.append_child(pugi::node_element);
    
    // Create another child to have more structure
    pugi::xml_node another_child = root.append_child(pugi::node_element);
    
    // 1. First insert_child_before
    // Use some data to determine node type
    pugi::xml_node_type type1 = pugi::node_element;
    if (Size > 0) {
        type1 = static_cast<pugi::xml_node_type>((Data[0] % 8) + 1); // +1 to skip node_null
    }
    pugi::xml_node new_node1 = root.insert_child_before(type1, ref_child);
    
    // 2. First first_child
    pugi::xml_node first_child = root.first_child();
    
    // 3. append_attribute
    if (first_child) {
        // Create attribute name from data
        std::string_view attr_name1;
        if (Size > 1) {
            size_t name_len = std::min(static_cast<size_t>(Data[1]), Size - 2);
            if (name_len > 0 && name_len < Size) {
                attr_name1 = std::string_view(reinterpret_cast<const char*>(Data + 2), name_len);
            }
        }
        
        if (!attr_name1.empty()) {
            // Ensure null termination
            std::string attr_name_str(attr_name1.data(), attr_name1.length());
            pugi::xml_attribute attr = first_child.append_attribute(attr_name_str.c_str());
            
            // Try to set attribute value if we have more data
            if (attr && Size > 10) {
                size_t value_offset = std::min(static_cast<size_t>(10), Size - 1);
                size_t value_len = std::min(static_cast<size_t>(Data[value_offset]), Size - value_offset - 1);
                if (value_len > 0 && value_offset + value_len < Size) {
                    std::string_view value_str(reinterpret_cast<const char*>(Data + value_offset + 1), value_len);
                    std::string value(value_str.data(), value_str.length());
                    attr.set_value(value.c_str());
                }
            }
        }
    }
    
    // 4. Second insert_child_before
    pugi::xml_node_type type2 = pugi::node_pcdata;
    if (Size > 3) {
        type2 = static_cast<pugi::xml_node_type>((Data[3] % 8) + 1);
    }
    pugi::xml_node new_node2 = root.insert_child_before(type2, another_child);
    
    // 5. Second first_child
    pugi::xml_node first_child2 = root.first_child();
    
    // 6. set_value
    if (first_child2) {
        // Create value from data
        std::string_view value_data;
        if (Size > 20) {
            size_t value_offset = std::min(static_cast<size_t>(20), Size - 1);
            size_t value_len = std::min(static_cast<size_t>(Data[value_offset]), Size - value_offset - 1);
            if (value_len > 0 && value_offset + value_len < Size) {
                value_data = std::string_view(reinterpret_cast<const char*>(Data + value_offset + 1), value_len);
            }
        }
        
        if (!value_data.empty()) {
            // Ensure null termination
            std::string value_str(value_data.data(), value_data.length());
            first_child2.set_value(value_str.c_str());
        }
    }
    
    // 7. save
    try {
        // Save to dummy writer
        doc.save(dummy_writer);
        
        // Also try saving with different parameters
        if (Size > 30) {
            const char* indent = "\t";
            unsigned int flags = pugi::format_default;
            pugi::xml_encoding encoding = pugi::encoding_auto;
            
            // Use data to vary parameters
            if (Data[30] % 2 == 0) {
                indent = "  ";
            }
            if (Data[30] % 3 == 0) {
                flags = pugi::format_indent;
            }
            if (Data[30] % 4 == 0) {
                encoding = pugi::encoding_utf8;
            }
            
            doc.save(dummy_writer, indent, flags, encoding);
        }
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    return 0;
}
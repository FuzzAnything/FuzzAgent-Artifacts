// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <pugixml.hpp>

// Helper to safely write data to a dummy file
static bool write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(data), size);
    return file.good();
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: Create a document
    pugi::xml_document doc;
    
    // Use a portion of input as XML content to parse
    std::vector<char> buffer(Size);
    for (size_t i = 0; i < Size; ++i) {
        buffer[i] = static_cast<char>(Data[i]);
    }

    // Try parsing the buffer into the document
    pugi::xml_parse_result res = doc.load_buffer(buffer.data(), buffer.size());
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // If parsing failed or no root, try to append_buffer to create nodes
    if (root.type() != pugi::node_element && root.type() != pugi::node_document) {
        if (root.type() == pugi::node_document) {
            res = root.append_buffer(buffer.data(), buffer.size());
            root = doc.root();
        }
    }

    // Fuzzing logic for target API functions

    // 1. pugi::xml_node::child_value
    // Overload 1: child_value()
    if (root) {
        const pugi::char_t* val1 = root.child_value();
        (void)val1;
    }
    
    // Overload 2: child_value(name)
    if (Size > 0) {
        std::vector<pugi::char_t> name_buf(Size + 1);
        for (size_t i = 0; i < Size; ++i) {
            name_buf[i] = static_cast<pugi::char_t>(Data[i]);
        }
        name_buf[Size] = 0;
        
        if (root) {
            const pugi::char_t* val2 = root.child_value(name_buf.data());
            (void)val2;
        }
    }

    // 2. pugi::xml_node::name
    if (root) {
        const pugi::char_t* name = root.name();
        (void)name;
    }

    // 3. pugi::xml_node::append_buffer
    if (root && (root.type() == pugi::node_element || root.type() == pugi::node_document)) {
        unsigned int options = pugi::parse_default;
        // Toggle valid flags based on input to vary behavior
        // Valid flags: parse_escapes, parse_wconv_attribute
        if (Data[0] & 1) options |= pugi::parse_escapes;
        if (Data[0] & 2) options |= pugi::parse_wconv_attribute;

        pugi::xml_encoding enc = pugi::encoding_auto;
        if (Size > 1) {
            int enc_idx = Data[1] % 9;
            enc = static_cast<pugi::xml_encoding>(enc_idx);
        }

        pugi::xml_parse_result res_append = root.append_buffer(buffer.data(), buffer.size(), options, enc);
        (void)res_append;
    }

    // 4. pugi::xml_node::print
    // Overload 1: print(std::basic_ostream<wchar_t>& os, ...)
    if (root) {
        std::wstringstream wss;
        
        unsigned int flags = pugi::format_default;
        if (Size > 2) {
            int flag_idx = Data[2] % 8;
            if (flag_idx & 1) flags |= pugi::format_raw;
            if (flag_idx & 2) flags |= pugi::format_no_escapes;
            if (flag_idx & 4) flags |= pugi::format_indent_attributes;
        }
        
        root.print(wss, PUGIXML_TEXT("\t"), flags, 0);
    }

    // Overload 2: print(xml_writer& writer, ...)
    if (root) {
        if (write_dummy_file(Data, Size)) {
            FILE* f = fopen("./dummy_file", "wb");
            if (f) {
                pugi::xml_writer_file writer(f);
                unsigned int flags = pugi::format_default;
                if (Size > 3) {
                    int flag_idx = Data[3] % 8;
                    if (flag_idx & 1) flags |= pugi::format_raw;
                    if (flag_idx & 2) flags |= pugi::format_no_escapes;
                }
                
                pugi::xml_encoding enc = pugi::encoding_auto;
                if (Size > 4) {
                    int enc_idx = Data[4] % 9;
                    enc = static_cast<pugi::xml_encoding>(enc_idx);
                }

                root.print(writer, PUGIXML_TEXT("\t"), flags, enc, 0);
                fclose(f);
            }
        }
    }

    return 0;
}
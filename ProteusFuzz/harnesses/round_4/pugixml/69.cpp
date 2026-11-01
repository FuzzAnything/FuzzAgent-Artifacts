// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <fstream>

static std::string generate_xml_from_fuzz(const uint8_t* data, size_t size) {
    std::string result = "<root>";
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = data[i];
        if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '&') {
            result += "&amp;";
        } else if (c == '"') {
            result += "&quot;";
        } else if (c == '\'') {
            result += "&apos;";
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    std::string xml_content = generate_xml_from_fuzz(Data, Size);
    
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    if (!result) {
        result = doc.load_string(xml_content.c_str(), pugi::parse_default | pugi::parse_embed_pcdata);
        if (!result) {
            return 0;
        }
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("dummy");
    }

    // Test pugi::xml_node::child_value (overload 1)
    const pugi::char_t* val1 = root.child_value();
    (void)val1;

    // Test pugi::xml_node::child_value (overload 2)
    const pugi::char_t* val2 = root.child_value("child");
    (void)val2;
    
    if (Size >= 4) {
        char name_buf[5] = {0};
        for (int i = 0; i < 4 && i < Size; ++i) {
            unsigned char c = Data[i];
            if (c >= 'a' && c <= 'z') name_buf[i] = c;
        }
        if (name_buf[0] != 0) {
            const pugi::char_t* val3 = root.child_value(name_buf);
            (void)val3;
        }
    }

    // Test pugi::xml_node::print (overload 1: std::basic_ostream<wchar_t>)
    {
        std::wstringstream wss;
        root.print(wss, PUGIXML_TEXT("\t"), pugi::format_default, 0);
        (void)wss.str();
    }

    // Test pugi::xml_node::print (overload 2: xml_writer)
    {
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.close();
            FILE* f = fopen("./dummy_file", "wb");
            if (f) {
                pugi::xml_writer_file writer(f);
                root.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8, 0);
                fclose(f);
            }
        }
    }

    // Test pugi::xml_node::append_buffer
    std::string fragment = "<item>";
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c == '<') fragment += "&lt;";
        else if (c == '>') fragment += "&gt;";
        else if (c == '&') fragment += "&amp;";
        else if (c >= 32 && c < 127) fragment += static_cast<char>(c);
    }
    fragment += "</item>";
    
    pugi::xml_parse_result append_res = root.append_buffer(
        fragment.c_str(), 
        fragment.size(), 
        pugi::parse_default, 
        pugi::encoding_utf8
    );
    (void)append_res;

    if (Size > 0) {
        pugi::xml_parse_result append_res2 = root.append_buffer(
            fragment.c_str(), 
            fragment.size(), 
            pugi::parse_embed_pcdata, 
            pugi::encoding_utf8
        );
        (void)append_res2;
    }

    // Test pugi::xml_node::name
    const pugi::char_t* name = root.name();
    (void)name;

    doc.reset();

    return 0;
}
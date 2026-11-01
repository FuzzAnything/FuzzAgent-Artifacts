// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
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

static void write_dummy_file(const char* path, const uint8_t* data, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static uint32_t read_uint32_safe(const uint8_t* data, size_t size, size_t offset) {
    if (size < offset + 4) {
        return 0;
    }
    return *reinterpret_cast<const uint32_t*>(data + offset);
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char* dummy_path = "./dummy_file";
    const wchar_t* dummy_wpath = L"./dummy_file";

    // Write input data to dummy file for load_file tests
    write_dummy_file(dummy_path, Data, Size);

    // Test load_file (char* overload)
    {
        pugi::xml_document doc;
        unsigned int options = 0;
        pugi::xml_encoding encoding = pugi::encoding_auto;

        if (Size >= 4) {
            options = read_uint32_safe(Data, Size, 0);
        }
        if (Size >= 8) {
            uint32_t enc_val = read_uint32_safe(Data, Size, 4);
            enc_val %= 10; // Valid range for xml_encoding enum
            encoding = static_cast<pugi::xml_encoding>(enc_val);
        }

        pugi::xml_parse_result result = doc.load_file(dummy_path, options, encoding);
        (void)result;
    }

    // Test load_file (wchar_t* overload)
    {
        pugi::xml_document doc;
        unsigned int options = 0;
        pugi::xml_encoding encoding = pugi::encoding_auto;

        if (Size >= 4) {
            options = read_uint32_safe(Data, Size, 0);
        }
        if (Size >= 8) {
            uint32_t enc_val = read_uint32_safe(Data, Size, 4);
            enc_val %= 10;
            encoding = static_cast<pugi::xml_encoding>(enc_val);
        }

        pugi::xml_parse_result result = doc.load_file(dummy_wpath, options, encoding);
        (void)result;
    }

    // Test as_utf8
    {
        if (Size >= 2) {
            // Construct a valid wchar_t string from input (simple conversion for testing)
            std::wstring wstr;
            size_t wlen = (Size / 2) + 1;
            wstr.resize(wlen);
            for (size_t i = 0; i < Size / 2; ++i) {
                wstr[i] = static_cast<wchar_t>(Data[i * 2]);
            }
            wstr[wlen - 1] = L'\0';

            std::string utf8_str = pugi::as_utf8(wstr.c_str());
            (void)utf8_str;
        }
    }

    // Test as_wide
    {
        if (Size > 0) {
            std::string narrow_str(reinterpret_cast<const char*>(Data), Size);
            std::wstring wide_str = pugi::as_wide(narrow_str);
            (void)wide_str;
        }
    }

    // Test save_file (char* overload)
    {
        pugi::xml_document doc;
        // Create a simple document to save
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child").append_child(pugi::node_pcdata).set_value("test");

        const pugi::char_t* indent = PUGIXML_TEXT("\t");
        unsigned int flags = 0;
        pugi::xml_encoding encoding = pugi::encoding_auto;

        if (Size >= 12) {
            flags = read_uint32_safe(Data, Size, 8);
            uint32_t enc_val = read_uint32_safe(Data, Size, 12);
            enc_val %= 10;
            encoding = static_cast<pugi::xml_encoding>(enc_val);
        }

        bool success = doc.save_file(dummy_path, indent, flags, encoding);
        (void)success;
    }

    // Test save_file (wchar_t* overload)
    {
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child").append_child(pugi::node_pcdata).set_value("test");

        const pugi::char_t* indent = PUGIXML_TEXT("\t");
        unsigned int flags = 0;
        pugi::xml_encoding encoding = pugi::encoding_auto;

        if (Size >= 12) {
            flags = read_uint32_safe(Data, Size, 8);
            uint32_t enc_val = read_uint32_safe(Data, Size, 12);
            enc_val %= 10;
            encoding = static_cast<pugi::xml_encoding>(enc_val);
        }

        bool success = doc.save_file(dummy_wpath, indent, flags, encoding);
        (void)success;
    }

    return 0;
}
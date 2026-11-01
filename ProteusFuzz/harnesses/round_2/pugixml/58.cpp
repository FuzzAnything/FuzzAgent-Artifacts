// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
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
#include <pugixml.hpp>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize a document and root node
    pugi::xml_document doc;
    
    // Fuzzing input might not be null-terminated.
    // load_string expects a null-terminated string.
    // We create a temporary buffer with a null terminator to avoid heap-buffer-overflow.
    // We limit the size to prevent excessive allocation if Size is huge, though fuzzer usually handles reasonable sizes.
    // A safe upper bound for typical fuzzing inputs is often sufficient, but here we just ensure null termination.
    // We copy data to a buffer that we control and ensure it ends with \0.
    
    // Check if the input already ends with null, if not we need to copy and append null.
    // However, load_string reads until \0. If Data contains no \0 within Size bytes, 
    // it will read past the buffer if we pass Data directly.
    // So we must create a safe copy.
    
    // Strategy: Create a vector of char_t, copy data, ensure null termination.
    // We use char_t which is typically char or wchar_t depending on compile flags.
    // Since Data is uint8_t, we assume the input is intended for char_t (usually char).
    // If PUGIXML_WCHAR_MODE is defined, char_t is wchar_t (usually 2 or 4 bytes).
    // For simplicity in fuzzing raw bytes, we assume the input is meant for the default char_t.
    // If the library is compiled with wchar_t, passing raw bytes might be tricky, but 
    // load_string(char_t*) will interpret them.
    
    // To be safe against non-null-terminated input:
    std::vector<pugi::char_t> safe_buffer(Size + 1);
    if (Size > 0) {
        // Copy bytes. If char_t is larger than 1 byte, this is a naive cast.
        // However, for fuzzing, we often treat input as raw bytes for the target encoding.
        // If char_t is wchar_t, we might need to handle encoding conversion, but 
        // standard fuzzing practice often just casts to the target type for "binary" fuzzing.
        // Let's assume char_t is char for this context or that the fuzzer provides valid data.
        // To be strictly safe with the cast:
        std::memcpy(safe_buffer.data(), Data, Size);
    }
    safe_buffer[Size] = 0; // Ensure null termination

    pugi::xml_parse_result result = doc.load_string(safe_buffer.data(), pugi::parse_default);
    
    pugi::xml_node root = doc.root();
    
    // 1. Test pugi::xml_node::print (ostream overload)
    {
        std::stringstream stream;
        if (!root.empty()) {
            root.print(stream, "\t", pugi::format_default, pugi::encoding_auto, 0);
        } else {
            pugi::xml_node empty_node;
            empty_node.print(stream, "\t", pugi::format_default, pugi::encoding_auto, 0);
        }
    }

    // 1. Test pugi::xml_node::print (xml_writer overload)
    {
        const char* dummy_file = "./dummy_file";
        FILE* f = fopen(dummy_file, "wb");
        if (f) {
            pugi::xml_writer_file writer(f);
            if (!root.empty()) {
                root.print(writer, "\t", pugi::format_default, pugi::encoding_auto, 0);
            } else {
                pugi::xml_node empty_node;
                empty_node.print(writer, "\t", pugi::format_default, pugi::encoding_auto, 0);
            }
            fclose(f);
        }
    }

    // 2. Test pugi::xml_node::append_buffer
    {
        pugi::xml_node target = root;
        if (target.type() != pugi::node_element && target.type() != pugi::node_document) {
            target = doc.append_child("test");
        }

        // append_buffer takes raw buffer and size, so we can use the original Data directly
        // as it handles the size explicitly and does not rely on null termination.
        pugi::xml_parse_result append_res = target.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
        
        if (Size > 0) {
            target.append_buffer(Data, Size, pugi::parse_embed_pcdata, pugi::encoding_auto);
            target.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_utf8);
            target.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_utf16_le);
        }
    }

    // 3. Test pugi::xml_node::name
    {
        const pugi::char_t* name_root = root.name();
        (void)name_root;

        if (root.first_child()) {
            const pugi::char_t* name_child = root.first_child().name();
            (void)name_child;
        }

        pugi::xml_node empty_node;
        const pugi::char_t* name_empty = empty_node.name();
        (void)name_empty;
    }

    // 4. Test pugi::xml_node::child_value (no name)
    {
        const pugi::char_t* val = root.child_value();
        (void)val;

        pugi::xml_node empty_node;
        const pugi::char_t* val_empty = empty_node.child_value();
        (void)val_empty;
    }

    // 4. Test pugi::xml_node::child_value (with name)
    {
        const pugi::char_t* val_named = root.child_value("test");
        (void)val_named;

        pugi::xml_node empty_node;
        const pugi::char_t* val_named_empty = empty_node.child_value("test");
        (void)val_named_empty;
    }

    return 0;
}
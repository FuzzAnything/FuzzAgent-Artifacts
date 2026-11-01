// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_writer_file::write at pugixml.cpp:5182:37 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_writer_stream::write at pugixml.cpp:5197:39 in pugixml.hpp
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

static pugi::xml_document* g_doc = nullptr;

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize document if not already done
    if (!g_doc) {
        g_doc = new pugi::xml_document();
    }

    pugi::xml_document& doc = *g_doc;

    // Reset document for each test case to ensure clean state
    doc.reset();

    // Write input data to a dummy file for load_file testing
    const char* dummy_path = "./dummy_file";
    FILE* f = fopen(dummy_path, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    } else {
        return 0;
    }

    // Randomize options and encoding for load_file
    unsigned int options = (Size > 0) ? (Data[0] & 0xFF) : 0;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>((Size > 1) ? (Data[1] & 0x0F) : 0);
    if (encoding > pugi::encoding_latin1) encoding = pugi::encoding_auto;

    // Test load_file
    pugi::xml_parse_result result = doc.load_file(dummy_path, options, encoding);
    (void)result; // Ignore result for fuzzing, but function is called

    if (!doc.first_child()) {
        // If parsing failed or produced no root, try to create a simple structure for other tests
        pugi::xml_node root = doc.append_child("root");
        if (Size > 2) {
            // Create some attributes or children based on input
            // Use fixed size buffers to prevent overflow
            char attr_name[16] = {0};
            char attr_val[32] = {0};
            
            // Calculate safe lengths
            size_t name_len = (Size - 2) > 15 ? 15 : (Size - 2);
            size_t val_len = (Size - 2 - name_len) > 31 ? 31 : (Size - 2 - name_len);
            
            if (name_len > 0) {
                std::memcpy(attr_name, Data + 2, name_len);
                attr_name[name_len] = '\0';
                
                if (val_len > 0) {
                    std::memcpy(attr_val, Data + 2 + name_len, val_len);
                    attr_val[val_len] = '\0';
                    root.append_attribute(attr_name).set_value(attr_val);
                } else {
                    root.append_attribute(attr_name).set_value("fuzz");
                }
            }
        }
    }

    // Test save_file
    doc.save_file(dummy_path, PUGIXML_TEXT("\t"), (Size > 2) ? (Data[2] & 0xFF) : 0, encoding);

    // Test save to wide stream
    {
        std::wstringstream wstream;
        doc.save(wstream, PUGIXML_TEXT("\t"), (Size > 3) ? (Data[3] & 0xFF) : 0);
    }

    // Test xml_writer_stream::write
    // The target function asserts that size is a multiple of sizeof(wchar_t) for wide streams.
    // We must align the size to avoid triggering the assertion.
    {
        std::wstringstream wstream;
        pugi::xml_writer_stream writer(wstream);
        if (Size > 4) {
            size_t raw_size = (Size - 4) > 1000 ? 1000 : (Size - 4);
            // Align size down to the nearest multiple of sizeof(wchar_t)
            size_t aligned_size = raw_size - (raw_size % sizeof(wchar_t));
            if (aligned_size > 0) {
                writer.write(reinterpret_cast<const void*>(Data + 4), aligned_size);
            }
        }
    }

    // Test xml_node::print
    {
        std::wstringstream wstream;
        pugi::xml_node node = doc.first_child();
        if (node) {
            node.print(wstream, PUGIXML_TEXT("\t"), (Size > 5) ? (Data[5] & 0xFF) : 0, (Size > 6) ? (Data[6] & 0xFF) : 0);
        } else {
            // Print empty node if no child
            pugi::xml_node empty;
            empty.print(wstream, PUGIXML_TEXT("\t"), (Size > 5) ? (Data[5] & 0xFF) : 0, (Size > 6) ? (Data[6] & 0xFF) : 0);
        }
    }

    // Test xml_writer_file::write
    // xml_writer_file writes raw bytes to a FILE*, so no alignment is needed.
    {
        FILE* f_write = fopen(dummy_path, "wb");
        if (f_write) {
            pugi::xml_writer_file writer(f_write);
            if (Size > 7) {
                size_t write_size = (Size - 7) > 1000 ? 1000 : (Size - 7);
                writer.write(reinterpret_cast<const void*>(Data + 7), write_size);
            }
            fclose(f_write);
        }
    }

    // Cleanup
    doc.reset();

    return 0;
}
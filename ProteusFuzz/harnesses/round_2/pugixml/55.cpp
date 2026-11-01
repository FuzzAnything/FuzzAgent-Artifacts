// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <pugixml.hpp>

// Custom writer for fuzzing print function
class fuzz_writer : public pugi::xml_writer {
public:
    fuzz_writer(std::vector<pugi::char_t>& buffer) : m_buffer(buffer) {}

    void write(const void* data, size_t size) override {
        if (!data) return;
        const pugi::char_t* char_data = static_cast<const pugi::char_t*>(data);
        m_buffer.insert(m_buffer.end(), char_data, char_data + size);
    }

private:
    std::vector<pugi::char_t>& m_buffer;
};

static int parse_and_test(const uint8_t* data, size_t size) {
    if (size == 0) return 0;

    // Convert input to string for parsing
    std::string input(reinterpret_cast<const char*>(data), size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    
    // If parsing fails, try with different encodings
    if (!result) {
        // Try UTF-8 explicitly
        result = doc.load_string(input.c_str(), pugi::parse_default);
        if (!result) {
            // Try UTF-16 LE
            if (size % 2 == 0) {
                std::vector<uint16_t> utf16_data(size / 2);
                for (size_t i = 0; i < size / 2; ++i) {
                    utf16_data[i] = data[i * 2] | (data[i * 2 + 1] << 8);
                }
                // Note: load_string for UTF16 requires specific handling or PUGIXML_WCHAR_MODE
                // For simplicity, we just try to load as UTF8 if the above fails
                // In a real scenario, we might need to use load_buffer with encoding
            }
        }
    }

    // Test xml_node::name
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        const pugi::char_t* name = node.name();
        (void)name; // Use the result to avoid unused variable warning
    }

    // Test xml_node::value
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        const pugi::char_t* value = node.value();
        (void)value;
    }

    // Test xml_node::child_value (both overloads)
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        const pugi::char_t* child_value1 = node.child_value();
        (void)child_value1;
        
        // Try with a specific name (using a common name)
        const pugi::char_t* child_value2 = node.child_value(PUGIXML_TEXT("test"));
        (void)child_value2;
        
        // Try with input data as name (safely truncated)
        size_t len = size < 100 ? size : 100;
        std::string name_str(input.begin(), input.begin() + len);
        // Remove null characters
        name_str.erase(std::remove(name_str.begin(), name_str.end(), '\0'), name_str.end());
        if (!name_str.empty()) {
            const pugi::char_t* child_value3 = node.child_value(name_str.c_str());
            (void)child_value3;
        }
    }

    // Test xml_text::get
    pugi::xml_text text_node;
    const pugi::char_t* text = text_node.get();
    (void)text;

    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        pugi::xml_text text = node.text();
        const pugi::char_t* text_val = text.get();
        (void)text_val;
    }

    // Test xml_node::print with various parameters
    std::vector<pugi::char_t> output_buffer;
    fuzz_writer writer(output_buffer);
    
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        // Test with default parameters
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        
        // Test with different indentation
        node.print(writer, PUGIXML_TEXT("  "), pugi::format_default, pugi::encoding_auto, 0);
        
        // Test with different flags
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_no_escapes, pugi::encoding_auto, 0);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_raw, pugi::encoding_auto, 0);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_indent, pugi::encoding_auto, 0);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_no_escapes | pugi::format_indent, pugi::encoding_auto, 0);
        
        // Test with different encodings
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8, 0);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf16_le, 0);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf16_be, 0);
        
        // Test with different depths
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 1);
        node.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 5);
    }

    // Test print on document itself
    doc.print(writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);

    // Test with file writer
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        pugi::xml_writer_file file_writer(file);
        
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            node.print(file_writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
            node.print(file_writer, PUGIXML_TEXT("  "), pugi::format_raw, pugi::encoding_utf8, 2);
        }
        
        doc.print(file_writer, PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto, 0);
        
        fclose(file);
    }

    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Handle empty input
    if (Size == 0) {
        return 0;
    }

    // Limit maximum size to prevent resource exhaustion
    if (Size > 1024 * 1024) { // 1MB limit
        return 0;
    }

    // Create dummy file if needed
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size, dummy);
        fclose(dummy);
    }

    // Run the test
    parse_and_test(Data, Size);

    return 0;
}
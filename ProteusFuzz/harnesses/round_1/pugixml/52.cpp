// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <fstream>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        // Fuzzing input may not be null-terminated or valid UTF-8.
        // We must ensure a null-terminated string is passed to load_string.
        // We copy the data to a temporary buffer and ensure termination.
        std::string buffer(reinterpret_cast<const char*>(data), size);
        
        // Remove any null bytes in the middle to ensure valid C-string behavior for load_string
        // or simply rely on the string's internal null termination.
        // However, load_string expects a null-terminated string. 
        // If the input contains embedded nulls, strlen will stop early, which is safe,
        // but if the input is all binary without a null, we need to ensure safety.
        // The safest approach for fuzzing is to create a copy and ensure it's null-terminated
        // if the original wasn't, but std::string already handles this.
        // The issue in the crash was likely accessing memory beyond the buffer if load_string
        // expects a null terminator and the input didn't have one, or if the input 
        // contained a null byte that truncated the string unexpectedly causing logic errors.
        // Actually, the crash was a heap-buffer-overflow in strlen inside pugixml.
        // This implies load_string was called with a pointer that wasn't null-terminated 
        // or the length calculation went wrong.
        // std::string::c_str() returns a null-terminated string.
        // Let's ensure we pass a valid C-string.
        doc.load_string(buffer.c_str(), pugi::parse_default);
    }
    return doc;
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Prepare environment
    pugi::xml_document doc = create_document_from_input(Data, Size);
    
    // Ensure we have a root element to work with for insertions
    if (doc.root().type() != pugi::node_element) {
        if (!doc.append_child(pugi::node_element)) {
            return 0;
        }
        doc.root().set_name("root");
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // 1. pugi::xml_node::insert_child_before
    // Insert a new element before the first child (or at the start if empty)
    pugi::xml_node target_sibling = root.first_child();
    pugi::xml_node new_node_1 = root.insert_child_before(pugi::node_element, target_sibling);
    if (new_node_1) {
        new_node_1.set_name("inserted_before_1");
    }

    // 2. pugi::xml_node::first_child
    pugi::xml_node first_child_1 = root.first_child();
    // Traverse to ensure state change is observed
    if (first_child_1) {
        first_child_1.first_child(); 
    }

    // 3. pugi::xml_node::append_attribute
    if (new_node_1) {
        pugi::xml_attribute attr = new_node_1.append_attribute("fuzz_attr");
        if (attr) {
            attr.set_value("fuzz_value");
        }
    }

    // 4. pugi::xml_node::insert_child_before
    // Insert another node, perhaps a comment or text, before the previously inserted node
    pugi::xml_node new_node_2 = root.insert_child_before(pugi::node_comment, new_node_1);
    if (new_node_2) {
        // set_value works on comments
        new_node_2.set_value("fuzz_comment");
    }

    // 5. pugi::xml_node::first_child
    pugi::xml_node first_child_2 = root.first_child();
    if (first_child_2) {
        // Try to access children of the new node if it exists
        first_child_2.first_child();
    }

    // 6. pugi::xml_node::set_value
    // Set value on the first child if it's a valid type (text, comment, etc.)
    if (first_child_2 && (first_child_2.type() == pugi::node_pcdata || 
                          first_child_2.type() == pugi::node_comment || 
                          first_child_2.type() == pugi::node_cdata ||
                          first_child_2.type() == pugi::node_pi)) {
        // Use a safe string derived from input or a constant
        // Ensure we don't pass a string with embedded nulls that might confuse set_value logic
        // though set_value takes a const char*, it usually expects null-terminated.
        // We'll use a fixed safe string to avoid issues with binary data.
        first_child_2.set_value("safe_fuzz_value");
    } else if (first_child_2 && first_child_2.type() == pugi::node_element) {
        // Elements return false for set_value, but we call it anyway to test behavior
        first_child_2.set_value("should_fail_on_element");
    }

    // 7. pugi::xml_document::save
    // Save to a file to trigger serialization logic
    write_dummy_file(Data, Size); // Ensure dummy_file exists or has content if needed for context
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        pugi::xml_writer_file writer(reinterpret_cast<void*>(file));
        // Use various flags and encoding to explore states
        doc.save(writer, "\t", pugi::format_write_bom | pugi::format_indent, pugi::encoding_utf8);
        fclose(file);
    }

    // Cleanup
    // xml_document destructor handles memory cleanup automatically
    return 0;
}
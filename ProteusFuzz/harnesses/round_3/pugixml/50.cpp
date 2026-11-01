// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    pugi::xml_document doc;
    
    // Fuzzing data may not be null-terminated or valid XML.
    // We must ensure null-termination before passing to load_string.
    // We create a temporary buffer with a null terminator.
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Ensure the string is null-terminated for C-style string functions used internally by pugixml
    // Although std::string is null-terminated, if the input contained embedded nulls, 
    // load_string might behave unexpectedly if it relies on strlen. 
    // However, load_string usually takes a pointer and length is implicit via strlen.
    // To be safe against heap-buffer-overflow caused by missing null terminator in the raw buffer 
    // if the fuzzer provides raw bytes without a trailing 0, we rely on std::string's c_str() 
    // which guarantees a null terminator.
    
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());
    
    // If parsing fails completely, we might still want to test on an empty doc or a minimal one
    if (!result) {
        // Create a minimal valid document to ensure we have nodes to work with
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root exists, create one
        root = doc.append_child(pugi::node_element);
        root.set_name("root");
    }

    // Ensure we have a sibling to insert before
    pugi::xml_node sibling = root.first_child();
    if (!sibling) {
        sibling = root.append_child(pugi::node_element);
        sibling.set_name("sibling");
    }

    // 2. Invoke target functions in order

    // Step 1: pugi::xml_node::insert_child_before
    // Insert a new element before the sibling
    pugi::xml_node new_node_1 = root.insert_child_before(pugi::node_element, sibling);
    if (new_node_1) {
        new_node_1.set_name("new_node_1");
    }

    // Step 2: pugi::xml_node::first_child
    // Retrieve the first child of the root
    pugi::xml_node first_child = root.first_child();
    (void)first_child; // Use variable to avoid unused warning

    // Step 3: pugi::xml_node::append_attribute
    // Add an attribute to the root
    pugi::xml_attribute attr = root.append_attribute("fuzz_attr");
    if (attr) {
        // Set a value based on input data if possible
        // Use a safe subset to avoid potential issues with very long strings or invalid chars
        if (Size > 0) {
            // Ensure we don't pass a string with embedded nulls that might truncate unexpectedly
            // though set_value usually handles this. We use the input string directly.
            attr.set_value(input_str.c_str());
        }
    }

    // Step 4: pugi::xml_node::insert_child_before
    // Insert another node, this time before the newly created node if it exists, otherwise before sibling
    pugi::xml_node target_sibling = new_node_1 ? new_node_1 : sibling;
    pugi::xml_node new_node_2 = root.insert_child_before(pugi::node_pcdata, target_sibling);
    if (new_node_2) {
        // PCData nodes usually get their value set directly, but we can try to set it
        if (Size > 0) {
            new_node_2.set_value(input_str.c_str());
        }
    }

    // Step 5: pugi::xml_node::first_child
    // Retrieve first child again to explore state
    pugi::xml_node first_child_2 = root.first_child();
    (void)first_child_2;

    // Step 6: pugi::xml_node::set_value
    // Attempt to set value on a node (must be non-element/non-attribute)
    // We try on the new_node_2 (which is PCData) or create a comment if needed
    pugi::xml_node target_node = new_node_2;
    if (!target_node || target_node.type() == pugi::node_element) {
        // If target is invalid or element, create a comment to set value on
        target_node = root.append_child(pugi::node_comment);
    }
    
    if (target_node) {
        // Use a safe subset of input or a fixed string to set value
        // Avoid setting huge strings if Size is huge to prevent OOM in fuzzer
        size_t val_size = (Size < 1024) ? Size : 1024;
        std::string val_str(input_str.substr(0, val_size));
        target_node.set_value(val_str.c_str());
    }

    // Step 7: pugi::xml_document::save
    // Save to a dummy file
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        // Save with default flags, or try different flags based on input byte
        unsigned int flags = pugi::format_default;
        if (Size > 0 && (Data[0] & 1)) {
            flags |= pugi::format_raw;
        }
        if (Size > 1 && (Data[1] & 1)) {
            flags |= pugi::format_indent;
        }
        
        doc.save(writer, "\t", flags, pugi::encoding_utf8);
        fclose(f);
    } else {
        // Fallback to memory stream if file creation fails (e.g. permissions)
        std::ostringstream oss;
        pugi::xml_writer_stream writer(oss);
        doc.save(writer, "\t", pugi::format_default, pugi::encoding_utf8);
    }

    // 3. Cleanup
    // xml_document destructor handles memory cleanup automatically
    // Remove the dummy file if it was created
    std::remove(dummy_file);

    return 0;
}
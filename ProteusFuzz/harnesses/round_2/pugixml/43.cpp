// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <cstdlib>

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse the fuzz data as XML
    // If it fails, we still proceed with an empty document to test edge cases
    pugi::xml_parse_result result = doc->load_buffer(data, size);
    if (!result) {
        // Parsing failed, but we keep the document (which might be empty or partially parsed)
        // This allows testing error handling paths
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document from fuzz data
    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->root();
    if (!root) {
        // If no root, create one to allow further operations
        root = doc->append_child("root");
    }

    // Ensure we have some children to work with for insert_child_before
    // Create a few initial children if none exist
    if (!root.first_child()) {
        root.append_child("child1");
        root.append_child("child2");
    }

    // Get a reference node to insert before
    pugi::xml_node ref_node = root.child("child1");
    if (!ref_node) {
        // Fallback if child1 doesn't exist
        ref_node = root.first_child();
    }

    // 1. pugi::xml_node::insert_child_before
    // Use both overloads as required
    pugi::xml_node new_node1 = root.insert_child_before(pugi::string_view_t("new_node_1"), ref_node);
    pugi::xml_node new_node2 = root.insert_child_before("new_node_2", ref_node);

    // 2. pugi::xml_node::child
    pugi::xml_node found1 = root.child("new_node_1");

    // 3. pugi::xml_node::child
    pugi::xml_node found2 = root.child("new_node_2");

    // 4. pugi::xml_node::child
    pugi::xml_node found3 = root.child("child1");

    // 5. pugi::xml_node::insert_child_before
    pugi::xml_node new_node3 = root.insert_child_before(pugi::string_view_t("new_node_3"), found1);

    // 6. pugi::xml_node::insert_child_before
    pugi::xml_node new_node4 = root.insert_child_before("new_node_4", found2);

    // 7. pugi::xml_node::insert_child_before
    pugi::xml_node new_node5 = root.insert_child_before(pugi::string_view_t("new_node_5"), found3);

    // 8. pugi::xml_node::insert_child_before
    pugi::xml_node new_node6 = root.insert_child_before("new_node_6", new_node3);

    // 9. pugi::xml_node::insert_child_before
    pugi::xml_node new_node7 = root.insert_child_before(pugi::string_view_t("new_node_7"), new_node4);

    // 10. pugi::xml_node::insert_child_before
    pugi::xml_node new_node8 = root.insert_child_before("new_node_8", new_node5);

    // Cleanup
    delete doc;

    return 0;
}
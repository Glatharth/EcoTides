#include "io/FileLoader.hpp"
#include <iostream>
#include <filesystem>
#include <unordered_map>

FileLoader::FileLoader() {
    loaded = LoadXML("src/xml/cards.xml"); // temp until another XML file appears
    if (!loaded) {
        std::cerr << "Failed to load default XML file 'cards.xml'" << std::endl;
    }
}

FileLoader::FileLoader(const std::string& xmlFilePath) {
    loaded = LoadXML(xmlFilePath);
}

FileLoader::~FileLoader() = default;

bool FileLoader::LoadXML(const std::string& filePath) {
    xmlFilePath = filePath;
    result = doc.load_file(filePath.c_str());

    if (result) {
        std::cout << "XML [" << filePath << "] loaded successfully" << std::endl;
        return true;
    }
    std::cerr << "Error loading XML [" << filePath << "]: "
              << result.description() << std::endl;
    return false;
}

bool FileLoader::IsLoaded() const {
    return loaded;
}

pugi::xml_node FileLoader::FindCardNode(int cardId) const {
    if (!loaded) return {};

    // Search for a card with the specified ID
    for (pugi::xml_node card : doc.child("cards").children("card")) {
        if (card.attribute("id").as_int() == cardId) {
            return card;
        }
    }

    return {}; // Return empty node if not found
}

std::string FileLoader::GetCardPath(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("path").as_string();
    }
    return "";
}

EventType FileLoader::GetCardEventType(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        static const std::unordered_map<std::string, EventType> eventTypeMap = {
            {"CREATE", EventType::CREATE},
            {"MODIFY", EventType::MODIFY},
            {"DELETE", EventType::DELETE}
        };

        const std::string eventTypeStr = card.attribute("eventType").as_string();
        if (const auto it = eventTypeMap.find(eventTypeStr); it != eventTypeMap.end()) {
            return it->second;
        }
    }
    return EventType::UNKNOWN;
}

bool FileLoader::CardExists(int cardId) const {
    return FindCardNode(cardId).type() != pugi::node_null;
}

bool FileLoader::PathExists(const std::string& path) {
    return std::filesystem::exists(path);
}

void FileLoader::LoadAllCards() const {
    if (!loaded) {
        std::cout << "XML was not loaded correctly" << std::endl;
        return;
    }

    pugi::xml_node cards = doc.child("cards");
    std::cout << "Cards found in XML:" << std::endl;

    for (pugi::xml_node card : cards.children("card")) {
        int id = card.attribute("id").as_int();
        std::string path = card.attribute("path").as_string();
        std::string eventType = card.attribute("eventType").as_string();

        std::cout << "Card ID: " << id
                  << ", Path: " << path
                  << ", EventType: " << eventType
                  << ", Path exists: " << (PathExists(path) ? "Yes" : "No")
                  << std::endl;
    }
}
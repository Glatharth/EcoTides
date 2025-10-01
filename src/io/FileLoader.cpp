#include "FileLoader.hpp"
#include <iostream>
#include <filesystem>

/**
 * @class FileLoader
 * @brief Responsible for loading and parsing card data from an XML file.
 * 
 * Provides methods for retrieving card paths, event types, resources,
 * parent relations, and checking for card existence.
 */

/**
 * @brief Default constructor.

 */
FileLoader::FileLoader() {
    loaded = false;  
    xmlFilePath = "";
}

/**
 * @brief Constructs and loads the XML file during instantiation.
 * 
 * @param xmlFilePath Path to the XML file to load.
 */
FileLoader::FileLoader(const std::string& xmlFilePath) {
    loaded = LoadXML(xmlFilePath); 
}

/**
 * @brief Default destructor.
 */
FileLoader::~FileLoader() = default;

/**
 * @brief Loads and parses an XML file.
 * 
 * @param filePath Path to the XML file.
 * @return true if the file was loaded successfully, false otherwise.
 */
bool FileLoader::LoadXML(const std::string& filePath) {
    xmlFilePath = filePath;
    result = doc.load_file(filePath.c_str());

    if (result) {
        std::cout << "XML [" << filePath << "] carregado com sucesso" << std::endl;
        return true;
    }
    std::cerr << "Erro ao carregar XML [" << filePath << "]: "
              << result.description() << std::endl;
    return false;
}

/**
 * @brief Checks whether the XML file was successfully loaded.
 * 
 * @return true if the file was loaded, false otherwise.
 */
bool FileLoader::IsLoaded() const {
    return loaded;
}

/**
 * @brief Finds a card node in the XML file by its ID.
 * 
 * @param cardId The ID of the card to search for.
 * @return The XML node corresponding to the card, or an empty node if not found.
 */
pugi::xml_node FileLoader::FindCardNode(int cardId) const {
    if (!loaded) return {};

    for (pugi::xml_node card : doc.child("cards").children("card")) {
        if (card.attribute("id").as_int() == cardId) {
            return card;
        }
    }

    return {};
}

/**
 * @brief Gets the file path of a card based on its ID.
 * 
 * @param cardId The ID of the card.
 * @return The file path as a string, or an empty string if not found.
 */
std::string FileLoader::GetCardPath(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("path").as_string();
    }
    return "";
}

/**
 * @brief Gets the event type of a card from its XML attributes.
 * 
 * @param cardId The ID of the card.
 * @return The event type, or EventType::UNKNOWN if not recognized.
 */
EventType FileLoader::GetCardEventType(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        static const std::unordered_map<std::string, EventType> eventTypeMap = {
            {"CREATE", EventType::CREATE},
            {"MODIFY", EventType::MODIFY},
            {"DELETE", EventType::DELETE}
        };

        std::string eventTypeStr = card.attribute("eventType").as_string();
        if (auto it = eventTypeMap.find(eventTypeStr); it != eventTypeMap.end()) {
            return it->second;
        }
    }
    return EventType::UNKNOWN;
}

/**
 * @brief Gets the event type of a card from its XML attributes.
 * 
 * @param cardId The ID of the card.
 * @return The event type, or EventType::UNKNOWN if not recognized.
 */
std::map<ResourceType, int> FileLoader::GetCardResources(const int cardId) const {
    std::map<ResourceType, int> resources;
    const pugi::xml_node card = FindCardNode(cardId);
    if (!card) return resources;

    for (pugi::xml_node res : card.children("resource")) {
        std::string typeStr = res.attribute("type").as_string();
        int value = res.attribute("value").as_int();

        if (typeStr == "Economy") resources[ResourceType::ECONOMY] = value;
        else if (typeStr == "Population Awareness") resources[ResourceType::POPULATION_AWARENESS] = value;
        else if (typeStr == "Waste Collection") resources[ResourceType::WASTE_COLLECTION] = value;
        else if (typeStr == "Waste Accumulation") resources[ResourceType::WASTE_ACCUMULATION] = value;
    }
    return resources;
}

/**
 * @brief Retrieves the value of a specific resource type for a given card.
 * 
 * @param cardId The ID of the card.
 * @param resource The type of resource to retrieve.
 * @return The value of the specified resource, or 0 if not found.
 */
int FileLoader::GetCardResourceType(const int cardId, const ResourceType resource) const {
    const auto resources = GetCardResources(cardId);
    if (const auto it = resources.find(resource); it != resources.end()) {
        return it->second;
    }
    return 0;
}

/**
 * @brief Retrieves the quantity attribute of a card.
 * 
 * @param cardId The ID of the card.
 * @return The quantity value, defaulting to 1 if not specified or card not found.
 */
int FileLoader::GetCardQuantity(const int cardId) const {
    if (const pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("quantity").as_int(1);
    }
    return 1;
}

/**
 * @brief Retrieves the list of parent cards and their step values.
 * 
 * @param cardId The ID of the child card.
 * @return A vector of pairs containing parent card IDs and number of steps.
 */
std::vector<std::pair<int, int>> FileLoader::GetCardParents(int cardId) const {
    std::vector<std::pair<int, int>> parents;
    pugi::xml_node card = FindCardNode(cardId);
    if (!card) return parents;
    for (pugi::xml_node parent : card.child("parents").children("parent")) {
        int parentId = parent.attribute("id").as_int();
        int steps = parent.attribute("steps").as_int();
        parents.emplace_back(parentId, steps);
    }
    return parents;
}

/**
 * @brief Checks if a card with the specified ID exists in the XML.
 * 
 * @param cardId The ID of the card.
 * @return true if the card exists, false otherwise.
 */
bool FileLoader::CardExists(int cardId) const {
    return FindCardNode(cardId).type() != pugi::node_null;
}

/**
 * @brief Checks whether a given file path exists in the filesystem.
 * 
 * @param path The path to check.
 * @return true if the path exists, false otherwise.
 */
bool FileLoader::PathExists(const std::string& path) {
    return std::filesystem::exists(path);
}

/**
 * @brief Loads and displays all cards found in the XML file.
 * 
 * Prints each card's ID, path, event type, and whether the file path exists.
 * Useful for debugging or validation.
 */
void FileLoader::LoadAllCards() const {
    if (!loaded) {
        std::cout << "XML não carregado corretamente" << std::endl;
        return;
    }

    pugi::xml_node cards = doc.child("cards");
    std::cout << "Cartas encontradas no XML:" << std::endl;

    for (pugi::xml_node card : cards.children("card")) {
        int id = card.attribute("id").as_int();
        std::string path = card.attribute("path").as_string();
        std::string eventType = card.attribute("eventType").as_string();

        std::cout << "Card ID: " << id
                  << ", Path: " << path
                  << ", EventType: " << eventType
                  << ", Path existe: " << (PathExists(path) ? "Sim" : "Não")
                  << std::endl;
    }
}

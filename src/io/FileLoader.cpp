#include "FileLoader.hpp"
#include <iostream>
#include <filesystem>

// ------------------ Construtores ------------------
FileLoader::FileLoader() {
    loaded = false;  // não carrega XML automaticamente
    xmlFilePath = "";
}

FileLoader::FileLoader(const std::string& xmlFilePath) {
    loaded = LoadXML(xmlFilePath); // carrega apenas uma vez
}

// ------------------ Destrutor ------------------
FileLoader::~FileLoader() = default;

// ------------------ Carregar XML ------------------
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

// ------------------ Verifica carregamento ------------------
bool FileLoader::IsLoaded() const {
    return loaded;
}

// ------------------ Buscar node da carta ------------------
pugi::xml_node FileLoader::FindCardNode(int cardId) const {
    if (!loaded) return {};

    for (pugi::xml_node card : doc.child("cards").children("card")) {
        if (card.attribute("id").as_int() == cardId) {
            return card;
        }
    }

    return {}; // node vazio se não achar
}

// ------------------ Pegar caminho da carta ------------------
std::string FileLoader::GetCardPath(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("path").as_string();
    }
    return "";
}

// ------------------ Tipo de evento da carta ------------------
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

// ------------------ Verifica existência de carta ------------------
bool FileLoader::CardExists(int cardId) const {
    return FindCardNode(cardId).type() != pugi::node_null;
}

// ------------------ Verifica existência do arquivo ------------------
bool FileLoader::PathExists(const std::string& path) {
    return std::filesystem::exists(path);
}

// ------------------ Listar todas as cartas ------------------
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
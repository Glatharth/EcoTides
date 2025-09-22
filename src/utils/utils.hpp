#pragma once
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <random>
#include "enum.hpp"
#include "io/FileLoader.hpp"

static std::map<ResourceType, float> resourceTypeWeights = {
    {ResourceType::ECONOMY, 0.3f},
    {ResourceType::POPULATION_AWARENESS, 0.2f},
    {ResourceType::WASTE_COLLECTION, 0.1f},
    {ResourceType::WASTE_ACCUMULATION, 0.6f},
};

static std::map<EventType, float> eventTypeWeights = {
    {EventType::CREATE, 0.5f},
    {EventType::MODIFY, 1.0f},
    {EventType::DELETE, 2.0f},
    {EventType::UNKNOWN, 0.0f}
};

inline void generateSeed(std::string* seed, GameDifficulty difficulty) {
    FileLoader loader;
    std::vector<std::pair<int, float>> cardPunishments;

    for (int cardId = 1; loader.CardExists(cardId); ++cardId) {
        auto resources = loader.GetCardResources(cardId);
        float punishment = 0.0f;
        for (const auto& [type, value] : resources) {
            float weight = resourceTypeWeights[type];
            if (type == ResourceType::WASTE_ACCUMULATION) {
                punishment += value * weight;
            } else {
                punishment -= value * weight;
            }
        }
        EventType eventType = loader.GetCardEventType(cardId);
        punishment += eventTypeWeights[eventType];

        int quantity = loader.GetCardQuantity(cardId);
        for (int i = 0; i < quantity; ++i) {
            cardPunishments.emplace_back(cardId, punishment);
        }
    }

    std::vector<int> low, medium, high;
    for (const auto& [cardId, score] : cardPunishments) {
        if (score < 1.0f) low.push_back(cardId);
        else if (score < 2.0f) medium.push_back(cardId);
        else high.push_back(cardId);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(low.begin(), low.end(), g);
    std::shuffle(medium.begin(), medium.end(), g);
    std::shuffle(high.begin(), high.end(), g);

    std::vector<int> orderedIds;
    if (difficulty == GameDifficulty::NORMAL || difficulty == GameDifficulty::HARD) {
        orderedIds.insert(orderedIds.end(), low.begin(), low.end());
        orderedIds.insert(orderedIds.end(), medium.begin(), medium.end());
        orderedIds.insert(orderedIds.end(), high.begin(), high.end());
    } else {
        orderedIds.insert(orderedIds.end(), high.begin(), high.end());
        orderedIds.insert(orderedIds.end(), medium.begin(), medium.end());
        orderedIds.insert(orderedIds.end(), low.begin(), low.end());
    }

    // --- Parent scheduling ---
    std::map<int, int> counts;
    for (int id : orderedIds) counts[id]++;
    std::vector<int> uniqueIds;
    for (const auto& [id, _] : counts) uniqueIds.push_back(id);

    std::map<int, std::vector<int>> scheduledParents; // step -> list of parent ids
    std::vector<int> result;
    int lastId = -1;
    int total = orderedIds.size();
    int step = 0;
    for (int i = 0; i < total; ++i) {
        // Insert scheduled parents for this step
        if (scheduledParents.count(step)) {
            for (int pid : scheduledParents[step]) {
                result.push_back(pid);
            }
        }
        // Find next card
        int nextId = -1, maxCount = -1;
        for (int id : uniqueIds) {
            if (id != lastId && counts[id] > maxCount) {
                nextId = id;
                maxCount = counts[id];
            }
        }
        if (nextId == -1) {
            for (int id : uniqueIds) {
                if (counts[id] > 0) {
                    nextId = id;
                    break;
                }
            }
        }
        if (nextId == -1) break;
        result.push_back(nextId);
        counts[nextId]--;
        lastId = nextId;

        // Schedule parents
        auto parents = loader.GetCardParents(nextId);
        for (const auto& [parentId, steps] : parents) {
            scheduledParents[step + steps].push_back(parentId);
        }
        step++;
    }

    // Fallback if result size is less than total
    if (result.size() < total) result = orderedIds;

    std::ostringstream oss;
    for (int id : result) {
        oss << std::setw(2) << std::setfill('0') << std::hex << id;
    }
    *seed = oss.str();
}

inline std::string getSeedCardIdsAsString(const std::string& seed, size_t idWidth = 2) {
    std::vector<std::string> ids;
    for (size_t i = 0; i + idWidth <= seed.size(); i += idWidth) {
        std::string hexId = seed.substr(i, idWidth);
        int id = std::stoi(hexId, nullptr, 16);
        ids.push_back(std::to_string(id));
    }
    std::ostringstream oss;
    for (size_t i = 0; i < ids.size(); ++i) {
        oss << ids[i];
        if (i + 1 < ids.size()) oss << ",";
    }
    return oss.str();
}

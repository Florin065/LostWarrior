#pragma once
#include <glm/glm.hpp>

#include "Core/Coordinator.hpp"
#include "Core/ResourceManager.hpp"

std::vector<std::vector<int>> generateRandomMap(int minWidth, int minHeight, int maxWidth, int maxHeight, int maxObstaclesPercentage);

void generateEntitiesFromMap(Coordinator& gCoordinator,
                             ResourceManager& gResourceManager,
                             int dimension,
                             std::vector<Entity>& entities,
                             Entity player);


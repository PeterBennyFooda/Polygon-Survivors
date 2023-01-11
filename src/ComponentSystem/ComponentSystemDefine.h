#pragma once
#include <bitset>

namespace ComponentSystem
{
//May have circular dependency so foward declare
//these classes first.
class GameEntity;
struct Component;
class EntityManager;

using ComponentID = std::size_t;
using Group = std::size_t;
constexpr std::size_t MaxComponents { 32 };
constexpr std::size_t MaxGroups { 32 };

//Let us group entities.
using GroupBitset = std::bitset<MaxGroups>;

//Make sure an entity can not get same type
//of component multiple times.
using ComponentBitset = std::bitset<MaxComponents>;
using ComponentArray = std::array<Component*, MaxComponents>;
}
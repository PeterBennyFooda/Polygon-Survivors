#pragma once
#include "ComponentSystemDefine.h"
#include "EntityManager.h"
#include <cassert>
#include <memory>

/////////////////////////////////////////////////
///This file define GameEntity and base Component.
///
///Really want to seperate template implementations from the header
///but it will make codes messier so I leave it here.
/////////////////////////////////////////////////
namespace ComponentSystem
{
//hide this implementation detail.
namespace Internal
{
inline ComponentID GetUniqueComponentID() noexcept
{
	static ComponentID lastID { 0u };
	return lastID++;
}
}

//Everytime we instantiate 'GetComponentTypeID()',
//the static 'lastID' will ++ , making the ID unique.
template <typename T>
inline ComponentID GetComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	static ComponentID typeID { Internal::GetUniqueComponentID() };
	return typeID;
}

/*
 * Base struct of Componenet. A Componenet has its
 * data and logic implemented in it and can work
 * on its own, mostly decoupled from others.
 */
struct Component
{
	// Use this to store parent entity.
	GameEntity* Entity { nullptr };

	virtual ~Component()
	{}

	virtual void Init()
	{}
	virtual void Update(float)
	{}
	virtual void Render()
	{}
};

/*
 * GameEntity is the most basic element in the system.
 * It is a container of Component.
 */
class GameEntity
{
private:
	EntityManager& manager;

	bool alive { true };
	std::vector<std::unique_ptr<Component>> components;
	ComponentArray componentArray;
	ComponentBitset componentBitset;

	GroupBitset groupBitset;

public:
	GameEntity(EntityManager& mManager) :
		manager(mManager)
	{}

	bool IsAlive() const;
	void Destroy();

	void Update(float mFT);
	void Render();

	bool HasGroup(Group group) const noexcept;
	void AddGroup(Group group) noexcept;
	void DeleteGroup(Group group) noexcept;

	template <typename T>
	bool HasComponent() const;

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs);

	template <typename T>
	T& GetComponent() const;
};

template <typename T>
bool GameEntity::HasComponent() const
{
	return componentBitset[GetComponentTypeID<T>()];
}

template <typename T>
T& GameEntity::GetComponent() const
{
	//Check if the Component T exists in the entity.
	//If not, we can not get anything, error.
	assert(HasComponent<T>());
	auto ptr(componentArray[GetComponentTypeID<T>()]);
	return *reinterpret_cast<T*>(ptr);
}

template <typename T, typename... TArgs>
T& GameEntity::AddComponent(TArgs&&... mArgs)
{
	//T must be a Component
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	//Check if the Component T exists.
	//If so, can't add because only one is allowed, error.
	assert(!HasComponent<T>());

	//Allocate component of type 'T' on the HEAP
	//and forward the arguments to its constructor
	T* c(new T(std::forward<TArgs>(mArgs)...));

	//Set the parent of the component to this instance
	c->Entity = this;

	//Wrap raw pointer to smart pointer
	std::unique_ptr<Component> uPtr { c };

	//Add the smart pointer to the 'components' container
	//Aso, smart pointer is not copyable so we must move it
	components.emplace_back(std::move(uPtr));

	//Instantiate an id and add the pointer to array.
	componentArray[GetComponentTypeID<T>()] = c;

	//The Component T so mark it in the bitset.
	//Note that static id is created so we always get
	//the same id when 'GetComponentTypeID<T>()'
	componentBitset[GetComponentTypeID<T>()] = true;

	//All set, let us Init the Component T.
	c->Init();

	return *c;
}

}

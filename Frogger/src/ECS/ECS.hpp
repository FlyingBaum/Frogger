#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

// Allow a maximum of 32 components.
constexpr std::size_t MAX_COMPONENTS = 32;

class Component;
class Entity;

// New type aliases.
using ComponentID = std::size_t;
using ComponentBitSet = std::bitset<MAX_COMPONENTS>; // 32 bits, each false or true to signal absence or existence.
using ComponentArray = std::array<Component*, MAX_COMPONENTS>;

// "inline" means the function call will be replaced with its inner code for performance.
inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

// Generic function to get component IDs.
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	// Assert that T is derived from or equal to Component class.
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component.");

	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {
private:
	bool isActive = true;
	std::vector<std::unique_ptr<Component>> components; // unique pointers, so memory is freed without "delete" when components leave scope.

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:
	void update() {
		for (auto& c : components) c->update();
	}
	void draw() {
		for (auto& c : components) c->draw();
	}
	bool getIsActive() const { return isActive; }
	void destroy() { isActive = false; }

	// Check if component of type T exists in entity.
	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	// Returns a reference to the added component after creating it. TArgs is the variadic template parameter type and mArgs is the actual arguments of that type.
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) { // TArgs&& to allow any forwarding types, including rvalue and lvalue.
		// New pointer c that is constructed with the passed arguments.
		T* c(new T(std::forward<TArgs>(mArgs)...)); // std::forward to forward rvalues and lvalues as they are.
		c->entity = this; 							// Set component entity.
		std::unique_ptr<Component> uPtr{ c };		// Create a unique pointer and transfer ownership over the component to it.
		components.emplace_back(std::move(uPtr));	// Transfer ownership to the components vector.

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c; // Dereference operator * gives us the actual component.
	}

	template <typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr); // Dereference operator * gives us the actual component.
	}
};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void update() {
		for (auto& e : entities) e->update();
	}
	void draw() {
		for (auto& e : entities) e->draw();
	}

	void refresh() {
		// Create a lambda for the condition when an entity should be considered "removed".
		auto isNotActive = [](const std::unique_ptr<Entity>& entity) {
			return !entity->getIsActive();
		};

		// Remove inactive entities by moving them to the end.
		auto newEnd = std::remove_if(std::begin(entities), std::end(entities), isNotActive);

		// Erase the "removed" entities from the vector by erasing between newEnd and the actual end.
		entities.erase(newEnd, std::end(entities));
	}

	// Create new entity, store it in a unique pointer and move ownership to the entities vector.
	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};
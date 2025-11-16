#pragma once

#include <cstdint>

namespace sw::ecs
{
	class Entity;
	class World;

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		// Основной метод обработки мира
		virtual void ProcessWorld(World& world) = 0;
	};

	class ServiceBase
	{
	public:
		explicit ServiceBase(const World& world) : _world(world) {}
		virtual ~ServiceBase() = default;

		// Проверка готовности сервиса
		virtual bool IsReady() const = 0;

	protected:
		const World& _world;
	};
}




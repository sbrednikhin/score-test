#pragma once

namespace sw::ecs
{
	class Entity;
	class World;

	// Фазы обновления системы
	enum class UpdatePhase
	{
		PreUpdate,  // Перед основным обновлением - для добавления новых сущностей
		Update,     // Основное обновление
		PostUpdate  // После основного обновления - для очистки перед уничтожением
	};

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		// Основной метод обработки мира (для обратной совместимости)
		virtual void ProcessWorld(World& world) { ProcessWorldPhase(world, UpdatePhase::Update); }

		// Метод обработки с указанием фазы
		virtual void ProcessWorldPhase(World& world, UpdatePhase phase) = 0;
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
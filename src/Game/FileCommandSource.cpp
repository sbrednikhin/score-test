#include "FileCommandSource.hpp"
#include "GameLogic.hpp"
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/March.hpp>
#include <iostream>

namespace sw
{
	FileCommandSource::FileCommandSource(const std::string& filename)
		: _filename(filename)
		, _isInitialized(false)
		, _isParsed(false)
	{
	}

	void FileCommandSource::Initialize()
	{
		if (_isInitialized) return;

		_file.open(_filename);
		if (_file.is_open()) {
			// Настраиваем парсер с обработчиками команд
			_parser.add<io::CreateMap>([](io::CreateMap&& command) {
				if (GameLogic::GetCommandManager().IsInitialized()) {
					GameLogic::GetCommandManager().AddCommand(
						std::make_shared<io::CreateMap>(std::move(command)));
				}
			})
			.add<io::SpawnSwordsman>([](io::SpawnSwordsman&& command) {
				if (GameLogic::GetCommandManager().IsInitialized()) {
					GameLogic::GetCommandManager().AddCommand(
						std::make_shared<io::SpawnSwordsman>(std::move(command)));
				}
			})
			.add<io::SpawnHunter>([](io::SpawnHunter&& command) {
				if (GameLogic::GetCommandManager().IsInitialized()) {
					GameLogic::GetCommandManager().AddCommand(
						std::make_shared<io::SpawnHunter>(std::move(command)));
				}
			})
			.add<io::March>([](io::March&& command) {
				if (GameLogic::GetCommandManager().IsInitialized()) {
					GameLogic::GetCommandManager().AddCommand(
						std::make_shared<io::March>(std::move(command)));
				}
			});

			_isInitialized = true;
		}
	}

	FileCommandSource::~FileCommandSource()
	{
		if (_file.is_open()) {
			_file.close();
		}
	}

	void FileCommandSource::ProcessCommands()
	{
		if (_isInitialized && !_isParsed && _file.is_open()) {
			_parser.parse(_file);
			_isParsed = true;
		}
		// Для файлового источника парсинг выполняется только однажды
	}

	bool FileCommandSource::IsAvailable() const
	{
		return _isInitialized && _file.is_open();
	}
}
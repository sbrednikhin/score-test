#pragma once

#include "ICommandSource.hpp"
#include <IO/System/CommandParser.hpp>
#include <fstream>
#include <string>
#include <memory>

namespace sw
{
	class FileCommandSource : public ICommandSource, public std::enable_shared_from_this<FileCommandSource>
	{
	public:
	FileCommandSource(const std::string& filename);
	~FileCommandSource() override;

	void Initialize();
	void ProcessCommands() override;
	bool IsAvailable() const override;

	private:
		std::string _filename;
		std::ifstream _file;
		io::CommandParser _parser;
		bool _isInitialized;
		bool _isParsed;
	};
}
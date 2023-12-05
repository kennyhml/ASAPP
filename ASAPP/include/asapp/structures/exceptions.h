#pragma once
#include "basestructure.h"
#include <exception>
#include <format>
#include <typeinfo>

namespace asa::structures::exceptions
{
	class StructureError : public std::exception
	{
		const BaseStructure* structure;
		std::string info;

	public:
		StructureError(const BaseStructure* structure);
		StructureError(const BaseStructure* structure, std::string info);

		const char* what() const noexcept override;
	};

	class StructureNotOpenedError : public StructureError
	{
	public:
		StructureNotOpenedError(const BaseStructure* structure);
	};

	class StructureNotClosedError : public StructureError
	{
	public:
		StructureNotClosedError(const BaseStructure* structure);
	};
}
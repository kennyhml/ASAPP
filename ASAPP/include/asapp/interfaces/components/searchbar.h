#pragma once
#include "iinterfacecomponent.h"
#include <string>

namespace asa::interfaces::components
{
	struct SearchBar : IInterfaceComponent
	{
		using IInterfaceComponent::IInterfaceComponent;

		virtual void Press() const;
		virtual void SearchFor(std::string term);
		void DeleteSearch();

		void SetTextCleared() { this->isTextEntered = false; }
		bool GetLastSearchedTerm(std::string& termOut) const { return 1; };
		bool IsSearching() const { return this->isSearching; }
		bool TextIsEntered() const { return this->isTextEntered; }

	protected:
		std::string lastSearchedTerm = "";
		bool isSearching{ false };
		bool isTextEntered{ false };
	};
}
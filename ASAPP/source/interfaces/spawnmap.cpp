#include "asapp/interfaces/spawnmap.h"
#include "../core/wrappers.h"
#include "asapp/game/resources.h"

const bool asa::interfaces::SpawnMap::IsOpen() const
{
	return window::MatchTemplate(
		this->regionsButton.area, resources::interfaces::regions);
}

void asa::interfaces::SpawnMap::SpawnAt(int regionIndex)
{
	this->regionsButton.Press();
	SleepFor(std::chrono::milliseconds(200));

	this->results[regionIndex].Press();
	while (!this->CanConfirmTarget()) {
	}
	this->confirmButton.Press();
}

void asa::interfaces::SpawnMap::SpawnAt(const std::string& bed)
{
	this->bedsButton.Press();
	SleepFor(std::chrono::milliseconds(200));

	this->searchbar.SearchFor(bed);
	SleepFor(std::chrono::milliseconds(400));
	this->SelectResult();

	while (!this->CanConfirmTarget()) {
	}
	this->confirmButton.Press();
	this->searchbar.SetTextCleared();
}

void asa::interfaces::SpawnMap::GoTo(const std::string& dest)
{
	return SpawnAt(dest);
}
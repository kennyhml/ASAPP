#include "spawnmap.h"
#include "../game/resources.h"

bool asa::interfaces::SpawnMap::IsOpen()
{
	return window::MatchTemplate(this->regionsButton.area, resources::regions);
}

void asa::interfaces::SpawnMap::SpawnAt(int regionIndex)
{
	this->regionsButton.Press();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	this->results[regionIndex].Press();
	while (!this->CanConfirmTarget()) {
	}
	this->confirmButton.Press();
}

void asa::interfaces::SpawnMap::SpawnAt(const std::string& bed)
{
	this->bedsButton.Press();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));


	this->searchbar.SearchFor(bed);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
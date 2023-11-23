#pragma once
#include <Windows.h>
#include <any>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

namespace asa::settings
{
	bool Load();
	bool OpenFile(
		bool verbose, std::filesystem::path path, std::ifstream& fileOut);

	inline namespace actionMappings
	{
		struct ActionMapping;
		inline std::unordered_map<std::string, ActionMapping*> inputMap{};

		struct ActionMapping
		{
			explicit ActionMapping(std::string name) : name(name)
			{
				inputMap[name] = this;
			};

			std::string name;
			bool shift{ false }, ctrl{ false }, alt{ false }, cmd{ false };
			std::string key;
		};
		std::ostream& operator<<(std::ostream& os, const ActionMapping& m);

		const auto inputsRelPath = std::filesystem::path(
			R"(ShooterGame\Saved\Config\Windows\Input.ini)");

		inline ActionMapping accessInventory("AccessInventory");
		inline ActionMapping crouch("Crouch");
		inline ActionMapping prone("Prone");
		inline ActionMapping orbitCam("OrbitCam");
		inline ActionMapping pauseMenu("PauseMenu");
		inline ActionMapping ping("Ping");
		inline ActionMapping poop("Poop");
		inline ActionMapping reload("Reload");
		inline ActionMapping showTribeManager("ShowTribeManager");
		inline ActionMapping toggleMap("ToggleMap");
		inline ActionMapping transferItem("TransferItem");
		inline ActionMapping use("Use");
		inline ActionMapping useItem1("UseItem1");
		inline ActionMapping useItem2("UseItem2");
		inline ActionMapping useItem3("UseItem3");
		inline ActionMapping useItem4("UseItem4");
		inline ActionMapping useItem5("UseItem5");
		inline ActionMapping useItem6("UseItem6");
		inline ActionMapping useItem7("UseItem7");
		inline ActionMapping useItem8("UseItem8");
		inline ActionMapping useItem9("UseItem9");
		inline ActionMapping useItem10("UseItem10");
		inline ActionMapping showMyInventory("ShowMyInventory");
		inline ActionMapping dropItem("DropItem");
		inline ActionMapping fire("Fire");
		inline ActionMapping targeting("Targeting");
		inline ActionMapping showExtendedInfo("ShowExtendedInfo");
		inline ActionMapping toggleTooltip("ToggleTooltip");
		inline ActionMapping jump("Jump");
		inline ActionMapping run("Run");

		bool LoadActionMappings(bool verbose = true);
	}

	inline namespace gameUserSettings
	{
		inline std::unordered_map<std::string, std::any> settingValueMap{};

		template <typename Type> struct UserSetting
		{
			UserSetting(std::string name) : name(name)
			{
				settingValueMap[name] = Type(0);
			};

			std::string name;

			Type get()
			{
				return std::any_cast<Type>(settingValueMap[this->name]);
			}
		};

		const auto userSettingsRelPath = std::filesystem::path(
			R"(ShooterGame\Saved\Config\Windows\GameUserSettings.ini)");

		bool LoadGameUserSettings(bool verbose = true);

		inline UserSetting<float> uiScale("UIScaling");
		inline UserSetting<float> hotbarScale("UIQuickbarScaling");
		inline UserSetting<float> cameraShake("CameraShakeScale");
		inline UserSetting<float> fov("FOVMultiplier");
		inline UserSetting<float> sensX("LookLeftRightSensitivity");
		inline UserSetting<float> sensY("LookUpDownSensitivity");

		inline UserSetting<bool> firstPersonRiding("bFirstPersonRiding");
		inline UserSetting<bool> thirdPerson("bThirdPersonPlayer");
		inline UserSetting<bool> showNotifs("bShowStatusNotificationMessages");
	}
}

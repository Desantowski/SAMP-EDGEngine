#include "TestGMPCH.hpp"

// Advanced GDK library
#include <SAMP-EDGEngine/Everything.hpp>

#include <filesystem>

void cmd_TeleportToLocation(agdk::CommandInput input_)
{
	auto params = input_.splitParams();
	if (params.size() >= 3)
	{
		agdk::math::Vector3f location;
		location.x = agdk::text::ascii::convert<float>(params[0]).value();
		location.y = agdk::text::ascii::convert<float>(params[1]).value();
		location.z = agdk::text::ascii::convert<float>(params[2]).value();
		input_.target.setLocation(location);
		if (params.size() >= 5)
		{
			std::int32_t world		= agdk::text::ascii::convert<std::int32_t>(params[3]).value();
			std::int32_t interior	= agdk::text::ascii::convert<std::int32_t>(params[4]).value();
			input_.target.setWorld(world);
			input_.target.setInterior(interior);
		}
	}
}

void cmd_SpawnVehicle(agdk::CommandInput input_)
{
	auto params = input_.splitParams();
	if (params.size() == 1)
	{
		auto vehPtr = GameMode->Map.beginConstruction<agdk::Vehicle>();
		vehPtr->setModel(agdk::Vehicle::findModelBestMatch(params[0]));
		vehPtr->setLocation(input_.target.getLocation());
		vehPtr->setFacingAngle(input_.target.getFacingAngle());
		vehPtr->setInterior(input_.target.getInterior());
		vehPtr->setWorld(input_.target.getWorld());
		auto &veh = GameMode->Map.finalizeConstruction(vehPtr);
		input_.target.putInVehicle(veh, 0);
	}
}

void cmd_ObjectCount(agdk::CommandInput input_)
{
	namespace txt = agdk::text::ascii;
	auto tracker = static_cast<agdk::default_streamer::PlayerWrapper*>(input_.target.getPlacementTracker());
	GameMode->chat->messagePlayer(input_.target, txt::compose("You have currently ", tracker->spawnedObjects.size(), " objects shown!"));
}

class MyGameMode
	: public agdk::IGameMode
{
public:
	MyGameMode() {	
		this->addPlayerClass(0, { 280, 180, 1010 }, 0, { agdk::Weapon(agdk::Weapon::Deagle, 999) });
	}
	
	virtual void setup() override
	{
		agdk::IGameMode::setup();

		chat = std::make_unique<agdk::DefaultChat>();

		this->setupCommands();
		this->loadObjects();
	}

	void loadObjects()
	{
		namespace fs = std::filesystem;

		for (auto& file : fs::directory_iterator("data/objects"))
		{
			if (fs::is_regular_file(file))
			{
				std::clog << "[i]: Reading file: " << file.path() << std::endl;
				std::ifstream stream{ file };
				auto scene = GameMode->Map.beginConstruction<agdk::Scene>();

				scene->loadFromStream(stream, agdk::DefaultSceneLoader{});

				GameMode->Map.finalizeConstruction(scene);
			}
		}

	}

	virtual void setupEvents() override {
		Server->Events.PlayerSpawns += { *this, &MyGameMode::whenPlayerSpawns };
	}

	void whenPlayerSpawns(agdk::Player & player)
	{
		player.setLocation({ 1958.33f, 1343.12f, 15.36f });
		player.setWorld(0);
		player.addWeapon(agdk::Weapon(agdk::Weapon::Deagle, 999));
	}

	void setupCommands()
	{
		commands.construct<agdk::ProcedureCommand>( agdk::CmdInvocations{ "tpc" }, cmd_TeleportToLocation );
		commands.construct<agdk::ProcedureCommand>( agdk::CmdInvocations{ "v" }, cmd_SpawnVehicle );
		commands.construct<agdk::ProcedureCommand>( agdk::CmdInvocations{ "objc" }, cmd_ObjectCount);
	}

	virtual agdk::SharedPtr<agdk::Player> newPlayerInstance(std::size_t const playerIndex_) const override
	{
		return agdk::IGameMode::newPlayerInstance(playerIndex_);
	}
};

int main()
{
	Server->setup(std::make_unique<MyGameMode>());
}
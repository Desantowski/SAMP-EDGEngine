// File description:
// Implements basic game mode interface.
#pragma once

// Precompiled header:
#include SAMP_EDGENGINE_PCH

// AGDK includes:

#include <SAMP-EDGEngine/Server/Chat.hpp>
#include <SAMP-EDGEngine/Server/Weapon.hpp>
#include <SAMP-EDGEngine/Server/PlayerPool.hpp>
#include <SAMP-EDGEngine/Server/CommandManager.hpp>
#include <SAMP-EDGEngine/World/Map.hpp>

#include <SAMP-EDGEngine/Core/BasicInterfaces/Streamer.hpp>
#include <SAMP-EDGEngine/Core/Pointers.hpp>
#include <SAMP-EDGEngine/Core/Events.hpp>


namespace agdk
{	

/// <summary>
/// Provides abstract interface for every gamemode.
/// </summary>
class IGameMode
	: public IEventReceiver
{
public:		
	/// <summary>
	/// Initializes a new instance of the <see cref="IGameMode"/> class.
	/// </summary>
	IGameMode();
		
	/// <summary>
	/// Finalizes an instance of the <see cref="IGameMode"/> class.
	/// </summary>
	virtual ~IGameMode();
				
	/// <summary>
	/// Creates new player instance.
	/// </summary>
	/// <param name="playerIndex_">Index of the player.</param>
	/// <returns>
	/// New player instance with specified index.
	/// </returns>
	virtual SharedPtr<Player> newPlayerInstance(std::size_t const playerIndex_) const;
		
	/// <summary>
	/// Adds the player class.
	/// </summary>
	/// <param name="modelIndex_">Index of the model.</param>
	/// <param name="location_">The location.</param>
	/// <param name="facingAngle_">The facing angle.</param>
	/// <param name="weapons_">The weapons.</param>
	void addPlayerClass(std::size_t modelIndex_, math::Vector3f const location_, float const facingAngle_, std::array<Weapon, 3> weapons_ = {});

	friend class ServerClass;
protected:
		
	/// <summary>
	/// Setups the game mode.
	/// </summary>
	virtual void setup();

	/// <summary>
	/// Setups the streamer. Override it if you want to use custom streamer.
	/// </summary>
	virtual void setupStreamer();

	/// <summary>
	/// Setups event receiving functions. Very important function!
	/// </summary>
	/// <remarks>
	/// <para>It is executed directly after GameMode creation, USE IT to setup event receivers, that receive events from server!</para>
	/// </remarks>
	virtual void setupEvents() {}
public:
	PlayerPool					Players;
	MapClass					Map;
	CommandManager				commands;

	UniquePtr<IStreamer>	Streamer;
	UniquePtr<IChat>		chat;
};

}
extern agdk::UniquePtr<agdk::IGameMode> GameMode;






#pragma once

// The precompiled header:
#include "../../../stdafx.h"

namespace agdk
{
	class Player;

	/// <summary>
	/// Class (interface) that manages game mode chat.
	/// </summary>
	class IChat
	{
	public:
		/*
		Default constructor.
		*/		
		/// <summary>
		/// Initializes a new instance of the <see cref="IChat"/> class.
		/// </summary>
		IChat() {
		}
		
		/// <summary>
		/// Sends simple message to player.
		/// </summary>
		/// <param name="player_">The player.</param>
		/// <param name="text_">The text message.</param>
		void messagePlayer(Player & player_, std::string const & text_);

		/// <summary>
		/// Sends simple message to all players.
		/// </summary>
		/// <param name="text_">The text message.</param>
		void messageAll(std::string const & text_);
		
		/// <summary>
		/// Sends multilingual message to player.
		/// </summary>
		/// <param name="player_">The player.</param>
		/// <param name="text_">The text message (each record in std::vector represents one language).</param>
		void messagePlayerML(Player & player_, std::vector<std::string> text_);

		/// <summary>
		/// Sends multilingual message to all players.
		/// </summary>
		/// <param name="text_">The text message (each record in std::vector represents one language).</param>
		void messageAllML(std::vector<std::string> text_);

		// Friendship with server logic is neccessary because it manages chat.
		friend class IGameMode;

	protected:
		/// <summary>
		/// Splits message into pieces when it finds new line character.
		/// </summary>
		/// <param name="text_">The text message.</param>
		/// <returns>Splitted message.</returns>
		std::vector<std::string> splitMessage(std::string const & text_);
	
		/// <summary>
		/// Callback called when some player sends text message.
		/// Since this method is pure virtual, derieved classes MUST implement it.
		/// </summary>
		/// <param name="player_">The player.</param>
		/// <param name="text_">The text.</param>
		virtual void onPlayerText(Player & player_, std::string_view const text_) = 0;
	};
	
	/// <summary>
	/// Class implementing default and very primitive chat behaviour.
	/// </summary>
	/// <seealso cref="IChat" />
	class DefaultChat
		: public IChat
	{
	public:
		// Nothing special.
	protected:
		/// <summary>
		/// Callback called when some player sends text message.
		/// </summary>
		/// <param name="player_">The player.</param>
		/// <param name="text_">The text.</param>
		virtual void onPlayerText(Player & player_, std::string_view const text_) override;
	};
}
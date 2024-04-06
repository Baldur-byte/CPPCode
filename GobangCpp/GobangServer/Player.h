#ifndef PLAYER_H
#define PLAYER_H
class ClientSocket;
class Room;

enum class ChessType {
	None,
	Black,
	White,
};

class Player
{
public:
	Player();
	Player(ClientSocket* clientSocket);
	~Player();
#pragma region Server->Client
	void UpdateChessBoard();
	void GameStart(ChessType type);
	void ChangePlayer();
	void GameFinish(int win);
#pragma endregion

#pragma region Client->Server
	void JoinRoom(Room* room);
	void ExitRoom();
	void StartGame();
	void QuitGame();
#pragma endregion

	void SetChessType(ChessType type);
	Room* GetRoom();

	bool isInRoom;

private:
	ClientSocket* clientSocket;
	ChessType chessType;
	Room* room;
};
#endif
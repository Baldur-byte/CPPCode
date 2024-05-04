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
	void UpdateChessBoard(int x, int y);
	void GameStart(ChessType type);
	void ChangePlayer();
	void GameFinish(int win);
	void RestartConfirm();
#pragma endregion

#pragma region Client->Server
	void JoinRoom(Room* room);
	void ExitRoom();
	void ReadyToStartGame();
	void QuitGame();
#pragma endregion

	void SetChessType(ChessType type);
	Room* GetRoom();

	bool isInRoom;
	bool isReady;
	bool ReadyToRestart;
private:
	ClientSocket* clientSocket;
	ChessType chessType;
	Room* room;
};
#endif
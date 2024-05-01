#pragma once
#include "Player.h"
#include "Robot.h"
#include "ChessBoard.h"

class GameStart;

class Game {
public:
    Game();
    ~Game();
    void Init(GameStart* gameStart);
    void JoinRoom(int roomId);
    int RoomID();
    void ReadToStart();
    void ClickChessBoard(int x, int y);
    void UpdateChessBoard();
    ChessBoardCell** GetChessBoardData();
    //void SetGameResult(int result);

    void GoToLobby();
    void GoToGame();
    void Quit();

    void GetRoomList();
    void ReceiveRoomList(int(*roomList)[2]);
    int** RoomList();

    void Restart();
    void AddNetworkRobot();
    void QuitToLobby();
    void QuitToStart();

private:
    GameStart* gameStart;
    Player player;
    Robot robot;
    ChessBoard chessBoard;
    int roomList[12][2];
    //ChessBoardCell chessBoard[15][15];
};

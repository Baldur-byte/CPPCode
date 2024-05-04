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
    void ReadyToStart();
    void ResetChessBoard();
    void ClickChessBoard(int x, int y);
    void UpdateChessBoard();
    ChessBoardCell** GetChessBoardData();

    void ShowResult(int result);
    void ShowMessage(const char* text);
    //void SetGameResult(int result);

    void GoToLobby();
    void GoToGame();
    void Quit();

    void GetRoomList();
    void ReceiveRoomList(short(*roomList)[2]);
    int** RoomList();

    void RestartRequest();
    void AddNetworkRobot();
    void QuitToLobby();
    void QuitToStart();

private:
    GameStart* gameStart;
    Player player;
    Robot robot;
    ChessBoard chessBoard;
    int roomList[12][2];
    bool isFinish;
    bool haveRobot;
    //ChessBoardCell chessBoard[15][15];
};

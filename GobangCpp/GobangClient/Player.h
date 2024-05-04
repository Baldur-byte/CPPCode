#pragma once
#include "ClientSocket.h"

class ChessBoard;

enum class ChessType {
    None,
    Black,
    White,
};

class Game;

class Player {
public:
    Player(Game* game);
    ~Player();
    void Start();
    bool IsInRoom();
    int RoomID();
    Game* CurGame();
    ChessType GetChessType();
    void SetRoomList(short(*roomList)[2]);
    void GetRoomList();
    void JoinRoom(int roomId, ChessBoard* chessBoard);
    void ExitRoom();
    virtual void ReadyToStartGame();
    virtual void SetChessType(ChessType type, ChessType turn);
    virtual void GameStart();
    virtual void PlaceChess(int x, int y);
    virtual void UpdateChessBoard(short(*chessBoard)[15]);
    virtual void Change(ChessType turn);
    virtual void GameFinish(int win);
    virtual void RestartGameRequest();
    virtual void RestartConfirm();

    bool isTurn;

    void RegistOperatorResultEvent(CSMessageType type, void (*action)(Game*));
    void UnregistOperatorResultEvent(CSMessageType type);

protected:
    int roomId;
    bool isInRoom;
    ChessType chessType;
    ClientSocket clientSocket;
private:
    ChessBoard* chessBoard;
    Game* curGame;
};

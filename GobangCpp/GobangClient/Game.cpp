#include "Game.h"
#include "GameStart.h"

Game::Game() : player(this), robot(this){
    gameStart = nullptr;
    for (int i = 0; i < 12; i++) {
        roomList[i][0] = -1; //房间id
        roomList[i][1] = -1; //房间人数
    }
    player.Start();
    isFinish = true;
    haveRobot = false;
}

Game::~Game() {
    delete gameStart;
}

void Game::Init(GameStart* gameStart) {
    this->gameStart = gameStart;
    isFinish = false;
}

void Game::JoinRoom(int roomId) {
    if (player.RoomID() != 0 && roomId != -1 && player.RoomID() == roomId) {
        GoToGame();
    }
    else {
        if (player.IsInRoom()) {
            player.ExitRoom();
            robot.ExitRoom();
            haveRobot = false;
        }
        player.RegistOperatorResultEvent(CSMessageType::JoinRoom, [](Game* curGame) {
            curGame->GoToGame();
            });
        player.JoinRoom(roomId, &chessBoard);
    }
}

int Game::RoomID() {
    return player.RoomID();
}

void Game::ReadyToStart() {
    gameStart->ShowMessage("准备开始...");
    player.ReadyToStartGame();
}

void Game::ResetChessBoard() {
    gameStart->DrawBoardFrame();
    gameStart->DrawChessBoard();
}

void Game::ClickChessBoard(int x, int y) {
    if (!player.isTurn)
    {
        return;
    }
    if (!chessBoard.CheckPosIsEmpty(x, y)) {
        return;
    }
    player.PlaceChess(x, y);
}

void Game::UpdateChessBoard() {
    gameStart->DrawChess();
}

ChessBoardCell** Game::GetChessBoardData() {
    return chessBoard.GetChessBoardData();
}

void Game::ShowResult(int result) {
    isFinish = true;
    if (result == 1) {
        gameStart->ShowMessage("你赢了");
    }
    else if (result == 0) {
        gameStart->ShowMessage("平局");
    }
    else if (result == -1) {
        gameStart->ShowMessage("你输了");
    }
    else {
        gameStart->ShowMessage("故障");
    }
}

void Game::ShowMessage(const char* text) {
    gameStart->ShowMessage(text);
}

void Game::GoToLobby() {
    player.ExitRoom();
    robot.ExitRoom();
    haveRobot = false;
    gameStart->ChangePage(1);
}

void Game::GoToGame() {
    gameStart->ChangePage(2);
}

void Game::Quit() {
    player.ExitRoom();
    robot.ExitRoom();
    haveRobot = false;
    gameStart->Quit();
}

void Game::GetRoomList() {
    player.GetRoomList();
}

void Game::ReceiveRoomList(short(*inRoomList)[2]) {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 2; j++) {
            this->roomList[i][j] = inRoomList[i][j];
        }
    }
}

int** Game::RoomList() {
    int** result = new int* [12];
    for (int i = 0; i < 12; i++) {
        result[i] = this->roomList[i];
    }
    return result;
}

void Game::RestartRequest() {
    gameStart->ShowMessage("等待对方确认...");
    player.RestartGameRequest();
}

void Game::AddNetworkRobot() {
    if (haveRobot) {
        return;
    }
    haveRobot = true;
    robot.Start();
    robot.JoinRoom();
    gameStart->ShowMessage("添加机器人");
    robot.ReadyToStartGame();
}

void Game::QuitToLobby() {
    player.ExitRoom();
    robot.ExitRoom();
    haveRobot = false;
    gameStart->ChangePage(1);
}

void Game::QuitToStart() {
    player.ExitRoom();
    robot.ExitRoom();
    haveRobot = false;
    gameStart->ChangePage(0);
}

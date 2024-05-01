#include "Game.h"
#include "GameStart.h"

Game::Game() : player(this), robot(this){
    gameStart = nullptr;
    for (int i = 0; i < 12; i++) {
        roomList[i][0] = -1; //房间id
        roomList[i][1] = -1; //房间人数
    }
    player.Start();
}

Game::~Game() {
    delete gameStart;
}

void Game::Init(GameStart* gameStart) {
    this->gameStart = gameStart;
}

void Game::JoinRoom(int roomId) {
    player.RegistOperatorResultEvent(CSMessageType::JoinRoom, [](Game* curGame) {
        curGame->GoToGame();
        });
    player.JoinRoom(roomId, &chessBoard);
}

int Game::RoomID() {
    return player.RoomID();
}

void Game::ReadToStart() {
    player.ReadyToStartGame();
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

//void Game::SetChessBoardData(short(*chessBoard)[15]) {
//    for (int i = 0; i < 15; i++) {
//        for (int j = 0; j < 15; j++) {
//            this->chessBoard[i][j].cellType = static_cast<CellType>(chessBoard[i][j]);
//        }
//    }
//    gameStart->DrawChess();
//}
//
//void Game::SetGameResult(int result) {
//    if (result == 1) {
//        gameStart->ShowMessage("你赢了");
//    }
//    else if (result == 0) {
//        gameStart->ShowMessage("平局");
//    }
//    else if (result == -1) {
//        gameStart->ShowMessage("你输了");
//    }
//    else {
//        gameStart->ShowMessage("故障");
//    }
//}

void Game::GoToLobby() {
    gameStart->ChangePage(1);
}

void Game::GoToGame() {
    gameStart->ChangePage(2);
}

void Game::Quit() {

}

void Game::GetRoomList() {
    player.GetRoomList();
}

void Game::ReceiveRoomList(int(*inRoomList)[2]) {
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

void Game::Restart() {

}

void Game::AddNetworkRobot() {
    robot.Start();
    robot.JoinRoom();
    gameStart->ShowMessage("添加机器人");
    robot.ReadyToStartGame();
}

void Game::QuitToLobby() {
    gameStart->ChangePage(1);
}

void Game::QuitToStart() {
    gameStart->ChangePage(0);
}

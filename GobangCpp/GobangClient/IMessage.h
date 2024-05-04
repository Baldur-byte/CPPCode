#pragma once
#include <stdexcept>
enum class SCMessageType
{
    None = 0,
    HeartBeatSC = 1,
    CreatePlayerSC = 2,
    PlayerInfo = 3,
    RoomList = 4,
    RoomInfo = 5,
    UpdateChessBoard = 6,
    OperationResult = 7,
    GameStart = 8,
    Change = 9,
    GameFinish = 10,
    RestartConfirm = 11,
};

enum class CSMessageType
{
    None = 0,
    HeartBeatCS = 1,
    CreatePlayerCS = 2,
    UpdatePlayerInfo = 3,
    GetPlayerInfo = 4,
    GetRoomList = 5,
    GetRoomInfo = 6,
    PlaceChessCS = 7,
    CreateRoom = 8,
    JoinRoom = 9,
    ExitRoom = 10,
    ReadyToStartGame = 11,
    QuitGame = 12,
    Update = 13,
    RestartRequest = 14,
};

__interface IMessage
{
};

#pragma region Server->Client
struct HeartBeatSC_Message : IMessage
{
    long long serverTime;
};

struct CreatePlayerSC_Message :IMessage
{
    int playerId;
    char name[];
};

struct PlayerInfo_Message : IMessage
{
    char name[];
};

struct RoomList_Message : IMessage
{
    short roomList[12][2]; //0  房间id   1  房间人数
};

struct RoomInfo_Message : IMessage
{
    int roomId;
    int playerIds[];
};

struct UpdateChessBoard_Message : IMessage
{
    short chessBoard[15][15];
};

struct OperationResult_Message : IMessage
{
    int messageType;
    int result; // 0 成功  1 失败
};

struct GameStart_Message : IMessage {
    int chessType; //0 白色  1 黑色
    int turn;
};

struct Change_Message : IMessage {
    int chessType; //0 白色  1 黑色
};

struct GameFinish_Message : IMessage
{
    int isWin;
};

struct RestartConfirm_Message : IMessage
{
};
#pragma endregion

#pragma region Client->Server
struct HeartBeatCS_Message : IMessage
{
};

struct CreatePlayerCS_Message :IMessage
{
};

struct UpdatePlayerInfo_Message : IMessage
{
};

struct GetPlayerInfo_Message : IMessage
{
    int playerId;
};

struct GetRoomList_Message : IMessage
{
};

struct GetRoomInfo_Message : IMessage
{
    int roomId;
};

struct PlaceChessCS_Message : IMessage
{
    short x;
    short y;
    short chess;
};

struct CreateRoom_Message : IMessage
{
    int roomId;
};

struct JoinRoom_Message : IMessage
{
    int roomId;
};

struct ExitRoom_Message : IMessage
{
};

struct ReadyToStartGame_Message : IMessage
{
};

struct QuitGame_Message : IMessage
{
};

struct Update_Message : IMessage
{
};

struct RestartRequest_Message : IMessage
{
};
#pragma endregion

struct MessagePack
{
    short len;
    short type;
    char content[1020];
};
//
//
//CSMessageType toCSMessageType(int value) {
//    switch (value) {
//    case 0: return CSMessageType::None;
//    case 1: return CSMessageType::HeartBeatCS;
//    case 2: return CSMessageType::CreatePlayerCS;
//    case 3: return CSMessageType::UpdatePlayerInfo;
//    case 4: return CSMessageType::GetPlayerInfo;
//    case 5: return CSMessageType::GetRoomList;
//    case 6: return CSMessageType::GetRoomInfo;
//    case 7: return CSMessageType::PlaceChessCS;
//    case 8: return CSMessageType::CreateRoom;
//    case 9: return CSMessageType::JoinRoom;
//    case 10: return CSMessageType::ExitRoom;
//    case 11: return CSMessageType::StartGame;
//    case 12: return CSMessageType::QuitGame;
//    case 13: return CSMessageType::Update;
//    default: throw std::invalid_argument("Invalid color value");
//    }
//}
//
//SCMessageType toSCMessageType(int value) {
//    switch (value) {
//    case 0: return SCMessageType::None;
//    case 1: return SCMessageType::HeartBeatSC;
//    case 2: return SCMessageType::CreatePlayerSC;
//    case 3: return SCMessageType::PlayerInfo;
//    case 4: return SCMessageType::RoomList;
//    case 5: return SCMessageType::RoomInfo;
//    case 6: return SCMessageType::PlaceChessSC;
//    case 7: return SCMessageType::OperationResult;
//    case 8: return SCMessageType::GameStart;
//    case 9: return SCMessageType::Change;
//    case 10: return SCMessageType::GameFinish;
//    default: throw std::invalid_argument("Invalid color value");
//    }
//}
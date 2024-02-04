#pragma once
enum class SCMessageType {
    None,
    HeartBeatSC,
    CreatePlayerSC,
    PlayerInfo,
    RoomList,
    RoomInfo,
    OperationResult,
    GameStart,
    Change,
    GameFinish,
};

enum class CSMessageType {
    None,
    HeartBeatCS,
    CreatePlayerCS,
    UpdatePlayerInfo,
    GetPlayerInfo,
    GetRoomList,
    GetRoomInfo,
    PlaceChess,
    CreateRoom,
    JoinRoom,
    ExitRoom,
    StartGame,
    QuitGame,
    Update,
};

__interface IMessage
{
};

struct HeartBeatSC : IMessage
{
    long long serverTime;
};

struct CreatePlayerSC :IMessage {
    int playerId;
    char name[];
};

struct PlayerInfo : IMessage
{
    char name[];
};

struct RoomList : IMessage
{
    int roomList[];
};

struct RoomInfo : IMessage
{
    int roomId;
    int playerIds[];
};

struct OperationResult : IMessage
{
    int result;
};

struct GameStart : IMessage {
    int chessType; //0 白色  1 黑色
};

struct Change : IMessage {
    int chessType; //0 白色  1 黑色
};

struct GameFinish : IMessage
{
    int isWin;
};

struct HeartBeatCS : IMessage
{
};

struct CreatePlayerCS :IMessage {
};

struct UpdatePlayerInfo : IMessage
{
};

struct GetPlayerInfo : IMessage
{
    int playerId;
};

struct GetRoomList : IMessage
{
};

struct GetRoomInfo : IMessage
{
    int roomId;
};

struct PlaceChess : IMessage
{
    int x;
    int y;
};

struct CreateRoom : IMessage
{
    int roomId;
};

struct JoinRoom : IMessage
{
    int roomId;
};

struct ExitRoom : IMessage
{
};

struct StartGame : IMessage
{
};

struct QuitGame : IMessage
{
};

struct Update : IMessage
{
};
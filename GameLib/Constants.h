#pragma once

const unsigned short SERVER_IP { 55001 };
const unsigned short CLIENT_IP_START{ 55002 }; 
const int BYTE_CAPACITY { 10240 };
const float TIMESTAMP_DEFAULT{ 10.f };
const int TRIES_DEFAULT { 2 };
const float T_SEND{ 0.5f };
const float T_SEND_COMMANDS{ 0.5f };
const float T_CHECK_COMMANDS{ 0.5f };
const float T_SAVE_COMMANDS{ 0.05f };
const float T_INACTIVITY { 60.f };
const float WIN_SIZE_X{ 800.f };
const float WIN_SIZE_Y{ 600.f };
const int MAX_GAME_PLAYERS{ 5 };
const float MAX_VEL_PLAYER{ 100.0f };
const float DELTA_TIME{ 0.04f };
#ifndef WOLFENSTEIN3D_LOBBY_H
#define WOLFENSTEIN3D_LOBBY_H

#include "../common_src/Thread.h"
#include "ThClient.h"
#include "Game.h"
#include <vector>
#include <string>

class Lobby : public Thread {
private:
    std::vector<ThClient*> clients;
    Game* game;
    Configuration& config;

    std::string mapName;
    std::atomic<int> maxPlayers;
    std::atomic<bool> gameInProgress;
    std::atomic<bool> gameIsOver;

public:
    /* Constructor */
    Lobby(ThClient* mainClient, Configuration& config, uint8_t mapID);

    void pushClient(ThClient* newPlayer);

    std::string getMapName();

    uint8_t getPlayersAmountInLobby();

    uint8_t getMaxPlayers();

    void run() override;

    void stop() override;

    bool finished() override;

    bool inProgress();

    /* Destructor */
    ~Lobby();

private:
    void _startGame();

    void _stopClients();
};


#endif //WOLFENSTEIN3D_LOBBY_H

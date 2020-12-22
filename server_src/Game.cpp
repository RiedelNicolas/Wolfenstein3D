#include "Game.h"
#include <iostream>
#include <unistd.h>
#include <cstring> //borrar

#define MAX_MSG_SIZE 256

const double TICK_DURATION = 1/128.f; /* miliseconds que tarda en actualizarse el juego */

Game::Game(std::vector<ThClient*>& _clients, const Configuration& config) :
            clients(_clients), map(config), items(config) {
    this->isRunning = true;
    for (size_t i = 0; i < this->clients.size(); i ++) {
        std::string player_number = "player_" + std::to_string(i);
        this->players.emplace_back(config, player_number);
    }
}

void Game::execute() {

    Timer timeBetweenUpdates;
    double lastTickTime = 0;

    try {
        while (this->isRunning) { //Cambiar, ahora es un while true (Esperar caracter para o esperar a que finalice la partida)
            timeBetweenUpdates.start();

            std::cout << "Nuevo Tick" << std::endl;
            this->getInstructions();
            this->update();  // Fixed Step-Time
            this->sendUpdate();

            timeBetweenUpdates.getTime();

            if (lastTickTime < TICK_DURATION * 1000) {
                usleep((TICK_DURATION * 1000 - lastTickTime) * 1000);
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error in Game Loop :(" << std::endl;
    }
}

void Game::getInstructions() {
    /* Recibir data de los clientes y actualizar "players" */
    uint8_t stateRecv = -1;
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (!this->clients[i]->isEmpty()) {
            stateRecv = this->clients[i]->pop();
            this->players[i].setState(stateRecv);
        }
    }
}

void Game::update() {
    for (size_t i = 0; i < this->players.size(); i++) {
        this->players[i].updatePlayer(this->map, this->items);
    }
}

void Game::sendUpdate() {
    /* Enviar update a los clientes */
    uint8_t msg[MAX_MSG_SIZE];
    int bytesToSend;
    for (size_t i = 0; i < this->clients.size(); i++) {
        bytesToSend = this->createMsg(msg, i);
        this->clients[i]->push(msg, bytesToSend);
    }
}

int Game::createMsg(uint8_t* msg, size_t clientNumber) {
    int playersLoaded = 0;
    uint8_t texture = Guard_0; //Harcodeado, despeus hacerlo bien

    this->players[clientNumber].getPositionDataWithPlane(msg + 1);
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (i != clientNumber) {
            this->players[i].getPositionData(msg + 1 + 24 + playersLoaded * 17); // era *16 sin el hardcodeo del guard
            memcpy(msg + 1 + 24 + playersLoaded * 17 + 16, &texture, 1); //harcodeado, despues hacerlo bien
            playersLoaded++;
        }
    }
    msg[0] = 24 + playersLoaded * 17;
    return msg[0] + 1;
}

void Game::stop() {
    this->isRunning = false;
}

Game::~Game() {}
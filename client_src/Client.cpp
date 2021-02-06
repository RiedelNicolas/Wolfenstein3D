#include <cstring>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "Client.h"
#include "../common_src/GameConstants.h"

#define NEW_GAME 0
#define JOIN_GAME 1
#define FLOAT_SIZE sizeof(float)
#define UINT_ATTRIBUTES 4
#define INT_ATTRIBUTES 2
#define PLAYER_INFO 4 + 2 * sizeof(int) // lives, hp, key, weapon, ammo(int), score(int)
#define PLAYER_ATTRIBUTES 6
#define OTHER_PLAYERS_ATTRIBUTES 5
#define MAX_MESSAGE_SIZE 256

Client::Client(std::string host, std::string service,
               BlockingQueue<int> &instructions,
               ProtectedQueue<DrawingInfo> &drawing_info):
        socket(host.c_str(), service.c_str(), 0),
        peer(socket.connect()), instructions(instructions),
        drawing_info(drawing_info), is_connected(true) {}


void Client::sendInstruction() {
    ssize_t sent = 0;
    try{
        while (is_connected && sent >= 0 && !instructions.isWorking()){
            uint8_t instruction = this->instructions.pop();
            sent = this->peer.send(&instruction, sizeof(uint8_t));
        }
    } catch (WolfensteinException& e){}
    std::cout<<"fin send\n";
}

void Client::lobbyInteraction() {
    int choice;
    uint8_t uint_choice;
    std::cout << "Ingrese 0 si desea crear una nueva partida o 1 si "
                 "desea unirse a una existente." << std::endl;

    std::cin >> choice;
    while (choice != NEW_GAME && choice != JOIN_GAME){
        std::cout << "Input incorrecto. Intente de nuevo." << std::endl;
        std::cin >> choice;
    }
    uint_choice = (uint8_t)choice;
    this->peer.send(&uint_choice, sizeof(uint8_t));

    if (choice == NEW_GAME) _createGame();
    else _joinGame();
}


void Client::_createGame() {
    int choice;
    uint8_t uint_choice;
    uint8_t maps_to_receive, string_length, max_players;
    uint8_t bytes_received[MAX_MESSAGE_SIZE]; //almacena info recibida
    memset(bytes_received, 0, MAX_MESSAGE_SIZE);

    std::vector<std::string> map_names;
    std::vector<int> players_amounts;

    this->peer.recv(&maps_to_receive, 1);

    for (int i=0; i<maps_to_receive; i++){
        this->peer.recv(&string_length, 1);
        this->peer.recv(bytes_received, string_length);
        this->peer.recv(&max_players, 1);

        std::ostringstream convert;
        for (int j = 0; j < string_length; j++) {
            convert << (char)bytes_received[j];
        }
        std::string map_name = convert.str();

        map_names.push_back(map_name);
        players_amounts.push_back(int(max_players));
    }

    std::cout << "ID: Mapa -> Tope de Jugadores" << std::endl;
    for (int i=0; i<maps_to_receive; i++)
        std::cout << i << ": " << map_names[i] << " -> " << players_amounts[i] << std::endl;

    std::cout << "Introduzca el ID del mapa deseado:" << std::endl;
    std::cin >> choice;
    while (choice < 0 || choice > maps_to_receive){
        std::cout << "Input incorrecto. Intente de nuevo." << std::endl;
        std::cin >> choice;
    }

    uint_choice = (uint8_t) choice;
    this->peer.send(&uint_choice, sizeof(uint8_t));
    std::cout << "Para comenzar la partida introduzca 'p'." << std::endl;

    std::cin >> uint_choice;
    while (uint_choice != 'p'){
        std::cout << "Input incorrecto. Intente de nuevo." << std::endl;
        std::cin >> choice;
    }
    this->peer.send(&uint_choice, sizeof(uint8_t));
}

void Client::_joinGame() {
    uint8_t games, game_id, string_length, actual_players, players_limit;
    uint8_t bytes_received[MAX_MESSAGE_SIZE]; //almacena info recibida
    memset(bytes_received, 0, MAX_MESSAGE_SIZE);

    std::vector<std::string> map_names;
    std::vector<int> ids, connected_players, max_players;

    this->peer.recv(&games, 1);

    for (int i=0; i<games; i++){
        this->peer.recv(&game_id, 1);
        this->peer.recv(&string_length, 1);
        this->peer.recv(bytes_received, string_length);
        this->peer.recv(&actual_players, 1);
        this->peer.recv(&players_limit, 1);

        std::ostringstream convert;
        for (int j = 0; j < string_length; j++) {
            convert << (char)bytes_received[j];
        }
        std::string map_name = convert.str();

        map_names.push_back(map_name);
        ids.push_back(int(game_id));
        connected_players.push_back(int(actual_players));
        max_players.push_back(int(players_limit));
    }

    std::cout << "ID: Mapa -> Jugadores conectados/Jugadores maximos" << std::endl;
    for (int i=0; i<games; i++) {
        std::cout << ids[i] << ": " << map_names[i] << " -> " << connected_players[i]
                  << "/" << max_players[i] << std::endl;
    }

    std::cout << "Introduzca el ID del mapa al que desea conectarse:" << std::endl;
    int choice;
    uint8_t uint_choice;
    std::cin >> choice;
    while (std::find(ids.begin(), ids.end(), choice) == ids.end()){
        std::cout << "Input incorrecto. Intente de nuevo." << std::endl;
        std::cin >> choice;
    }
    std::cout << "Conectando..." << std::endl;
    uint_choice = (uint8_t) choice;
    this->peer.send(&uint_choice, sizeof(uint8_t));
}

ssize_t Client::receiveInformation() {
    while(is_connected){
        uint8_t bytes_to_receive;
        uint8_t bytes_received[MAX_MESSAGE_SIZE]; //almacena info recibida
        memset(bytes_received, 0, MAX_MESSAGE_SIZE);

        this->peer.recv(&bytes_to_receive, 1);
        this->peer.recv(bytes_received, bytes_to_receive);
        DirectedPositionable player(0, 0, 0, 0, None);
        PlayerView view;
        std::vector<float> coordinates;
        std::vector<int> player_info;
        std::vector<Positionable> objects;
        std::vector<DirectedPositionable> directed_objects; // jugadores y objetos moviles
        std::vector<std::pair<int,int>> sliders_changes;

        sliders_changes.emplace_back(0,3);
        _assignPlayerInfo(player_info, bytes_received);
        _assignPlayerCoordenates(player, view, coordinates, bytes_received);
        _assignOtherPlayersCoordenates(bytes_received, bytes_to_receive, directed_objects, coordinates);
        DrawingInfo new_info(player, view, player_info,objects, directed_objects, sliders_changes);
        this->drawing_info.push(new_info);
    }
    std::cout<<"fin recv\n";
    return 0;
}


void Client::shutdown() {
    this->is_connected = false;
    this->socket.stop();
    this->instructions.doneAdding();
}

Client::~Client() {
    if (is_connected) shutdown();
}


//-------------------------- Metodos privados --------------------------------//
// Asigna la informacion del jugador (vida, balas, arma, ...) a sus atributos
void Client::_assignPlayerInfo(std::vector<int> &info, uint8_t *bytes_received) {
    uint8_t received_uint8;
    int received_int;
    for(int i=0; i< UINT_ATTRIBUTES; i++){
        memcpy(&received_uint8, bytes_received + i * sizeof(uint8_t), sizeof(uint8_t));
        info.push_back(int(received_uint8));
    }
    for(int i=0; i< INT_ATTRIBUTES; i++){
        memcpy(&received_int, bytes_received +
                UINT_ATTRIBUTES*sizeof(uint8_t) + i*sizeof(int), sizeof(int)); //ammo
        info.push_back(received_int);
    }
}

// Asigna las coordenadas recibidas a los atributos del jugador
void
Client::_assignPlayerCoordenates(DirectedPositionable &player, PlayerView &view,
                                 std::vector<float> &coordinates,
                                 uint8_t *bytes_received) {
    float received;
    for (std::size_t i = 0; i < PLAYER_ATTRIBUTES; i++) {
        memcpy(&received, bytes_received + PLAYER_INFO + FLOAT_SIZE * i, FLOAT_SIZE);
        coordinates.push_back(received);
    }
    player.setX(coordinates[0]);
    player.setY(coordinates[1]);
    player.setDirX(coordinates[2]);
    player.setDirY(coordinates[3]);
    view.movePlaneX(coordinates[4]);
    view.movePlaneY(coordinates[5]);
}

// Asigna las coordenadas recibidasa los demas jugadores del mapa.
void Client::_assignOtherPlayersCoordenates(uint8_t *bytes_received,
                                            uint8_t bytes_to_receive,
                                            std::vector<DirectedPositionable> &players,
                                            std::vector<float> &coordinates) {

    float received;
    uint8_t texture;
    int user_size = PLAYER_ATTRIBUTES * FLOAT_SIZE + PLAYER_INFO;
    int players_size = ((OTHER_PLAYERS_ATTRIBUTES - 1) * FLOAT_SIZE + 1);
    int players_amount = (bytes_to_receive - user_size) / players_size;

    for (int i = 0; i < players_amount; i++) {
        memcpy(&received, bytes_received + user_size + i*players_size, FLOAT_SIZE);
        coordinates.push_back(received);
        memcpy(&received, bytes_received + user_size + i*players_size + FLOAT_SIZE, FLOAT_SIZE);
        coordinates.push_back(received);
        memcpy(&received, bytes_received + user_size + i*players_size + 2*FLOAT_SIZE, FLOAT_SIZE);
        coordinates.push_back(received);
        memcpy(&received, bytes_received + user_size + i*players_size + 3*FLOAT_SIZE, FLOAT_SIZE);
        coordinates.push_back(received);
        memcpy(&texture, bytes_received + user_size + i*players_size + 4*FLOAT_SIZE, 1);
        coordinates.push_back((float)texture);
    }

    for(std::size_t j=PLAYER_ATTRIBUTES; j < coordinates.size(); j+=OTHER_PLAYERS_ATTRIBUTES){
        DirectedPositionable other_player(coordinates[j],
                                          coordinates[j + 1],
                                          coordinates[j+2],
                                          coordinates[j+3],
                                          TextureID(coordinates[j+4]));
        players.push_back(other_player);
    }
}






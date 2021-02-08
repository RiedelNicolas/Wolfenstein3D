#ifndef TP3_TALLER_CLIENT_H
#define TP3_TALLER_CLIENT_H
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <atomic>
#include "PlayerView.h"
#include "DrawingInfo.h"
#include "../common_src/Map.h"
#include "../common_src/Peer.h"
#include "../common_src/Socket.h"
#include "../common_src/BlockingQueue.h"
#include "../common_src/ProtectedQueue.h"
#include "../common_src/DirectedPositionable.h"

class Client {
private:
    Socket socket;
    Peer peer;
    BlockingQueue<int>& instructions;
    ProtectedQueue<DrawingInfo>& drawing_info;
    std::atomic<bool> is_connected;

public:
    // Crea un cliente listo para ser utilizado
    Client(std::string host, std::string service,
           BlockingQueue<int>& instructions,
           ProtectedQueue<DrawingInfo>& drawing_info);

    // Se borran el constructor por copia y el operador =.
    Client(const Client&) = delete;
    Client operator=(const Client&) = delete;

    std::vector<std::vector<int>> receiveMap();

    // Lleva a cabo la interaccion de seleccion de partida con el servidor.
    void lobbyInteraction();

    // Envia al servidor la proxima instruccion contenida en 'instructions',
    // hasta que surja un error o se corte la conexion.
    void sendInstruction();

    // Recibe informacion del servidor y la escribe en el archivo 'file'.
    ssize_t receiveInformation();

    // Realiza un shut down del cliente de acuerdo al modo recibido.
    // 0 -> SHUT_RD, cierra el socket para lectura.
    // 1 -> SHUT_WR, cierra el socket para escritura.
    // 2 -> SHUT_RDWR, cierra el socket para lectoescritura.
    void shutdown();

    // Libera los recursos utilizados por el cliente.
    ~Client();

private:
    void _createGame();
    void _joinGame();
    void _assignPlayerInfo(std::vector<int> &info, uint8_t *bytes_received);
    void
    _assignPlayerCoordenates(DirectedPositionable &player, PlayerView &view,
                             std::vector<float> &coordinates,
                             uint8_t *bytes_received);
    void
    _assignOtherPlayersCoordenates(uint8_t *bytes_received,
                                   uint8_t bytes_to_receive,
                                   std::vector<DirectedPositionable> &players,
                                   std::vector<float> &coordinates);

};


#endif //TP3_TALLER_CLIENT_H

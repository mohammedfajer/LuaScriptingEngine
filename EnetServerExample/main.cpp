#include <iostream>



#pragma comment(lib,"WS2_32")
#pragma comment(lib, "winmm")
#include <enet/enet.h>
#include <iostream>
#include <unordered_map>

struct Player {
    enet_uint32 id;
    float x, y;
};

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet!" << std::endl;
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetHost *server;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = 7777;

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        std::cerr << "Failed to create server!" << std::endl;
        return EXIT_FAILURE;
    }

    std::unordered_map<enet_uint32, Player> players;

    while (true) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "A new client connected!" << std::endl;
                Player newPlayer = { event.peer->connectID, 400.0f, 300.0f };
                players[event.peer->connectID] = newPlayer;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                Player *playerUpdate = reinterpret_cast<Player *>(event.packet->data);
                if (players.find(playerUpdate->id) != players.end()) {
                    players[playerUpdate->id].x = playerUpdate->x;
                    players[playerUpdate->id].y = playerUpdate->y;
                }
                enet_packet_destroy(event.packet);

                // Broadcast all player positions to all clients
                std::vector<Player> playerData;
                for (const auto &pair : players) {
                    playerData.push_back(pair.second);
                }
                ENetPacket *packet = enet_packet_create(playerData.data(), playerData.size() * sizeof(Player), ENET_PACKET_FLAG_RELIABLE);
                enet_host_broadcast(server, 0, packet);
                enet_host_flush(server);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << "Client disconnected!" << std::endl;
                players.erase(event.peer->connectID);
                break;
            }
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    return 0;
}

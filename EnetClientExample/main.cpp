


#include <SFML/Graphics.hpp>
#include <enet/enet.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#pragma comment(lib, "WS2_32")
#pragma comment(lib, "winmm")

struct Player {
    enet_uint32 id;
    float x, y;
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float SQUARE_SIZE = 50.0f;
const float PLAYER_SPEED = 200.0f;

int CustomEntryPointClient() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet!" << std::endl;
        return EXIT_FAILURE;
    }

    ENetHost *client;
    ENetAddress address;
    ENetPeer *peer;
    ENetEvent event;

    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << "Failed to create client!" << std::endl;
        return EXIT_FAILURE;
    }

    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "No available peers for initiating connection!" << std::endl;
        return EXIT_FAILURE;
    }

    enet_uint32 playerID = 0;
    sf::Vector2f playerPosition(400.0f, 300.0f);
    std::unordered_map<enet_uint32, sf::Vector2f> otherPlayers;

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "ENet Multiplayer Game");
    sf::RectangleShape playerSquare(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    playerSquare.setFillColor(sf::Color::Green);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        sf::Event eventSfml;
        while (window.pollEvent(eventSfml)) {
            if (eventSfml.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            playerPosition.x -= PLAYER_SPEED * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerPosition.x += PLAYER_SPEED * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            playerPosition.y -= PLAYER_SPEED * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            playerPosition.y += PLAYER_SPEED * dt;
        }

        // Send position to server
        if (playerID != 0) {
            Player playerUpdate = { playerID, playerPosition.x, playerPosition.y };
            ENetPacket *packet = enet_packet_create(&playerUpdate, sizeof(Player), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
            enet_host_flush(client);
        }

        // Poll for network events
        while (enet_host_service(client, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                const char *data = reinterpret_cast<const char *>(event.packet->data);
                size_t offset = 0;

                while (offset < event.packet->dataLength) {
                    Player otherPlayer;
                    std::memcpy(&otherPlayer.id, data + offset, sizeof(otherPlayer.id));
                    offset += sizeof(otherPlayer.id);
                    std::memcpy(&otherPlayer.x, data + offset, sizeof(otherPlayer.x));
                    offset += sizeof(otherPlayer.x);
                    std::memcpy(&otherPlayer.y, data + offset, sizeof(otherPlayer.y));
                    offset += sizeof(otherPlayer.y);

                    if (otherPlayer.id != playerID) {
                        otherPlayers[otherPlayer.id] = sf::Vector2f(otherPlayer.x, otherPlayer.y);
                    }
                    else {
                        playerID = otherPlayer.id;
                    }
                }

                enet_packet_destroy(event.packet);
            }
        }

        // Render
        window.clear();

        // Draw the player
        playerSquare.setPosition(playerPosition);
        window.draw(playerSquare);

        // Draw other players
        for (const auto &pair : otherPlayers) {
            sf::RectangleShape otherPlayerShape(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            otherPlayerShape.setFillColor(sf::Color::Red);
            otherPlayerShape.setPosition(pair.second);
            window.draw(otherPlayerShape);
        }

        window.display();
    }

    enet_peer_disconnect(peer, 0);
    enet_host_destroy(client);
    enet_deinitialize();

    return 0;
}

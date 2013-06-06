#include "client_packet.h"

size_t GamePacket_Create(GamePacket *packet,
        unsigned char *keysPressed, unsigned char *keysReleased,
        unsigned char numDown, unsigned char numUp,
        unsigned short mouseX, unsigned short mouseY) 
{
    int keyStateChanges = numDown + numUp;
    size_t keyBuffSize = keyStateChanges * sizeof(char);
    size_t packetSize = sizeof(PackedInput) + keyBuffSize;
    int i;

    packet = (GamePacket*)malloc(packetSize);

    packet->input.numUpAndDown = (numUp << 4) | numDown;
    packet->input.mouseX = mouseX;
    packet->input.mouseY = mouseY;

    for(i = 0; i < numDown; i++) {
        packet->keys[i] = keysPressed[i];
    }
    for(i = 0; i < numUp; i++) {
        packet->keys[numDown+i] = keysReleased[i];
    }
    return packetSize;
}

void ClientPacket_Destroy(GamePacket *packet) 
{
    /* Free the packet keyboard change array. */
    free(packet);
}

#ifndef _CLIENT_PACKET_H
#define _CLIENT_PACKET_H

#include <malloc.h>

/**
 * A structure for packets sent by the client to the server.
 * These packets contain the client's input.
 */
typedef struct tagPackedInput {
    /**
     * A byte containing the number of keys pressed down in the least 
     * significant nybble, and the number of keys released in the most
     * significant nybble.
     */
    unsigned char numUpAndDown;
    /**
     * The coordinates of the x and y mouse positions.
     */
    unsigned short mouseX, mouseY;
}PackedInput;

typedef struct tagGamePacket{
    /**
     * The structure of number of keys pressed and mouse position.
     */
    PackedInput input;
    /**
     * An array of keys that changed state. 
     * Elements 0 to (numUpAndDown & 0x0f) represent the keys that were pressed
     * and elements 0 to ((numUpAndDown & 0xf0)>>4) the released keys.
     */
    unsigned char keys[];
}GamePacket;

/**
 * Create a new packet for this client to be eventually sent to the server.
 * Note that the caller is responsible for freeing the keysPressed and 
 * keysReleased arrays if they were not created on the stack.
 * @param keysPressed an array of keyboard keys that have been pressed.
 * @param keysReleased an array of keyboard keys that have been released.
 * @param numDown the number of keys pressed.
 * @param numUp the number of keys released.
 * @param mouseX the current mouse X position.
 * @param mouseY the current mouse Y position.
 * @return the length of the created packet's data.
 */
size_t GamePacket_Create(GamePacket *packet,
        unsigned char *keysPressed, unsigned char *keysReleased,
        unsigned char numDown, unsigned char numUp,
        unsigned short mouseX, unsigned short mouseY);
/**
 * Destroy the given ClientPacket. You MUST call this after you are done with
 * the packet in question.
 * @param cp the packet to destroy.
 */
void GamePacket_Destroy(GamePacket *packet);

#endif

#include "server.h"

int main(int argc, char **argv)
{
    ENetAddress address;
    ENetHost *server;
    ENetEvent event;
    address.host = ENET_HOST_ANY;
    address.port = 1234;
    int shutdown = 0;
    int serviceResult;

    puts("Initializing server");
    if(enet_initialize() != 0) {
        fprintf(stderr, "Error: ENet failed to initialize.\n");
        return -1;
    }
    /*create a server allowing up to 32 clients over up to 2 channels with 
     * any amount for incoming and outcoming bandwidth assumed. */
    server = enet_host_create(&address, 32, 2, 0, 0);
    if(server == NULL) {
        fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
        return -1;
    }
    
    puts("Starting server");
    while(!shutdown) {
        serviceResult = 1;
        while(serviceResult > 0) {
            serviceResult = enet_host_service(server, &event, 1000);
            if(serviceResult > 0) {
                switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("new clinet connected from %x:%u.\n", 
                    event.peer->address.host,
                    event.peer->address.port);
                    /* Store any relevant client information here. */
                    event.peer->data = (void*)"Client Information";
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("length: %u data: %s channel %u\n",
                            event.packet->dataLength,
                            event.packet->data,
                            event.peer->data,
                            event.channelID);
                    /* Tell all clients about this message. */
                    //TODO: enet_host_broadcast(server, 0, event.packet);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%s disconnected.\n", event.peer->data);
                    /* Reset the peer's client information. */
                    event.peer->data = NULL;
                    break;
                }
            }
            else if(serviceResult > 0) {
                puts("Error servicing the server");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* Destroy server */
    puts("Destroying server");
    enet_host_destroy(server);
    enet_deinitialize();
}


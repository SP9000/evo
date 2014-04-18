#include "client.h"

static void (*app_update)();
static ENetHost *client;
static ENetPeer *peer;
static ENetAddress address;
static ENetEvent event;

void tv_client_init()
{
    /* Initialize the client */
    puts("\nInitializing client\n"
    "****************************************");

    /* Initialize SDL. */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize draw. */
    puts("Initializing draw");
	if(tv_draw_init() != 0) {
        fprintf(stderr, "Error initializing Draw\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the texture system */
    puts("Initializing texture");
	if(tv_texture_init() != 0) {
        fprintf(stderr, "Error initializing texture\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the scene */
    puts("Initializing scene");
	if(tv_scene_init() != 0) {
        fprintf(stderr, "Error: could not initialize the scene\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the input system */
	if(tv_input_init() != 0) {
    }

	if(tv_entity_init() != 0) {
		fprintf(stderr, "Error: could not initialize the entity system\n");
		exit(EXIT_FAILURE);
	}

	if(tv_component_init() != 0) {
		fprintf(stderr, "Error: could not initialize the component system\n");
		exit(EXIT_FAILURE);
	}

    puts("Starting client");
    /* Initialize ENet */
    if(enet_initialize() != 0) {
        fprintf(stderr, "Error initializing enet\n");
        exit(EXIT_FAILURE);
    }

    /* Create a client host with 1 outgoing connection, 2 channels being used,
     * 56k downstream bandwidth, and 14k upstream bandwidth. */
    client = enet_host_create(NULL, 1, 2, 57600, 14400);
    if(client == NULL) {
        fprintf(stderr, "Error: could not create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }

    /* Set address. */
    enet_address_set_host(&address, "localhost");
    address.port = 1234;

    /* Connect to server. */
    peer = enet_host_connect(client, &address, 2, 0);
    if(peer == NULL) {
        fprintf(stderr, "No available peers for initiating an ENet "
                "connection\n");
        exit(EXIT_FAILURE);
    }

    /*Try to connect to the server within 5 seconds */
    if(enet_host_service(client, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT) {
        puts("Connection to server succeeded.");
    }
    else {
        /* Either the 5 seconds are up or a disconnect event was received.
         * Reset the peer in the event the 5 seconds had run out without
         * significant event. */
        enet_peer_reset(peer);
        fprintf(stderr, "Connection to server failed.\n");
        exit(EXIT_FAILURE);
    }
    
	puts("Registering components.");
	/*transform must be the first component registered */
	tv_transform_register();

	tv_model_register();
	tv_material_register();
	tv_camera_register();
	tv_renderer_register();
	tv_model_renderer_register();
	tv_text_renderer_register();

	printf("%d  %d  %d\n", tv_renderer_id(), tv_model_renderer_id(), tv_text_renderer_id());

    /* start the application */
    puts("Core initialized...\n"
        "***************************************");
}

void tv_client_start() 
{
	SDL_Event inputEvent;
	int serviceResult;
	int run = 1;

	puts("Client starting.\n");

	/* Capture mouse */
    /* SDL_WM_GrabInput(SDL_GRAB_ON); */
    /* Main game loop. */
    while(run) {
        unsigned char keysPressed[16];
        unsigned char keysReleased[16];
        int numPressed, numReleased, mouseMoved;
        int mouseX, mouseY;
        GamePacket *packet = NULL;

        serviceResult = 1;

        /* Keep doing host_service until no events are left. */
        while(serviceResult > 0) {
            serviceResult = enet_host_service(client, &event, 0);
            if(serviceResult > 0) {
                switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("new clinet connected from %x:%u.\n", 
                    event.peer->address.host,
                    event.peer->address.port);
                    event.peer->data = (void*)"New User";
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("A packet of length %u containing %s was received "
                           "from %s on channel %u.\n",
                            (unsigned int)event.packet->dataLength,
                            (char*)event.packet->data,
                            (char*)event.peer->data,
                            (unsigned int)event.channelID);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%s disconnected.\n", (char*)event.peer->data);
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
                }
            }
            else if(serviceResult > 0) {
                puts("Error with servicing the client");
                exit(EXIT_FAILURE);
            }
        }

        /* Reset input flags. */
        numPressed = 0;
        numReleased = 0;
        mouseMoved = 0;

        /* Poll for new input events. */
        while(SDL_PollEvent(&inputEvent)) {
            switch(inputEvent.type) {
            case SDL_QUIT: 
                run = 0;
                break;
            case SDL_VIDEORESIZE:
				tv_draw_resize_screen(inputEvent.resize.w, inputEvent.resize.h);
                break;
            case SDL_KEYDOWN:
                if(inputEvent.key.keysym.sym == SDLK_ESCAPE) {
                    run = 0;
                }
                keysPressed[numPressed++] = inputEvent.key.keysym.sym;
                break;
            case SDL_KEYUP:
                keysReleased[numReleased++] = inputEvent.key.keysym.sym;
                break;
            case SDL_MOUSEMOTION:
                mouseX = inputEvent.motion.x;
                mouseY = inputEvent.motion.y;
                mouseMoved = 1;
                break;
            }
        }
        /* If mouse is at edge of screen, pan */
        SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseX < 5) {
            //Draw_MoveCamera(-0.1f, 0, 0);
        }
        else if(mouseX > screen->w - 5) {
            //Draw_MoveCamera(0.1f, 0, 0);
        }
        if(mouseY < 5) {
            //Draw_MoveCamera(0, 0.1f, 0);
        }
        else if(mouseY > screen->h - 5) {
            //Draw_MoveCamera(0, -0.1f, 0);
        }

        /* If there is new input, create/send a packet containing the info to the server. */
        if(mouseMoved || numReleased > 0 || numPressed > 0) {
            size_t packetLen = GamePacket_Create(packet, 
                    keysPressed, keysReleased, 
                    numPressed, numReleased, 
                    (short)mouseX, (short)mouseY);
            ENetPacket *epacket = enet_packet_create(packet, packetLen, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, epacket);
        }

        /* update input information */
		tv_input_update();

        /* perform collision detection */
		tv_collision_detect();

        /* update time */
		tv_time_update();
    
        /* update entities */
		tv_entity_update();

		/* update post-entity update handlers */
		tv_component_update_post_handlers();

        /* update the app */
        app_update();

        /* Render */
		tv_draw_start_frame();
		
		/* update pre-entity update handlers TODO: should be before tv_entity_update */
		tv_component_update_pre_handlers();

		tv_draw_scene();
        SDL_GL_SwapBuffers();

        /* Render GUI TODO */
        //Draw_GUI();

        SDL_Delay(100);
    }
    /* Shut down the client. */

    /* Destroy ENet client. */
    enet_host_destroy(client);
    enet_deinitialize();

	tv_input_quit();
	tv_draw_quit();
    SDL_Quit();
}

void tv_client_set_update_func(void (*update_func)())
{
	app_update = update_func;
}
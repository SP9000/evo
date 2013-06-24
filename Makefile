CLIENT_LIBS = -lenet -lSDL -lGL -lGLU -lGLEW -ldl -lm  -lglib-2.0
SERVER_LIBS = -lenet

CFLAGS = `pkg-config --cflags --libs glib-2.0` -Wall

#client source files
CLIENT_SOURCES=AppSpec/app.c client.c client_packet.c draw.c entity.c guilayout.c material.c matrix.c model.c modelgen.c scene.c util.c cJSON.c
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

#server source files
SERVER_SOURCES=server/server.c
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)

#execuables
CLIENT_EXECUTABLE=Client
SERVER_EXECUTABLE=Server

all: $(SOURCES) $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE) 
    
$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJECTS) $(CLIENT_LIBS) 

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJECTS) $(SERVER_LIBS) 

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o


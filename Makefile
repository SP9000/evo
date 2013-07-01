CC = c99
CLIENT_LIBS = -lenet -lSDL -lGL -lGLU -lGLEW -ldl -lm  -lglib-2.0
SERVER_LIBS = -lenet

# -DOPENGL for OpenGL 2.1+ rendering, -DSW_RENDER for software rendering */
CFLAGS = `pkg-config --cflags --libs glib-2.0` -Wall -DOPENGL_2

# component sources 
COMPONENT_SOURCES=$(wildcard Components/*.c)
COMPONENT_OBJECTS=$(addprefix obj/,$(notdir $(COMPONENT_SOURCES:.c=.o)))

#client source files
CLIENT_SOURCES=AppSpec/app.c client.c client_packet.c collision.c component.c draw.c entity.c guilayout.c matrix.c modelgen.c scene.c util.c cJSON.c 
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

#server source files
SERVER_SOURCES=server/server.c
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)

#execuables
CLIENT_EXECUTABLE=Client
SERVER_EXECUTABLE=Server

all: $(SOURCES) $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE) 
    
$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS) $(COMPONENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJECTS) $(COMPONENT_OBJECTS) $(CLIENT_LIBS) 

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJECTS) $(SERVER_LIBS) 

.c.o:
	$(CC) $(CFLAGS) -UBUILD -c $< -o $@ 

obj/%.o: Components/%.c
	$(CC) $(CFLAGS) -DBUILD -c $< -o $@ 

clean:
	rm -f *.o
	rm -f obj/*.o

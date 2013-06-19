INCLUDE = .
LIBS = -lenet -lSDL -lGL -lGLU -lGLEW -ldl -lm  -lglib-2.0
SERVERLIBS = -lenet

CFLAGS = `pkg-config --cflags --libs glib-2.0` 

all: Client Server
    
Client: client.o app.o draw.o model.o client_packet.o matrix.o collision.o guilayout.o material.o modelgen.o scene.o util.o cJSON.o
	$(CC) $(CFLAGS) -o $@ client.o app.o matrix.o draw.o model.o client_packet.o collision.o guilayout.o material.o modelgen.o scene.o util.o cJSON.o $(LIBS)

Server: server.o 
	$(CC) $(CFLAGS) -o $@ server.o $(SERVERLIBS) 

client.o: client.c draw.c model.c client_packet.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

server.o: server/server.c 
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

app.o: AppSpec/app.c draw.c model.c guilayout.c modelgen.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

draw.o: draw.c model.c guilayout.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

guilayout.o: guilayout.c 
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

collision.o: collision.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

model.o: model.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

modelgen.o: modelgen.c model.c draw.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

material.o: material.c cJSON.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

matrix.o: matrix.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

util.o: util.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

scene.o: scene.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

client_packet.o: client_packet.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

cJSON.o: cJSON.c
	$(CC) -c $(CFLAGS) $? -I $(INCLUDE)

all: Client Server

clean:
	rm -f *.o


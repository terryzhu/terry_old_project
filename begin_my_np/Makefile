all:Client UDPClient UDPServer
Client:SockConn.o client.o SockClient.o ErrUtil.o
	g++ -o Client SockConn.o client.o SockClient.o ErrUtil.o
SockConn.o:SockConn.cpp
	g++ -o SockConn.o -c SockConn.cpp
client.o:client.cpp
	g++ -o client.o -c client.cpp
SockClient.o:SockClient.cpp
	g++ -o SockClient.o -c SockClient.cpp

UDPClient:broadcast_client.cpp ErrUtil.o
	g++ -o UDPClient  broadcast_client.cpp ErrUtil.o

UDPServer:broadcast_server.cpp ErrUtil.o
	g++ -o UDPServer  broadcast_server.cpp ErrUtil.o

ErrUtil.o:ErrUtil.cpp
	g++ -o ErrUtil.o -c ErrUtil.cpp

clean:
	rm *.o
	rm Client
	rm UDPClient
	rm UDPServer

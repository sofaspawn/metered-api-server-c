server: webserver.c
	clang webserver.c -o server && ./server

client: webclient.c
	clang webclient.c -lcurl -o client 


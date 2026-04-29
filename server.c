#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>
int init_server() {
	int sck;
	struct addrinfo server_info, *res;
	memset(&server_info, 0, sizeof(struct addrinfo));
	server_info.ai_family  = AF_INET;
	server_info.ai_socktype = SOCK_STREAM;
	inet_pton(server_info.ai_family, "127.0.0.1", &((struct sockaddr_in *) &server_info.ai_addr)->sin_addr);
	// 
	if(getaddrinfo(NULL, "3090", &server_info, &res) == -1){
		perror("Ocurrio un error en la obtencion de nombre de socket");
		exit(0);
	}
	struct sockaddr_in *addr = (struct sockaddr_in *) res->ai_addr;
	if((sck = socket(addr->sin_family, SOCK_STREAM, 0)) == -1){
		perror("Ocurrio un error al intentar crear el socket");
		exit(0);
	}
	if(bind(sck, (struct sockaddr *) addr, res->ai_addrlen) == -1){
		perror("Ocurrio un error al intentar vincular la propiedades del socket");
		exit(EXIT_FAILURE);
	}
	
	freeaddrinfo(res);
	if(listen(sck, 3) == -1){
		perror("Ocurrio un error al intetar habilitar la escuchar para el socket");
	}
	char ipv4[INET_ADDRSTRLEN];
	inet_ntop(server_info.ai_family, addr, ipv4, INET_ADDRSTRLEN);
	printf("servidor listo y ecuchando en: %s:%d\n", ipv4, ntohs(addr->sin_port));
	return sck;
}

typedef struct {
	struct pollfd *poll;
	unsigned int poll_index;
	unsigned int poll_lenght;
} Client_poll;

Client_poll *new_poll(){
	Client_poll *poll = (Client_poll *) malloc(sizeof(Client_poll));
	poll->poll = (struct pollfd *) malloc(16 * sizeof(struct pollfd));
	if(poll->poll == NULL){
		free(poll);
		return  NULL;
	}
	poll->poll_lenght = 16;
	poll->poll_index = 0;
	return poll;
}
void free_poll(Client_poll *poll){
	if(poll != NULL){
		free(poll->poll);
		free(poll);
	}	
}
int polladd(Client_poll *poll ,int fd) {
	if(poll->poll_index >= poll->poll_lenght)
		return  -1;
	
	struct pollfd *s = &poll->poll[poll->poll_index++];
	s->fd = fd;
	s->events = POLLIN /*| POLLHUP*/;
	s->revents = 0;
	return 0;
}
int pollremove_fd(Client_poll *poll, int fd){
	if(poll->poll_index < 1) 
		return -1;
	int i = 0;
	while(i <= poll->poll_index ){
		struct pollfd *i_fd = &poll->poll[i];
		if(i_fd->fd == fd){
			close(fd);
			*i_fd = poll->poll[poll->poll_index];
			poll->poll_index--;
			break;
		}
		i++;
	}
	return 0;
}

void connection_handler(int main_fd, Client_poll *connection_poll){
	int new_sock;
	struct sockaddr_storage from;
	socklen_t from_size = sizeof(struct sockaddr_storage);
	int isInclude;
	
	if((new_sock = accept(main_fd, (struct sockaddr *) &from, &from_size)) == -1){
		perror("Error, refused new connection request");
		return;
	}
	
	if(polladd(connection_poll, new_sock) == -1){
		fputs("Error al intentar agregar un socket al poll \n", stderr);
		close(new_sock);
		return;
	};
	
	printf("se ha creado una nueva conexion, sin detalles por el momento :v\n");
}

void message_handler(int sockfd, Client_poll *connection_poll) {
	char buff[1024];
	int len = recv(sockfd, buff, 1024, 0);
	if(len == 0) {
		puts("se ha cerrado una conexio\n");
		pollremove_fd(connection_poll ,sockfd);
		return;
	}
	buff[len -1] = '\0';
	printf("nuevo mensaje recibido: %s\n", buff);
	char *resmsg = "Respuesta servidor\n";
	send(sockfd, resmsg, strlen(resmsg), 0);
}

void poll_handler(int events, Client_poll *poll_arr){
	struct pollfd *poll_p;
	for(int i = 0; i <= poll_arr->poll_index && events > 0; i++){
		poll_p = &poll_arr->poll[i];
		/*new connection */
		if(poll_p->revents & POLLIN){
			if(i == 0){
				connection_handler(poll_p->fd, poll_arr);
			}else{
				message_handler(poll_p->fd, poll_arr);
			}
			events--;
		}
		/*disconnection */
		else if(poll_p->revents & POLLHUP){
			events--;
		}
		poll_p->revents = 0;
	}
}

int main(void) {
	Client_poll *connections;
	int main_socket; 
	
	main_socket = init_server();
	if((connections = new_poll()) == NULL) {
		fputs("Ocurrio un error de memoria al intentar crear el poll de conexiones\n", stderr);
	};
	if(polladd(connections, main_socket) == -1){
		fputs("Ocurrio un error al intentar agregar un socket al poll\n", stderr);
	};
	
	while(1){
		puts("Esperando por un Evento...\n");
		int events_count = poll(connections->poll, connections->poll_lenght, -1);
		printf("Ocurrieron %d eventos\n", events_count);
		
		poll_handler(events_count, connections);
	}
	free_poll(connections);
}

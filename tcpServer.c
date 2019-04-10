/*
 * =====================================================================================
 *
 *       Filename:  TCP_server.c
 *
 *    Description:  Code to create a TCP server built on top of/ by  https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=2ahUKEwjC9YiX0ZPhAhUJ8HMBHYzIDeQQFjACegQIBBAB&url=https%3A%2F%2Fwww.cs.cmu.edu%2Fafs%2Fcs%2Facademic%2Fclass%2F15213-f99%2Fwww%2Fclass26%2Ftcpserver.c&usg=AOvVaw1ZwUkY8jZDhz-iq9Xp8VEG
 *
 *        Version:  1.0
 *        Created:  Thursday 21 March 2019 08:02:35  IST
 *       Revision:  none
 *       Compiler:  gcc
 *	Original Autthor : cs.cmu.edu
 *        Modified by:  Aswin P Ajayan (), aswinp2008@gmail.com
 *   Organization:  iitb
 *
 * 
 * =====================================================================================
 */


/*tcp server.c - A simple TCP echo 
 * usage: tcpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>


#define CMD_SIZE 128
#define BUFSIZE 1024
#define PACKET_SIZE BUFSIZE*2

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

/* shared buffered locked through the mutex*/

unsigned short in_buf[BUFSIZE]; /* message buf  data from MCU*/
char out_buf[CMD_SIZE]; /*control messages from UI*/
pthread_mutex_t lock_buf = PTHREAD_MUTEX_INITIALIZER;
pthread_t server_t_id;

void* socketThread(void *arg){
  char  recv_buf[PACKET_SIZE];	
  int server_sockfd; /* socket */
  int client_sockfd;
  int portno; /* port to listen on */
  unsigned int clientlen; /* byte size of client's address */
  unsigned int serverlen;
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  //struct hostent *hostp; /* client host info */
  //struct addrinfo *host_info;
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  int i;

  /*FILE IO*/
  FILE *out_fp;
  printf("from socketTHread");

  /* 
   * check command line arguments 
   */
  portno = atoi((char*)arg);

  /* 
   * socket: create the parent socket 
   */
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, 
  (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

printf("going to bind ;assert on error");
  /* 
   printf("binding complete");
  * bind: associate the parent socket with a port 
   */
  serverlen = sizeof(serveraddr);
  if (bind(server_sockfd, (struct sockaddr *) &serveraddr, 
	   serverlen) < 0) 
    error("ERROR on binding");
  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {
	listen(server_sockfd,3);

    if( (client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, (socklen_t*)&clientlen))>=0){
   	 /*                                                                            		}
   	  * recv: receive a TCP client
   	  */
   	 bzero(recv_buf, PACKET_SIZE);
   	 n = read(client_sockfd, recv_buf, PACKET_SIZE);
   	         if (n < 0)
   	   error("ERROR in read");
   	 
   	 pthread_mutex_lock(&lock_buf);
   	 bzero(in_buf,BUFSIZE);
   	// memcpy(in_buf,recv_buf,sizeof(recv_buf)+1);
   	 hostaddrp = inet_ntoa(clientaddr.sin_addr);
   	 printf("server received %ld/%d bytes :  %s\n", sizeof(recv_buf), n,recv_buf);
   	     out_fp = fopen("./inputdata.txt","w+");
   	     if(out_fp== NULL)
   	     	printf("error fatal");
   	     for(i = 0 ; i < BUFSIZE  ; i +=2){
   	     		
   	     	
   	     	in_buf[i] = recv_buf[i+1]*256 + recv_buf[i];
   	     	fprintf(out_fp,"%u\n",(int)in_buf[i]);
   	     }
   	     fclose(out_fp);

   	     pthread_mutex_unlock(&lock_buf);

  }	
 
    /* 
     * sendto: echo the input back to the client 
     */
  //  n = sendto(sockfd, out_buf, strlen(out_buf), 0, 
//	       (struct sockaddr *) &clientaddr, clientlen);
 //   if (n < 0) 
  //    error("ERROR in sendto");
  }
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
printf("tsetasetasa.....................................");	
 //test initialisation of command buffer 
 
	memcpy(out_buf,"TEST_COMMAND",12);
  /* 
   * check command line arguments 
   */
	printf("from main 1\n");
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
	printf("from main");
	pthread_create(&server_t_id,NULL,&socketThread,argv[1]);
pthread_join(server_t_id,NULL);


}


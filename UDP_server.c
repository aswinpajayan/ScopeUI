/*
 * =====================================================================================
 *
 *       Filename:  UDP_server.c
 *
 *    Description:  Code to create a UDP server built on top of/ by  https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=2ahUKEwjC9YiX0ZPhAhUJ8HMBHYzIDeQQFjACegQIBBAB&url=https%3A%2F%2Fwww.cs.cmu.edu%2Fafs%2Fcs%2Facademic%2Fclass%2F15213-f99%2Fwww%2Fclass26%2Fudpserver.c&usg=AOvVaw1ZwUkY8jZDhz-iq9Xp8VEG
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


/*udp server.c - A simple UDP echo server 
 * usage: udpserver <port>
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
#define BUFSIZE 256
#define PACKET_SIZE 512

#define PORT_NUMBER 50001


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
pthread_mutex_t lock_buf = PTHREAD_MUTEX_INITIALIZER; //mutex to lock the shared buffer
pthread_t server_t_id; 

/* global variables which takes care of inter process communication*/
pthread_cond_t suspend_plotting = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock_plotter = PTHREAD_MUTEX_INITIALIZER;


void* socketThread(void *arg){
  unsigned char  recv_buf[PACKET_SIZE];	
  int sockfd; /* socket */
  int portno; /* port to listen on */
  unsigned int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  //struct hostent *hostp; /* client host info */
  //struct addrinfo *host_info;
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  int i,j;

  /*FILE IO*/
  //FILE *out_fp;

  
  /* 
   * check command line arguments 
   */

  printf("recieved argument is : %s\n",(char *)arg);  //just for debugging
  portno = atoi("50001");
  printf("from socket thread , port number is : %d\n",portno);
  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
  * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {
	
	    /*
	     * recvfrom: receive a UDP datagram from a client
	     */
	  bzero(recv_buf, PACKET_SIZE);
	
	  //socket Thread acquire the cpu
	  //this causes the plotter thread to wait till
	  //the mutex lock_plotter  is released
	
	
	  n = recvfrom(sockfd, recv_buf, PACKET_SIZE, 0,(struct sockaddr *) &clientaddr, &clientlen);
	  if (n < 0)
	      error("ERROR in recvfrom");
	    
	  pthread_mutex_lock(&lock_buf);
	  bzero(in_buf,BUFSIZE);
	  hostaddrp = inet_ntoa(clientaddr.sin_addr);
	  printf("\nserver received %ld/%d bytes \n", sizeof(recv_buf), n);
	  //out_fp = fopen("./inputdata.txt","w+");
	  //if(out_fp== NULL)
	  	//printf("error fatal");
	
	  //repack the recieved byte array back to unsigned short
	  j = 0;
	  for(i = 0 ; i < PACKET_SIZE  ; i ++){
	  	in_buf[j] = (recv_buf[i+1]<<8) & 0x000000300;
			in_buf[j] = (in_buf[j]) | (recv_buf[i] & 0x0000FF);
	 	//	fprintf(out_fp,"%u\n",in_buf[j]);
	 		i++;
	 		j++;
	
	  	//fprintf(out_fp,"%u\n",in_buf[i]);  //for debugging write the value on to a file
	  }
	  //fclose(out_fp);
	  pthread_mutex_unlock(&lock_buf);
	  printf("mutex unlocked\n");
	  printf("signalling plotter thread .now the plotter thread can run\n");
	  pthread_cond_signal(&suspend_plotting);
	  pthread_mutex_unlock(&lock_plotter);
	 
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


/*this function is just for testing the functionality of the code*/
int starter_main(int argc, char **argv) {
	
 //test initialisation of command buffer 
 
	memcpy(out_buf,"TEST_COMMAND",12);
  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

	pthread_create(&server_t_id,NULL,&socketThread,argv[1]);
pthread_join(server_t_id,NULL);
return 0;

}


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "a1_lib.h"

#define BUFSIZE   1024

void parse_line(Message *message, char user_input[]);

// typedef struct message{
//   char command[10];
//   char arg1[10];
//   char arg2[10];
//   int sockid;
// }my_message;

int main(int argc, char * argv[]) {
  Message *message = (Message *)malloc(sizeof(Message));
  int sockfd;
  char user_input[BUFSIZE] = { 0 };
  char server_msg[BUFSIZE] = { 0 };
  uint16_t port = atoi(argv[2]);

  if (connect_to_server(argv[1], port, &sockfd) < 0) {
    fprintf(stderr, "oh no\n");
    return -1;
  }

  while (true) {
    char command[4][100];

    command[0][0] = '\0';
    command[1][0] = '\0';
    command[2][0] = '\0';
    command[3][0] = '\0';

    int j = 0;
    int wordCount = 0;

    printf(">> ");

    memset(user_input, 0, sizeof(user_input));
    memset(server_msg, 0, sizeof(server_msg));

    // read user input from command line
    fgets(user_input, BUFSIZE, stdin);

    parse_line(message, user_input);

    // send the input to server
    send_message(sockfd, message, BUFSIZE);

/*
    // delete newlines
    char *tmp = NULL;
    if ((tmp = strstr(user_input, "\n"))) {
      *tmp = '\0';
    }

    //    printf("Client: %s\n", msg);

    for(int i = 0; i <= (strlen(user_input)); i++) {
      if(user_input[i] == ' ' || user_input[i] == '\0') {
        command[wordCount][j] = '\0';
        wordCount++;
        j = 0;
      }
      else {
        command[wordCount][j] = user_input[i];
        j++;
      }
    }

    // int i = 0;
    //
    // for(i = 0;i < 4; i++)
    // {
    //     printf("\nString [%d] : %s", i, command[i]);  fflush(stdout);
    // }


    // my_message m;
    // strcpy(m.command, command[0]);
    // strcpy(m.arg1, command[1]);
    // strcpy(m.arg2, command[2]);

    send_message(sockfd, (char*)&m, sizeof(struct my_message)); */

//    send_RPC(sockfd, command, BUFSIZE);

    // send the input to server
    // send_message(sockfd, user_input, strlen(user_input));
    // receive a msg from the server
    ssize_t byte_count = recv_message(sockfd, server_msg, sizeof(server_msg));

    printf("Server: %s\n", server_msg);

    if ((strcmp(server_msg, "Bye!") == 0)) {
      break;
    }

    if (!strcmp(user_input, "quit\n")) {
      return 0;
    }

    if (byte_count <= 0) {
      break;
    }
  }

  return 0;
}

void parse_line(Message *message, char user_input[])
{
    char *token = strtok(user_input, " ");
    int index = 1;

    while (token != NULL)
    {
        if (index == 1)
        {
            // parse command
            strcpy(message->command, token);
            index++;
            token = strtok(NULL, " ");
        }
        else if (index == 2)
        {
            // parse first paramter
            strcpy(message->param_1, token);
            index++;
            token = strtok(NULL, " ");
        }
        else if (index == 3)
        {
            // parse second parameter
            strcpy(message->param_2, token);
            index++;
            token = strtok(NULL, " ");
        }
    }
}

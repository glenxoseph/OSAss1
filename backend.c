#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "a1_lib.h"


#define BUFSIZE   1024
// int addInts(int a, int b);
// int multiplyInts(int a, int b);
// uint16_t factorial (int x);
// float divideFloats(float a, float b);

int addInts(int a, int b) {
  return a + b;
}

int multiplyInts(int a, int b) {
  return a * b;
}

uint16_t factorial (int x) {
  uint16_t result = 1;
  while (x > 0) {
    result = result * x;
    x--;
  }
  return result;
}

float divideFloats(float a, float b) {
  float c = a / b;
  return c;
}

int main(int argc, char * argv[]) {
  int sockfd, clientfd;
  char msg[BUFSIZE];
  const char *greeting = "hello, world\n";
  int running = 1;

  uint16_t port = atoi(argv[2]);

  if (create_server(argv[1], port, &sockfd) < 0) {
    fprintf(stderr, "oh no\n");
    return -1;
  }

  if (accept_connection(sockfd, &clientfd) < 0) {
    fprintf(stderr, "oh no\n");
    return -1;
  }

  fprintf(stderr, "Server listening on: %s", argv[1]);
  fprintf(stderr, ": %s\n", argv[2]);

  while (true) {

    char command[4][100];

    command[0][0] = '\0';
    command[1][0] = '\0';
    command[2][0] = '\0';
    command[3][0] = '\0';


    int j = 0;
    int wordCount = 0;
    memset(msg, 0, sizeof(msg));
    ssize_t byte_count = recv_message(clientfd, msg, BUFSIZE);

    // delete newlines
    char *tmp = NULL;
    if ((tmp = strstr(msg, "\n"))) {
      *tmp = '\0';
    }

    //    printf("Client: %s\n", msg);

    for(int i = 0; i <= (strlen(msg)); i++) {
      if(msg[i] == ' ' || msg[i] == '\0') {
        command[wordCount][j] = '\0';
        wordCount++;
        j = 0;
      }
      else {
        command[wordCount][j] = msg[i];
        j++;
      }
    }

    //ADD
    if (strcmp(command[0], "add") == 0) {
      int a = atoi(command[1]);
      int b = atoi(command[2]);
      if (command[1][0] == '\0' || command[2][0] == '\0' || command[3][0] != '\0') {
        command[3][0] = '\0';
        char *error = "Error: Command \"add\" takes 2 arguments";
        send_message(clientfd, error, strlen(error));
      }
      else {
        int sum = addInts(a, b);
        char *sumStr[100];
        sprintf(sumStr, "%d", sum);
        send_message(clientfd, sumStr, strlen(sumStr));
        command[0][0] = '\0';
        command[1][0] = '\0';
        command[2][0] = '\0';
      }
    }

    //MULTIPLY
    else if (strcmp(command[0], "multiply") == 0) {
      int a = atoi(command[1]);
      int b = atoi(command[2]);
      if (command[1][0] == '\0' || command[2][0] == '\0' || command[3][0] != '\0') {
        command[3][0] = '\0';
        char *error = "Error: Command \"multiply\" takes 2 arguments";
        send_message(clientfd, error, strlen(error));
      }
      else {
        int product = multiplyInts(a, b);
        char *productStr[100];
        sprintf(productStr, "%d", product);
        send_message(clientfd, productStr, strlen(productStr));
        command[0][0] = '\0';
        command[1][0] = '\0';
        command[2][0] = '\0';
      }
    }

    //FACTORIAL
    else if (strcmp(command[0], "factorial") == 0) {
      int a = atoi(command[1]);
      if (command[2][0] != '\0') {
        command[2][0] = '\0';
        char *error = "Error: Command \"factorial\" takes 1 argument";
        send_message(clientfd, error, strlen(error));
      }
      else {
        //        uint64_t result = factorial(a);
        uint64_t result = factorial(a);
        char *resultStr[100];
        sprintf(resultStr, "%d", result);
        send_message(clientfd, resultStr, strlen(resultStr));
        command[0][0] = '\0';
        command[1][0] = '\0';
      }
    }

    //DIVIDE
    else if (strcmp(command[0], "divide") == 0) {
      if (command[1][0] == '\0' || command[2][0] == '\0' || command[3][0] != '\0') {
        command[3][0] = '\0';
        char *error = "Error: Command \"multiply\" takes 2 arguments";
        send_message(clientfd, error, strlen(error));
      }
      else {
        float b = atof(command[2]);
        if (b == 0.0) {
          char *error = "Error: Division by zero";
          send_message(clientfd, error, strlen(error));
        }
        else {
          float a = atof(command[1]);
          //        float quotient = (float)a / (float)b;
          float quotient = divideFloats(a, b);
          char *quotientStr[100];
          sprintf(quotientStr, "%.6f", quotient);
          send_message(clientfd, quotientStr, strlen(quotientStr));
        }
        command[0][0] = '\0';
        command[1][0] = '\0';
        command[2][0] = '\0';
      }
    }

    //SLEEP
    else if (strcmp(command[0], "sleep") == 0) {
      int a = atoi(command[1]);
      if (command[1][0] == '\0' || command[2][0] != '\0') {
        command[2][0] = '\0';
        char *error = "Error: Command \"sleep\" takes 1 argument";
        send_message(clientfd, error, strlen(error));
      }
      else {
        char * sleepMsg = "Finished Sleeping";
        sleep(a);
        send_message(clientfd, sleepMsg, strlen(sleepMsg));
        command[0][0] = '\0';
        command[1][0] = '\0';
      }
    }

    //QUIT
    else if ((strcmp(msg, "quit") == 0) || (strcmp(msg, "shutdown") == 0)) {
      char * byeMsg = "Bye!";
      send_message(clientfd, byeMsg, strlen(byeMsg));
      return(0);
    }

    //FALSE COMMANDS
    else {
      char s1[] = "Error: Command \"";
      char s2[] = "\" not found";
      char error[10000] = {0};
      snprintf(error, sizeof(error), "%s%s%s", s1, command[0], s2);
      send_message(clientfd, error, strlen(error));
      command[0][0] = '\0';
      command[1][0] = '\0';
      command[2][0] = '\0';
      command[3][0] = '\0';
    }



    if (byte_count <= 0) {
      break;
    }
    //   send_message(clientfd, greeting, strlen(greeting));
  }

  return 0;
}

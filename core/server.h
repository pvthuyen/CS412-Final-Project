#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread

#include "configurations.h"
#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
#include "utils/utils.h"

void processQuery(const string &queryName, string &outputFile) {
	AppData *app = AppData::getInstance();
	int nDocs = app->path.size();
	debugVar(queryName);

    string tmp = queryName;
    tmp.replace(tmp.size() - 3, 3, "mat");

    string kpPath = kpFolder + "/" + tmp;
    string siftPath = siftFolder + "/" + tmp;
    string tempPath = tempFolder + "/" + tmp;
    string imgPath = queryFolder + "/" + queryName;
    string weightPath = weightFolder + "/" + tmp;
    string termIDPath = termIDFolder + "/" + tmp;

    arma::mat _kp, _sift;
    debugInfo("Extracting queries' features");
    extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath, false);

    arma::vec _weights;
    arma::uvec _termID;

    debugInfo("Building query BOW");
    buildBoW(_sift, _weights, _termID, weightPath, termIDPath, false);
    
    debugInfo("Building query tfidf");
    vector<double> qTfidf = app->ivt.makeQueryTfidf(_weights, _termID);

    debugInfo("Computing queries' ranked list");

    debugInfo("Computing scores");
    Score score(computeAllScores(qTfidf));

    debugInfo("Intializing ranked list");
    vector<int> rankedList(nDocs);
    for (int i = 0; i < nDocs; i++)
        rankedList[i] = i;
    debugInfo("Sorting ranked list");
    sort(rankedList.begin(), rankedList.end(), score);

    debugInfo("Outputing ranked list");
    string rankedListPath = rankedListFolder + "/" + getFileBaseName(queryName) + ".txt";
    FILE *rankedListFile = fopen(rankedListPath.c_str(), "w");
    for (int i = 0; i < nDocs; i++) {
        fprintf(rankedListFile, "%s\n", getFileBaseName(app->path[rankedList[i]]).c_str());
    }
    outputFile = getFileBaseName(queryName) + ".txt";
    fclose(rankedListFile);
}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000], response[2000];
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        string output;
    	processQuery(string(client_message), output);
        sprintf(response, "%s", output.c_str());
        //Send the message back to client
        write(sock , response , strlen(response));
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}

int runServer() {
	int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( portNumber );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = (int *)malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}

#endif // SERVER_H_INCLUDED
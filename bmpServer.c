// Samuel Clarke - z5045865
// Wed13-Sitar
// 16/04/2016
// BMP Mandelbrot server.

/*
 *  Socket code created by Tim Lambert on 02/04/12.
 *  Furthered by Richard Buckland on 28/01/11.
 *  Modifed to serve Mandelbrot BMPs by Sam Clarke 16/04/2016.
 *  Copyright 2012 Licensed under Creative Commons SA-BY-NC 3.0. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "mandelbrot.h"
#include "pixelColor.h"

// General
#define TRUE  1 
#define FALSE 0
// BMP header specific
#define RESERVED         0
#define NUM_COLORS       0
#define BYTES_PER_PIXEL  3
#define BITS_PER_PIXEL   (BYTES_PER_PIXEL*8)
#define NUMBER_PLANES    1
#define PIX_PER_METRE    2835
#define MAGIC_NUMBER     0x4d42
#define NO_COMPRESSION   0
#define OFFSET           54
#define PIXEL_WIDTH      512
#define NUM_PIXELS       (PIXEL_WIDTH * PIXEL_WIDTH)
#define DIB_HEADER_PIXEL_WIDTH 40
// Socket/Server specific
#define SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define NUMBER_OF_PAGES_TO_SERVE 1000

// Function prototypes
int parseRequest( char *request, double *x, double *y, int *z );
int waitForConnection (int serverSocket);
int makeServerSocket (int portno);
void writeHeader ( int socket );
void serveBMP (int socket, double x, double y, int z);
void serveJS( int socket );


int main (int argc, char *argv[]) {
    int z;
    double x, y;
    int numberServed = 0;
    char request[REQUEST_BUFFER_SIZE];
    // Create socket instance
    int serverSocket = makeServerSocket( DEFAULT_PORT );   
    // Print proof-of-life banner
    char *banner = "***********************************************";
    printf( "%s\n", banner );
    printf( "BMP server (version %.1f ALPHA)\n", 
                                                 SERVER_VERSION );
    printf( "Serving sweet looking Mandelbrots since 2016\n" );   
    printf( "Listening on port:%d\n", DEFAULT_PORT );
    printf( "%s\n", banner );
    
    while ( numberServed < NUMBER_OF_PAGES_TO_SERVE ) {
      int bytesRead;
      // Wait for a connection (blocking) 
      int connectionSocket = waitForConnection( serverSocket );
      // Once we get a connection, read the request
      bytesRead = read( connectionSocket, request, sizeof(request)-1 );
      assert(bytesRead >= 0); 
      // Parse the request for X,Y and Z values
      int count = parseRequest( request, &x, &y, &z );
      // If we got all three
      if( count == 3 ) {
          // Serve BMPs to correctly formatted URL
          serveBMP( connectionSocket, x, y, z );
          numberServed++;
      // Otherwise it could be a root request
      } else {
          // Init check char to NUL
          char rootCheck = '\x00';
          // Check for root request
          sscanf( request, "GET /%cHTTP", &rootCheck );
          // If it is, then
          if( rootCheck == ' ' ){
              // Serve JS to root directory
              serveJS( connectionSocket );
              numberServed++;
          }
      }
      // We're either done, or it was 
      // a bad request. Close connection
      close( connectionSocket );
    }
    // Served up max pages, shut down
    close ( serverSocket );
    return EXIT_SUCCESS; 
}


int parseRequest( char *request, double *x, double *y, int *z ){
    // Remove information using a format string
    int count = sscanf(request, "GET /tile_x%lf_y%lf_z%d.bmp", x, y, z);
    // Return number of items found
    return count;
}


void serveJS( int socket ){
    char *message;
    // Tiny HTML site to call JS
    message = "HTTP/1.0 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "\r\n"
              "<!DOCTYPE html>\r\n"
              "<script src="
              "\"http://almondbread.cse.unsw.edu.au/tiles.js\">"
              "</script>\r\n"
              "</html>\r\n";
    write( socket, message, strlen(message) );
}


void serveBMP ( int socket, double x, double y, int z ) {
    char* message;
    // HTTP response header
    message = "HTTP/1.0 200 OK\r\n"
              "Content-Type: image/bmp\r\n"
              "\r\n";
    // Send HTTP response header
    write( socket, message, strlen(message) );
    // Followed by BMP header
    writeHeader( socket );
    // through x,y (i,j)
    int i, j, r, g, b, steps;
    // scale = 2^(-zoom)
    double scale = pow( 2, -z );
    // From bottom left pixel
    for( j = -256; j < 256; j++ ){
      for ( i = -256; i < 256; i++ ){
          // Get escape steps for that scaled location
          steps = escapeSteps( x+i*scale, y+j*scale );
          // Get RGB values for steps
          r = stepsToRed( steps );
          g = stepsToGreen( steps );
          b = stepsToBlue( steps );
          // Write BGR pixels to socket
          write( socket, &b, sizeof( uint8_t ) );
          write( socket, &g, sizeof( uint8_t ) );
          write( socket, &r, sizeof( uint8_t ) );
      }
    }
}


// start the server listening on the specified port number
int makeServerSocket( int portNumber ) { 
    // create socket
    int serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
    assert(serverSocket >= 0);      
    // bind socket to listening port
    struct sockaddr_in serverAddress;
    memset( (char *) &serverAddress, 0, sizeof(serverAddress) );
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port        = htons( portNumber );
    // let the server start immediately after a previous shutdown
    int optionValue = 1;
    setsockopt (
        serverSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &optionValue, 
        sizeof( int )
    );
    int bindSuccess = 
        bind (
           serverSocket, 
           (struct sockaddr *) &serverAddress,
           sizeof( serverAddress )
        );
    assert( bindSuccess >= 0 );
    // if this assert fails wait a short while to let the 
    // operating system clear the port.
    return serverSocket;
}


// wait for a browser to request a connection, return socket.
int waitForConnection( int serverSocket ) {
    // listen for a connection
    const int serverMaxBacklog = 10;
    listen (serverSocket, serverMaxBacklog);
    // Could check IP/USER AGENT etc here
    // accept the connection
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof( clientAddress );
    int connectionSocket = 
        accept (
           serverSocket, 
           (struct sockaddr *) &clientAddress, 
           &clientLen
        );
    assert( connectionSocket >= 0 );
    // error on accept
    return connectionSocket;
}


void writeHeader ( int socket ) {
    // File signature - not magic :/
    uint16_t magicNumber = MAGIC_NUMBER;
    write( socket, &magicNumber, sizeof(uint16_t) );
    //Filesize (bytes)
    uint32_t fileSze = OFFSET+(PIXEL_WIDTH * 
                                PIXEL_WIDTH * BYTES_PER_PIXEL);
    write( socket, &fileSze, sizeof(fileSze) );
    // Reserved
    uint32_t reserved = 0;
    write( socket, &reserved, sizeof(uint32_t) );
    // Pixel data offset
    uint32_t offset = OFFSET;
    write( socket, &offset, sizeof(uint32_t) );
    // DIB header size
    uint32_t dibHeaderSize = DIB_HEADER_PIXEL_WIDTH;
    write( socket, &dibHeaderSize, sizeof(uint32_t) );
    // Width and height (pixels)
    uint32_t width = PIXEL_WIDTH;
    uint32_t height = width;
    write( socket, &width, sizeof(uint32_t) );
    write( socket, &height, sizeof(uint32_t) );
    // Plane
    uint16_t planes = NUMBER_PLANES;
    write( socket, &planes, sizeof(uint16_t) );
    // Bit depth
    uint16_t bitsPerPixel = BITS_PER_PIXEL;
    write( socket, &bitsPerPixel, sizeof(uint16_t) );
    // Compression type
    uint32_t compression = NO_COMPRESSION;
    write( socket, &compression, sizeof(uint32_t) );
    // Image size (bytes)
    uint32_t imageSze = (PIXEL_WIDTH * PIXEL_WIDTH * BYTES_PER_PIXEL);
    write( socket, &imageSze, sizeof(imageSze) );
    // Image resolution
    uint32_t hResolution = PIX_PER_METRE;
    uint32_t vResolution = hResolution;
    write( socket, &hResolution, sizeof(uint32_t) );
    write( socket, &vResolution, sizeof(uint32_t) );
    // Colour info
    uint32_t numColors = NUM_COLORS;
    uint32_t importantColors = numColors;
    write( socket, &numColors, sizeof(uint32_t) );
    write( socket, &importantColors, sizeof(uint32_t) );
}
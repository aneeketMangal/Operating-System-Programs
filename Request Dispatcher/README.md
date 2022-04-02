
# CS303 Assignment 1
Submitter name: Aneeket Mangal\
Roll No.: 2019CSB1071\
Course:  CS303

## 1. What does this program do
This C program simulates how processes are managed by the dispatcher in an OS. The program is made to run on a Linux OS (Tested on Ubuntu 20.04). 
It involves communication between a multi threaded server and clients, where client request to load a DLL function. The socket connection spawns new threads to handle the queries/messages sent by clients.

## 2. A description of how this program works 
The following the process flow of the program:-
* A server is first set up that listens to the incoming client request.
* A client should send the request at the port at which our server is listening.
* The server validate and serialise the request message and create a ```RequestBody``` struct. 
* ```RequestQueue``` is a data structure that stores the incoming request in a ```FIFO``` manner. It is implemented using a array of ```struct RequestBody``` which contains details of a request.
* If the ```RequestQueue``` is not full, then the ```RequestBody``` is enqueued for further processing.
*  While the enqueue process, the queue is locked using ```pthread_mutex_lock``` so as to ensure no other thread changes it during the process. A signal is sent to dispatcher after enqueue, and further a suspended thread tries to look for a request.
* A dispatcher is also put in place as a separate thread process which looks for unprocessed requests in the ```RequestQueue```.
* Each request is processed on a separate thread.
*  For processing a request, it is first dequeued from the ```RequestQueue``` and then sent to DLL invoker which then loads the library and completes the request.
* Supported DLL functions: \
"cos", "sin", "tan", "tanh", "cosh", "sinh", "ceil", "floor","pow", "sqrt", "log"

## 3. How to compile and run this program


* ### To run the client
    * Navigate to ```/source``` inside main directory.
    * Compile using ```gcc client.c -o client```.
    * Run the client using ```./client hostname port```.
    * Enter a request query.
    * A request message should be of the format ```arg1,arg2,arg3....``` where the first two arguements i.e.  ```arg1``` and ```arg2``` denotes the DLL name and the function name respectively and the others denote the function arguements.
    * A sample request query looks like: \
    ```/lib/x86_64-linux-gnu/libm.so.6,sqrt,196```
* ### To run the server
    * Navigate to ```/source``` inside main directory.
    * Compile using ```gcc server.c -o server -pthread -ldl```.
    * Run the client using ```./server [port] [dispatcher thread pool size] [file pool size] [memory pool size] port```.
* ### To run the server
    * Navigate to ```\test``` inside main directory.
    * There are 9 test files, that test the dll invoker module.
    * All these test are located in the test folder.
    * To run all the tests together, first compile using ```gcc test_dll_invoker.c -o test```.
    * Run the client using ```./test```.

## 4. Provide a snapshot of a sample run
* Starting the server
![1](images/1.png)
* Running multiple request
![1](images/2.png)
* Test results
![1](images/3.png)

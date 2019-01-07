# Distributed-Lookup-Service
This is a network socket programming project to achieve the distributed lookup service. The project managed data transfer over TCP and UDP by setting up stream sockets at client side/AWS and datagram sockets at AWS/back-end server and implemented prefix and suffix search function between client, AWS and back-end server through sockets.
## Each code file:
1. serverA.c: boot up and set up a UDP connection with AWS through source port number 21482 and destination port number 24482, after receiving the "function" and "input" from AWS, do the according function in file "backendA.txt" and get the result of it.
2. serverB.c: boot up and set up a UDP connection with AWS through source port number 22482 and destination port number 24482, after receiving the "function" and "input" from AWS, do the according function in file "backendB.txt" and get the result of it.
3. serverC.c: boot up and set up a UDP connection with AWS through source port number 23482 and destination port number 24482, after receiving the "function" and "input" from AWS, do the according function in file "backendB.txt" and get the result of it.
4. aws.c: boot up and set up a UDP connection with back-end server A, B and C through source port number 24482 and destination port number 21482, 22482 and 23482, receive the "function" and "input" from client by TCP connection through port number 25482, send the receiving "function" and "input" to all three back-end servers, set up a TCP connection with monitor through port number 26482.
5. client.c: boot up and set up a TCP connection with AWS, read the input from the keyboard and send it to AWS.
6. monitor.c: boot up and set up a TCP connection with AWS.
## Sample input & output:
1. serverA:
The Server A is up and running using UDP on port 21482.
The Server A received input <Graduate> and operation <search> 
The Server A has found < 1 > matches
The Server A received input <Grad> and operation <prefix> 
The Server A has found < 2 > matches
The Server A received input <ate> and operation <suffix> 
The Server A has found < 36 > matches
2. serverB:
The Server B is up and running using UDP on port 22482.
The Server B received input <Graduate> and operation <search> 
The Server B has found < 1 > matches
The Server B received input <Grad> and operation <prefix> 
The Server B has found < 3 > matches
The Server B received input <ate> and operation <suffix> 
The Server B has found < 39 > matches
3. serverC:
The Server C is up and running using UDP on port 23482.
The Server C received input <Graduate> and operation <search> 
The Server C has found < 0 > matches
The Server C received input <Grad> and operation <prefix> 
The Server C has found < 2 > matches
The Server C received input <ate> and operation <suffix> 
The Server C has found < 31 > matches
4. aws:
The AWS is up and running. 
The AWS received input=Graduate and function=search from the client using TCP over port 20448. 
The AWS sent <Graduate> and <search> to Backend-Server A 
The AWS sent <Graduate> and <search> to Backend-Server B 
The AWS sent <Graduate> and <search> to Backend-Server C 
The AWS received input=Grad and function=prefix from the client using TCP over port 22496. 
The AWS sent <Grad> and <prefix> to Backend-Server A 
The AWS sent <Grad> and <prefix> to Backend-Server B 
The AWS sent <Grad> and <prefix> to Backend-Server C 
The AWS received input=ate and function=suffix from the client using TCP over port 22752. 
The AWS sent <ate> and <suffix> to Backend-Server A 
The AWS sent <ate> and <suffix> to Backend-Server B 
The AWS sent <ate> and <suffix> to Backend-Server C 
5. monitor:
The monitor is up and running. 
6. client:
INPUT: ./client search Graduate
The client is up and running. 
The client sent Graduate and search to AWS.
INPUT: ./client prefix Grad
The client is up and running. 
The client sent Grad and prefix to AWS.
INPUT: ./client suffix ate
The client is up and running. 
The client sent ate and suffix to AWS.

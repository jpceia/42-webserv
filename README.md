# HTTP:

### What is HTTP? 

    HTTP stands for Hyper Text Transfer Protocol.
    WWW is about communication between web clients and servers.
    Communication between client computers and web servers is done by sending HTTP Requests and
    receiving HTTP Responses.

    Clients are often browsers (Chrome, Edge, Safari), but they can be any type of program or device.
    Servers are most often computers in the cloud.

### HTTP Request / Response 

    Communication between clients and servers is done by requests and responses:

    1. A client (a browser) sends an HTTP request to the web.
    2. A web server receives the request.
    3. The server runs an application to process the request.
    4. The server returns an HTTP response (output) to the browser.
    5. The client (the browser) receives the response.

### The HTTP Request Circle

    A typical HTTP request / response circle:

    1. The browser requests an HTML page. The server returns an HTML file.
    2. The browser requests a style sheet. The server returns a CSS file.
    3. The browser requests an JPG image. The server returns a JPG file.
    4. The browser requests JavaScript code. The server returns a JS file.
    5. The browser requests data. The server returns data (in XML or JSON).


# HTML:

### What is HTML?
    
    HTML stands for Hyper Text Markup Language.
    HTML is the standard markup language for Web pages.
    HTML elements are the building blocks of HTML pages.
    HTML elements are represented by <> tags.


# OSI Model

https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

### What is OSI Model (Open Systems Interconnection Model)

    It's a conceptual model (a representation of a system, consisting of concepts used to help
    people know, understand, or simulate a subject the model represents) that characterises
    and standardises the communication functions of a telecommunication or computing system without
    regard to its underlying internal structure and technology (example: Mac to a Windows).
    Its goal is the interoperability of diverse communication systems with standard communication
    protocols. 

    The OSI model characterizes computing functions into a universal set of rules and requirements in
    order to support interoperability between different products and software.    

    The communications between a computing system are split into seven different abstraction layers:
    Physical, Data Link, Network, Transport, Session, Presentation, and Application.

    OSI was published in 1984 by the International Organization for Standardization (ISO). Though it
    does not always map directly to specific systems, the OSI Model is still used today as a means
    to describe Network Architecture.

### 7 Layers of OSI Model

            Application Layer                   \
            Presentation Layer                   > Software Layers
            Session Layer                       /
            Transport Layer     > Heart of OSI
            Network Layer                       \
            Data Link Layer                      > Hardware Layers
            Physical Layer                      /


    Layer 1 - Physical Layer:

        The lowest layer of the OSI Model is concerned with electrically or optically transmitting raw
        unstructured data bits across the network from the physical layer of the sending device to the
        physical layer of the receiving device. It can include specifications such as voltages, pin layout,
        cabling, and radio frequencies. At the physical layer, one might find “physical” resources such as
        network hubs, cabling, repeaters, network adapters or modems.

    Layer 2 – Data Link

        The Data Link Layer provides node-to-node data transfer (between two directly connected nodes),
        and also handles error correction from the physical layer. In the networking world, most switches
        operate at Layer 2. But it’s not that simple. Some switches also operate at Layer 3 in order to
        support virtual LANs that may span more than one switch subnet, which requires routing capabilities.
   
    Layer 3 - Network

        Here at the Network Layer is where you’ll find most of the router functionality that most networking
        professionals care about and love. In its most basic sense, this layer is responsible for packet
        forwarding, including routing through different routers. You might know that your Boston computer
        wants to connect to a server in California, but there are millions of different paths to take. Routers
        at this layer help do this efficiently.

    Layer 4 – Transport

        The Transport Layer deals with the coordination of the data transfer between end systems and hosts.
        How much data to send, at what rate, where it goes, etc. The best known example of the Transport Layer is
        the Transmission Control Protocol (TCP), which is built on top of the Internet Protocol (IP), commonly known
        as TCP/IP. TCP and UDP port numbers work at Layer 4, while IP addresses work at Layer 3, the Network Layer.

    Layer 5 - Session

        When two computers or other networked devices need to speak with one another, a session needs to be created,
        and this is done at the Session Layer. Functions at this layer involve setup, coordination (how long should
        a system wait for a response, for example) and termination between the applications at each end of the session.

    Layer 6 - Presentation

        The Presentation Layer represents the area that is independent of data representation at the application
        layer. In general, it represents the preparation or translation of application format to network format, or
        from network formatting to application format. In other words, the layer “presents” data for the application
        or the network. A good example of this is encryption and decryption of data for secure transmission; this
        happens at Layer 6.

    Layer 7 - Application Layer:

        At this layer, both the end user and the application layer interact directly with the software
        application. This layer sees network services provided to end-user applications such as a web
        browser or Office 365. The application layer identifies communication partners, resource
        availability, and synchronizes communication.


#### Layer 4 – Transport:

    The transport layer's tasks include error correction as well as segmenting and desegmenting data before
    and after it's transported across the network. This layer is also responsible for flow control (the
    mechanism that ensures the rate at which a sender is transmitting is in proportion with the receiver’s
    receiving capabilities) and making sure that segmented data is delivered over the network in the correct sequence.

    Layer 4 (the transport layer) uses the transmission control protocol (TCP) & user data protocol (UDP) to
    carry out its tasks. 

    TCP:

        TCP is a layer 4 protocol which provides acknowledgement of the received packets and is also reliable as it
        resends the lost packets. It is better than UDP but due to these features it has an additional overhead. It is
        used by application protocols like HTTP and FTP. 

    UDP:

        UDP is also a layer 4 protocol but unlike TCP it doesn’t provide acknowledgement of the sent packets. Therefore,
        it isn’t reliable and depends on the higher layer protocols for the same. But on the other hand it is simple,
        scalable and comes with lesser overhead as compared to TCP. It is used in video and voice streaming. 


# RCA (Request for Comments)

### What is RCA:

    A Request for Comments (RFC) is a formal document from the Internet Engineering Task Force ( IETF ) that is the
    result of committee drafting and subsequent review by interested parties. Some RFCs are informational in nature.
    Of those that are intended to become Internet standards, the final version of the RFC becomes the standard and no
    further comments or changes are permitted.

    HTTP server RFC documents are at:
 
        RFC 7230    -   https://www.rfc-editor.org/info/rfc7230
        RFC 7231    -   https://www.rfc-editor.org/info/rfc7231
        RFC 7232    -   https://www.rfc-editor.org/info/rfc7232
        RFC 7233    -   https://www.rfc-editor.org/info/rfc7233
        RFC 7234    -   https://www.rfc-editor.org/info/rfc7234
        RFC 7235    -   https://www.rfc-editor.org/info/rfc7235



# Socket

### What is Socket:

    A socket is the mechanism that most popular operating systems provide to give programs access to the network.
    It allows messages to be sent and received between applications (unrelated processes) on different networked machines.

    The sockets mechanism has been created to be independent of any specific type of network. IP, however, is by far
    the most dominant network and the most popular use of sockets.

### How it Works:

    Normally, a server runs on a specific computer and has a socket that is bound to a specific port number.
    The server just waits, listening to the socket for a client to make a connection request.

    On the client-side: The client knows the hostname of the machine on which the server is running and the port number
    on which the server is listening. To make a connection request, the client tries to rendezvous with the server on
    the server's machine and port. The client also needs to identify itself to the server so it binds to a local port
    number that it will use during this connection. This is usually assigned by the system.

    If everything goes well, the server accepts the connection. Upon acceptance, the server gets a new socket bound to
    the same local port and also has its remote endpoint (combination of an IP address and a port number) set to the
    address and port of the client. It needs a new socket so that it can continue to listen to the original socket
    for connection requests while tending to the needs of the connected client.

    On the client side, if the connection is accepted, a socket is successfully created and the client can use the
    socket to communicate with the server.

    The client and server can now communicate by writing to or reading from their sockets.


# Research:

    https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
    https://ncona.com/2019/04/building-a-simple-server-with-cpp/

    https://www.youtube.com/watch?v=Kc1kwm1WyVM
    https://www.youtube.com/watch?v=YqEqjODUkWY



# Git:

### Go to a Branch

    git clone https://github.com/jpceia/42-webserv/tree/master
    git checkout dock

### Make a new Branch

    git clone https://github.com/jpceia/42-webserv/tree/master
    git branch dock
    git add
    git commit
    git push

### Make a new Branch and change to it

    git clone https://github.com/jpceia/42-webserv/tree/master
    git checkout -b [name_of_your_new_branch]
    git add
    git commit
    git push -u origin [name_of_your_new_branch]

### Commit to a Branch

    git add .
    git commit -m "bla bla"
    git push -u origin docker

### Merge to a Dev Branch

    git checkout dock
    git merger dev
    [solve potential conflicts]
    [git add <conflicting files>]
    [git commit]
    git push
    git checkout dev
    git merge dock
    git push

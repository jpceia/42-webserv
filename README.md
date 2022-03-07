# HTTP:

### What is HTTP?

    HTTP stands for Hyper Text Transfer Protocol.
    Hyper Text = It's the HTML part of the Hyper Text.
    Transfer Protocol = How a message is formated when sent back.

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

### Programming with TCP/IP sockets

    There are a few steps involved in using sockets:

        1- Create the socket

            A socket, is created with the socket system call
            int socket_fd = socket(domain, type, protocol);

        2- Identify the socket

            When we talk about naming a socket, we are talking about assigning a transport
            address to the socket (a port number in IP networking). In sockets, this operation
            is called binding an address and the bind system call is used for this.

            int bind(int socket, const struct sockaddr *address, socklen_t address_len);

            struct sockaddr_in {
                __uint8_t         sin_len;
                sa_family_t       sin_family;
                in_port_t         sin_port;
                struct in_addr    sin_addr;
                char              sin_zero[8];
            };

        3- On the server, wait for an incoming connection

            Before a client can connect to a server, the server should have a socket that is
			prepared to accept the connections. The listen system call tells a socket that
			it should be capable of accepting incoming connections.

        4- Send and receive messages

            The same read and write system calls that work on files also work on sockets.

            int valread = read( new_socket , buffer, 1024);
            printf(“%s\n”,buffer );
            if(valread < 0)
            {
                printf("No bytes are there to read");
            }

        5- Close the socket

            Close a socket with the close system call — the same close that is used for files.

# Select()

### What is Select?

    select() allow a program to monitor multiple file descriptors, waiting until one or more of the file
    descriptors become "ready" for some class of I/O operation (e.g., input possible).
    A file descriptor is considered ready if it is possible to perform the corresponding I/O operation (e.g., read(2))
    without blocking.

    It takes a group of File Descriptors. Select() is going to tell you when there is something to read or write
    on any of them.

    Select() is destructive. It destroys the fd_set each time, that's why you should keep 2 copies of it.

### fd_set

    A collection of file descriptors sets.

        {
            fd_set      current_sockets; // This one goes on Select() and gets destroyed
            fd_set      ready_sockets;   // This one is a copy of current_sockets. You keep two, because
                                         // current_sockets get destroyed.
        }

### FD_ZERO

    Zero out or initialize a current set.

        {
            fd_set      current_sockets;
            fd_set      ready_sockets;

            FD_ZERO(&current_sockets);
        }

### FD_SET

    This macro adds the file descriptor fd to set. Adding a file descriptor that is already present in the set is a
    no-op, and does not produce an error.

        {
            int                             server_socket;
            struct sockaddr_in              _address;

            fd_set                          current_sockets;
            fd_set                          ready_sockets;

            if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                exit(EXIT_FAILURE);

            // We fill the sockaddr_in struct
            _address.sin_family = AF_INET;
            inet_pton(AF_INET, _ipAddress, &_address.sin_addr);
            _address.sin_port = htons(port);
            memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

            if (bind(server_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
                exit(EXIT_FAILURE);
            if (listen(server_socket, SOMAXCONN) < 0)
                exit(EXIT_FAILURE);

            FD_ZERO(&current_sockets);
            FD_SET(server_socket, &current_sockets);
        }

### Select()

    int select(int nfds,                                // Range of file descriptors to check
               fd_set *restrict readfds,                // Check file descriptors for Reading
               fd_set *restrict writefds,               // Check file descriptors for Writing
               fd_set *restrict exceptfds,              // Check file descriptors for Errors
               struct timeval *restrict timeout);       // Timeout to wait a certain time for changes


# Poll()

### What is Poll?

    The poll() function provides applications with a mechanism for multiplexing input/output over a set of file
    descriptors. For each member of the array pointed to by fds, poll() shall examine the given file descriptor
    for the event(s) specified in events.
    The poll() function shall identify those file descriptors on which an application can read or write data,
    or on which certain events have occurred.

    It is used to watch for file descriptors and see if any of them are ready for an I/O operation.

### Format

				int poll(struct pollfd fds[], nfds_t nfds, int timeout);

	fds:
		An array of pollfd structures.

		struct pollfd
		{
			int fd;					/* File descriptor to poll.  */
			short int events;		/* Types of events poller cares about.  */
			short int revents;		/* Types of events that actually occurred.  */
		};

		fd:
			An open file descriptor. The file descriptor can be a nonsocket file descriptor or
			a socket descriptor.

		events (Requested Events):
			Event types that can be polled for.  These bits may be set in `events'
			to indicate the interesting event types; they will appear in `revents'
			to indicate the status of the file descriptor.

			POLLIN				/* There is data to read.  */
			POLLPRI				/* There is urgent data to read.  */

			POLLOUT				/* Writing now will not block.  */

			POLLRDNORM			/* Normal data may be read.  */
			POLLRDBAND			/* Priority data may be read.  */

			POLLWRNORM			/* Writing now will not block. Equivalent to POLLOUT */
			POLLWRBAND			/* Priority data may be written.  */

			These events can be together. For instance POLLIN can be POLLRDNORM | POLLRDBAND.

		revents (Returned Events):
			Returned events. All requested events can be returned events,
			in addition to following:

			POLLERR
				An error has occurred. This option has meaning only for sockets.

			POLLHUP
				A connected socket has shut down on both ends. This option has meaning
				only for TCP sockets. POLLHUP and POLLOUT (or POLLWRNORM) are mutually
				exclusive.

			POLLNVAL
				The specified fd value is not valid.

	nfds:
		An unsigned integer indicating the number of pollfd structures in the fds array.

	timeout:
		Maximum time, in milliseconds, to wait for the poll function to complete.

		A value of 0 means poll does not wait before returning to the caller.
		A value of -1 means poll waits for the requested event to occur or for an interrupt.


    Upon successful completion, poll() shall return a non-negative value. A positive value
	indicates the total number of file descriptors that have been selected (that is, file
	descriptors for which the revents member is non-zero). A value of 0 indicates that the
	call timed out and no file descriptors have been selected. Upon failure, poll() shall
	return -1 and set errno to indicate the error.

	https://www.ibm.com/docs/en/ztpf/1.1.0.15?topic=apis-pollmonitor-activity-file-descriptors
	https://pubs.opengroup.org/onlinepubs/009696799/functions/poll.html


# NGINX:

### What is NGINX?

	NGINX is open source software for web serving, reverse proxying, caching, load balancing,
	media streaming, and more. It started out as a web server designed for maximum performance
	and stability. In addition to its HTTP server capabilities, NGINX can also function as a
	proxy server for email (IMAP, POP3, and SMTP) and a reverse proxy and load balancer for
	HTTP, TCP, and UDP servers.

### NGINX Configuration File

	NGINX consists of modules which are controlled by directives specified in the configuration
	file. Directives are divided into simple directives and block directives. A block directive
	has the same structure as a simple directive, but instead of the semicolon it ends with
	a set of additional instructions surrounded by braces ({ and }). If a block directive can
	have other directives inside braces, it is called a context (examples: events, http,
	server, and location).

	Directives placed in the configuration file outside of any contexts are considered to
	be in the main context. The events and http directives reside in the main context, server
	in http, and location in server.

### Server Block and Location Block

	A server block is a subset of Nginx’s configuration that defines a virtual server used
	to handle requests of a defined type. Administrators often configure multiple server
	blocks and decide which block should handle which connection based on the requested
	domain name, port, and IP address.

	A location block lives within a server block and is used to define how Nginx should
	handle requests for different resources and URIs for the parent server. The URI space
	can be subdivided in whatever way the administrator likes using these blocks. It is an
	extremely flexible model.

### Example of Server Block

	server {
		listen		443;
		error 		PWD/www/errorPages;
		location / {
			methods 	GET, POST;
			root 		PWD/www/content;
			index 		oldindex.html;
		}
	}

	server {
		listen 		80;
		server_name	test;
		error 		PWD/www/errorPages;
		location / {
			index 	oldindex.html;
			methods GET;
			root 	PWD/www/content;
		}
		location /directory {
			methods		GET, POST;
			root 		PWD/www/YoupiBanane;
			index 		youpi.bad_extension;
			CGI 		.bla;
			exec 		PWD/cgi_tester;
		}
		location /put_test {
			methods		PUT;
			root 		PWD/www/content;
		}
		location /post_body {
			methods 	POST;
			root 		PWD/www/content;
			index 		oldindex.html;
			max_body 	100;
		}
	}

# Subjects Configuration File

### What it accepts

	If the server block is missing, then the configuration file is invalid.
	You can have more than one server block in the configuration file.
	You can have an empty server block, if it has an empty server block it will be
	filled with the default values.

	Built-in:

		- listen ( http://nginx.org/en/docs/http/ngx_http_core_module.html#listen )
		- server_name ( http://nginx.org/en/docs/http/ngx_http_core_module.html#server_name )
		- error_page ( http://nginx.org/en/docs/http/ngx_http_core_module.html#error_page )
		- client_max_body_size ( http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size )
		- root ( http://nginx.org/en/docs/http/ngx_http_core_module.html#root )
		- index ( http://nginx.org/en/docs/http/ngx_http_index_module.html#index )
		- auto-index ( http://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex )

	Not Built-in:

		- methods ( GET, POST, DELETE )
		- redirect ( first arg a 300 status code, second arg the path to redirect )
		- cgi
		- upload ( bool if it accepted or not, and the path )

### Defaults

    - listen
        ip = 0.0.0.0
        port = 80
    - server_name
        server_name "";
        (If no server_name is defined in a server block then nginx uses the empty name as the server name.)
    - error_page
        error_page { -- };
    - client_max_body_size
        client_max_body_size 1m;
    - root
        The path where wwwroot is.
    - index
        index index.html;
    - auto-index
        autoindex off;
    
    - methods
        methods GET;
    - redirect 
        redirect { -- }
    - cgi
        cgi { -- }
    - upload
        off




	// https://github-com.translate.goog/GuDimz/Webserver?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=pt-PT&_x_tr_pto=wapp

	// https://github.com/tvacherat/42_webserv


# Research:

    https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
    https://ncona.com/2019/04/building-a-simple-server-with-cpp/

    https://www.youtube.com/watch?v=Kc1kwm1WyVM
    https://www.youtube.com/watch?v=YqEqjODUkWY

	https://www.digitalocean.com/community/tutorials/understanding-the-nginx-configuration-file-structure-and-configuration-contexts


	https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms

	http://nginx.org/en/docs/dirindex.html



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

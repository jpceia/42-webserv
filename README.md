## HTTP:
<code>

What is HTTP?

    HTTP stands for Hyper Text Transfer Protocol.
    WWW is about communication between web clients and servers.
    Communication between client computers and web servers is done by sending HTTP Requests and receiving HTTP Responses.

    Clients are often browsers (Chrome, Edge, Safari), but they can be any type of program or device.
    Servers are most often computers in the cloud.

HTTP Request / Response

    Communication between clients and servers is done by requests and responses:

    1. A client (a browser) sends an HTTP request to the web.
    2. A web server receives the request.
    3. The server runs an application to process the request.
    4. The server returns an HTTP response (output) to the browser.
    5. The client (the browser) receives the response.

The HTTP Request Circle

    A typical HTTP request / response circle:

    1. The browser requests an HTML page. The server returns an HTML file.
    2. The browser requests a style sheet. The server returns a CSS file.
    3. The browser requests an JPG image. The server returns a JPG file.
    4. The browser requests JavaScript code. The server returns a JS file.
    5. The browser requests data. The server returns data (in XML or JSON).</code>


## HTML:
<code>

What is HTML?
    
    HTML stands for Hyper Text Markup Language.
    HTML is the standard markup language for Web pages.
    HTML elements are the building blocks of HTML pages.
    HTML elements are represented by <> tags.
</code>



## Git:

<code>

Go to a Branch:

    git clone https://github.com/jpceia/42-webserv/tree/master
    git checkout dock

Make a new Branch:

    git clone https://github.com/jpceia/42-webserv/tree/master
    git branch dock
    git add
    git commit
    git push

Make a new Branch and change to it:

    git clone https://github.com/jpceia/42-webserv/tree/master
    git checkout -b [name_of_your_new_branch]
    git add
    git commit
    git push -u origin [name_of_your_new_branch]

Commit to a Branch:

    git add .
    git commit -m "bla bla"
    git push -u origin docker

Merge to a Dev Branch:

    git checkout dock
    git merger dev
    [solve potential conflicts]
    [git add <conflicting files>]
    [git commit]
    git push
    git checkout dev
    git merge dock
    git push
</code>

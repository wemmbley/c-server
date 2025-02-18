# C-Server with PHP interpreter WIN32

When I set out to understand how the web itself works, I decided to create my own server that would respond with raw responses to understand the basic principle. 

Having implemented this, I went further - adding a PHP interpreter as an intermediary between request-response; input-output.

And so this work of art came out, launched from under Windows on local port 8080. 

I haven’t optimized anything, it saves the output locally, parses it and sends it to the client.

## Compile

`gcc server.c -o server.exe -lws2_32`

Be sure you installed PHP interpreter locally.

`php -v`

## Run 

Just start `server.exe` program and run `http://localhost:8080`.

🚀 You're welcome!

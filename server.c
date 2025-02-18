#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Подключаем библиотеку WinSock

int main() {
    // Инициализация структур данных
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_size;
    char buffer[1024];

    // Инициализация WinSock
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Настройка адреса сервера
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Привязка сокета
    bind(server_socket, (struct sockaddr*)&server, sizeof(server));
    
    // Ожидание подключений
    listen(server_socket, 3);
    
    printf("Server started at port 8080...\n");
    
    while (1) {
        // Принятие входящего соединения
        client_size = sizeof(client);
        client_socket = accept(server_socket, (struct sockaddr*)&client, &client_size);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed!\n");
            continue;
        }

        printf("Client connected!\n");

        // Чтение данных от клиента
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Добавляем завершающий нулевой символ
            printf("Received: %s\n", buffer);
        }

        // Запускаем PHP интепретатор и кладем содержимое ответа в текстовый файл
        system("php -f server.php > output.txt");

        FILE* file = fopen("output.txt", "rb");  // Открываем файл в бинарном режиме

        // Определяем размер файла
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        // Выделяем память и читаем файл
        char* file_content = (char*)malloc(file_size);
        fread(file_content, 1, file_size, file);
        fclose(file);

        // Формируем HTTP-ответ
        char header[256];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n",
            file_size);

        // Отправляем заголовки
        send(client_socket, header, strlen(header), 0);

        // Отправляем содержимое файла
        send(client_socket, file_content, file_size, 0);

        free(file_content);
    }

    // Закрытие соединений
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

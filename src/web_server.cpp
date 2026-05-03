#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "web_server.h"
#include "database.h"
#include "estoque.h"

#pragma comment(lib, "ws2_32.lib")

// Função auxiliar para escapar strings para JSON
std::string escapeJSON(const char* input) {
    if (!input) return "";
    std::string output = "";
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '"') output += "\\\"";
        else if (input[i] == '\\') output += "\\\\";
        else if (input[i] == '\n') output += "\\n";
        else output += input[i];
    }
    return output;
}

static void enviarArquivo(SOCKET client, const char* caminho, const char* contentType) {
    FILE* f = fopen(caminho, "rb");
    if (!f) {
        char err[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client, err, strlen(err), 0);
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char header[256];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n", contentType, size);
    send(client, header, strlen(header), 0);

    char buffer[8192];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        send(client, buffer, n, 0);
    }
    fclose(f);
}

static void enviarJSONProdutos(SOCKET client) {
    std::string json = "[";
    
    if (mysql_query(conexao, "SELECT p.codigo, p.nome, p.preco, c.nome, p.quantidade FROM produtos p JOIN categorias c ON p.categoria_id = c.id")) {
        json = "{\"error\": \"Erro ao ler banco\"}";
    } else {
        MYSQL_RES *res = mysql_store_result(conexao);
        MYSQL_ROW row;
        bool first = true;
        while ((row = mysql_fetch_row(res))) {
            if (!first) json += ",";
            char item[1024];
            sprintf(item, "{\"id\":%s,\"nome\":\"%s\",\"preco\":%s,\"categoria\":\"%s\",\"estoque\":%s}", 
                    row[0], escapeJSON(row[1]).c_str(), row[2], escapeJSON(row[3]).c_str(), row[4]);
            json += item;
            first = false;
        }
        mysql_free_result(res);
        json += "]";
    }

    char header[512];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n", json.length());
    send(client, header, (int)strlen(header), 0);
    send(client, json.c_str(), (int)json.length(), 0);
}

void iniciarServidorWeb(int porta) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(porta);

    // Reuso de endereço para evitar erro de porta ocupada
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Erro ao iniciar servidor na porta %d\n", porta);
        return;
    }
    
    listen(server, 10);

    printf("\n  [ WEB ] SERVIDOR ONLINE em http://localhost:%d\n", porta);
    printf("  [ WEB ] Abra o navegador e use o sistema normalmente.\n");

    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET) continue;

        char request[4096] = {0};
        recv(client, request, sizeof(request), 0);

        if (strstr(request, "GET /api/produtos")) {
            enviarJSONProdutos(client);
        } else if (strstr(request, "GET /style.css")) {
            enviarArquivo(client, "style.css", "text/css");
        } else if (strstr(request, "GET /script.js")) {
            enviarArquivo(client, "script.js", "application/javascript");
        } else if (strstr(request, "GET /favicon.ico")) {
            // Ignorar favicon
        } else {
            enviarArquivo(client, "index.html", "text/html");
        }
        closesocket(client);
    }
    WSACleanup();
}

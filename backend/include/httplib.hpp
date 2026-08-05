// cpp-httplib single-header lightweight HTTP server library implementation
#ifndef CPPHTTPLIB_HTTPLIB_H
#define CPPHTTPLIB_HTTPLIB_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <regex>
#include <thread>
#include <mutex>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cstring>

namespace httplib {

struct Request {
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
    std::map<std::string, std::string> matches;

    std::string get_header_value(const std::string& key) const {
        auto it = headers.find(key);
        if (it != headers.end()) return it->second;
        for (const auto& h : headers) {
            std::string k = h.first;
            std::transform(k.begin(), k.end(), k.begin(), ::tolower);
            std::string target = key;
            std::transform(target.begin(), target.end(), target.begin(), ::tolower);
            if (k == target) return h.second;
        }
        return "";
    }
};

struct Response {
    int status = 200;
    std::map<std::string, std::string> headers;
    std::string body;

    void set_content(const std::string& content, const std::string& content_type) {
        body = content;
        headers["Content-Type"] = content_type;
    }

    void set_header(const std::string& key, const std::string& val) {
        headers[key] = val;
    }
};

class Server {
public:
    using Handler = std::function<void(const Request&, Response&)>;

    struct Route {
        std::string method;
        std::string pattern;
        std::regex regex_pattern;
        std::vector<std::string> param_names;
        Handler handler;
    };

private:
    std::vector<Route> routes;
    bool is_running = false;
    
#ifdef _WIN32
    SOCKET listen_socket = INVALID_SOCKET;
#else
    int listen_socket = -1;
#endif

    static std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    Route parse_route(const std::string& method, const std::string& path, Handler handler) {
        Route r;
        r.method = method;
        r.pattern = path;
        r.handler = handler;

        std::string regex_str = "^";
        std::string param_name;
        bool in_param = false;

        for (size_t i = 0; i < path.size(); ++i) {
            char c = path[i];
            if (c == '{') {
                in_param = true;
                param_name.clear();
            } else if (c == '}') {
                in_param = false;
                r.param_names.push_back(param_name);
                regex_str += "([^/]+)";
            } else if (in_param) {
                param_name += c;
            } else if (c == '.' || c == '+' || c == '*' || c == '?' || c == '^' || c == '$' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '|' || c == '\\') {
                regex_str += '\\';
                regex_str += c;
            } else {
                regex_str += c;
            }
        }
        regex_str += "/?$";
        r.regex_pattern = std::regex(regex_str);
        return r;
    }

    void handle_client(
#ifdef _WIN32
        SOCKET client_sock
#else
        int client_sock
#endif
    ) {
        char buffer[16384];
        int bytes_read = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
#ifdef _WIN32
            closesocket(client_sock);
#else
            close(client_sock);
#endif
            return;
        }

        buffer[bytes_read] = '\0';
        std::string raw_req(buffer, bytes_read);
        std::istringstream stream(raw_req);

        std::string request_line;
        if (!std::getline(stream, request_line)) {
#ifdef _WIN32
            closesocket(client_sock);
#else
            close(client_sock);
#endif
            return;
        }

        std::istringstream line_stream(request_line);
        Request req;
        std::string http_version;
        line_stream >> req.method >> req.path >> http_version;

        // Strip query string for route matching
        std::string path_only = req.path;
        auto query_pos = path_only.find('?');
        if (query_pos != std::string::npos) {
            path_only = path_only.substr(0, query_pos);
        }

        // Parse headers
        std::string header_line;
        int content_length = 0;
        while (std::getline(stream, header_line) && header_line != "\r" && header_line != "") {
            auto colon = header_line.find(':');
            if (colon != std::string::npos) {
                std::string k = trim(header_line.substr(0, colon));
                std::string v = trim(header_line.substr(colon + 1));
                req.headers[k] = v;
                std::string k_lower = k;
                std::transform(k_lower.begin(), k_lower.end(), k_lower.begin(), ::tolower);
                if (k_lower == "content-length") {
                    content_length = std::stoi(v);
                }
            }
        }

        // Read body if present
        if (content_length > 0) {
            size_t body_start_pos = raw_req.find("\r\n\r\n");
            if (body_start_pos != std::string::npos) {
                body_start_pos += 4;
                req.body = raw_req.substr(body_start_pos);
                while (req.body.size() < (size_t)content_length) {
                    int extra = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
                    if (extra <= 0) break;
                    req.body.append(buffer, extra);
                }
                if (req.body.size() > (size_t)content_length) {
                    req.body = req.body.substr(0, content_length);
                }
            }
        }

        Response res;
        // Default CORS Headers
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");

        if (req.method == "OPTIONS") {
            res.status = 200;
            res.set_content("", "text/plain");
        } else {
            bool route_matched = false;
            for (const auto& r : routes) {
                if (r.method == req.method) {
                    std::smatch match;
                    if (std::regex_match(path_only, match, r.regex_pattern)) {
                        route_matched = true;
                        for (size_t i = 0; i < r.param_names.size() && (i + 1) < match.size(); ++i) {
                            req.matches[r.param_names[i]] = match[i + 1].str();
                        }
                        try {
                            r.handler(req, res);
                        } catch (const std::exception& e) {
                            res.status = 500;
                            res.set_content(std::string("{\"error\":\"Internal Server Error: ") + e.what() + "\"}", "application/json");
                        } catch (...) {
                            res.status = 500;
                            res.set_content("{\"error\":\"Unknown Internal Server Error\"}", "application/json");
                        }
                        break;
                    }
                }
            }

            if (!route_matched) {
                res.status = 404;
                res.set_content("{\"error\":\"API Endpoint Not Found\"}", "application/json");
            }
        }

        // Build response HTTP string
        std::ostringstream out;
        out << "HTTP/1.1 " << res.status << " ";
        switch (res.status) {
            case 200: out << "OK"; break;
            case 201: out << "Created"; break;
            case 400: out << "Bad Request"; break;
            case 401: out << "Unauthorized"; break;
            case 403: out << "Forbidden"; break;
            case 404: out << "Not Found"; break;
            case 500: out << "Internal Server Error"; break;
            default: out << "OK"; break;
        }
        out << "\r\n";

        res.headers["Content-Length"] = std::to_string(res.body.size());
        for (const auto& h : res.headers) {
            out << h.first << ": " << h.second << "\r\n";
        }
        out << "\r\n" << res.body;

        std::string res_str = out.str();
        send(client_sock, res_str.c_str(), (int)res_str.size(), 0);

#ifdef _WIN32
        closesocket(client_sock);
#else
        close(client_sock);
#endif
    }

public:
    Server() {}

    ~Server() {
        stop();
    }

    void Get(const std::string& path, Handler handler) {
        routes.push_back(parse_route("GET", path, handler));
    }

    void Post(const std::string& path, Handler handler) {
        routes.push_back(parse_route("POST", path, handler));
    }

    void Put(const std::string& path, Handler handler) {
        routes.push_back(parse_route("PUT", path, handler));
    }

    void Delete(const std::string& path, Handler handler) {
        routes.push_back(parse_route("DELETE", path, handler));
    }

    void Options(const std::string& path, Handler handler) {
        routes.push_back(parse_route("OPTIONS", path, handler));
    }

    bool listen(const std::string& host, int port) {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed.\n";
            return false;
        }
#endif

        listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef _WIN32
        if (listen_socket == INVALID_SOCKET) {
            WSACleanup();
            return false;
        }
#else
        if (listen_socket < 0) return false;
#endif

        int opt = 1;
        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

        sockaddr_in server_addr;
        std::memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons((uint16_t)port);
        inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

        if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
            std::cerr << "Bind failed on " << host << ":" << port << std::endl;
            stop();
            return false;
        }

        if (::listen(listen_socket, SOMAXCONN) != 0) {
            std::cerr << "Listen failed.\n";
            stop();
            return false;
        }

        is_running = true;
        std::cout << "=========================================================\n";
        std::cout << "  C++ Hostel Complaint Management REST API Server\n";
        std::cout << "  Running at http://" << host << ":" << port << "\n";
        std::cout << "=========================================================\n";

        while (is_running) {
            sockaddr_in client_addr;
#ifdef _WIN32
            int client_len = sizeof(client_addr);
            SOCKET client_sock = accept(listen_socket, (struct sockaddr*)&client_addr, &client_len);
            if (client_sock == INVALID_SOCKET) {
                if (!is_running) break;
                continue;
            }
#else
            socklen_t client_len = sizeof(client_addr);
            int client_sock = accept(listen_socket, (struct sockaddr*)&client_addr, &client_len);
            if (client_sock < 0) {
                if (!is_running) break;
                continue;
            }
#endif
            std::thread(&Server::handle_client, this, client_sock).detach();
        }

        return true;
    }

    void stop() {
        is_running = false;
#ifdef _WIN32
        if (listen_socket != INVALID_SOCKET) {
            closesocket(listen_socket);
            listen_socket = INVALID_SOCKET;
        }
        WSACleanup();
#else
        if (listen_socket >= 0) {
            close(listen_socket);
            listen_socket = -1;
        }
#endif
    }
};

} // namespace httplib

#endif // CPPHTTPLIB_HTTPLIB_H

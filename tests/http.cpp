#include "../http_v1/http.h"
#include <cassert>
// 回归：无正文 GET 路由、查询参数、POST 正文和 MIME 后缀。
int main() {
 HttpServer server;server.InsertService("/login",[](HttpRequest& request){HttpResponse response;response.addCode(200,"OK");response.addBodytext("Hello World");return response;});
 std::string raw="GET /login HTTP/1.1\r\nHost: localhost\r\n\r\n";
 auto response=server.HttpHandler(raw);assert(response.find("200 OK")!=std::string::npos);assert(response.find("Hello World")!=std::string::npos);
 HttpRequest post;post.Deserialize("POST /data.html HTTP/1.1\r\nContent-Length: 4\r\n\r\nbody");assert(post.GetRequestBody()=="body");assert(post.Suffix()==".html");
 HttpResponse reply;reply.addCode(200,"OK");reply.addBodytext("x");assert(reply.Serialize()==reply.Serialize());
}

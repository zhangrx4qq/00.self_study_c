#include "mongoose.h"


char* mystrncpy(const char *string,int n){
    char *p=string;
    if(p==NULL){
        return NULL;
    }else{
        int i=0;
        while(*p!='\0'){//循环直到达n个字符串终止
            if(i==n){
                break;
            }
            i++;
            p++;
        }
        *(p++)='\0';//赋值结束字符串
        return string;
    }
}

void print_http_message(struct mg_http_message *hm){
    printf("method=%s\n",mystrncpy(hm->method.ptr,hm->method.len));
    printf("uri=%s\n",mystrncpy(hm->uri.ptr,hm->uri.len));
    printf("query=%s\n",mystrncpy(hm->query.ptr,hm->query.len));
    printf("proto=%s\n",mystrncpy(hm->proto.ptr,hm->proto.len));
    for(int i=0;i<MG_MAX_HTTP_HEADERS;i++){
        if(hm->headers[i].name.ptr != NULL){
            printf("header[%d]=%s:%s\n",i,
               mystrncpy(hm->headers[i].name.ptr,hm->headers[i].name.len),
               mystrncpy(hm->headers[i].value.ptr,hm->headers[i].value.len));
        }
    }
    printf("body=%s\n",mystrncpy(hm->body.ptr,hm->body.len));
    printf("head=%s\n",mystrncpy(hm->head.ptr,hm->head.len));
    printf("chunk=%s\n",mystrncpy(hm->chunk.ptr,hm->chunk.len));
    printf("message=%s\n",mystrncpy(hm->message.ptr,hm->message.len));

}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) 
{
    //处理HTTP事件
    if (ev == MG_EV_HTTP_MSG) {
        //取得HTTP消息
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        //打印HTTP消息
        //print_http_message(hm);

        //如果请求的Uri=/
        //http://localhost:8000/
        if (mg_http_match_uri(hm, "/")) {
            //在浏览器里显示根目录结构
            struct mg_http_serve_opts opts = {.root_dir = "."};   
            mg_http_serve_dir(c, hm, &opts);       
        }
        //如果请求的Uri=/view
        //http://localhost:8000/view
        if (mg_http_match_uri(hm, "/view")) {
            //
            size_t i, max = sizeof(hm->headers) / sizeof(hm->headers[0]);
            mg_printf(c, "%s%s%s\r\n",
                "HTTP/1.1 200 OK\r\n",               // Output response line
                "Transfer-Encoding: chunked\r\n",   // Chunked header
                "Content-Type: text/plain\r\n");    // and Content-Type header
            mg_printf(c, "Request headers:\n");
            // Iterate over request headers, and print them one by one
            for (i = 0; i < max && hm->headers[i].name.len > 0; i++) {
                struct mg_str *k = &hm->headers[i].name, *v = &hm->headers[i].value;
                mg_http_printf_chunk(c, "%.*s -> %.*s\n", (int) k->len, k->ptr, (int) v->len, v->ptr);
            }
            //mg_http_write_chunk(c, "", 0);  // Final empty chunk      
        }
        //如果请求的Uri=/helloworld
        //http://localhost:8000/helloworld
        if (mg_http_match_uri(hm, "/helloworld")) {
            //返回显示文本内容给浏览器：Hello,world
            mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "world");
        } 
        //如果请求的Uri=/api/hello
        //http://localhost:8000/api/hello
        if (mg_http_match_uri(hm, "/api/hello")) {
            //返回JSON格式内容给浏览器：{"status":1}
            mg_http_reply(c, 200, "", "{%m:%d}\n",MG_ESC("status"), 1);
        } 
        //如果请求的Uri=/api/sum
        //http://localhost:8000/api/sum
        if (mg_http_match_uri(hm, "/api/sum")) {
            // Expecting JSON array in the HTTP body, e.g. [ 123.38, -2.72 ]
            double num1, num2;
            if (mg_json_get_num(hm->body, "$[0]", &num1) && mg_json_get_num(hm->body, "$[1]", &num2)) {
                mg_http_reply(c, 200, "Content-Type: application/json\r\n","{%m:%g}\n",mg_print_esc, 0, "result", num1 + num2);
            } else {
                mg_http_reply(c, 500, NULL, "Parameters missing\n");
            }
        } 
    }
}

int main() 
{
    //事件管理器(Event manager)
    struct mg_mgr mgr;
    //初始化事件管理器
    mg_mgr_init(&mgr);
    //监听HTTP连接
    mg_http_listen(&mgr, 
                   "http://0.0.0.0:8000", //url
                   fn,                    //事件处理函数
                   NULL);  
    //事件处理循环
    for (;;) {
        mg_mgr_poll(&mgr, 1000);                       
    }

    mg_mgr_free(&mgr);  
    
    return 0;
}
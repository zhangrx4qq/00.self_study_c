#include <stdio.h>
#include <SDL.h>

//示例0:创建空窗口,消息循环

typedef enum
{
   TRUE=1, FALSE=0
}BOOL;

#define WindowWidth   640   //SDL窗口的宽度
#define WindowHeigth  480   //SDL窗口的高度

//循环处理消息
void event_loop(){
    BOOL quit = FALSE;                                      
    SDL_Event e;
    while (!quit) {
        //获取到了消息事件
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = TRUE;
            }
        }
    }
}

int main(){
    //SDL_Window:声明SDL窗口指针并初始化为空
    SDL_Window *m_window = NULL;
    //SDL:初始化
    if(SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("SDL_Init错误:%s",SDL_GetError());
        return -1;
    }
    //SDL_Window:创建SDL窗口
    m_window = SDL_CreateWindow("Test Window",
                              SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                              WindowWidth,WindowHeigth,SDL_WINDOW_SHOWN);
    //SDL_Window:创建SDL窗口失败，主程序退出
    if (m_window == NULL) {
        SDL_Log("SDL_CreateWindow错误:%s",SDL_GetError());
        return -1;
    }
    
    //循环处理消息
    event_loop();

    //SDL_Window:销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //SDL:退出、释放资源
    SDL_Quit();

    return 0;
}


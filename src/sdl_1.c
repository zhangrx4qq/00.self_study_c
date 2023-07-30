#include <stdio.h>
#include <SDL.h>

//示例1:在窗口绘制不同颜色矩形区域,直接修改窗口区域的像素点

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

    //绘制不同颜色矩形区域
    SDL_Surface *m_screen = SDL_GetWindowSurface(m_window);
    SDL_Rect whiteRect = {0,0,WindowWidth,WindowHeigth};
    SDL_FillRect(m_screen,&whiteRect,0xfffffff);//白色
    SDL_Rect redRect = {0,0,100,100};
    SDL_FillRect(m_screen,&redRect,0xffff0000);//红色
    //修改像素点，在窗口中间画一条蓝色线条
    for(int i=0;i<WindowWidth;i++){
        ((uint32_t *)(m_screen->pixels))[i+WindowHeigth/2*WindowWidth] = 0xff0000ff;//蓝色
    }
    SDL_UpdateWindowSurface(m_window);

    //循环处理消息
    event_loop();

    //SDL_Window:销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //SDL:退出、释放资源
    SDL_Quit();

    return 0;
}


#include <stdio.h>
#include <SDL.h>

//示例2:绘制动画、控制帧频

typedef enum
{
   TRUE=1, FALSE=0
}BOOL;

#define WindowWidth   640   //SDL窗口的宽度
#define WindowHeigth  480   //SDL窗口的高度
#define FrameRate 30        //帧率

int x = 0;
int y = 0;

//绘制窗体
void draw(SDL_Surface *screen,SDL_Window *window){
    SDL_Rect whiteRect = {0,0,WindowWidth,WindowHeigth};
    SDL_FillRect(screen,&whiteRect,0xfffffff);//白色
    SDL_Rect redRect = {x,y,100,100};
    SDL_FillRect(screen,&redRect,0xffff0000);//红色
    SDL_UpdateWindowSurface(window);
    x++;
    if(x==WindowWidth){
        x = 0;
        y=y+100;
        if(y>=WindowHeigth){
            x=0;
            y=0;
        }
    }
}
//循环处理消息
void event_loop(SDL_Surface *screen,SDL_Window *window){
    BOOL quit = FALSE;                                      
    SDL_Event e;
    while (!quit) {
        long t_begin = SDL_GetTicks();
        draw(screen,window);

        //获取到了消息事件
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = TRUE;
            }
        }

        long t_current = SDL_GetTicks();
        long delay = 1000/FrameRate -(t_current-t_begin);
        if(delay > 0){
            SDL_Delay(delay);
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

    SDL_Surface *m_screen = SDL_GetWindowSurface(m_window);
    event_loop(m_screen,m_window);

    //SDL_Window:销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //SDL:退出、释放资源
    SDL_Quit();

    return 0;
}


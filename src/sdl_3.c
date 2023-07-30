#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

//示例3:使用图片

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
void draw(SDL_Surface *screen,SDL_Surface *img,SDL_Window *window){
    SDL_Rect src = {0,0,img->w,img->h};
    SDL_BlitSurface(img,&src,screen,&src);
    SDL_UpdateWindowSurface(window);
}

//循环处理消息
void event_loop(SDL_Surface *screen,SDL_Surface *img,SDL_Window *window){
    BOOL quit = FALSE;                                      
    SDL_Event e;
    while (!quit) {
        draw(screen,img,window);
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

    //SDL_Surface:使用sdl_image装载图片，创建SDL Surface
    //SDL_Surface *img = IMG_Load("../resources/gameover.bmp");
    //SDL_Surface:使用sdl直接装载BMP图片，创建SDL Surface
    SDL_Surface *img = SDL_LoadBMP("../resources/gameover.bmp");

    SDL_Surface *m_screen = SDL_GetWindowSurface(m_window);

    event_loop(m_screen,img,m_window);

    //SDL_Surface:释放使用完毕的SDL Surface
    SDL_FreeSurface(img);
    //SDL_Window:销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //SDL:退出、释放资源
    SDL_Quit();

    return 0;
}


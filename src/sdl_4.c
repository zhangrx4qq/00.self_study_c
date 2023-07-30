#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

//示例4:绘制文字

typedef enum
{
   TRUE=1, FALSE=0
}BOOL;

#define WindowWidth   640   //SDL窗口的宽度
#define WindowHeigth  480   //SDL窗口的高度
#define Font_Size     32    //文字字体大小

//绘制窗体
void draw(SDL_Surface *screen,TTF_Font *font,SDL_Window *window){
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *text = TTF_RenderUTF8_Blended(font, "This is TEXT to show", color);
    SDL_Rect src = {0, 0, text->w, text->h};
    SDL_BlitSurface(text, &src, screen, &src);
    SDL_FreeSurface(text);
    SDL_UpdateWindowSurface(window);
}

//循环处理消息
void event_loop(SDL_Surface *screen,TTF_Font *font,SDL_Window *window){
    BOOL quit = FALSE;                                      
    SDL_Event e;
    while (!quit) {
        draw(screen,font,window);
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

    //TTF:初始化
    if (TTF_Init()) {
        SDL_Log("TTF_Init错误:%s", TTF_GetError());
        return -1;
    }
    //TTF:创建字体对象
    TTF_Font *m_font = TTF_OpenFont("../resources/m5x7.ttf", Font_Size);
    if (m_font == NULL) {
        SDL_Log("TTF_OpenFont错误！");
        return -1;
    }

    SDL_Surface *m_screen = SDL_GetWindowSurface(m_window);

    event_loop(m_screen,m_font,m_window);

    //TTF:关闭字体对象
    TTF_CloseFont(m_font);
    //SDL_Window:销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //SDL:退出、释放资源
    SDL_Quit();

    return 0;
}


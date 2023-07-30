#include <stdio.h>
#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

//示例5:渲染

typedef enum
{
   TRUE=1, FALSE=0
}BOOL;

#define WindowWidth   640   //SDL窗口的宽度
#define WindowHeigth  480   //SDL窗口的高度
#define Font_Size     32    //文字字体大小

//绘制窗体
void draw(SDL_Renderer *renderer,SDL_Texture *texture){
    //设置渲染器的画笔颜色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//不透明白色
    //使用指定颜色清屏
    SDL_RenderClear(renderer);

    //填充矩形
    SDL_Rect r = {0, 0, 100, 100};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//不透明红色
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);//半透明蓝色
    SDL_Rect r1 = {50, 50, 100, 100};
    SDL_RenderFillRect(renderer, &r1);

    //绘制多条直线(三角形)
    const int m_pointnum = 4;
    const SDL_Point m_points[4] = {
        {320, 200},
        {300, 240},
        {340, 240},
        {320, 200}
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//不透明红色
    SDL_RenderDrawLines(renderer, m_points, m_pointnum);

    //图片渲染
    SDL_Rect src_rect = {0, 0, 256, 256};
    SDL_Rect destRect = {WindowWidth-256, WindowHeigth-256, 256, 256};
    SDL_RenderCopy(renderer, texture, &src_rect, &destRect);
    
    //显示渲染数据
    SDL_RenderPresent(renderer);
}

//循环处理消息
void event_loop(SDL_Renderer *renderer,SDL_Texture *texture){
    BOOL quit = FALSE;                                      
    SDL_Event e;
    while (!quit) {
        draw(renderer,texture);
        //获取到了消息事件
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = TRUE;
            }
        }
    }
}

void em_loop(SDL_Renderer *renderer,SDL_Texture *texture) {
    draw(renderer,texture);
}

int main(){
    //SDL_Window:声明SDL窗口指针并初始化为空
    SDL_Window *m_window = NULL;
    //SDL_Renderer:声明SDL渲染器指针并初始化为空  
    SDL_Renderer *m_renderer = NULL;
    //SDL_Texture:声明SDL纹理指针并初始化为空
    SDL_Texture *m_texture = NULL;

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
    //SDL_Renderer:基于已经创建的SDL窗口创建SDL渲染器
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);//使用显卡进行图形渲染
    //SDL_Renderer:创建SDL渲染器失败，主程序退出，释放资源
    if (!m_renderer) {
        SDL_Log("SDL_CreateRenderer错误:%s",SDL_GetError());
        SDL_DestroyWindow(m_window);
        return -1;
    }

    SDL_Surface* m_surface = SDL_LoadBMP("../resources/gameover.bmp");
    if (!m_surface) {
        SDL_Log("SDL_LoadBMP错误:%s",SDL_GetError());
        SDL_DestroyWindow(m_window);
        SDL_DestroyRenderer(m_renderer);
        return -1;
    }
    //const SDL_Color m_KeyColor = {118, 66, 138, 255};
    //SDL_SetColorKey(m_surface, SDL_TRUE, 
    //                SDL_MapRGB(m_surface->format,m_KeyColor.r, m_KeyColor.g, m_KeyColor.b));
    m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_FreeSurface(m_surface);
    //SDL_Texture:创建SDL纹理失败，主程序退出，释放资源
    if (!m_texture) {
        SDL_Log("SDL_CreateTextureFromSurface错误:%s",SDL_GetError());
        SDL_DestroyWindow(m_window);
        SDL_DestroyRenderer(m_renderer);
        return -1;
    }


#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(em_loop,0,1);
#else // __EMSCRIPTEN__
    event_loop(m_renderer,m_texture);
#endif

    //SDL_Texture:销毁创建的SDL纹理、释放占用的资源
    SDL_DestroyTexture(m_texture);
    //SDL_Renderer:销毁创建的SDL渲染器、释放占用的资源
    SDL_DestroyRenderer(m_renderer);
    //SDL_Window:销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //SDL:退出、释放资源
    SDL_Quit();

    return 0;
}


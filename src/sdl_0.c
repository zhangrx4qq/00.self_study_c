/*SDL(Simple DirectMedia Layer)
一套开源的跨平台多媒体开发库，用C语言写的。应用于游戏开发，模拟器，媒体播放器等多媒体应用领域。
1.SDL子系统(SDL将功能分成下列数个子系统):
SDL_INIT_TIMER：定时器
SDL_INIT_AUDIO：音频
SDL_INIT_VIDEO：视频
SDL_INIT_JOYSTICK：摇杆
SDL_INIT_HAPTIC：触摸屏
SDL_INIT_GAMECONTROLLER：游戏控制器
SDL_INIT_EVENTS：事件
SDL_INIT_EVERYTHING：包含上述所有选项

2.SDL Window显示：SDL视频显示函数简介:
SDL_Init()：初始化SDL系统
SDL_CreateWindow()：创建窗口SDL_Window
SDL_CreateRenderer()：创建渲染器SDL_Renderer
SDL_CreateTexture()：创建纹理SDL_Texture
SDL_UpdateTexture()：设置纹理的数据
SDL_RenderCopy()：将纹理的数据拷贝给渲染器
SDL_RenderPresent()：显示
SDL_Delay()：工具函数，用于延时
SDL_Quit()：退出SDL系统

设置画笔颜色
int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r,Uint8 g,Uint8 b,Uint8 a)
绘制一个点
int SDL_RenderDrawPoint(SDL_Renderer* renderer, int x, int y)
绘制多个点
int SDL_RenderDrawPoints(SDL_Renderer* renderer, const SDL_Point* points, int count)
绘制直线
int SDL_RenderDrawLine(SDL_Renderer* renderer,int x1,int y1,int x2,in y2)
绘制多条线
int SDL_RenderDrawLines(SDL_Renderer* renderer, const SDL_Point* points, int count)
绘制矩形
int SDL_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect)
填充矩形
int SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect)
填充多块矩形
int SDL_RenderDrawRects(SDL_Renderer* renderer, const SDL_Rect* rects, int count)


3.SDL Windows显示：SDL数据结构简介:
SDL_Window 代表了一个“窗口”
SDL_Renderer 代表了一个“渲染器”
SDL_Texture 代表了一个“纹理”
SDL_Rect 一个简单的矩形结构

*/
#include <stdio.h>
#include <SDL.h>

typedef enum
{
   TRUE=1, FALSE=0
}BOOL;

int main()
{
    //a.声明SDL窗口指针并初始化为空
    SDL_Window *m_window = NULL;
    //SDL窗口的宽度
    int m_windowWidth = 640;
    //SDL窗口的高度
    int m_windowHeight = 480;
    //b.声明SDL渲染器指针并初始化为空  
    SDL_Renderer *m_renderer = NULL;
    //c.声明SDL纹理指针并初始化为空
    SDL_Texture *m_texture = NULL;
    SDL_Texture *m_texture_fromSurface = NULL;

    //初始化SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    //a.创建SDL窗口
    m_window = SDL_CreateWindow("Test Window",
                              SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                              m_windowWidth,m_windowHeight,SDL_WINDOW_SHOWN);
    //a.创建SDL窗口失败，主程序退出
    if (m_window == NULL) {
        return -1;
    }
    //b.基于已经创建的SDL窗口创建SDL渲染器
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    //b.创建SDL渲染器失败，主程序退出，释放资源
    if (!m_renderer) {
        //a.销毁创建的SDL窗口、释放占用的资源
        SDL_DestroyWindow(m_window);
        return -1;
    }
    //c.基于已经创建的SDL渲染器创建SDL纹理
    m_texture = SDL_CreateTexture(m_renderer,
                                  SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_TARGET,
                                  m_windowWidth,m_windowHeight); 
    //C.创建SDL纹理失败，主程序退出，释放资源
    if (!m_texture) {
        //a.销毁创建的SDL窗口、释放占用的资源
        SDL_DestroyWindow(m_window);
        //b.销毁创建的SDL渲染器、释放占用的资源
        SDL_DestroyRenderer(m_renderer);
        return -1;
    }
    //c.基于已经创建的SDL渲染器,从SDL Surface创建SDL纹理
    const SDL_Color m_KeyColor = {118, 66, 138, 255};
    SDL_Surface* m_surface = SDL_LoadBMP("../resources/gameover.bmp");
    if (!m_surface) {
                //a.销毁创建的SDL窗口、释放占用的资源
        SDL_DestroyWindow(m_window);
        //b.销毁创建的SDL渲染器、释放占用的资源
        SDL_DestroyRenderer(m_renderer);
        return -1;
    }
    SDL_SetColorKey(m_surface, SDL_TRUE, 
                    SDL_MapRGB(m_surface->format,m_KeyColor.r, m_KeyColor.g, m_KeyColor.b));
    m_texture_fromSurface = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_FreeSurface(m_surface);
    //C.创建SDL纹理失败，主程序退出，释放资源
    if (!m_texture_fromSurface) {
        //a.销毁创建的SDL窗口、释放占用的资源
        SDL_DestroyWindow(m_window);
        //b.销毁创建的SDL渲染器、释放占用的资源
        SDL_DestroyRenderer(m_renderer);
        return -1;
    }

    /*使用纹理进行渲染*/
    //设置渲染器的渲染目标为纹理
    SDL_SetRenderTarget(m_renderer, m_texture);
    //设置渲染器的画笔颜色
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 255); 
    //使用指定颜色清屏
    SDL_RenderClear(m_renderer); 
    //恢复默认，设置渲染器的渲染目标为窗口
    SDL_SetRenderTarget(m_renderer, NULL); 
    //拷贝纹理到渲染器(全窗口拷贝)
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL); 
    //拷贝纹理到渲染器(指定矩形区拷贝)
    SDL_Rect src_rect = {0, 0, 256, 256};
    SDL_RenderCopy(m_renderer, m_texture_fromSurface, &src_rect, &src_rect);

    /*使用渲染器进行渲染*/
    const int m_pointnum = 4;
    const SDL_Point m_points[4] = {
        {320, 200},
        {300, 240},
        {340, 240},
        {320, 200}
    };
    //设置渲染器的画笔颜色
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    //绘制多条直线
    SDL_RenderDrawLines(m_renderer, m_points, m_pointnum);
    //绘制矩形
    SDL_Rect rect = { 200, 300, 100, 100 };
    SDL_RenderDrawRect(m_renderer, &rect);
    //填充矩形
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(m_renderer, &rect);
    
    //输出渲染器内容到目标窗口
    SDL_RenderPresent(m_renderer); 

    //循环处理消息
    BOOL quit = FALSE;                                      
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = TRUE;
            }
        }
    }

    //c.销毁创建的SDL纹理、释放占用的资源
    SDL_DestroyTexture(m_texture);
    //b.销毁创建的SDL渲染器、释放占用的资源
    SDL_DestroyRenderer(m_renderer);
    //a.销毁创建的SDL窗口、释放占用的资源
    SDL_DestroyWindow(m_window); 
    //退出SDL、释放资源
    SDL_Quit();

    return 0;
}


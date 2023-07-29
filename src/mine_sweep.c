#include <stdlib.h>
#include<time.h>
#include "SDL.h"

typedef enum
{
   TRUE=1, FALSE=0
}BOOL;

//地图结构体
struct Tile{
    int nBomb; //-1:地雷，其他：周边地雷数量(0-8)
    BOOL isVisiable;//是否可见
    BOOL isFlaged;//是否标记为地雷
};

const char g_title[]="MineSweep"; //游戏窗体的标题
const int g_tileLen = 32;//游戏窗体正方形单元格的长和宽的大小
const int g_tileNumW = 20; //游戏窗体中每行的单元格数量
const int g_tileNumH = 20; //游戏窗体中每列的单元格数量
const int g_windowWidth = g_tileLen * g_tileNumW;//游戏窗体的宽度
const int g_windowHeight = g_tileLen * g_tileNumH;//游戏窗体的高度
const int g_mineCount = 50;//含地雷单元格的数量

const SDL_Color g_NormalTileColor = {150, 150, 150, 255};
const SDL_Color g_HoverTileColor = {200, 200, 200, 255};
const SDL_Color g_BorderTileColor = {0, 0, 0, 255};
const SDL_Color g_NakedTileColor = {50, 50, 50, 255};
const SDL_Color g_KeyColor = {118, 66, 138, 255};

//util
//input:g_tileLen
SDL_Point calcTileCoord(int mouseX, int mouseY) {
    SDL_Point point;
    point.x = mouseX / g_tileLen ;
    point.y = mouseY / g_tileLen ;
    return point;
}
//input:
BOOL IsIn(int x, int y,int w, int h) {
    if(x >= 0 && x < w && y >= 0 && y < h) {
        return TRUE;
    }else{
        return FALSE;
    }
}

//
//input:renderer,图片文件名,KeyColor
SDL_Texture* loadTexture(SDL_Renderer* renderer,const char *bmpFilename,const SDL_Color *keycolor) {
    SDL_Surface* surface = SDL_LoadBMP(bmpFilename);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "%s load failed", bmpFilename);
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, keycolor->r, keycolor->g, keycolor->b));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_RESERVED1, "texture create from %s failed", bmpFilename);
    }
    SDL_FreeSurface(surface);
    return texture;
}

//随机生成地图数组
//input：地图数组，地雷数量，地图数组的宽和高
void CreateRandomMap(struct Tile *map,int bombCount, int w, int h){
    const int MaxBombSetupCount = 100;
    while (bombCount > 0) {
        BOOL setupBomb = FALSE;
        int setupCount = MaxBombSetupCount;
        srand((unsigned int)time(NULL));//随机数种子
        //设置地雷标志为FALSE，且设置的地雷的数量小于最大指定数量
        while (!setupBomb && setupCount > 0) {
            int x = rand() % w; //生成0-w内的随机数
            int y = rand() % h; //生成0-h内的随机数
            if(map[x+y*w].nBomb != -1){
                map[x+y*w].nBomb = -1;
                setupBomb = TRUE;
            }else{
                setupCount--;
            }
        }
        if (setupCount == 0) {
            for(int i=0;i<w*h;i++){
                if(map[i].nBomb != -1){
                    map[i].nBomb = -1;
                    break;
                }
            }
        }
        bombCount --;
    }
    //不含地雷的情况下，计算(x,y)单元格周围含有地雷的单元格数量
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if(map[y+x*h].nBomb == -1){
                continue;
            }
            int mineCount = 0;
            for (int nx = -1; nx <= 1; nx ++) {
                for (int ny = -1; ny <= 1; ny++) {
                    int detectX = x + nx;
                    int detectY = y + ny;
                    if (IsIn(detectX, detectY,w,h)) {
                        if(map[detectY+detectX*h].nBomb == -1){
                            mineCount ++;
                        }
                    }
                }
            }
            map[y+x*h].nBomb = mineCount;
        }
    }

}

void floodFill(struct Tile *map, int x, int y, int w, int h,int* nakkedCount) {
    if (!IsIn(x,y,w,h)) {
        return;
    }
    struct Tile tile = map[x+y*w];
    if (!tile.isVisiable && !tile.isFlaged && tile.nBomb != -1) {
        map[x+y*w].isVisiable = TRUE;
        int tmp = *nakkedCount;
        *nakkedCount = tmp + 1;
        if (tile.nBomb == 0) {
            floodFill(map, x - 1, y,w,h,nakkedCount);
            floodFill(map, x + 1, y,w,h,nakkedCount);
            floodFill(map, x, y - 1,w,h,nakkedCount);
            floodFill(map, x, y + 1,w,h,nakkedCount);
        }
    }
}

void SetColor(SDL_Renderer* renderer,const SDL_Color* c) {
    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
}
void DrawTexture(SDL_Renderer* renderer,SDL_Texture* texture, const SDL_Rect *rect, int x, int y) {
    SDL_Rect dst = {x, y, rect->w, rect->h};
    SDL_RenderCopy(renderer, texture, rect, &dst);
}
void drawOneTile(SDL_Renderer* renderer,
                 SDL_Texture* mineImage,SDL_Texture* numberImage,SDL_Texture* flagImage,
                 int x, int y, const struct Tile *tile,BOOL debugMode) 
{
    int tileX = x * g_tileLen;
    int tileY = y * g_tileLen;
    SDL_Rect rect = {tileX, tileY, g_tileLen, g_tileLen};
    SDL_Point mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    if (SDL_PointInRect(&mousePos, &rect)) {
        SetColor(renderer,&g_HoverTileColor);
    } else {
        SetColor(renderer,&g_NormalTileColor);
    }
    if (tile->isVisiable && tile->nBomb >= 0) {
        SetColor(renderer,&g_NakedTileColor);
    }
    SDL_RenderFillRect(renderer, &rect);
    SetColor(renderer,&g_BorderTileColor);
    SDL_RenderDrawRect(renderer, &rect);

    SDL_Rect rect_tmp = {0, 0, g_tileLen, g_tileLen};

    if (tile->isVisiable) {
        if (tile->nBomb == -1) {
            DrawTexture(renderer,mineImage,&rect_tmp, tileX, tileY);
        } else {
            int mineCount = tile->nBomb;
            if (mineCount > 0) {
                SDL_Rect rect_tmp1 = {309 / 8 * (mineCount - 1), 0, g_tileLen, g_tileLen};
                DrawTexture(renderer,numberImage,&rect_tmp1,tileX, tileY);
            }
        }
    } else {
        if (tile->isFlaged) {
            DrawTexture(renderer,flagImage, &rect_tmp, tileX, tileY);
        }
    }

    if (debugMode) {
        if (tile->nBomb == -1) {
            DrawTexture(renderer,mineImage,&rect_tmp, tileX, tileY);
        } else {
            int mineCount = tile->nBomb;;
            if (mineCount > 0) {
                SDL_Rect rect_tmp1 = {309 / 8 * (mineCount - 1), 0, g_tileLen, g_tileLen};
                DrawTexture(renderer,numberImage,&rect_tmp1,tileX, tileY);
            }
        }
    }
}


int main(int argc, char** argv) {

    SDL_Window *m_window;     //游戏窗口
    SDL_Renderer *m_renderer; //游戏窗口对应的渲染器
    struct Tile *m_map;     //地图

    SDL_Texture *m_numberImage;    //标记数字的纹理（Texture）
    SDL_Texture *m_mineImage;      //标记地雷的纹理（Texture）
    SDL_Texture *m_flagImage;      //标记旗子的纹理（Texture）
    SDL_Texture *m_gameoverImage;  //标记游戏结束的纹理（Texture）
    SDL_Texture *m_winImage;       //标记赢得游戏的纹理（Texture）
    SDL_Event m_event;            //SDL事件

    enum GameState {Gaming, Win, Explode,} m_state = Gaming; //游戏程序的状态

    int nakkedCount = 0;       //已经挖完的单元格数量
    int mineCount = 0;         //已经设置的地雷数量
   
    BOOL debugMode = FALSE;   //调试模式
    BOOL shouldClose = FALSE;

    //初始化SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    //创建窗口（Window）
    m_window = SDL_CreateWindow(g_title, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 
                     g_windowWidth, g_windowHeight, SDL_WINDOW_SHOWN);
    //基于窗口创建渲染器（Render）
    m_renderer = SDL_CreateRenderer(m_window,-1,0);

    //创建纹理（Texture）
    m_numberImage = loadTexture(m_renderer,"../resources/font.bmp",&g_KeyColor);
    m_mineImage = loadTexture(m_renderer,"../resources/mine.bmp",&g_KeyColor);
    m_flagImage = loadTexture(m_renderer,"../resources/flag.bmp",&g_KeyColor);
    m_gameoverImage = loadTexture(m_renderer,"../resources/gameover.bmp",&g_KeyColor);
    m_winImage = loadTexture(m_renderer,"../resources/win.bmp",&g_KeyColor);

    //创建map
    m_map = (struct Tile*)calloc(g_tileNumW*g_tileNumH,sizeof(struct Tile));
    for(int i=0;i<g_tileNumW;i++){
        for(int j=0;j<g_tileNumH;j++){
            m_map[i+j*g_tileNumW].nBomb=0;
            m_map[i+j*g_tileNumW].isFlaged=FALSE;
            m_map[i+j*g_tileNumW].isVisiable=FALSE;
        }
    }
    CreateRandomMap(m_map,g_mineCount,g_tileNumW,g_tileNumH);
    mineCount = g_mineCount;
    //消息循环
    while(!shouldClose) {
        //Run()
        while(SDL_PollEvent(&m_event)) {
            //SDL事件=退出
            if (m_event.type == SDL_QUIT) {
                shouldClose = TRUE;
            }
            //HandleEvent(m_event)
            //游戏未开始
            if (m_state != Gaming) {
                //SDL事件=按下鼠标：随机设置地雷，状态为游戏开始
                if (m_event.type == SDL_MOUSEBUTTONDOWN) {
                    for(int i=0;i<g_tileNumW;i++){
                        for(int j=0;j<g_tileNumH;j++){
                            m_map[i+j*g_tileNumW].nBomb=0;
                            m_map[i+j*g_tileNumW].isFlaged=FALSE;
                            m_map[i+j*g_tileNumW].isVisiable=FALSE;
                        }
                    }
                    CreateRandomMap(m_map,g_mineCount,g_tileNumW,g_tileNumH);
                    mineCount = g_mineCount;
                    nakkedCount = 0;
                    m_state = Gaming;
                }
                break;
            }
            //SDL事件=按下鼠标
            if (m_event.type == SDL_MOUSEBUTTONDOWN) {
                //SDL事件=按下鼠标左键
                if (m_event.button.button == SDL_BUTTON_LEFT) {
                    //handleMouseLeftBtnDown(event.button);
                    //
                    SDL_Point tileCoord = calcTileCoord(m_event.button.x, m_event.button.y);
                    //不在游戏窗体范围内，退出事件处理循环
                    if (!IsIn(tileCoord.x, tileCoord.y,g_tileNumW,g_tileNumH)) {
                        break;
                    }
                    struct Tile curTile = m_map[tileCoord.x+tileCoord.y*g_tileNumW];
                    //可见或已经设置标记，退出事件处理循环
                    if(curTile.isVisiable || curTile.isFlaged){
                        break;
                    }
                    //不可见且包含地雷
                    if(!curTile.isVisiable && curTile.nBomb==-1){
                        m_state = Explode;
                        for(int i=0;i<g_tileNumW;i++){
                            for(int j=0;j<g_tileNumH;j++){
                                m_map[i+j*g_tileNumW].isFlaged=FALSE;
                                m_map[i+j*g_tileNumW].isVisiable=TRUE;
                            }
                        }
                        break;
                    }
                    floodFill(m_map, tileCoord.x, tileCoord.y,g_tileNumW,g_tileNumH,&nakkedCount);
                    if (nakkedCount == g_tileNumW*g_tileNumH - mineCount) {
                        m_state = Win;
                    }
                }
                //SDL事件=按下鼠标右键
                if (m_event.button.button == SDL_BUTTON_RIGHT) {
                    //handleMouseRightBtnDown(event.button);
                    SDL_Point tileCoord = calcTileCoord(m_event.button.x, m_event.button.y);
                     //不在游戏窗体范围内，退出事件处理循环
                    if (!IsIn(tileCoord.x, tileCoord.y,g_tileNumW,g_tileNumH)) {
                        break;
                    }
                    if (!m_map[tileCoord.x+tileCoord.y*g_tileNumW].isVisiable) {
                        m_map[tileCoord.x+tileCoord.y*g_tileNumW].isFlaged = !m_map[tileCoord.x+tileCoord.y*g_tileNumW].isFlaged;
                    }
                }
            }
            //SDL事件=按下键盘
            if (m_event.type == SDL_KEYDOWN) {
                //handleKeyDown(event.key);
                    if (m_event.key.keysym.scancode == SDL_SCANCODE_G) {
                        debugMode = !debugMode;
                    }
            }
        }
        SDL_SetRenderDrawColor(m_renderer, g_HoverTileColor.r, g_HoverTileColor.g,
                                           g_HoverTileColor.b, g_HoverTileColor.a);
        SetColor(m_renderer, &g_HoverTileColor);
        SDL_RenderClear(m_renderer);
        //DrawMap()
        for (int y = 0; y < g_tileNumH; y++) {
            for (int x = 0; x < g_tileNumW; x++) {
                struct Tile tile = m_map[x+y*g_tileNumW];
                drawOneTile(m_renderer,m_mineImage,m_numberImage,m_flagImage,x, y, &tile,debugMode);
            }
        }
        SDL_Rect rect = {0, 0, 256, 256};
        if (m_state == Explode) {
            DrawTexture(m_renderer,m_gameoverImage, &rect,(g_windowWidth - 256) / 2,(g_windowWidth - 256) / 2);
        }
        if (m_state == Win) {
            DrawTexture(m_renderer,m_winImage,&rect,(g_windowWidth - 256) / 2,(g_windowWidth - 256) / 2);
        }

        SDL_RenderPresent(m_renderer);
        SDL_Delay(30);
    }
    //destory
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    free(m_map);
    SDL_DestroyTexture(m_numberImage);
    SDL_DestroyTexture(m_mineImage);
    SDL_DestroyTexture(m_flagImage);
    SDL_DestroyTexture(m_gameoverImage);
    SDL_DestroyTexture(m_winImage);
    
    SDL_Quit();

    return 0;
}
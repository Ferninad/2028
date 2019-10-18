#include "common.h"
#include "cmath"
#include "vector"

bool Init();
void CleanUp();
void Run();
void DrawGrid();
void Spawn();
void Draw();
void Play(string dir);
void Restart();
double ScaleNum(double n, double minN, double maxN, double min, double max);

SDL_Window *window;
SDL_GLContext glContext;
SDL_Surface *gScreenSurface = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Rect pos;

int screenWidth = 500;
int screenHeight = 500;
int gridSize = screenWidth / 4;
bool w = false;
bool a = false;
bool s = false;
bool d = false;

vector<vector<int>> squares;


bool Init()
{
    if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,   
        SDL_WINDOW_OPENGL);

    //Check that the window was succesfully created
    if (window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return false;
    }
    else{
        gScreenSurface = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Log("Window Successful Generated");
    }
    //Map OpenGL Context to Window
    glContext = SDL_GL_CreateContext(window);

    return true;
}

int main()
{
    //Error Checking/Initialisation
    if (!Init())
    {
        printf("Failed to Initialize");
        return -1;
    }

    // Clear buffer with black background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Run();

    CleanUp();
    return 0;
}

void CleanUp()
{
    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Run()
{
    bool gameLoop = true;
    srand(time(NULL));
    
    Restart();

    while (gameLoop)
    {   
        string dir;
        if(w){
            dir = "UP";
            w = false;
        }
        else if(a){
            dir = "LEFT";
            a = false;
        }
        else if(s){
            dir = "DOWN";
            s = false;
        }
        else if(d){
            dir = "RIGHT";
            d = false;
        }
        
        Play(dir);
        dir = "";
        Draw();
        DrawGrid();
        SDL_RenderPresent(renderer);
        
        SDL_RenderPresent(renderer);
        pos.x = 0;
        pos.y = 0;
        pos.w = screenWidth;
        pos.h = screenHeight;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &pos);
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameLoop = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        gameLoop = false;
                        break;
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym){
                    case SDLK_w:
                        w = true;
                        break;
                    case SDLK_s:
                        s = true;
                        break;
                    case SDLK_a:
                        a = true;
                        break;
                    case SDLK_d:
                        d = true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void Play(string dir){
    vector<int> line;
    vector<int> temp;
    vector<vector<int>> check = squares;

    if(dir == "UP"){
        for(int x = 0; x < 4; x++){
            for(int y = 0; y < 4; y++){
                line.push_back(squares[x][y]);
            }
            for(int j = 0; j < 4; j++){
                for(int i = 0; i < 4; i++){
                    if(line[i] == 0){ 
                        line.erase(line.begin()+i);
                        line.push_back(0);
                    }
                }
            }
           
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    if(line[i] == line[i + 1] && line[i] != 0){
                        line[i]*=2;
                        line.erase(line.begin()+i+1);
                        line.push_back(0);
                    }
                }
            }
            for(int i = 0; i < 4; i++){
                squares[x][i] = line[i];
            }
            line.clear();
        }
        if(check == squares){}
        else{
            Spawn();
        }
    } else if(dir == "LEFT"){
        for(int y = 0; y < 4; y++){
            for(int x = 0; x < 4; x++){
                line.push_back(squares[x][y]);
            }
            for(int j = 0; j < 4; j++){
                for(int i = 0; i < 4; i++){
                    if(line[i] == 0){ 
                        line.erase(line.begin()+i);
                        line.push_back(0);
                    }
                }
            }
           
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    if(line[i] == line[i + 1] && line[i] != 0){
                        line[i]*=2;
                        line.erase(line.begin()+i+1);
                        line.push_back(0);
                    }
                }
            }
            for(int i = 0; i < 4; i++){
                squares[i][y] = line[i];
            }
            line.clear();
        }
        if(check == squares){}
        else{
            Spawn();
        }
    } else if(dir == "DOWN"){
        for(int x = 0; x < 4; x++){
            for(int y = 3; y >= 0; y--){
                line.push_back(squares[x][y]);
            }
            for(int j = 0; j < 4; j++){
                for(int i = 0; i < 4; i++){
                    if(line[i] == 0){ 
                        line.erase(line.begin()+i);
                        line.push_back(0);
                    }
                }
            }
           
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    if(line[i] == line[i + 1] && line[i] != 0){
                        line[i]*=2;
                        line.erase(line.begin()+i+1);
                        line.push_back(0);
                    }
                }
            }
            for(int i = 0; i < 4; i++){
                squares[x][3 - i] = line[i];
            }
            line.clear();
        }
        if(check == squares){}
        else{
            Spawn();
        }
    } else if(dir == "RIGHT"){
        for(int y = 0; y < 4; y++){
            for(int x = 3; x >= 0; x--){
                line.push_back(squares[x][y]);
            }
            for(int j = 0; j < 4; j++){
                for(int i = 0; i < 4; i++){
                    if(line[i] == 0){ 
                        line.erase(line.begin()+i);
                        line.push_back(0);
                    }
                }
            }
           
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    if(line[i] == line[i + 1] && line[i] != 0){
                        line[i]*=2;
                        line.erase(line.begin()+i+1);
                        line.push_back(0);
                    }
                }
            }
            for(int i = 0; i < 4; i++){
                squares[3 - i][y] = line[i];
            }
            line.clear();
        }
        if(check == squares){}
        else{
            Spawn();
        }
    }
    else{}
}

void Draw(){
    for(int x = 0; x < squares.size(); x++){
        for(int y = 0; y < squares[0].size(); y++){
            if(squares[x][y] > 0){
                double color = log(squares[x][y])/log(2);
                color = ScaleNum(color, 1, 11, 205, 55);
                pos.x = x * gridSize;
                pos.y = y * gridSize;
                pos.w = gridSize;
                pos.h = gridSize;
                SDL_SetRenderDrawColor(renderer, color, log(squares[x][y])/log(2)*10, log(squares[x][y])/log(2)*20 - 10, 255);
                SDL_RenderFillRect(renderer, &pos);
            }
        }
    }
}

void Spawn(){
    vector<int> empty;
    
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            if(squares[x][y] == 0){
                empty.push_back(x);
                empty.push_back(y);
            }
        }
    }
    
    int num = rand() % 5 + 1;
    if(num == 1)
        num = 4;
    else
        num = 2;
    
    int randSquare;
    if(empty.size() == 1){
        randSquare = 0;
    }
    else if(empty.size() == 0){
        Restart();
    }
    else{
        randSquare = rand() % (empty.size() / 2);
    }
    if(empty.size() != 0){
        squares[empty[randSquare * 2]][empty[randSquare * 2 + 1]] = num;
    }
}

void Restart(){
    vector<int> temp;
    if(squares.size() != 0)
        cout << "GameOver!" << endl;
    squares.clear();
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp.push_back(0);
        }
        squares.push_back(temp);
        temp.clear();
    }
    Spawn();
}

void DrawGrid(){
    for(int x = 0; x <= screenWidth; x+=gridSize){
        pos.x = x;
        pos.y = 0;
        pos.w = 1;
        pos.h = screenHeight;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &pos);
    }
    for(int y = 0; y <= screenHeight; y+=gridSize){
        pos.x = 0;
        pos.y = y;
        pos.w = screenWidth;
        pos.h = 1;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &pos);
    }
    pos.x = screenWidth - 1;
    pos.y = 0;
    pos.w = 1;
    pos.h = screenHeight;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &pos);

    pos.x = 0;
    pos.y = screenHeight - 1;
    pos.w = screenWidth;
    pos.h = 0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &pos);
}

double ScaleNum(double n, double minN, double maxN, double min, double max){
    return (((n - minN) / (maxN - minN)) * (max - min)) + min;
}
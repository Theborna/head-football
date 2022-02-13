#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <SDL2/SDL_mixer.h> //channel 2 : bgMusic, others:effects
#include <string>
#include <sstream>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fstream>
using namespace std;
// initiailizing functions and classses
// details and explanitions in the end of the screen
// functions
// to specify the state for the game loop
typedef enum gameState
{
    START_MENU,
    SETTINGS_MENU,
    LEADER_BOARD,
    SELECTION_MENU,
    INGAME,
    INGAME_PAUSED,
    END_MENU,
    GAME_START,
    POWERUPS_MENU, // insane
    COLLECTION_MENU,
    CLOSED,
    TRANSITION // insanity cuz it doesn't even exist
};
typedef struct characterCard;
class character;
typedef struct player;
class ball;
class game;
class button;
class bubble;
const int H = 800, W = 1280; // screen dimentions
float fastSine(float x);
Uint32 ColourToUint(int R, int G, int B);
float easeBackinOut(float p1, float p2, int time, int totalTime, float overshoot = 1);
int *faseleReact(int x, int y);
float parabola(float P1, float Pmax, int time, int totalTime);
float easeJump(float P1, float Pmax, int &time, int totalTime);
bool mousePress(SDL_Event *event);
void getSlider(int &slide, SDL_Rect rect, SDL_Event *event);
void getTypingInput(string &s, SDL_Event *event);
void getInputIngame(SDL_Event *Event, character &Character1, character &Character2);
void setDefaultAttributes(characterCard &stats, SDL_Renderer *renderer);
void setDefaultStats(characterCard *stats);
void betterText(SDL_Renderer *renderer, string S, int x, int y, int r, int g, int b, int a, int size, string Font = "arial");
void drawZoozanaghe(int x[4], int y[4], SDL_Color *mainColor, SDL_Color *borderColor, SDL_Renderer *renderer);
void drawShadedZoozanaghe(int x[4], int y[4], SDL_Color *mainColor, SDL_Renderer *renderer);
void selectionMenuText(string S1, string S2, int &animTime, SDL_Renderer *renderer, bool which, string S3 = "");
void selectionScreenButtonsInit(button *buttons, SDL_Renderer *renderer);
void initStartMenuButtons(button *startButtons, SDL_Renderer *renderer);
void initPauseMenuButtons(button *startButtons, SDL_Renderer *renderer);
void endMenuButtonInit(button &homeButton, SDL_Renderer *renderer);
void settingsMenuButtonsInit(button *settingButtons, SDL_Renderer *renderer);
void ingameFxInit(Mix_Chunk **chunks);
void showEndScreen(player player1, player player2, SDL_Renderer *renderer, int time);
void screenShot(SDL_Renderer *renderer); // takes a screenshot of the screen and saves it to screenshot.bmp
void getStatsFromFile(string S);
string tdigitnum(int num);
string timeplayed(int t);
string timeplaying(int t);
bool lowercasehast(char a);
bool uppercasehast(char a);
char lowercasee(char a);
bool issmaller(string a, string b);
bool yekian(string a, string b);
void quicksort(string arr[], int n, int s);
void reversescores(string names[], int goalzade[], int goalkhorde[], int score[], int duration[], int timesplayed[], int gameswon[], int gameslost[], int n);
void quicksortscores(string names[], int goalzade[], int goalkhorde[], int score[], int duration[], int timesplayed[], int gameswon[], int gameslost[], int n, int s);
bool hast(string a, string b);
void savescore(string playername, double duration, int goalzade, int goalkhorde, int score);
int afterspace(int n, string line);
string namefromline(string line);
int goalzadefromline(string line);
int goalkhordefromline(string line);
int scorefromline(string line);
int durationfromline(string line);
void setarrnull(string arr[], int n);
void setintarrnull(int arr[], int n);
void readfile(string names[], int goalzade[], int goalkhorde[], int score[], int duration[], int timesplayed[], int gameswon[], int gameslost[], int &n);
void readnames(string names[], int &n);
void namesuggestion(string arrr[], string s, int &j);
int Mp = 10, Mbc = 2, Mb = 2;
float g = 3;
const double B = 0.05, Alpha = 0.7;
const double dt = 1;
const int tirakPos[3] = {100, W - 100, 250}, rtirak = 20;
bool DAAAANNCEEE = false;
// classes
class button // button class by Borna Khodabandeh
{
private:
    double x, y;
    double h, w, w0, h0;
    bool pressed;
    int animationVar;
    int animationTime, pressTime;

public:
    string name;
    SDL_Texture *texture;
    button() // default constructor
    {        // chon delam mikhad XD
    }
    button(double X, double Y, double H, double W, SDL_Texture *Texture, int AnimationVar, string Name = "") // fancy
    {
        x = X;
        y = Y;
        h = H;
        w = W;
        h0 = h;
        w0 = w;
        texture = Texture;
        pressed = false;
        animationVar = AnimationVar;
        animationTime = 0;
        pressTime = 0;
        name = Name;
    }
    void drawButton(SDL_Renderer *renderer)
    {
        SDL_Rect rect;
        rect.x = x - w / 2;
        rect.y = y - h / 2;
        rect.h = h;
        rect.w = w;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
    bool isHowering(int xp, int yp)
    {
        return ((xp > x - w / 2) && (xp < x + w / 2)) && ((yp > y - h / 2) && (yp < y + h / 2));
    }
    void checkPress(SDL_Event *event)
    {
        pressTime++;
        int xp, yp;
        SDL_GetMouseState(&xp, &yp);
        if (isHowering(xp, yp) && mousePress(event))
            pressed = true;
        else
            pressed = false;
        pressTime %= 10;
        pressed *= pressTime != 0;
    }
    void animate()
    {
        int xp, yp;
        SDL_GetMouseState(&xp, &yp);
        switch (animationVar)
        {
        case 1:
            if (!isHowering(xp, yp))
            {
                if (animationTime > 0)
                    animationTime -= 2;
                else
                    break;
                SDL_SetTextureAlphaMod(texture, easeBackinOut(250, 150, animationTime, 25, 0));
            }
            SDL_SetTextureAlphaMod(texture, easeBackinOut(250, 150, animationTime, 25, 0));
            if (animationTime < 25)
                animationTime++;
            break;
        default:
            if (!isHowering(xp, yp))
            {
                if (animationTime > 0)
                    animationTime -= 5;
                else
                    break;
                w = easeBackinOut(w0, 1.15 * w0, animationTime, 48);
                h = easeBackinOut(h0, 1.15 * h0, animationTime, 48);
            }
            w = easeBackinOut(w0, 1.15 * w0, animationTime, 48);
            h = easeBackinOut(h0, 1.15 * h0, animationTime, 48);
            if (animationTime < 48)
                animationTime += 2;
            break;
        }
    }
    bool pressGet() { return pressed; }
    void pressSet(bool Press) { pressed = Press; }
    void resetAnimation() { animationTime = 0; }
    void setposition(int p[2])
    {
        x = p[0];
        y = p[1];
    }
};
class particle // particle class by Borna Khodabandeh
{
protected:
    string type;          // dust,fire,water,confetti,default,etc.
    double xv, yv;        // speed
    double mouseAcc;      // variable to specify reaction to the mouse
    double angle;         // angle of the particle texture
    SDL_Texture *texture; // loaded texture
    double size;          // size of texture
    int avgSize;          // average size of textures
    int radius;           // the radius which particles generate in
    int variant;          // variant of the texture of each type

public:
    SDL_Renderer *renderer;
    double killer;              // rate of destruction
    double xspeed, yspeed;      // speed of environment
    double xp, yp;              // position
    double defaultAcceleration; // constant acceleration for Y axis
    particle()                  // default constructor
    {
        type = ' ';
        defaultAcceleration = 0;
        xp = 0;
        yp = 0;
        xv = 0;
        yv = 0;
        mouseAcc = 0;
        angle = 0;
        texture = 0;
        size = 0;
        avgSize = 25;
        radius = 50;
        variant = 0;
        xspeed = 0;
        yspeed = 0;
        killer = 0.95;
    } // full constructor if fancy
    particle(string Type, double DefaultAcceleration, double Xp, double Yp, double Xv, double Yv, double MouseAcc, double Angle, SDL_Texture *Texture, double Size, int AvgSize, int Radius, int Variant, SDL_Renderer *Renderer)
    {
        type = Type;
        defaultAcceleration = DefaultAcceleration;
        xp = Xp;
        yp = Yp;
        xv = Xv;
        yv = Yv;
        mouseAcc = MouseAcc;
        angle = Angle;
        texture = Texture;
        size = Size;
        avgSize = AvgSize;
        radius = Radius;
        variant = Variant;
        xspeed = 0;
        yspeed = 0;
        renderer = Renderer;
    }
    void randomInit(int seed = 0) // sets random initial conditions (position , speed , angle , size)
    {
        seed += time(NULL);
        srand(seed);
        int x = rand() % (2 * radius) - radius;
        int y = rand() % (2 * radius) - radius;
        while (x * x + y * y > radius * radius)
        {
            x = rand() % (2 * radius) - radius;
            y = rand() % (2 * radius) - radius;
        }
        xp = x;
        yp = y;
        int xspd = rand() % 20 - 10;
        int yspd = rand() % 20 - 10;
        while (xspd * xspd + yspd * yspd > 10 * 10)
        {
            xspd = rand() % 20 - 10;
            yspd = rand() % 20 - 10;
        }
        xv = xspd;
        yv = yspd;
        angle = rand() % 360;
        size = avgSize * (1 + 0.05 * (rand() % 10));
    }
    void drawParticle(int x0, int y0) // renders the particle
    {
        SDL_Rect rect;
        rect.x = x0;
        rect.y = y0;
        rect.h = size;
        rect.w = size;
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle * M_PI / 180.0, NULL, SDL_FLIP_NONE);
    }
    void update() // updates conditions (position , speed , angle , size)
    {
        xp += xv;
        yp += yv;
        xp += xspeed;
        yp += yspeed;
        angle += 100;
        yv += defaultAcceleration;
        xv -= mouseAcc * xp;
        yv -= mouseAcc * yp;
        if (size > 1)
            size *= killer;
    }
    void setTexture() // sets particle texture as a given pic
    {
        switch (type[0])
        {
        case 'f':
            switch (variant % 3)
            {
            case 0:
                texture = IMG_LoadTexture(renderer, "./textures/fire1.jpg");
                break;
            case 1:
                texture = IMG_LoadTexture(renderer, "./textures/fire2.jpg");
                break;
            case 2:
                texture = IMG_LoadTexture(renderer, "./textures/fire3.jpg");
                break;
            }
            break;
        case 'd':
            switch (variant % 3)
            {
            case 0:
                texture = IMG_LoadTexture(renderer, "./textures/dust1.jpg");
                break;
            case 1:
                texture = IMG_LoadTexture(renderer, "./textures/dust2.jpg");
                break;
            case 2:
                texture = IMG_LoadTexture(renderer, "./textures/dust3.jpg");
                break;
            }
            break;
        case 'w':
            switch (variant % 3)
            {
            case 0:
                texture = IMG_LoadTexture(renderer, "./textures/water1.jpg");
                break;
            case 1:
                texture = IMG_LoadTexture(renderer, "./textures/water2.jpg");
                break;
            case 2:
                texture = IMG_LoadTexture(renderer, "./textures/water3.jpg");
                break;
            }
            break;
        case 'c':
            switch (variant % 5)
            {
            case 0:
                texture = IMG_LoadTexture(renderer, "./textures/confetti1.png");
                break;
            case 1:
                texture = IMG_LoadTexture(renderer, "./textures/confetti2.png");
                break;
            case 2:
                texture = IMG_LoadTexture(renderer, "./textures/confetti3.png");
                break;
            case 3:
                texture = IMG_LoadTexture(renderer, "./textures/confetti4.png");
                break;
            case 4:
                texture = IMG_LoadTexture(renderer, "./textures/confetti5.png");
                break;
            }
            break;
        default:
            texture = IMG_LoadTexture(renderer, "./textures/particle.jpg");
            break;
        }
    }
    void setType(string S = " ") // sets the particle type and modifies default acceleration
    {
        type = S;
        switch (type[0])
        {
        case 'f':
            defaultAcceleration = -1;
            break;
        case 'd':
            defaultAcceleration = 1;
            break;
        case 'w':
            defaultAcceleration = 5;
            break;
        default: // includes confetti
            defaultAcceleration = 0;
            break;
        }
    }
    void defaultTypeChange(string S) // to change both the texture and behaviour
    {
        setType(S);
        setTexture();
    }
    void setRadius(int x) { radius = x; }                          // radius setter
    void setMouseAcceleration(double x) { mouseAcc = x / 1000.0; } // mouseacc setter
    void setPicVariant(int x) { variant = x; }                     // shuffles through varients of pictures for set type
    void setAvgSize(int x) { avgSize = x; }                        // avgSize setter
};
class alottaParticles
{
private:
    SDL_Renderer *renderer;
    string type;
    int N;
    particle *particles;

public:
    bool draw, stream;
    int xspeed, yspeed;
    int streamTime;
    alottaParticles(SDL_Renderer *Renderer, string Type, int n, int r, double mouseAcc, int Size, double defaultAcc = 0, double killrate = 0.95)
    {
        renderer = Renderer;
        type = Type;
        N = n;
        draw = false;
        stream = false;
        streamTime = 0;
        particles = new particle[N];
        xspeed = 0;
        yspeed = 0;
        for (int i = 0; i < N; i++)
        {
            particles[i].renderer = renderer;
            particles[i].setPicVariant(i);
            particles[i].setType(Type);
            particles[i].setTexture();
            particles[i].randomInit(i);
            particles[i].setRadius(r);
            particles[i].defaultTypeChange(type);
            particles[i].setMouseAcceleration(mouseAcc);
            particles[i].setAvgSize(Size);
            particles[i].defaultAcceleration = defaultAcc;
            particles[i].killer = killrate;
        }
    }
    void UpdateAndRender(int x0, int y0)
    {
        for (int i = 0; i < N && draw; i++)
        {
            particles[i].xspeed = xspeed;
            particles[i].yspeed = yspeed;
            particles[i].update();
            particles[i].drawParticle(x0 + particles[i].xp, y0 + particles[i].yp);
        }
    }
    void randomize()
    {
        for (int i = 0; i < N; i++)
            particles[i].randomInit(i);
    }
    void changeSize(int x)
    {
        for (int i = 0; i < N; i++)
            particles[i].setAvgSize(x);
    }
    void clickStream(SDL_Event *event)
    {
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            draw = true;
            if (event->button.button == SDL_BUTTON_LEFT)
                stream = false;
            if (event->button.button == SDL_BUTTON_RIGHT)
                stream = true;
        }
        if (event->type == SDL_MOUSEBUTTONUP)
        {
            draw = false;
            randomize();
        }
        int x, y;
        SDL_GetMouseState(&x, &y);
        drawStream(x, y);
    }
    void drawStream(int x0, int y0)
    {
        UpdateAndRender(x0, y0);
        for (int i = 0; i < 10 && stream; i++)
            particles[(5 * streamTime + i) % N].randomInit(5 * streamTime + i);
        if (stream)
            streamTime++;
    }
};
typedef struct characterCard
{
public:
    string name;                                               // name of the football star
    int jump, maxJump;                                         // maximum jump height
    int shootingSpeed;                                         // abolute value of the speed
    int maxShoot;                                              // maximum shooting height for secondary shot
    float scale, scale0;                                       // relative scale of the character
    double speed;                                              // speed on the x axis
    string team;                                               // name of the team
    int powerUp, powerUp0;                                     // the specific powerups used by each character
    SDL_Texture *Tcard, *Thead, *Tbody, *Tfoot, *Tfan, *Thand; // textures used
};
class character // character class by Borna Khodabandeh & Sepehr heidari
{
public:
    short int x, y, y0, x0, xpush, deltaPush;                            // general position for movement
    double vx, vy;                                                       // speed element
    bool stunned, cloned, sizeCheat;                                     // stunned state or no / cloned or no
    bool jump, pushback;                                                 // is jumping or no
    bool powerUp, getPower;                                              // player has activated powerup
    clock_t powerUpAct, stunAct;                                         // timers
    int jumpTime, animationTime, tiltTime, pushbackTime, kicknPunchTime; // time integer for the jump and movement animations
    int powerUpTime, stunTime;                                           // time in the poweruptimer and time waiting for stun
    characterCard *stats;                                                // general stats of the character
    SDL_Renderer *renderer;                                              // renderer
    SDL_Rect handRect, headRect, bodyRect, footRect;                     // body parts
    bool flipped;                                                        // flipping character for p1 and p2/ def:p1
    int variant;                                                         // default/kicking/punching
    character()
    { // doooooosst daraaaaammmm
    }
    character(short int X, short int Y, characterCard *Stats, SDL_Renderer *Renderer, bool Flipped)
    {
        x = X;
        y = Y;
        x0 = x;
        y0 = y;
        deltaPush = 200;
        vy = 0;
        vx = 0;
        getPower = true;
        stats = Stats;
        powerUp = false;
        powerUpAct = clock();
        stunAct = clock();
        pushback = false;
        cloned = false;
        renderer = Renderer;
        jump = false;
        jumpTime = 0;
        animationTime = 0;
        tiltTime = 0;
        powerUpTime = 0;
        pushbackTime = 0;
        kicknPunchTime = 0;
        stunTime = 0;
        stunned = false;
        flipped = Flipped;
        variant = 0;
        sizeCheat = false;
        rectSetup();
    }
    void drawCharacter()
    {
        vx /= 2;
        int signHightAng = (vx > 0) ? 1 : -1;
        float hightAng = 10 * (y0 - y) / (float)stats->jump * signHightAng;
        vx += hightAng;
        SDL_Point pointHand, pointFoot, pointBody;
        // rotation centers
        if (!flipped)
        {
            pointHand = {20, 0};
            pointFoot = {20, 0};
            pointBody = {48, 80};
        }
        else
        {
            pointHand = {20, 0};
            pointFoot = {60, 0};
            pointBody = {48, 80};
        }
        // set scale
        pointBody.x *= stats->scale * 1.4;
        pointBody.y *= stats->scale * 1.4;
        pointHand.x *= stats->scale * 1.4;
        pointHand.y *= stats->scale * 1.4;
        pointFoot.x *= stats->scale * 1.4;
        pointFoot.y *= stats->scale * 1.4;
        rectSetup();
        // tilt
        double tiltState;
        if (tiltTime < 11)
            tiltState = easeBackinOut(-0.5, 1, tiltTime, 10, 2);
        else
            tiltState = 1;
        if (vx - hightAng == 0) // not to be silly
            tiltTime = 0;
        else
            tiltTime++;
        // adjust for tilt in movement
        headRect.x += 1.4 * tiltState * stats->scale * (180 * 3.14 * (stunned ? (2 * flipped - 1) * 30 : vx) / 180) / 2;
        headRect.y += 1.4 * tiltState * stats->scale * (180 * (3.14 * (stunned ? (2 * flipped - 1) * 30 : vx)) / 180) * ((3.14 * vx) / 180) / 2;
        handRect.x += 1.4 * tiltState * stats->scale * ((30 * 3.14 * vx) / 180 + 20 * ((3.14 * vx) / 180) * ((3.14 * vx) / 180)) / 2;
        handRect.y += 1.4 * tiltState * stats->scale * (30 * ((3.14 * vx) / 180) * ((3.14 * vx) / 180) + (20 * 3.14 * vx) / 180) * (2 * flipped - 1) / 2;
        footRect.x -= 1.4 * tiltState * stats->scale * (10 * 3.14 * vx) / 180;
        footRect.y -= 1.4 * tiltState * stats->scale * 10 * ((3.14 * vx) / 180) * ((3.14 * vx) / 180) / 2;
        // punching and kicking
        float extraSize = 1, extraAngle = 0;
        if (variant)
        {
            extraSize = parabola(1, 2, kicknPunchTime, 12);
            extraAngle = parabola(0, 120, kicknPunchTime, 12);
            kicknPunchTime++;
            if (kicknPunchTime == 12)
                variant = 0;
            kicknPunchTime %= 12;
        }
        // rendering
        SDL_RenderCopyEx(renderer, stats->Thand, NULL, &handRect, 5 * fastSine(animationTime) + tiltState * vx, &pointHand, (!flipped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        handRect.x -= (2 * flipped - 1) * 70 * stats->scale * 1.4;
        handRect.w /= 1.25;
        handRect.h /= 1.25;
        if (variant == 1)
        {
            handRect.w *= extraSize;
            handRect.h *= extraSize;
        }
        SDL_RenderCopyEx(renderer, stats->Tfoot, NULL, &footRect, !(y < y0) * tiltState * vx * fastSine(animationTime / 1.5) + tiltState * (jump)*30 * ((vx - hightAng != 0) ? ((vx - hightAng < 0) ? (-1) : (1)) : ((2 * flipped - 1) * (y0 - y) / (float)stats->jump)), &pointFoot, (flipped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        footRect.x += (2 * flipped - 1) * 10 * stats->scale * 1.4;
        footRect.y += (4 * flipped - 1) * 2 * stats->scale * 1.4;
        if (variant == 2)
        {
            footRect.x += footRect.w / 2;
            footRect.y += footRect.h / 2;
            footRect.w *= extraSize / 1.2;
            footRect.h *= extraSize / 1.2;
            pointFoot.x *= extraSize / 1.2;
            pointFoot.y *= extraSize / 1.2;
            pointFoot.y -= 30;
            pointFoot.x -= (2 * flipped - 1) * 30;
            footRect.x -= footRect.w / 2;
            footRect.y -= footRect.h / 2;
        }
        SDL_RenderCopyEx(renderer, stats->Tfoot, NULL, &footRect, (variant == 2) * (2 * flipped - 1) * extraAngle / 3 - 1 * !(y < y0) * tiltState * vx * fastSine(animationTime / 1.5) + tiltState * (jump)*30 * ((vx - hightAng != 0) ? ((vx - hightAng < 0) ? (-1) : (1)) : ((2 * flipped - 1) * (y0 - y) / (float)stats->jump)), &pointFoot, (flipped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        if (variant == 1)
            flipped = !flipped; // flip the hand accodwdrdingly
        SDL_RenderCopyEx(renderer, stats->Thand, NULL, &handRect, (variant == 1) * (2 * !flipped - 1) * extraAngle + 5 * fastSine(animationTime) + tiltState * vx, &pointHand, (flipped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        if (variant == 1)
            flipped = !flipped;
        SDL_RenderCopyEx(renderer, stats->Tbody, NULL, &bodyRect, tiltState * vx - (2 * flipped - 1) * stunned * 15, &pointBody, (!flipped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, stats->Thead, NULL, &headRect, tiltState * vx / 1.5 - (2 * flipped - 1) * stunned * 15 / 1.5, NULL, (flipped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        if (vx != 0)
            animationTime++;
        vx -= hightAng;
        vx *= 2;
        // drawing the stun animation
        if (stunned)
        {
            if (stunTime > 0)
                stunTime = stunAct + 3000 - clock();
            else
                stunTime = 0;
            SDL_Texture *frame = IMG_LoadTexture(renderer, ("./textures/frame" + to_string((stunTime / 50) % 10 + 1) + ".png").c_str());
            SDL_Rect halo = {x - 75, y - 350 * stats->scale, 150, 80};
            SDL_RenderCopy(renderer, frame, NULL, &halo);
            SDL_DestroyTexture(frame);
            if (!stunTime)
                stunned = false;
        }
    }
    void UpdateCharacter()
    {
        if (stunned)
        {
            vx = 0;
            jump = false;
        }
        // wall collisions
        if (x < 80)
            x = 80;
        if (x > 1200)
            x = 1200;
        // jumping mechanism
        int tempY = y;
        if (jump)
            y = easeJump(y0, y0 - stats->jump, jumpTime, 24);
        else if (jumpTime)
            y = easeJump(y0, y0 - stats->jump, jumpTime, 24);
        else
            y = y0;
        vy = y - tempY; // chon doost nadashtam vy bezaram be shoma che
        if (pushback && !pushbackTime)
            xpush = x;
        int khkh = pushbackTime + 1;
        if (pushback)
            if (pushbackTime < ((stunned) ? 12 : 6))
                x += easeJump(xpush, xpush + deltaPush, khkh, ((stunned) ? 24 : 12)) - easeJump(xpush, xpush + deltaPush, pushbackTime, ((stunned) ? 24 : 12));
        if (pushbackTime == 6)
            pushback = false;
        jumpTime %= 24;
        pushbackTime %= ((stunned) ? 12 : 6);
        x += vx;
    }
    int *hitBoxes()
    // [0] : head radius / [1] : head x / [2] : head y
    // [3] : body x1 / [4] : body y1 / [5] : body x2 / [6] : body y2
    // [7] : foot x1 / [8] : foot y1 / [9] : foot x2 / [10] : foot y2
    {
        rectSetup();
        // tilt
        double tiltState;
        if (tiltTime < 11)
            tiltState = easeBackinOut(-0.5, 1, tiltTime, 10, 2);
        else
            tiltState = 1;
        if (vx == 0)
            tiltTime = 0;
        else
            tiltTime++;
        headRect.x += 1.4 * tiltState * stats->scale * ((180 * 3.14 * vx) / 180) / 2;
        headRect.y += 1.4 * tiltState * stats->scale * (180 * ((3.14 * vx) / 180) * ((3.14 * vx) / 180)) / 2;
        footRect.x -= 1.4 * tiltState * stats->scale * (10 * 3.14 * vx) / 180;
        footRect.y -= 1.4 * tiltState * stats->scale * 10 * ((3.14 * vx) / 180) * ((3.14 * vx) / 180) / 2;
        int headcenterH, headcenterW, headradius;
        headcenterH = headRect.y + headRect.h / 2;
        headcenterW = headRect.x + headRect.w / 2;
        headradius = (headRect.w / 5) + (headRect.h / 5);
        short int s = 2 * flipped - 1;
        int *mokhtasat = new int[33];
        mokhtasat[0] = headradius;
        mokhtasat[1] = headcenterW;
        mokhtasat[2] = headcenterH;
        mokhtasat[3] = bodyRect.x + bodyRect.w / 4 - flipped * bodyRect.w / 8;
        mokhtasat[4] = bodyRect.y;
        mokhtasat[5] = bodyRect.x + bodyRect.w - bodyRect.w / 8 - flipped * bodyRect.w / 8;
        mokhtasat[6] = bodyRect.y + bodyRect.h;
        mokhtasat[7] = footRect.x + s * footRect.w / 6;
        mokhtasat[8] = footRect.y + footRect.h / 3;
        mokhtasat[9] = footRect.x + footRect.w + s * footRect.w / 6;
        mokhtasat[10] = footRect.y + footRect.h;
        if (variant == 2)
        {
            mokhtasat[7] -= s * footRect.w / 2;
            mokhtasat[9] -= s * footRect.w / 2;
            mokhtasat[8] -= footRect.h / 3;
        }
        for (int i = 11; i < 33; i++)
            mokhtasat[i] = mokhtasat[i % 11];
        if (cloned)
            for (int i = 6, k = 2 * i; i < 16; i++, (i <= 10) ? (k = 2 * i) : (k = 2 * i + 1))
                mokhtasat[k] = mokhtasat[k - 11] + 120 * s;
        else
            for (int i = 6, k = 2 * i; i < 16; i++, (i <= 10) ? (k = 2 * i) : (k = 2 * i + 1))
                mokhtasat[k] = -2000;
        // check
        // circleRGBA(renderer, mokhtasat[1], mokhtasat[2], mokhtasat[0], 255, 255, 255, 255);
        // rectangleRGBA(renderer, mokhtasat[3], mokhtasat[4], mokhtasat[5], mokhtasat[6], 255, 255, 255, 255);
        // rectangleRGBA(renderer, mokhtasat[7], mokhtasat[8], mokhtasat[9], mokhtasat[10], 255, 255, 255, 255);
        // circleRGBA(renderer, mokhtasat[1 + 11], mokhtasat[2 + 11], mokhtasat[0 + 11], 255, 255, 255, 255);
        // rectangleRGBA(renderer, mokhtasat[3 + 11], mokhtasat[4 + 11], mokhtasat[5 + 11], mokhtasat[6 + 11], 255, 255, 255, 255);
        // rectangleRGBA(renderer, mokhtasat[7 + 11], mokhtasat[8 + 11], mokhtasat[9 + 11], mokhtasat[10 + 11], 255, 255, 255, 255);
        // circleRGBA(renderer, mokhtasat[1 + 22], mokhtasat[2 + 22], mokhtasat[0 + 22], 255, 255, 255, 255);
        // rectangleRGBA(renderer, mokhtasat[3 + 22], mokhtasat[4 + 22], mokhtasat[5 + 22], mokhtasat[6 + 22], 255, 255, 255, 255);
        // rectangleRGBA(renderer, mokhtasat[7 + 22], mokhtasat[8 + 22], mokhtasat[9 + 22], mokhtasat[10 + 22], 255, 255, 255, 255);
        return mokhtasat;
    }
    void colisionBetweenTwo(character *character2) // needs to be called once
    {
        SDL_Rect rect1, rect2;
        int *arr1 = hitBoxes(), *arr2 = character2->hitBoxes();
        rect1.x = arr1[3];
        rect1.y = arr1[4];
        rect1.w = arr1[9] - arr1[3];
        rect1.h = arr1[10] - arr1[4];
        rect2.x = arr2[7];
        rect2.y = arr2[4];
        rect2.w = arr2[5] - arr2[7];
        rect2.h = arr2[10] - arr2[4];
        // boxColor(renderer, rect1.x, rect1.y, rect1.x + rect1.w, rect1.y + rect1.h, 0xFFFFFFFF);
        // boxColor(renderer, rect2.x, rect2.y, rect2.x + rect2.w, rect2.y + rect2.h, 0xFFFFFFFF);
        SDL_bool isColliding = SDL_HasIntersection(&rect1, &rect2);
        if (isColliding == SDL_TRUE)
        {
            vx = 0;
            character2->vx = 0;
            int s = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2);
            if (abs(s) > 40)
            {
                x += (stats->scale + character2->stats->scale) * 4 * 500 * 500 / (s * s * s);
                character2->x -= (stats->scale + character2->stats->scale) * 4 * 500 * 500 / (s * s * s);
            }
            else if (s == 0)
            {
                x += 2;
                character2->x -= 2;
            }
            else
            {
                x += s;
                character2->x -= s;
            }
        }
    }
    void resetCharacter()
    {
        x = x0;
        y = y0;
        vx = 0;
    }
    void rectSetup()
    {
        if (!flipped)
        {
            handRect = {-20 - 35, -32 - 5 + 25 - 5 - 25 - 50, 40, 64};
            headRect = {-48 + 3, -64 - 80 - 5 - 25 - 50, 96, 128};
            bodyRect = {-48, -48 - 5 - 25 - 50, 96, 96};
            footRect = {-40 + 40, -20 + 45 - 25 - 50, 70, 50};
        }
        else
        {
            handRect = {-20 + 35, -32 - 5 + 25 - 5 - 25 - 50, 40, 64};
            headRect = {-48 - 3, -64 - 80 - 5 - 25 - 50, 96, 128};
            bodyRect = {-48, -48 - 5 - 25 - 50, 96, 96};
            footRect = {-40 - 30, -20 + 45 - 25 - 50, 70, 50};
        }
        {
            if (stats->name == "moSaleh")
            {
                headRect.x += headRect.w / 2;
                headRect.w *= 1.3;
                headRect.x -= headRect.w / 2;
            }
            handRect.h *= stats->scale * 1.4;
            handRect.w *= stats->scale * 1.4;
            handRect.x *= stats->scale * 1.4;
            handRect.y *= stats->scale * 1.4;
            headRect.h *= stats->scale * 1.4;
            headRect.w *= stats->scale * 1.4;
            headRect.x *= stats->scale * 1.4;
            headRect.y *= stats->scale * 1.4;
            footRect.h *= stats->scale * 1.4;
            footRect.w *= stats->scale * 1.4;
            footRect.x *= stats->scale * 1.4;
            footRect.y *= stats->scale * 1.4;
            bodyRect.h *= stats->scale * 1.4;
            bodyRect.w *= stats->scale * 1.4;
            bodyRect.x *= stats->scale * 1.4;
            bodyRect.y *= stats->scale * 1.4;
            headRect.x += x;
            headRect.y += y;
            handRect.x += x;
            handRect.y += y;
            footRect.x += x;
            footRect.y += y;
            bodyRect.x += x;
            bodyRect.y += y;
        }
    }
    void activatePowerUp(character &Character2, ball &Ball);
    void jumpToBall(ball &Ball); // for head fire powerUp
};
class bubble
{
public:
    int x, y, r, x0, y0;
    int spawntime, duration, abillity;
    bool khordeshode, isonfire, toopegonde;
    SDL_Texture *tbubble;
    SDL_Rect rectbubble;
    SDL_Renderer *renderer;
    character *Characters[2];
    alottaParticles *particles;
    bubble(int Spawntime, SDL_Renderer *Renderer, character *Character1, character *Character2)
    {
        x0 = -200;
        y0 = 3000;
        isonfire = false;
        Characters[0] = Character1;
        Characters[1] = Character2;
        khordeshode = false;
        spawntime = Spawntime;
        r = 40;
        x = r + 25 + rand() % (W - 50 - 2 * r);
        y = r + 100 + rand() % (H - 300 - 2 * r);
        duration = 10 + rand() % 10;
        abillity = rand() % 5;
        renderer = Renderer;
        tbubble = IMG_LoadTexture(renderer, ("./bubbles/" + to_string(abillity) + ".png").c_str());
        rectbubble = {x - r, y - r, 2 * r, 2 * r};
        particles = new alottaParticles(renderer, "water", 20, r, 0, 15);
    }
    void showbubble()
    {
        if (time(NULL) <= (spawntime + duration) && time(NULL) >= spawntime && !khordeshode)
            SDL_RenderCopy(renderer, tbubble, NULL, &rectbubble);
        particles->UpdateAndRender(x0, y0);
    }
    void khordeshod(int playerekhordeshode)
    {
        khordeshode = true;
        switch (abillity)
        {
        case 0:
            Characters[playerekhordeshode]->powerUpAct = 0;
            Characters[playerekhordeshode]->powerUpTime = 20000;
            break;
        case 1:
            Characters[playerekhordeshode]->powerUpAct = clock() - 4000;
            Characters[playerekhordeshode]->powerUpTime = 0;
            break;
        case 2:
            isonfire = true;
            break;
        case 3:
            break;
        case 4:
            toopegonde = true;
            break;
        default:
            break;
        }
        nextbubble();
    }
    void nextbubble()
    {
        x0 = x;
        y0 = y;
        spawntime = time(NULL) + rand() % 10 + 10;
        khordeshode = false;
        x = r + 25 + rand() % (W - 50 - 2 * r);
        y = r + 100 + rand() % (H - 300 - 2 * r);
        duration = 10 + rand() % 10;
        abillity = rand() % 5;
        tbubble = IMG_LoadTexture(renderer, ("./bubbles/" + to_string(abillity) + ".png").c_str());
        rectbubble = {x - r, y - r, 2 * r, 2 * r};
        particles->randomize();
        particles->draw = true;
    }
};
class ball // ball class by Borna Khodabandeh & Sepehr heidari
{
private:
    SDL_Renderer *renderer;
    string ballname;                                         // to specify which ball texture to use
    double vx, vy;                                           // speed components
    double theta;                                            // angle the texture is drawn with
    double b, alpha;                                         // energy dissapassion
    character *Characters[2];                                // ingame characters to act accordingly to
    int fireTime;                                            // how much time the ball is on fire
    Mix_Chunk **effects;                                     //[0] grass / [1] fire
    alottaParticles *fireParticles, *grassParticles, *trail; // particles cuz im fancy like that

public:
    int x, y;                 // pos
    int r0, r;                // radius and init radius if we wanna make it bigger
    SDL_Texture *textureball; // le texture
    bool hittirak;            // bounce off tirak or nah
    bool **hitplayer;         // [i][j]: j represents the character we hit and i represents the body part
    bool ingoal1, ingoal2;    // did we goal or nah
    bool tuhava;              // rolling around or jumping around
    bool invis, onFire;       // powerup boolians
    int ximpact, yimpact;     // grass
    int lastplayercollison;
    bubble *Bubble;
    ball(string Ballname, int R0, double Vx, double Vy, SDL_Renderer *Renderer, character *character1, character *character2, bubble *bubble0)
    {
        Bubble = bubble0;
        lastplayercollison = 0;
        ballname = Ballname;
        startposition();
        r0 = R0;
        r = r0;
        vx = Vx;
        vy = Vy;
        theta = 0;
        b = B;
        alpha = Alpha;
        hittirak = false;
        hitplayer = new bool *[3];
        for (int i = 0; i < 3; i++)
        {
            hitplayer[i] = new bool[6];
            for (int j = 0; j < 6; j++)
                hitplayer[i][j] = false;
        }
        ingoal1 = false;
        ingoal2 = false;
        tuhava = true;
        renderer = Renderer;
        textureball = IMG_LoadTexture(renderer, ("./balls/" + ballname + ".png").c_str());
        invis = false;
        onFire = false;
        fireTime = 0;
        Characters[0] = character1;
        Characters[1] = character2;
        Mix_Chunk **BallFx;
        BallFx[0] = Mix_LoadWAV("./soundEffects/grass1.wav");
        BallFx[1] = Mix_LoadWAV("./soundEffects/fire.wav");
        BallFx[2] = Mix_LoadWAV("./soundEffects/tirak.wav");
        effects = BallFx;
        fireParticles = new alottaParticles(renderer, "fire", 200, r, 10, 20, -0.5);
        fireParticles->draw = true;
        fireParticles->stream = true;
        grassParticles = new alottaParticles(renderer, "dust", 20, r, 5, 5, 1);
        grassParticles->draw = true;
        ximpact = -200;
        yimpact = -200;
        trail = new alottaParticles(renderer, "bobos", 60, r / 5, 50, 10);
        trail->draw = true;
        trail->stream = true;
    }
    void startposition()
    {
        r = r0;
        x = 640;
        y = 200;
        vx = 2 * rand() % 8 - 8;
        vy = 0;
        theta = 0;
        textureball = IMG_LoadTexture(renderer, ("./balls/" + ballname + ".png").c_str());
    }
    void manipBallSpeed(float xAlpha, float yAlpha)
    {
        vx *= xAlpha;
        vy *= yAlpha;
    }
    void showball()
    {
        int hb, wb;
        SDL_QueryTexture(textureball, NULL, NULL, &wb, &hb);
        SDL_Rect rectball = {x - r, y - r, 2 * r * wb / hb, 2 * r};
        fireParticles->xspeed = -vx / 2;
        fireParticles->yspeed = -vy / 2;
        trail->xspeed = -vx;
        trail->yspeed = -vy;
        if (onFire)
            fireParticles->drawStream(x, y);
        else if (vx * vx + vy * vy > 9)
            trail->drawStream(x, y);
        SDL_RenderCopyEx(renderer, textureball, NULL, &rectball, 180 * theta / M_PI, NULL, SDL_FLIP_NONE);
    }
    void hitbywall(int wallnum)
    {
        if (onFire)
            alpha = 1;
        switch (wallnum)
        {
        case 1:
            vy = alpha * fabs(vy);
            break;
        case 2:
            vx = -alpha * fabs(vx);
            x = W - r;
            break;
        case 3:
            vy = -alpha * fabs(vy);
            vy += g;
            y = H - 50 - r;
            grassParticles->randomize();
            if (fabs(vy) > 4)
                Mix_PlayChannel(2, effects[0], 0);
            break;
        case 4:
            vx = alpha * fabs(vx);
            x = r;
            break;
        default:
            break;
        }
        alpha = Alpha;
        if (onFire && tuhava)
            fireTime = -1;
    }
    void hitgooshetirak(int tiraknum)
    {
        if (!hittirak)
        {
            double vbx = ((vx * double(x - tirakPos[tiraknum]) + vy * double(y - tirakPos[2])) / (double(x - tirakPos[tiraknum]) * double(x - tirakPos[tiraknum]) + double(y - tirakPos[2]) * double(y - tirakPos[2]))) * double(x - tirakPos[tiraknum]);
            double vby = ((vx * double(x - tirakPos[tiraknum]) + vy * double(y - tirakPos[2])) / (double(x - tirakPos[tiraknum]) * double(x - tirakPos[tiraknum]) + double(y - tirakPos[2]) * double(y - tirakPos[2]))) * double(y - tirakPos[2]);
            vx -= 2.0 * vbx;
            vy -= 2.0 * vby;
            hittirak = true;
            Mix_PlayChannel(0, effects[2], 0);
            // sex
        }
    }
    void hitbalayetirak()
    {
        if (!hittirak)
        {
            if (ingoal1 || ingoal2)
                vy = fabs(vy);
            else
                vy = -fabs(vy);
            hittirak = true;
            // sex
        }
    }
    // player body part collisions
    void hitbyplayerhead(int playernum, double vxp, double vyp, int xhp, int yhp, int pos = 0)
    {
        if (!hitplayer[pos][playernum])
        {
            double vbx = (((vx - vxp) * double(x - xhp) + (vy - vyp) * double(y - yhp)) / (double(x - xhp) * double(x - xhp) + double(y - yhp) * double(y - yhp))) * double(x - xhp);
            double vby = (((vx - vxp) * double(x - xhp) + (vy - vyp) * double(y - yhp)) / (double(x - xhp) * double(x - xhp) + double(y - yhp) * double(y - yhp))) * double(y - yhp);
            vx -= 2.0 * (double(Mp) / double(Mp + Mb)) * vbx;
            vy -= 2.0 * (double(Mp) / double(Mp + Mb)) * vby;
            hitplayer[pos][playernum] = true;
        }
        Characters[playernum / 3]->pushback = true;
        int s = (x - xhp) * Characters[playernum / 3]->stats->scale / 0.8;
        if (abs(s) > 30)
            Characters[playernum / 3]->deltaPush = -2.0 * 400 * (double(Mb) / double(Mp + Mb)) * fabs(vxp - vx) / (s);
        else if (s == 0)
            cout << "boos bede" << endl;
        else
            Characters[playernum / 3]->deltaPush = -2.0 * s * (double(Mb) / double(Mp + Mb)) * fabs(vxp - vx) / 10;
    }
    void hitbyplayertrunk(int playernum, double vxp, int x1, int x2)
    {
        int s = (x - Characters[playernum / 3]->x) * Characters[playernum / 3]->stats->scale / 0.8;
        if (!hitplayer[1][playernum])
        {
            vx += 2.0 * (double(Mp) / double(Mp + Mb)) * (vxp - vx);
            hitplayer[1][playernum] = true;
        }
        else
        {
            if (x - r <= x2 && x + r >= x2)
                x = x2 + r;
            else if (x + r >= x1 && x - r <= x1)
                x = x1 - r;
        }
        Characters[playernum / 3]->pushback = true;
        if (abs(s) > 30)
            Characters[playernum / 3]->deltaPush = -2.0 * 400 * (double(Mb) / double(Mp + Mb)) * fabs(vxp - vx) / s;
        else if (s == 0)
            vy += 10;
        else
            Characters[playernum / 3]->deltaPush = -2.0 * s * (double(Mb) / double(Mp + Mb)) * fabs(vxp - vx) / 10;
    }
    void hitbyplayerfeet(int playernum, double vxp, double vyp, int x1, int x2, bool bala)
    {
        if (!hitplayer[2][playernum])
        {
            if (bala)
            {
                if (tuhava)
                    vy += 2.0 * (double(Mp) / double(Mp + Mb)) * (vyp - vy);
                else
                {
                    if ((x2 - x) >= (x - x1))
                        vx -= 100;
                    else
                        vx += 100;
                }
            }
            else
            {
                vx += 2.0 * (double(Mp) / double(Mp + Mb)) * (vxp - vx);
                Characters[playernum / 3]->pushback = true;
                int s = (x - Characters[playernum / 3]->x) * Characters[playernum / 3]->stats->scale / 0.8;
                if (abs(s) > 30)
                    Characters[playernum / 3]->deltaPush = -2.0 * 400 * (double(Mb) / double(Mp + Mb)) * fabs(vxp - vx) / s;
                else if (s == 0)
                    vy += 10;
                else
                    Characters[playernum / 3]->deltaPush = -2.0 * s * (double(Mb) / double(Mp + Mb)) * fabs(vxp - vx) / 10;
            }
            hitplayer[2][playernum] = true;
        }
    }
    void motion() // physics of the ball
    {
        Mb = ((double(r) / double(r0)) * (double(r) / double(r0))) * double(Mbc);
        if (0.5 * Mb * (vx * vx + vy * vy) + Mb * g * (H - y - 50 - r) < 10)
        {
            vy = 0;
            y = H - 50 - r;
        }
        x += int(vx * dt);
        y += int(vy * dt);
        if (onFire)
            theta = atan(vy / vx) - ((vx > 0) ? 45 : 225);
        else
            theta += (vx * dt / double(r));
        b *= !onFire;
        if (tuhava)
        {
            vx += -b * vx * dt;
            vy += (-b * vy + g) * dt;
        }
        else
            vx -= b * vx * dt;
        b = B;
        if (onFire)
            fireTime++;
        fireTime %= 75;
        if (!fireTime && onFire)
        {
            onFire = false;
            textureball = IMG_LoadTexture(renderer, ("./balls/" + ballname + ".png").c_str());
        }
    }
    void checkgoal()
    {
        if ((x < tirakPos[0]) && (y > tirakPos[2]))
            ingoal1 = true;
        else if ((x > tirakPos[1]) && (y > tirakPos[2]))
            ingoal2 = true;
    }
    void checktirakcollison()
    {
        SDL_Rect r1 = {x - r, y - r, 2 * r, 2 * r}, r2 = {0, tirakPos[2], tirakPos[0], H}, r3 = {tirakPos[1], tirakPos[2], W, H};
        if (SDL_HasIntersection(&r1, &r2))
            hitbalayetirak();
        else if (SDL_HasIntersection(&r1, &r3))
            hitbalayetirak();
        else if (((x - tirakPos[0]) * (x - tirakPos[0]) + (y - tirakPos[2]) * (y - tirakPos[2])) <= ((rtirak + r) * (r + rtirak)))
            hitgooshetirak(0);
        else if (((x - tirakPos[1]) * (x - tirakPos[1]) + (y - tirakPos[2]) * (y - tirakPos[2])) <= ((rtirak + r) * (r + rtirak)))
            hitgooshetirak(1);
        else
            hittirak = false;
        // boxColor(renderer, 0, tirakPos[2], tirakPos[0], H, 0x88FFFFFF);
        // boxColor(renderer, tirakPos[1], tirakPos[2], W, H, 0x88FFFFFF);
        // filledCircleColor(renderer, tirakPos[0], tirakPos[2], rtirak, 0x88FFFFFF);
        // filledCircleColor(renderer, tirakPos[1], tirakPos[2], rtirak, 0x88FFFFFF);
    }
    void checkwallcollison()
    {
        if (x <= r + 1)
            hitbywall(4);
        if (y <= r + 1)
            hitbywall(1);
        if ((x + r) >= 1279)
            hitbywall(2);
        if (y + r >= H - 50)
        {
            hitbywall(3);
            tuhava = false;
            if (fabs(vy) > 5)
            {
                ximpact = x;
                yimpact = y;
            }
        }
        else
            tuhava = true;
        grassParticles->changeSize((int)fabs(vy) / 3);
        grassParticles->UpdateAndRender(ximpact, yimpact);
    }
    void checkplayercollison(int number)
    {
        int *arr1 = Characters[number]->hitBoxes(), i;
        for (i = 0; i < ((Characters[number]->cloned) ? 3 : 1); i++)
        {
            if ((arr1[1 + 11 * i] - x) * (arr1[1 + 11 * i] - x) + (arr1[2 + 11 * i] - y) * (arr1[2 + 11 * i] - y) <= (arr1[0 + 11 * i] + r) * (arr1[0 + 11 * i] + r))
                hitbyplayerhead(i + 3 * number, Characters[number]->vx, Characters[number]->vy, arr1[1 + 11 * i], arr1[2 + 11 * i]);
            else
                hitplayer[0][i + 3 * number] = false;
            SDL_Rect r1 = {x - r, y - r, 2 * r, 2 * r}, r2 = {arr1[3 + 11 * i], arr1[4 + 11 * i], arr1[5 + 11 * i] - arr1[3 + 11 * i], arr1[6 + 11 * i] - arr1[4 + 11 * i]};
            if (SDL_HasIntersection(&r1, &r2) == SDL_TRUE)
                hitbyplayertrunk(i + 3 * number, Characters[number]->vx, r2.x, r2.x + r2.w);
            else
                hitplayer[1][i + 3 * number] = false;
            r2 = {arr1[7 + 11 * i], arr1[8 + 11 * i], arr1[9 + 11 * i] - arr1[7 + 11 * i], arr1[10 + 11 * i] - arr1[8 + 11 * i]};
            bool pos;
            SDL_Point pu = {x, y - r}, pr = {x + r, y}, pd = {x, y + r}, pl = {x - r, y};
            if (SDL_PointInRect(&pu, &r2) || SDL_PointInRect(&pd, &r2))
                pos = true;
            else if (SDL_PointInRect(&pr, &r2) || SDL_PointInRect(&pl, &r2))
                pos = false;
            else
                pos = false;
            if (SDL_HasIntersection(&r1, &r2) == SDL_TRUE)
                hitbyplayerfeet(i + 3 * number, Characters[number]->vx, Characters[number]->vy, arr1[7 + 11 * i], arr1[9 + 11 * i], pos);
            else
                hitplayer[2][0 + number * 3 + i] = false;
        }
        if (i == 1)
            for (int k = 0; k < 3; k++)
            {
                hitplayer[k][1 + number * 3] = 0;
                hitplayer[k][2 + number * 3] = 0;
            }
        bool assualt = (Characters[!number]->stats->powerUp == 5 || Characters[!number]->stats->powerUp == 6) && (!Characters[!number]->getPower);
        if (onFire && assualt && Characters[number]->pushback)
        {
            Characters[number]->deltaPush = (2 * Characters[number]->flipped - 1) * 200;
            Characters[number]->stunned = true;
            Characters[number]->stunAct = clock();
            Characters[number]->stunTime = 3000;
        }
    }
    void checkbubble()
    {
        if (Characters[0]->pushback)
            lastplayercollison = 0;
        else if (Characters[1]->pushback)
            lastplayercollison = 1;
        if ((x - Bubble->x) * (x - Bubble->x) + (y - Bubble->y) * (y - Bubble->y) <= (r + Bubble->r) * (r + Bubble->r) && !(Bubble->khordeshode) && time(NULL) <= (Bubble->spawntime + Bubble->duration) && time(NULL) >= Bubble->spawntime)
            (lastplayercollison) ? Bubble->khordeshod(1) : Bubble->khordeshod(0);
        if (Bubble->isonfire)
        {
            setOnFire();
            Bubble->isonfire = false;
        }
        if (Bubble->toopegonde)
        {
            if (!DAAAANNCEEE)
                r = 1.5 * r0;
            Bubble->toopegonde = false;
        }
        if (time(NULL) >= (Bubble->spawntime + Bubble->duration) && !(Bubble->khordeshode))
            Bubble->nextbubble();
    }
    void checkball()
    {
        checkbubble();
        checkgoal();
        checktirakcollison();
        checkwallcollison();
        checkplayercollison(0);
        checkplayercollison(1);
        shootzad(0);
        shootzad(1);
    }
    void setOnFire()
    {
        if (!onFire)
            textureball = IMG_LoadTexture(renderer, ("./balls/fireball.png"));
        manipBallSpeed(1.2, 0.8);
        onFire = true;
        Mix_PlayChannel(0, effects[1], 0);
    }
    void shootzad(int playernum)
    {
        bool fireBall = Characters[playernum]->powerUp && Characters[playernum]->stats->powerUp == 5 && onFire;
        if (Characters[playernum]->variant == 2)
        {
            int *arr = Characters[playernum]->hitBoxes();
            int sx = x - (arr[7] + arr[9]) / 2, sy = y - (arr[8] + arr[10]) / 2;
            if (fireBall)
            {
                sx = x - arr[1];
                sy = y - arr[2];
            }
            int s = (2 * Characters[playernum]->flipped - 1);
            if (sx * s < 0)
                if (sx * sx + sy * sy < (r + 75 * Characters[playernum]->stats->scale) * (r + 75 * Characters[playernum]->stats->scale))
                {
                    if (4 * y < (3 * arr[8] + arr[10])) // shoot up
                    {
                        vy = -100;
                        vx = -100 * s;
                    }
                    else if (y > arr[10]) // shoot down
                    {

                        vy = 100;
                        vx = -100 * s;
                    }
                    else // straight forward
                    {
                        vy = -26;
                        vx = -100 * s;
                    }
                    if (fireBall && abs(W * !playernum - x) > abs(tirakPos[2] / 2 + H / 2 - y))
                    {
                        vx = W * !playernum - x;
                        vy = tirakPos[2] / 2 + H / 2 - y;
                    }
                    double alpha = Characters[playernum]->stats->shootingSpeed * 0.71 / sqrt(vx * vx + vy * vy);
                    manipBallSpeed(alpha, alpha);
                }
            // filledCircleColor(renderer, (arr[7] + arr[9]) / 2, (arr[8] + arr[10]) / 2, 75, 0xFFFFFFFF);
        }
    }
};
void character::jumpToBall(ball &Ball)
{
    if (Ball.y <= headRect.y && !Ball.onFire)
    {
        SDL_Delay(16);
        Ball.manipBallSpeed(0.1, 0.5);
        pushback = true;
        deltaPush = Ball.x + Ball.r - headRect.x;
        jump = true;
        stats->jump = y0 - Ball.y - Ball.r;
    }
}
void character::activatePowerUp(character &Character2, ball &Ball) // poweruptime manfi: in the move , mosbat: reloading, capped at max time
{
    int totalTime = 20000, activatedTime = 4000;
    if (stats->powerUp == 2 || stats->powerUp == 6 || stats->powerUp == 5)
        activatedTime = 600;
    if (abs(powerUpAct - clock()) < 10)
        powerUp = true;
    if ((totalTime > powerUpTime))
        powerUpTime = clock() - powerUpAct - activatedTime;
    else
    {
        powerUpTime = totalTime;
        getPower = true;
    }
    // drawing the bar
    Uint32 color = 0xFF00FF00 + 0x0000FF * (powerUpTime + activatedTime) / (activatedTime + totalTime);
    if (color > 0xFF00FFFF)
        color = 0xFF00FFFF;
    if (!flipped)
    {
        roundedBoxColor(renderer, 45, 75, 305, 145, 35, 0xFF000000);
        roundedBoxColor(renderer, 50, 80, 300, 140, 30, 0xFFFFFFFF);
        roundedBoxColor(renderer, 60, 90, 60 + 40 + 190, 130, 20, 0x55000000);
        roundedBoxColor(renderer, 60, 90, 60 + 40 + abs(190 * ((powerUpTime < totalTime) ? powerUpTime : totalTime) / totalTime), 130, 20, color);
    }
    else
    {
        roundedBoxColor(renderer, 1280 - 45, 75, 1280 - 305, 145, 35, 0xFF000000);
        roundedBoxColor(renderer, 1280 - 50, 80, 1280 - 300, 140, 30, 0xFFFFFFFF);
        roundedBoxColor(renderer, 1280 - 60, 90, 1280 - 60 - 40 - 190, 130, 20, 0x55000000);
        roundedBoxColor(renderer, 1280 - 60, 90, 1280 - 60 - 40 - abs(190 * ((powerUpTime < totalTime) ? powerUpTime : totalTime) / totalTime), 130, 20, color);
    }
    // doing the effect and actual powerUp
    if (powerUp && (powerUpTime == totalTime))
    {
        powerUpTime = -activatedTime;
        srand(time(NULL));
        if (stats->powerUp0 == 7)
            stats->powerUp = rand() % 6 + 1;
        getPower = false;
    }
    if (powerUp && powerUpTime < 0)
    {
        switch (stats->powerUp)
        {
        case 1:
            stats->scale = 2;
            break;
        case 2:
        {
            variant = 1;
            int s = (x - Character2.x) * (x - Character2.x) + (y - Character2.y) * (y - Character2.y);
            if (s < 60000)
            {
                Character2.stunned = true;
                Character2.stunAct = clock();
                Character2.stunTime = 3000;
                Character2.pushback = true;
                Character2.deltaPush = (2 * Character2.flipped - 1) * 200;
            }
            break;
        }
        case 3:
            cloned = true;
            x += 120 * (2 * flipped - 1);
            drawCharacter();
            x += 120 * (2 * flipped - 1);
            drawCharacter();
            x -= 240 * (2 * flipped - 1);
            break;
        case 4:
            Ball.invis = true;
            break;
        case 5:
            jumpToBall(Ball);
        case 6:
        {
            SDL_Rect *sex;
            (stats->powerUp == 5) ? (sex = &headRect) : (sex = &footRect);
            int sx = Ball.x - (sex->x + sex->w / 2), sy = Ball.y - (sex->y + sex->h / 2);
            bool closeEnough = sx * sx + sy * sy < (Ball.r + 150) * (Ball.r + 150);
            if (closeEnough)
                Ball.setOnFire();
            variant = 2;
            // aacircleColor(renderer, (arr[7] + arr[9]) / 2, (arr[8] + arr[10]) / 2, 150, 0x88FFFFFF);
            break;
        }
        default:
            break;
        }
    }
    if (powerUpTime >= 0)
    {
        powerUp = false;
        if (!sizeCheat)
            stats->scale = stats->scale0;
        if (variant != 2)
            variant = 0;
        cloned = false;
        if (stats->powerUp == 4)
            Ball.invis = false;
        if (jumpTime == 0 && !jump)
            stats->jump = stats->maxJump;
    }
}
typedef struct player
{
    string name;
    int goal;
    int score;
    characterCard *character;
};
// minigame
class brick
{
private:
    int lives;
    bool inGame = true;
    SDL_Renderer *m_renderer;

public:
    SDL_Rect b_rect{0, 0, 100, 50};
    brick()
    {
    }
    brick(int x, int y, int health, SDL_Renderer *Renderer)
    {
        m_renderer = Renderer;
        b_rect.x = x;
        b_rect.y = y;
        this->lives = health;
        render();
    }
    void render()
    {
        if (!inGame)
            return;
        roundedBoxRGBA(m_renderer, b_rect.x - 3, b_rect.y - 3, b_rect.x + b_rect.w + 3, b_rect.y + b_rect.h + 3, b_rect.h / 4, 255 * lives / 4, 40, 110, 255);
        roundedBoxRGBA(m_renderer, b_rect.x, b_rect.y, b_rect.x + b_rect.w, b_rect.y + b_rect.h, b_rect.h / 4, 255 * lives / 4, 50, 170, 255);
    }
    void loseHealth()
    {
        lives--;
        if (lives == -1)
            inGame = false;
    }
    bool isInGame()
    {
        return inGame;
    }
};
class miniGame
{
private:
    SDL_Texture *background;
    SDL_Renderer *m_renderer;

public:
    int imax, jmax;
    brick **gameBricks;
    SDL_Rect rectSlider;
    miniGame(int Imax, int Jmax, SDL_Renderer *Renderer)
    {
        m_renderer = Renderer;
        imax = Imax;
        jmax = Jmax;
        rectSlider = {W / 2 - 50, H - 100 - 20, 100, 40};
        srand(time(NULL));
        gameBricks = new brick *[imax];
        for (int i = 0; i < imax; i++)
        {
            gameBricks[i] = new brick[jmax];
            for (int j = 0; j < jmax; j++)
                gameBricks[i][j] = brick(W / 2 - imax * 120 / 2 + i * 120, j * 60, rand() % 4, m_renderer);
        }
    }
    void showMiniGame()
    {
        for (int i = 0; i < imax; i++)
            for (int j = 0; j < jmax; j++)
                gameBricks[i][j].render();
        updateAndRenderSlider();
    }
    void updateAndRenderSlider()
    {
        SDL_PumpEvents();
        const Uint8 *keyboardArr = SDL_GetKeyboardState(NULL);
        rectSlider.x += 10 * (keyboardArr[SDL_SCANCODE_RIGHT] - keyboardArr[SDL_SCANCODE_LEFT]);
        roundedBoxRGBA(m_renderer, rectSlider.x - 10, rectSlider.y - 10, rectSlider.x + rectSlider.w + 10, rectSlider.y + rectSlider.h + 10, rectSlider.h / 2 + 10, 90, 40, 110, 255);
        roundedBoxRGBA(m_renderer, rectSlider.x, rectSlider.y, rectSlider.x + rectSlider.w, rectSlider.y + rectSlider.h, rectSlider.h / 2, 130, 50, 170, 255);
    }
    void reset()
    {
        for (int i = 0; i < imax; i++)
            for (int j = 0; j < jmax; j++)
                gameBricks[i][j] = brick(W / 2 - imax * 120 / 2 + i * 120, j * 60, rand() % 4, m_renderer);
    }
};
class mBall
{
private:
    // position of center
    int xpos = W / 2, ypos = 3 * H / 4;
    int default_velocity = 5;
    int xvel = default_velocity, yvel = -default_velocity;
    int radius = 30;
    SDL_Renderer *m_renderer;

public:
    SDL_Rect b_rect;
    mBall(SDL_Renderer *Renderer)
    {
        m_renderer = Renderer;
    }
    void render()
    {
        filledCircleColor(m_renderer, xpos, ypos, radius, 0xfffcf7de);
    }
    bool reflection() // reflect off of walls //returns false if player loses
    {
        if (ypos + radius >= H)
            return false;
        if (xpos + radius >= W || xpos - radius <= 0)
            xvel *= (-1);
        if (ypos - radius <= 0)
            yvel *= (-1);
        return true;
    }
    bool collision(miniGame *m_game) // ingame collisions
    {
        xpos += xvel;
        ypos += yvel;
        if (!reflection())
            return false;
        b_rect = {xpos - radius, ypos - radius, 2 * radius, 2 * radius};
        // reflect from slider
        if (SDL_HasIntersection(&b_rect, &m_game->rectSlider) && xpos >= m_game->rectSlider.x && xpos <= m_game->rectSlider.x + m_game->rectSlider.w)
        {
            yvel = -yvel;
            ypos = m_game->rectSlider.y - radius - 2;
        }
        for (int i = 0; i < m_game->imax; i++)
            for (int j = 0; j < m_game->jmax; j++)
                if (m_game->gameBricks[i][j].isInGame())
                {
                    int state = checkCollisionState(&m_game->gameBricks[i][j]);
                    if (state != 0)
                        m_game->gameBricks[i][j].loseHealth();
                    if (state == 1)
                        xvel = -xvel;
                    else if (state == 2)
                        yvel = -yvel;
                }
        return true;
    }
    int checkCollisionState(brick *m_brick) // returns 0 if no collisions occur, 1 if x-col, 2 if y-col
    {
        if (ypos >= m_brick->b_rect.y && ypos <= m_brick->b_rect.y + m_brick->b_rect.h && xpos + radius >= m_brick->b_rect.x && xpos - radius <= m_brick->b_rect.x + m_brick->b_rect.w)
        { // x-col
            if (xvel < 0)
                xpos = m_brick->b_rect.x + m_brick->b_rect.w + radius + 1;
            else
                xpos = m_brick->b_rect.x - radius - 1;
            return 1;
        }
        if (xpos >= m_brick->b_rect.x && xpos <= m_brick->b_rect.x + m_brick->b_rect.w && ypos + radius >= m_brick->b_rect.y && ypos - radius <= m_brick->b_rect.y + m_brick->b_rect.h)
        { // y-col
            if (yvel < 0)
                ypos = m_brick->b_rect.y + m_brick->b_rect.h + radius + 1;
            else
                ypos = m_brick->b_rect.y - radius - 1;
            return 2;
        }
        return 0;
    }
    void reset()
    {
        xpos = W / 2, ypos = 3 * H / 4;
        xvel = default_velocity, yvel = -default_velocity;
    }
};
class game
{
private:
    SDL_Renderer *renderer;
    int goal;            // 0: non / 1:player1 / 2:player2
    Mix_Chunk **effects; // 0 : whistle / 1:ding / 2:cheer
    bool getScore;
    SDL_Texture *Tbackground;
    SDL_Texture *Tl1, *Tl2;
    alottaParticles *particles;

public:
    bool endState[2]; // 0:goal limit / 1: time limit
    ball *toop;
    string map;
    player *player1, *player2;
    character *character1, *character2;
    int gameTime, maxGameTime, fanJump, waitTime;
    game(SDL_Renderer *Renderer, string Map, player *Player1, player *Player2, character *Character1, character *Character2, int MaxGameTime, Mix_Chunk **Effects, ball *Toop)
    {
        cout << "hi" << endl;
        renderer = Renderer;
        map = Map;
        player1 = Player1;
        player2 = Player2;
        character1 = Character1;
        character2 = Character2;
        gameTime = 0;
        maxGameTime = MaxGameTime;
        goal = 0;
        getScore = true;
        waitTime = 0;
        fanJump = 0;
        effects = Effects;
        toop = Toop;
        endState[0] = true;
        endState[1] = true;
        particles = new alottaParticles(renderer, "confetti", 100, 50, 0, 30, 1);
        Tl1 = IMG_LoadTexture(renderer, "./textures/light0.png");
        Tl2 = IMG_LoadTexture(renderer, "./textures/light1.png");
    }
    void setBackground()
    {
        Tbackground = IMG_LoadTexture(renderer, ("./textures/" + map + "Background.png").c_str());
    }
    void showFans()
    {
        int hFan, wFan = 0;
        SDL_QueryTexture(character1->stats->Tfan, NULL, NULL, &wFan, &hFan);
        hFan *= 10;
        wFan *= 10;
        SDL_Rect fanRect = {275, 400, wFan, hFan};
        srand(time(NULL));
        for (size_t i = 0; i < 20; i++)
        {
            if (goal == 1)
                fanRect.y = ((i < 10) ? 400 : 420) - parabola(0, 30, fanJump + fastSine(8 * i), 24);
            SDL_RenderCopyEx(renderer, character1->stats->Tfan, NULL, &fanRect, NULL, NULL, (rand() % 2) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
            fanRect.x += 1.25 * wFan;
            if (i == 9)
            {
                fanRect.y += 20;
                fanRect.x = 280;
            }
        }
        fanRect = {650, 400, wFan, hFan};
        for (size_t i = 0; i < 20; i++)
        {
            if (goal == 2)
                fanRect.y = ((i < 10) ? 400 : 420) - parabola(0, 30, fanJump + fastSine(8 * i), 24);
            SDL_RenderCopyEx(renderer, character2->stats->Tfan, NULL, &fanRect, NULL, NULL, (rand() % 2) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
            fanRect.x += 1.25 * wFan;
            if (i == 9)
            {
                fanRect.y += 20;
                fanRect.x = 645;
            }
        }
        fanRect = {250, 200, wFan, hFan};
        for (int i = -4; i < 30; i++)
        {
            if (goal == 1)
                fanRect.y = ((i < 20) ? ((i < 10) ? ((i < 0) ? 200 : 220) : 240) : 260) - parabola(0, 30, fanJump + fastSine(8 * i), 24);
            SDL_RenderCopyEx(renderer, character1->stats->Tfan, NULL, &fanRect, NULL, NULL, (rand() % 2) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
            fanRect.x += 1.25 * wFan;
            if (i == -1)
            {
                fanRect.y += 20;
                fanRect.x = 255;
            }
            if (i == 9)
            {
                fanRect.y += 20;
                fanRect.x = 260;
            }
            if (i == 19)
            {
                fanRect.y += 20;
                fanRect.x = 265;
            }
        }
        fanRect = {885, 200, wFan, hFan};
        for (int i = -4; i < 30; i++)
        {
            if (goal == 2)
                fanRect.y = ((i < 20) ? ((i < 10) ? ((i < 0) ? 200 : 220) : 240) : 260) - parabola(0, 30, fanJump + fastSine(8 * i), 24);
            SDL_RenderCopyEx(renderer, character2->stats->Tfan, NULL, &fanRect, NULL, NULL, (rand() % 2) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
            fanRect.x += 1.25 * wFan;
            if (i == -1)
            {
                fanRect.y += 20;
                fanRect.x = 655;
            }
            if (i == 9)
            {
                fanRect.y += 20;
                fanRect.x = 650;
            }
            if (i == 19)
            {
                fanRect.y += 20;
                fanRect.x = 645;
            }
        }
        if (goal)
            fanJump++;
        fanJump %= 24;
        if (goal)
            particles->UpdateAndRender(goal * W / 3, H / 4);
    }
    int showGameScreen(SDL_Event *e)
    {
        SDL_Rect Rbackground = {0, 0, W, H};
        SDL_RenderCopy(renderer, Tbackground, NULL, &Rbackground);
        showFans();
        getInputIngame(e, *character1, *character2);
        betterText(renderer, to_string(player1->goal), W / 2 - 144, 130, 0, 170, 255, 255, 72, "Baloo");
        betterText(renderer, to_string(player2->goal), W / 2 + 144, 130, 255, 0, 100, 255, 72, "Baloo");
        betterText(renderer, to_string(player1->score), W / 2 - 144, 172, 0, 170, 255, 255, 24, "Baloo");
        betterText(renderer, to_string(player2->score), W / 2 + 144, 172, 255, 0, 100, 255, 24, "Baloo");
        betterText(renderer, (player1->name), W / 2 - 120, 24, 0, 170, 255, 255, 36, "Baloo");
        betterText(renderer, (player2->name), W / 2 + 120, 24, 255, 0, 100, 255, 36, "Baloo");
        betterText(renderer, timeplaying(gameTime), W / 2, 150, 255, 255, 255, 200, 48, "Baloo");
        if (endState[1])
            betterText(renderer, timeplaying(maxGameTime), W / 2, 185, 255, 255, 255, 100, 24, "Baloo");
        character1->activatePowerUp(*character2, *toop);
        character2->activatePowerUp(*character1, *toop);
        character1->drawCharacter();
        character2->drawCharacter();
        character1->colisionBetweenTwo(character2);
        character1->UpdateCharacter();
        character2->UpdateCharacter();
        if (!toop->invis)
            toop->showball();
        toop->Bubble->showbubble();
        toop->checkball();
        toop->motion();
        if (DAAAANNCEEE)
            SDL_RenderCopy(renderer, (time(NULL) % 2) ? Tl1 : Tl2, NULL, &Rbackground);
        goalZad();
        if (e->key.keysym.sym == SDLK_ESCAPE)
            return 1;
        return 0;
    }
    void resetGame()
    {
        g = 3;
        player1->goal = 0;
        player2->goal = 0;
        player1->score = 0;
        player2->score = 0;
        character1->x = character1->x0;
        character2->x = character2->x0;
        character1->stats->scale = character1->stats->scale0;
        character2->stats->scale = character2->stats->scale0;
        toop->startposition();
        getScore = true;
        waitTime = 0;
    }
    void showIntro()
    {
        toop->Bubble->x0 = -100;
        toop->Bubble->y0 = 3000;
        int tot = 120;
        for (int startAnim = 0; startAnim < tot; startAnim++)
        {
            SDL_RenderClear(renderer);
            SDL_Texture *bg = IMG_LoadTexture(renderer, ("./textures/" + map + "Background.png").c_str());
            SDL_Rect rect = {0, 0, W, H};
            SDL_RenderCopy(renderer, bg, NULL, &rect);
            showFans();
            betterText(renderer, to_string(player1->goal), W / 2 - 144, 130, 0, 170, 255, 255, 72, "Baloo");
            betterText(renderer, to_string(player2->goal), W / 2 + 144, 130, 255, 0, 100, 255, 72, "Baloo");
            betterText(renderer, to_string(player1->score), W / 2 - 144, 172, 0, 170, 255, 255, 24, "Baloo");
            betterText(renderer, to_string(player2->score), W / 2 + 144, 172, 255, 0, 100, 255, 24, "Baloo");
            betterText(renderer, (player1->name), W / 2 - 120, 24, 0, 170, 255, 255, 36, "Baloo");
            betterText(renderer, (player2->name), W / 2 + 120, 24, 255, 0, 100, 255, 36, "Baloo");
            betterText(renderer, timeplaying(gameTime), W / 2, 150, 255, 255, 255, 200, 48, "Baloo");
            if (endState[1])
                betterText(renderer, timeplaying(maxGameTime), W / 2, 185, 255, 255, 255, 100, 24, "Baloo");
            boxRGBA(renderer, 0, 0, W, H, 90, 0, 190, 100 * (tot - startAnim) / tot);
            betterText(renderer, to_string(3 - 4 * startAnim / tot) + "!", W / 2, H / 2, 255, 255, 255, 255, 288, "Comic");
            character1->x = easeBackinOut(W / 2 - 200, character1->x0, startAnim, tot, 4);
            character2->x = easeBackinOut(W / 2 + 200, character2->x0, startAnim, tot, 4);
            character1->y = easeBackinOut(W / 2, character1->y0, startAnim, tot, 4);
            character2->y = easeBackinOut(W / 2, character2->y0, startAnim, tot, 4);
            character1->drawCharacter();
            character2->drawCharacter();
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(bg);
            if (!(startAnim % (tot / 4)) && startAnim != 3 * tot / 4)
                Mix_PlayChannel(0, effects[1], 0);
            if (startAnim == tot / 2)
                Mix_PlayChannel(1, effects[0], 0);
        }
        SDL_Delay(300); // prevents errors
        toop->startposition();
        DAAAANNCEEE = false;
    }
    void goalZad()
    {
        if (getScore)
        {
            if (toop->ingoal1)
            {
                character2->powerUpAct -= 5000;
                character1->powerUpAct += 500;
                goal = 2;
                particles->draw = true;
                particles->randomize();
                player2->goal++;
                player2->score += 100;
                player1->score -= 50;
                getScore = false;
                character1->stunned = true;
                character1->stunAct = clock();
                character1->stunTime = 3000;
            }
            if (toop->ingoal2)
            {
                character1->powerUpAct -= 5000;
                character2->powerUpAct += 500;
                goal = 1;
                player1->goal++;
                particles->draw = true;
                particles->randomize();
                player1->score += 100;
                player2->score -= 50;
                getScore = false;
                character2->stunTime = 3000;
                character2->stunAct = clock();
                character2->stunned = true;
            }
        }
        if (!getScore)
        {
            if (!waitTime)
                Mix_PlayChannel(0, effects[2], 0);
            toop->ingoal1 = false;
            toop->ingoal2 = false;
            toop->manipBallSpeed(0.8, 0.8);
            boxRGBA(renderer, 0, 0, W, H, 90, 0, 190, (waitTime < 50) ? waitTime * 2 : 100);
            if (goal == 1)
                betterText(renderer, player1->name + " Scored!", W / 2, H / 2, 255, 255, 255, 255, 100, "Comic");
            if (goal == 2)
                betterText(renderer, player2->name + " Scored!", W / 2, H / 2, 255, 255, 255, 255, 100, "Comic");
            waitTime++;
        }
        if (waitTime == 100)
        {
            toop->startposition();
            getScore = true;
            waitTime = 0;
            goal = 0;
            toop->r = toop->r0;
            showIntro();
        }
    }
    bool endGame()
    {
        if ((gameTime == maxGameTime && endState[1]) || (endState[0] && (player1->goal == 5 || player2->goal == 5 || (player1->goal == 4 && player2->goal == 4))))
            return true;
        return false;
    }
};
class menu
{
protected:
    SDL_Renderer *renderer;
    int bgAnimTime;
    button *buttons;
    Mix_Chunk *tick;
    SDL_Texture *background;

public:
    menu(SDL_Renderer *Renderer, button *Buttons, Mix_Chunk *Tick, SDL_Texture *Background)
    {
        renderer = Renderer;
        bgAnimTime = 0;
        buttons = Buttons;
        tick = Tick;
        background = Background;
    }
};
class selectionMenu : menu
{
private:
    game *TheGame;          // modifying the game
    int animTime, scanTime; // animation and loading images time
    int stage;              // stage of the menu
    SDL_Texture *card1, *card2, *head1, *head2;

public:
    selectionMenu(SDL_Renderer *Renderer, button *Buttons, Mix_Chunk *Tick, game *thegame, int Stage)
        : menu(Renderer, Buttons, Tick, IMG_LoadTexture(Renderer, "./textures/tiling.png"))
    {
        TheGame = thegame;
        animTime = 0; // 48 frames for the start
        stage = Stage;
        scanTime = 0;
    }
    int showSelectionMenu(SDL_Event *event)
    {
        // buttons:
        // 0-6 : team cards
        // 7-13 : player heads
        // 14-16 : map buttons
        scanTime %= 50; // to make for efficient
        bgAnimTime %= 2000;
        int xmouse, ymouse;
        SDL_GetMouseState(&xmouse, &ymouse);
        if (stage > 1 && stage < 6)
        {
            for (int i = 0; i < 14; i++)
            {
                int p[2] = {(i % 7 + 1) * W / 8 + faseleReact(xmouse - (i % 7 + 1) * W / 8, ymouse - H + 110)[0], H - 110 + faseleReact(xmouse - (i % 7 + 1) * W / 8, ymouse - H + 110)[1]};
                buttons[i].setposition(p);
                if (buttons[i].isHowering(xmouse, ymouse))
                    Mix_PlayChannel(1, tick, 0);
                buttons[i].checkPress(event);
            }
        }
        SDL_Rect Rtiles = {0, 0, 400, 400};
        Rtiles.x = bgAnimTime / 5 - 400 - 300 * abs(xmouse % W - W / 2) * (xmouse % W - W / 2) / (W * W);
        Rtiles.y = bgAnimTime / 5 - 400 - 300 * abs(ymouse % H - H / 2) * (ymouse % H - H / 2) / (H * H);
        int x[4] = {0, W, 0, W}, y[4] = {0, 0, H, H};
        SDL_Color selectColor = {220, 80, 120};
        SDL_Color shadeColor = {0, 0, 0};
        drawZoozanaghe(x, y, &selectColor, &selectColor, renderer);
        drawShadedZoozanaghe(x, y, &shadeColor, renderer);
        for (size_t i = 0; i < 30; i++)
        {
            SDL_RenderCopy(renderer, background, NULL, &Rtiles);
            Rtiles.x += 400;
            if (i % 6 == 0 && i != 0)
            {
                Rtiles.x = bgAnimTime / 5 - 400 - 300 * abs(xmouse % W - W / 2) * (xmouse % W - W / 2) / (W * W);
                Rtiles.y += 400;
            }
        }
        bgAnimTime += 2;
        switch (stage)
        {
        case 0:
        case 1:
        {
            string arr[5];
            int j;
            namesuggestion(arr, (stage == 0) ? TheGame->player1->name : TheGame->player2->name, j);
            SDL_Texture *chatbox = IMG_LoadTexture(renderer, "./textures/chatBox.png");
            SDL_Rect rect = {W / 2 - 200, H / 2 - 50 + 10 - 100, 400, 100};
            SDL_RenderCopy(renderer, chatbox, NULL, &rect);
            for (int i = 0; i < min(3, j); i++)
                betterText(renderer, arr[i], W / 2, H / 2 + 100 * (i + 1) + 10 - 100, 255, 255, 255, 200, 30);
            roundedBoxColor(renderer, W / 2 - 200, H / 2 - 50 + 10 - 100, W / 2 + 200, H / 2 - 50 + 100 * min(4, j + 1) - 100, 20, 0x88999999);
            SDL_DestroyTexture(chatbox);
            selectionMenuText((stage == 0) ? "enter player1's name" : "enter player2's name", "press enter to go next", animTime, renderer, 1, "press tab to go back");
            getTypingInput((stage == 0) ? TheGame->player1->name : TheGame->player2->name, event);
            if (bgAnimTime % 100 < 50)
                betterText(renderer, (stage == 0) ? TheGame->player1->name : TheGame->player2->name, W / 2, H / 2 + 10 - 100, 255, 255, 255, 255, 30);
            else
                betterText(renderer, (stage == 0) ? TheGame->player1->name : TheGame->player2->name + "|", W / 2, H / 2 + 10 - 100, 255, 255, 255, 255, 30);
            if (mousePress(event) && j)
                for (size_t i = 0; i < min(3, j); i++)
                    if (xmouse <= W / 2 + 200 && xmouse >= W / 2 - 200 && ymouse <= H / 2 - 50 + 100 * min(4, j + 1) - 100 && ymouse >= H / 2 - 50 + 10 - 100)
                        ((stage == 0) ? TheGame->player1->name : TheGame->player2->name) = arr[(ymouse - (H / 2 + 10 - 100) - 50) / 100];
            break;
        }
        case 2:
        case 3:
            selectionMenuText("Press enter to go next", "Press tab to go back", animTime, renderer, 0);
            betterText(renderer, "Pick ur team " + ((stage == 2) ? TheGame->player1->name : TheGame->player2->name) + "!", W / 2, 180, 255, 255, 255, 250 * (animTime) / 72 + 2, 72, "Baloo");
            for (size_t i = 0; i < 7; i++)
            {
                buttons[i].animate();
                buttons[i].drawButton(renderer);
                if (buttons[i].isHowering(xmouse, ymouse))
                    betterText(renderer, buttons[i].name + "?", W / 2, H / 2, 255, 255, 255, 255, 48, "Baloo");
                if (buttons[i].pressGet())
                    ((stage == 2) ? (TheGame->player1->character->team) : TheGame->player2->character->team) = buttons[i].name;
            }
            break;
        case 4:
        case 5:
            selectionMenuText("Press enter to go next", "Press tab to go back", animTime, renderer, 0);
            betterText(renderer, "Pick ur football star " + ((stage == 4) ? TheGame->player1->name : TheGame->player2->name) + "!", W / 2, 180, 255, 255, 255, 250 * (animTime) / 72 + 2, 72, "Baloo");
            for (size_t i = 7; i < 14; i++)
            {
                buttons[i].animate();
                buttons[i].drawButton(renderer);
                if (buttons[i].isHowering(xmouse, ymouse))
                    betterText(renderer, buttons[i].name + "?", W / 2, H / 2, 255, 255, 255, 255, 48, "Baloo");
                if (buttons[i].pressGet())
                    ((stage == 4) ? (TheGame->player1->character->name) : TheGame->player2->character->name) = buttons[i].name;
            }
            break;
        case 6:
            selectionMenuText("Press enter to go next", "Press tab to go back", animTime, renderer, 0);
            betterText(renderer, "Pick the map!", W / 2, 180, 255, 255, 255, 250 * (animTime) / 72 + 2, 72, "Baloo");
            for (size_t i = 0; i < 3; i++)
            {
                buttons[i + 14].animate();
                buttons[i + 14].drawButton(renderer);
                buttons[i + 14].checkPress(event);
                if (buttons[i + 14].isHowering(xmouse, ymouse))
                    betterText(renderer, buttons[14 + i].name + "?", W / 2, H / 2, 255, 255, 255, 255, 48, "Baloo");
                if (buttons[i + 14].pressGet())
                    TheGame->map = buttons[i + 14].name;
            }
            break;
        case 7:
            selectionMenuText("Press enter to start!", "Press tab to go back", animTime, renderer, 0);
            betterText(renderer, "Pick the ending states!", W / 2, 180, 255, 255, 255, 250 * (animTime) / 72 + 2, 72, "Baloo");
            filledCircleColor(renderer, 2 * W / 3 + 20, H - H / 5 - 30, 28, 0xFF000000);
            filledCircleColor(renderer, 2 * W / 3 + 20, H - H / 5 + 90 - 30, 28, 0xFF000000);
            filledCircleColor(renderer, 2 * W / 3 + 20, H - H / 5 - 30, 25, 0xFFFFFFFF);
            filledCircleColor(renderer, 2 * W / 3 + 20, H - H / 5 + 90 - 30, 25, 0xFFFFFFFF);
            filledCircleColor(renderer, 2 * W / 3 + 20, H - H / 5 - 30, 20, TheGame->endState[1] ? 0xFF00FF00 : 0x44000000);
            filledCircleColor(renderer, 2 * W / 3 + 20, H - H / 5 + 90 - 30, 20, TheGame->endState[0] ? 0xFF00FF00 : 0x44000000);
            betterText(renderer, "Have a time limit?", W / 2 - 50, H - H / 5 - 30, 255, 255, 255, 255, 48, "Baloo");
            betterText(renderer, "Have a goal limit?", W / 2 - 50, H - H / 5 - 30 + 90, 255, 255, 255, 255, 48, "Baloo");
            if (((xmouse - (2 * W / 3 + 20)) * (xmouse - (2 * W / 3 + 20)) + (ymouse - (H - H / 5 - 30)) * (ymouse - (H - H / 5 - 30)) <= 400) && event->type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Delay(100);
                TheGame->endState[1] = !TheGame->endState[1];
            }
            if (((xmouse - (2 * W / 3 + 20)) * (xmouse - (2 * W / 3 + 20)) + (ymouse - (H - H / 5 - 30 + 90)) * (ymouse - (H - H / 5 - 30 + 90)) <= 400) && event->type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Delay(100);
                TheGame->endState[0] = !TheGame->endState[0];
            }
        default:
            break;
        }
        // show the current build
        if (stage > 1)
        {
            if (!scanTime)
            {
                card1 = IMG_LoadTexture(renderer, ("./textures/" + TheGame->player1->character->team + "Card.png").c_str());
                card2 = IMG_LoadTexture(renderer, ("./textures/" + TheGame->player2->character->team + "Card.png").c_str());
                head1 = IMG_LoadTexture(renderer, ("./textures/" + TheGame->player1->character->name + "Head.png").c_str());
                head2 = IMG_LoadTexture(renderer, ("./textures/" + TheGame->player2->character->name + "Head.png").c_str());
            }
            scanTime++;
            SDL_Rect cardrect = {W / 4 - 100 + 50, H / 2 - 150 - 15, 200, 300};
            SDL_Rect headrect = {W / 4 - 75 + 50, H / 2 - 75 - 15, 150, 200};
            SDL_RenderCopy(renderer, card1, NULL, &cardrect);
            SDL_RenderCopy(renderer, head1, NULL, &headrect);
            cardrect.x += W / 2 - 100;
            headrect.x += W / 2 - 100;
            SDL_RenderCopy(renderer, card2, NULL, &cardrect);
            SDL_RenderCopyEx(renderer, head2, NULL, &headrect, NULL, NULL, SDL_FLIP_HORIZONTAL);
            if (TheGame->map != "")
                betterText(renderer, ("\"" + TheGame->map + "\""), W / 2, H / 3 + 60, 255, 255, 255, 255, 48, "Baloo");
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN])
        {
            stage++;
            SDL_Delay(120);
            // Mix_PlayChannel(1,affarin,0);
        }
        if (stage && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_TAB])
        {
            stage--;
            SDL_Delay(120);
        }
        if (stage == 8)
        {
            if (TheGame->map == "" || TheGame->player1->name == "" || TheGame->player1->character->name == "" || TheGame->player1->character->team == "")
            {
                stage--;
                betterText(renderer, "selection incomplete", W / 2, H / 2, 255, 0, 0, 255, 48, "Baloo");
                SDL_RenderPresent(renderer);
                SDL_Delay(500);
            }
            else if (TheGame->player2->name == "" || TheGame->player2->character->name == "" || TheGame->player2->character->team == "")
            {
                stage--;
                betterText(renderer, "selection incomplete", W / 2, H / 2, 255, 0, 0, 255, 48, "Baloo");
                SDL_RenderPresent(renderer);
                SDL_Delay(500);
            }
            else
                return 1;
        }
        if (stage == 0 && event->key.keysym.sym == SDLK_TAB)
            return 2;
        return 0;
    }
    void reset()
    {
        stage = 0;
    }
};
class menuStart : public menu
{
protected:
    int kodoom, next;
    mBall miniball = NULL;
    miniGame minigame = miniGame(5, 5, NULL);

public:
    bool loading = true;
    menuStart(SDL_Renderer *Renderer, button *Buttons, Mix_Chunk *Tick)
        : menu(Renderer, Buttons, Tick, IMG_LoadTexture(Renderer, "./textures/background.jpg"))
    {
        next = 0;
        kodoom = 0;
        miniball = mBall(renderer);
        minigame = miniGame(5, 5, renderer);
    }
    // returns the number of the button pressed
    virtual int showMenu(SDL_Event *event) // first 4 buttons: right ; the rest on the left
    {
        float zarib;
        if (bgAnimTime < 144 && bgAnimTime > -1)
            zarib = easeBackinOut(0, 1, bgAnimTime, 144, 9);
        else if (bgAnimTime < -10)
            zarib = easeBackinOut(0, 1, -1 * bgAnimTime, 144, 9);
        else if (bgAnimTime == -10)
            return next;
        else
            zarib = 1;
        bgAnimTime++;
        loading = zarib != 1;
        SDL_Rect bgR = {0, 0, W, H};
        if (bgAnimTime > 36 || bgAnimTime < 0)
            SDL_RenderCopy(renderer, background, NULL, &bgR);
        else if (bgAnimTime > 0)
            boxColor(renderer, 0, 0, W, H, 0xFFFFFFFF);
        // boxColor(renderer , 0 , 0 , W*(1 - 3*zarib)/2 , H , 0xFFFFFFFF);
        // boxColor(renderer , W , 0 , W - W*(1 - 3*zarib)/2 , H , 0xFFFFFFFF);
        int xl[4] = {0, 300 * zarib, 0, 150 * zarib}, yl[4] = {0, 0, H, H};
        int xa[4] = {0, 150, 0, W * (1 - zarib) / 2}, ya[4] = {0, 0, H, H};
        int xba[4] = {W - W * (1 - zarib) / 2, W, W - 150, W}, yba[4] = {0, 0, H, H};
        SDL_Color barColor = {190, 15, 64};
        SDL_Color barBorderColor = {190, 15, 64};
        SDL_Color shadeColor = {0, 0, 0};
        SDL_Color white = {255, 255, 255, 255};
        // boxRGBA(renderer , 0 , 0 , W , H , 255 , 255 , 255 , 100*(1-zarib));
        drawZoozanaghe(xa, ya, &white, &white, renderer);
        drawZoozanaghe(xba, yba, &white, &white, renderer);
        drawZoozanaghe(xl, yl, &barColor, &barBorderColor, renderer);
        int xr[4] = {W - 150 * zarib, W, W - 300 * zarib, W}, yr[4] = {0, 0, H, H};
        drawZoozanaghe(xr, yr, &barColor, &barBorderColor, renderer);
        drawShadedZoozanaghe(xr, yr, &shadeColor, renderer);
        drawShadedZoozanaghe(xl, yl, &shadeColor, renderer);
        int xb[4], yb[4];
        int bx[6][4], by[6][4];
        by[0][0] = H / 2 - 150;
        by[0][1] = by[0][0];
        by[0][2] = by[0][0] + 100;
        by[0][3] = by[0][2];
        for (int j = 0; j < 3; j++)
            for (int i = 0; i < 4; i++)
                by[j + 1][i] = by[j][i] + 120;
        bool lol = false;
        for (int i = 0; i < 4; i++, lol = false)
            for (int j = 0; j < 4; j++, lol = !lol)
                bx[i][j] = (lol) ? (W) : (xr[0] + int((xr[2] - xr[0]) * (by[i][j] / double(H))));
        by[4][0] = H / 4;
        by[4][1] = by[4][0];
        by[4][2] = H / 4 + 100;
        by[4][3] = by[4][2];
        by[5][0] = H / 4 + 150;
        by[5][1] = H / 4 + 150;
        by[5][2] = H / 4 + 250;
        by[5][3] = by[5][2];
        lol = true;
        for (int i = 4; i < 6; i++, lol = true)
            for (int j = 0; j < 4; j++, lol = !lol)
                bx[i][j] = (lol) ? (0) : (xl[1] + int((xl[3] - xl[1]) * (by[i][j] / double(H))));
        int *tempxb = xb, *tempyb = yb;
        bool moveBala = false, moveDown = false;
        for (int i = 0; i < 6; i++)
            buttons[i].checkPress(event);
        if (event->type == SDL_KEYDOWN)
        {
            switch (event->key.keysym.sym)
            {
            case SDLK_UP:
                kodoom--;
                event->type = 0;
                moveBala = true;
                Mix_PlayChannel(-1, tick, 0);
                break;
            case SDLK_DOWN:
                kodoom++;
                event->type = 0;
                moveDown = true;
                Mix_PlayChannel(-1, tick, 0);
                break;
            case SDLK_RETURN:
                buttons[kodoom].pressSet(true);
                event->type = 0;
                break;
            }
        }
        kodoom = (kodoom % 6);
        if (kodoom == -1)
            kodoom = 5;
        for (int i = 0; i < 4; i++)
        {
            xb[i] = bx[kodoom][i];
            yb[i] = by[kodoom][i];
        }
        if (moveDown)
        {
            tempxb = bx[(kodoom == 0) ? 5 : kodoom - 1];
            tempyb = by[(kodoom == 0) ? 5 : kodoom - 1];
        }
        else if (moveBala)
        {
            tempxb = bx[(kodoom == 5) ? 0 : kodoom + 1];
            tempyb = by[(kodoom == 5) ? 0 : kodoom + 1];
        }
        buttons[kodoom].animate();
        SDL_Color selectColor = {220, 80, 120};
        // comedy
        if (!loading)
        {
            roundedBoxColor(renderer, W / 2 - 300 - 20, -10, W / 2 + 300, 310, 20, 0xAA666666);
            minigame.showMiniGame();
            miniball.render();
            // minigame.rectSlider.x = miniball.b_rect.x;
            if (!miniball.collision(&minigame))
            {
                betterText(renderer, "press R to reset", W / 2, H / 2, 255, 255, 255, 255, 48, "Baloo");
                if (event->key.keysym.sym == SDLK_r)
                {
                    minigame.reset();
                    miniball.reset();
                }
            }
        }
        if ((moveBala | moveDown))
        {
            screenShot(renderer);
            SDL_RenderCopy(renderer, IMG_LoadTexture(renderer, "screenshot.bpm"), NULL, &bgR);
            int x[4], y[4], xt[4], yt[4];
            for (int i = 0; i < 48; i++)
            {
                // SDL_RenderCopy(renderer,IMG_LoadTexture(renderer,"screenshot.bpm"),NULL,&bgR);
                for (int j = 0; j < 4; j++)
                {
                    x[j] = easeBackinOut(tempxb[j], xb[j], i, 48, 1.5);
                    y[j] = easeBackinOut(tempyb[j], yb[j], i, 48, 1.5);
                }
                SDL_Color frameColor = {200, 0, 0, 0};
                if (x[0] == 0 || x[1] == W)
                    drawZoozanaghe(x, y, &selectColor, NULL, renderer);
                for (int s = 0; s < 6; s++)
                    buttons[s].drawButton(renderer);
                SDL_RenderPresent(renderer);
                SDL_Delay(5);
            }
        }
        drawZoozanaghe(xb, yb, &selectColor, NULL, renderer);
        for (int i = 0; i < 6; i++)
        {
            buttons[i].drawButton(renderer);
            int xp, yp;
            SDL_GetMouseState(&xp, &yp);
            if (buttons[i].isHowering(xp, yp))
                kodoom = i;
        }
        for (int i = 0; i < 6; i++)
            if (i != kodoom)
            {
                buttons[i].resetAnimation();
                buttons[i].animate();
            }
        if (buttons[0].pressGet())
        {
            bgAnimTime = -144;
            next = 1;
        }
        if (buttons[2].pressGet())
        {
            bgAnimTime = -144;
            next = 3;
        }
        if (buttons[4].pressGet())
        {
            bgAnimTime = -144;
            next = 5;
        }
        if (buttons[1].pressGet())
        {
            bgAnimTime = -1000;
            return 2;
        }
        if (buttons[3].pressGet())
            return 4;
        return 0;
    }
    virtual void reset()
    {
        for (size_t i = 0; i < 6; i++)
            buttons[i].pressSet(false);
        (bgAnimTime == -1000) ? (bgAnimTime = 144) : (bgAnimTime = 0);
    }
};
class menuPause : public menuStart
{
private:
    game *TheGame;
    bool cheats, typingCheats;
    string code;

public:
    menuPause(SDL_Renderer *Renderer, button *Buttons, Mix_Chunk *Tick, game *mainGame)
        : menuStart(Renderer, Buttons, Tick)
    {
        cheats = false;
        typingCheats = false;
        code = "";
        TheGame = mainGame;
    }
    void cheat(SDL_Event *event)
    {
        SDL_Texture *logo;
        if (!cheats)
            logo = IMG_LoadTexture(renderer, "./textures/Mack-eon.png");
        else
            logo = IMG_LoadTexture(renderer, "./textures/Hack-eon.png");
        SDL_Rect rect = {W - ((bgAnimTime < 48) ? easeBackinOut(0, (400 + 20), bgAnimTime, 48) : 420), -50, 400, 400};
        SDL_RenderCopy(renderer, logo, NULL, &rect);
        int xm, ym;
        SDL_GetMouseState(&xm, &ym);
        SDL_Point p = {xm, ym};
        if (SDL_PointInRect(&p, &rect) == SDL_TRUE && mousePress(event))
            if (!cheats)
            {
                cheats = true;
                SDL_Delay(500);
            }
            else
                typingCheats = true;
        SDL_DestroyTexture(logo);
    }
    // returns the number of the button pressed
    int showMenu(SDL_Event *event) // first 4 buttons: right ; the rest on the left
    {
        float zarib = 1;
        if (bgAnimTime < 48)
            zarib = easeBackinOut(0, 1, abs(bgAnimTime), 48);
        bgAnimTime++;
        SDL_Color barColor = {190, 15, 64};
        SDL_Color barBorderColor = {190, 15, 64};
        SDL_Color shadeColor = {0, 0, 0};
        SDL_Color white = {255, 255, 255, 255};
        int xr[4] = {W - 400 * zarib, W, W - 600 * zarib, W}, yr[4] = {0, 0, H, H};
        drawZoozanaghe(xr, yr, &barColor, &barBorderColor, renderer);
        cheat(event);
        drawShadedZoozanaghe(xr, yr, &shadeColor, renderer);
        int xb[4], yb[4];
        int bx[4][4], by[4][4];
        by[0][0] = H / 2 - 150 + 40;
        by[0][1] = by[0][0];
        by[0][2] = by[0][0] + 100;
        by[0][3] = by[0][2];
        for (int j = 0; j < 3; j++)
            for (int i = 0; i < 4; i++)
                by[j + 1][i] = by[j][i] + 120;
        bool lol = false;
        for (int i = 0; i < 4; i++, lol = false)
            for (int j = 0; j < 4; j++, lol = !lol)
                bx[i][j] = (lol) ? (W) : (xr[0] + int((xr[2] - xr[0]) * (by[i][j] / double(H))));
        int *tempxb = xb, *tempyb = yb;
        bool moveBala = false, moveDown = false;
        for (int i = 0; i < 4; i++)
            buttons[i].checkPress(event);
        if (event->type == SDL_KEYDOWN && !typingCheats)
        {
            switch (event->key.keysym.sym)
            {
            case SDLK_UP:
                kodoom--;
                event->type = 0;
                moveBala = true;
                Mix_PlayChannel(-1, tick, 0);
                break;
            case SDLK_DOWN:
                kodoom++;
                event->type = 0;
                moveDown = true;
                Mix_PlayChannel(-1, tick, 0);
                break;
            case SDLK_RETURN:
                buttons[kodoom].pressSet(true);
                event->type = 0;
                break;
            }
        }
        kodoom = (kodoom % 4);
        if (kodoom == -1)
            kodoom = 3;
        for (int i = 0; i < 4; i++)
        {
            xb[i] = bx[kodoom][i];
            yb[i] = by[kodoom][i];
        }
        if (moveDown)
        {
            tempxb = bx[(kodoom == 0) ? 3 : kodoom - 1];
            tempyb = by[(kodoom == 0) ? 3 : kodoom - 1];
        }
        else if (moveBala)
        {
            tempxb = bx[(kodoom == 3) ? 0 : kodoom + 1];
            tempyb = by[(kodoom == 3) ? 0 : kodoom + 1];
        }
        SDL_Color selectColor = {220, 80, 120};
        if ((moveBala | moveDown))
        {
            int x[4], y[4], xt[4], yt[4];
            for (int i = 0; i < 24; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    x[j] = easeBackinOut(tempxb[j], xb[j], i, 24, 1.5);
                    y[j] = easeBackinOut(tempyb[j], yb[j], i, 24, 1.5);
                }
                SDL_Color frameColor = {200, 0, 0, 0};
                if (x[0] == 0 || x[1] == W)
                    drawZoozanaghe(x, y, &selectColor, NULL, renderer);
                for (int s = 0; s < 4; s++)
                    buttons[s].drawButton(renderer);
                SDL_RenderPresent(renderer);
                SDL_Delay(5);
            }
        }
        drawZoozanaghe(xb, yb, &selectColor, NULL, renderer);
        for (int i = 0; i < 4; i++)
        {
            buttons[i].drawButton(renderer);
            int xp, yp;
            SDL_GetMouseState(&xp, &yp);
            if (buttons[i].isHowering(xp, yp) && !typingCheats)
                kodoom = i;
        }
        for (int i = 0; i < 4; i++)
            if (i != kodoom)
            {
                buttons[i].resetAnimation();
                buttons[i].animate();
            }
        buttons[kodoom].animate();
        if (typingCheats)
        {
            boxColor(renderer, 0, 0, W, H, 0xAABE0F40);
            SDL_Texture *msg = IMG_LoadTexture(renderer, "./textures/chatBox.png");
            SDL_Rect chatBox = {W / 2 - 250, H / 2 - 50, 500, 100};
            SDL_RenderCopy(renderer, msg, NULL, &chatBox);
            getTypingInput(code, event);
            if (bgAnimTime % 100 < 50)
                betterText(renderer, code, W / 2, H / 2, 255, 255, 255, 255, 48);
            else
                betterText(renderer, code + "|", W / 2, H / 2, 255, 255, 255, 255, 48);
            SDL_DestroyTexture(msg);
            if (event->key.keysym.sym == SDLK_RETURN)
                execute();
        }
        if (buttons[0].pressGet())
            bgAnimTime = -48;
        if (bgAnimTime == -1)
            return 1;
        if (buttons[1].pressGet())
            return 2;
        if (buttons[2].pressGet())
            return 3;
        if (buttons[3].pressGet())
            return 4;
        return 0;
    }
    void reset()
    {
        for (size_t i = 0; i < 4; i++)
            buttons[i].pressSet(false);
        bgAnimTime = 0;
        cheats = false;
        typingCheats = false;
        code = "";
    }
    void execute()
    {
        if (yekian(code, "BigMakk"))
        {
            TheGame->character1->stats->scale = 2;
            TheGame->character2->stats->scale = 2;
            TheGame->character1->sizeCheat = true;
            TheGame->character2->sizeCheat = true;
        }
        if (yekian(code, "moon Gravity"))
            g = 0.3;
        if (yekian(code, "hame too dance"))
        {
            TheGame->toop->textureball = IMG_LoadTexture(renderer, "./balls/discoBall.png");
            TheGame->toop->r *= 3;
            Mix_PlayMusic(Mix_LoadMUS("./music/با رفیقام، همه تو دانس، همه بالانس.mp3"), 20);
            DAAAANNCEEE = true;
        }
        reset();
    }
};
class EndScreen : menu
{
private:
    player *player1, *player2;
    bool setScores;
    alottaParticles *hooray;

public:
    EndScreen(SDL_Renderer *Renderer, player *P1, player *P2, button *MainButton)
        : menu(Renderer, MainButton, NULL, NULL)
    {
        player1 = P1;
        player2 = P2;
        setScores = true;
        hooray = new alottaParticles(renderer, "confetti", 200, 40, 0, 40, NULL, 0.99);
        hooray->draw = true;
    }
    int showEndScreen(SDL_Event *event)
    {
        string Player1 = player1->name, Player2 = player2->name;
        int goal1 = player1->goal, goal2 = player2->goal, score1 = player1->score, score2 = player2->score;
        if (setScores)
        {
            if (goal1 > goal2)
                player1->score += 300;
            else if (goal2 > goal1)
                player2->score += 300;
            else
            {
                player1->score += 100;
                player2->score += 100;
            }
        }
        setScores = false;
        float zarib;
        float cardZarib = 1 + 0.05 * fastSine(bgAnimTime / 32.0);
        if (bgAnimTime < 32)
            zarib = easeBackinOut(0, 1, bgAnimTime, 32);
        else
            zarib = 1;
        bgAnimTime++;
        if (bgAnimTime == 100)
            hooray->draw = false;
        boxRGBA(renderer, 0, 0, W, H, 119, 0, 221, 100 * (zarib + 0.1));
        SDL_Color nameColor = {255, 255, 255, 255};
        SDL_SetTextureAlphaMod(player1->character->Tcard, 232 * (zarib + 0.05));
        SDL_SetTextureAlphaMod(player2->character->Tcard, 232 * (zarib + 0.05));
        int hGoal, wGoal;
        SDL_QueryTexture(player1->character->Tcard, NULL, NULL, &wGoal, &hGoal);
        SDL_Rect goalRect;
        wGoal *= cardZarib;
        hGoal *= cardZarib;
        goalRect.h = hGoal;
        goalRect.w = wGoal;
        goalRect.x = W / 2 - wGoal / 2;
        goalRect.y = int(0.5 * H) - hGoal / 2 - 20;
        betterText(renderer, Player1, W / 2 - 220, int(0.8 * H) - 50 + 5, 0, 0, 0, nameColor.a, 95 * zarib, "ComicBold");
        betterText(renderer, Player2, W / 2 + 220, int(0.8 * H) - 50 + 5, 0, 0, 0, nameColor.a, 95 * zarib, "ComicBold");
        betterText(renderer, Player1, W / 2 - 220, int(0.8 * H) - 50, nameColor.r, nameColor.g, nameColor.b, nameColor.a, 72 * zarib, "ComicBold");
        betterText(renderer, Player2, W / 2 + 220, int(0.8 * H) - 50, nameColor.r, nameColor.g, nameColor.b, nameColor.a, 72 * zarib, "ComicBold");
        goalRect.x -= 220;
        SDL_RenderCopy(renderer, player1->character->Tcard, NULL, &goalRect);
        goalRect.x += 440;
        SDL_RenderCopy(renderer, player2->character->Tcard, NULL, &goalRect);
        roundedBoxRGBA(renderer, W / 2 - 76, int(0.5 * H) - 20 - 29, W / 2 + 76, int(0.5 * H) - 20 + 29, 15, 0, 0, 0, 232 * (zarib + 0.05));
        roundedBoxRGBA(renderer, W / 2 - 72, int(0.5 * H) - 20 - 25, W / 2 + 72, int(0.5 * H) - 20 + 25, 12, 255, 255, 255, 232 * (zarib + 0.05));
        betterText(renderer, to_string(goal1), W / 2 - 220, int(0.5 * H) - 50, nameColor.r, nameColor.g, nameColor.b, nameColor.a, 144 * zarib, "Comic");
        betterText(renderer, to_string(goal2), W / 2 + 220, int(0.5 * H) - 50, nameColor.r, nameColor.g, nameColor.b, nameColor.a, 144 * zarib, "Comic");
        betterText(renderer, ((score1 > 0) ? "+" : "") + to_string(score1), W / 2 - 220, int(0.5 * H) + 65, nameColor.r, nameColor.g, nameColor.b, nameColor.a, 36 * zarib, "Comic");
        betterText(renderer, ((score2 > 0) ? "+" : "") + to_string(score2), W / 2 + 220, int(0.5 * H) + 65, nameColor.r, nameColor.g, nameColor.b, nameColor.a, 36 * zarib, "Comic");
        if (goal1 > goal2)
            betterText(renderer, Player1 + " wins!", W / 2, H / 6, 255, 255, 255, 255, 90, "ComicBold");
        else if (goal2 > goal1)
            betterText(renderer, Player2 + " wins!", W / 2, H / 6, 255, 255, 255, 255, 90, "ComicBold");
        else
            betterText(renderer, "Draw!", W / 2, H / 6, 255, 255, 255, 255, 90, "ComicBold");
        buttons->animate();
        buttons->drawButton(renderer);
        buttons->checkPress(event);
        hooray->UpdateAndRender(W / 2, H / 2);
        if (buttons->pressGet())
        {
            hooray->randomize();
            hooray->draw = true;
            bgAnimTime = 0;
            return 1;
        }
        return 0;
    }
};
class menuSettings : menu
{
private:
    SDL_Window *window;
    int brightness, volumeMus, volumeFx;

public:
    int track;
    menuSettings(SDL_Renderer *Renderer, button *Buttons, Mix_Chunk *Tick, SDL_Window *Window)
        : menu(Renderer, Buttons, Tick, NULL)
    {
        window = Window;
        brightness = 100;
        volumeFx = 100;
        volumeMus = 100;
        track = 0;
        setSettings();
    }
    void setSettings()
    {
        SDL_SetWindowBrightness(window, brightness / 200.0 + 0.5);
        Mix_Volume(1, MIX_MAX_VOLUME * volumeFx / 100);
        Mix_Volume(0, MIX_MAX_VOLUME * volumeFx / 100);
        Mix_Volume(2, MIX_MAX_VOLUME * volumeFx / 100);
        Mix_Volume(-1, MIX_MAX_VOLUME * volumeFx / 100);
        Mix_VolumeMusic(MIX_MAX_VOLUME * volumeMus / 400);
    }
    int showMenu(SDL_Event *event)
    {
        float zarib;
        if (bgAnimTime < 48)
            zarib = easeBackinOut(0, 1, bgAnimTime, 48, 0);
        else
            zarib = 1;
        bgAnimTime++;
        boxRGBA(renderer, 0, 0, W, H, 119, 0, 221, 80 * (zarib + 0.1));
        int wb = 400 * zarib, hb = 300 * zarib;
        roundedBoxColor(renderer, W / 2 - wb * 1.01, H / 2 - hb * 1.01, W / 2 + wb * 1.01, H / 2 + hb * 1.01, 40 * zarib, 0xFF000000 + ColourToUint(90, 0, 50));
        roundedBoxColor(renderer, W / 2 - wb, H / 2 - hb, W / 2 + wb, H / 2 + hb, 40 * zarib, 0xFF000000 + ColourToUint(122, 0, 65));
        // text
        betterText(renderer, "Settings", W / 2, H / 2 - hb + 48, 255, 255, 255, 250 * zarib + 5, 64, "Baloo");
        betterText(renderer, "Music volume", W / 2 - wb + 180, H / 2 - hb + 170, 255, 255, 255, 250 * zarib + 5, 48, "Baloo");
        betterText(renderer, "SoundFX volume", W / 2 - wb + 215, H / 2 - hb + 250, 255, 255, 255, 250 * zarib + 5, 48, "Baloo");
        betterText(renderer, "Brightness", W / 2 - wb + 150, H / 2 - hb + 330, 255, 255, 255, 250 * zarib + 5, 48, "Baloo");
        betterText(renderer, "Soundtrack", W / 2 - wb + 165, H / 2 - hb + 410, 255, 255, 255, 250 * zarib + 5, 48, "Baloo");
        // rects
        SDL_Rect rect = {W / 2 + wb - 60 - 180 * zarib, H / 2 - hb + 170 - 20, 180 * zarib, 40};
        // sliders
        for (int i = 0; i < 3; i++)
        {
            roundedBoxRGBA(renderer, rect.x - 20, rect.y, rect.x + rect.w + 20, rect.y + rect.h, 20, 255, 255, 255, 255 * zarib);
            getSlider((i == 0) ? volumeMus : ((i == 1) ? volumeFx : brightness), rect, event);
            roundedBoxRGBA(renderer, rect.x - 20, rect.y, rect.x + rect.w * ((i == 0) ? volumeMus : ((i == 1) ? volumeFx : brightness)) / 100 + 20, rect.y + rect.h, 20, 0, 255, 100, 255 * zarib);
            rect.y += 80;
        }
        if (mousePress(event))
            setSettings();
        rect.y -= 80;
        betterText(renderer, to_string(track + 1), rect.x + rect.w / 2, rect.y + 100, 255, 255, 255, 254 * zarib + 1, 72, "Baloo");
        for (size_t i = 0; i < 3; i++)
        {
            SDL_SetTextureAlphaMod(buttons[i].texture, 254 * zarib + 1);
            buttons[i].animate();
            buttons[i].drawButton(renderer);
            buttons[i].checkPress(event);
        }
        if (buttons[0].pressGet())
        {
            track++;
            SDL_Delay(500);
        }
        if (buttons[1].pressGet())
        {
            track--;
            SDL_Delay(120);
        }
        track %= 5;
        if (track == -1)
            track = 0;
        if (buttons[2].pressGet())
            return 1;
        return 0;
    }
    void reset()
    {
        for (size_t i = 0; i < 3; i++)
            buttons[i].pressSet(false);
        bgAnimTime = 0;
    }
};
class collections : menu
{
private:
    double **arr;
    int k;
    SDL_Texture *tnemudar, *tbody, *tpowerup;
    SDL_Event *event;

public:
    collections(SDL_Renderer *Renderer, button *Buttons, SDL_Event *Event)
        : menu(Renderer, Buttons, NULL, IMG_LoadTexture(Renderer, "./textures/tiling.png"))
    {
        arr = new double *[7];
        tnemudar = IMG_LoadTexture(renderer, "./textures/nemoodar.png");
        beriztush("beyranvand", 6);
        beriztush("messi", 0);
        beriztush("moSaleh", 5);
        beriztush("neymar", 2);
        beriztush("ramos", 4);
        beriztush("ronaldo", 1);
        beriztush("suarez", 3);
        event = Event;
        tbody = IMG_LoadTexture(renderer, ("./fullBody/" + to_string(0) + ".png").c_str());
        tpowerup = IMG_LoadTexture(renderer, ("./powerups/" + to_string(0) + ".png").c_str());
        k = 0;
    }
    void beriztush(string name, int i)
    {
        ifstream in;
        in.open(("./stats/" + name + ".txt").c_str());
        arr[i] = new double[5];
        for (int j = 0; j < 5; j++)
            in >> arr[i][j];
        in.close();
    }
    void nemudarbekesh(int i)
    {
        SDL_Rect nemudarrect = {W - 300, H / 2 - 120, 240, 240};
        SDL_RenderCopy(renderer, tnemudar, NULL, &nemudarrect);
        for (int j = 0; j < 5; j++)
        {
            const Sint16 arrpx[4] = {(arr[i][0] - 180) / 3 + j + W - 300 + 120, +420 * arr[i][1] - 330 + j + W - 300 + 120, -5 * arr[i][2] + 50 + j + W - 300 + 120, -arr[i][4] + 50 + j + W - 300 + 120};
            const Sint16 arrpy[4] = {(arr[i][0] - 180) / 3 + H / 2 + j, -420 * arr[i][1] + 330 + H / 2 - j, -5 * arr[i][2] + 50 + H / 2 - j, arr[i][4] - 50 + H / 2 + j};
            aapolygonColor(renderer, arrpx, arrpy, 4, 0XFFFFFFFF);
        }
    }
    int bekesh()
    {
        bgAnimTime %= 1000;
        SDL_Rect Rtiles = {0, 0, 400, 400};
        SDL_Rect poweruprect = {W / 2 - 250, H / 2 - 150 - 120, 500, 300};
        int xmouse, ymouse;
        SDL_GetMouseState(&xmouse, &ymouse);
        Rtiles.x = bgAnimTime / 5 - 400 - 300 * abs(xmouse % W - W / 2) * (xmouse % W - W / 2) / (W * W);
        Rtiles.y = bgAnimTime / 5 - 400 - 300 * abs(ymouse % H - H / 2) * (ymouse % H - H / 2) / (H * H);
        int x[4] = {0, W, 0, W}, y[4] = {0, 0, H, H};
        SDL_Color selectColor = {220, 80, 120};
        SDL_Color shadeColor = {0, 0, 0};
        drawZoozanaghe(x, y, &selectColor, &selectColor, renderer);
        drawShadedZoozanaghe(x, y, &shadeColor, renderer);
        for (size_t i = 0; i < 30; i++)
        {
            SDL_RenderCopy(renderer, background, NULL, &Rtiles);
            Rtiles.x += 400;
            if (i % 6 == 0 && i != 0)
            {
                Rtiles.x = bgAnimTime / 5 - 400 - 300 * abs(xmouse % W - W / 2) * (xmouse % W - W / 2) / (W * W);
                Rtiles.y += 400;
            }
        }
        bgAnimTime += 2;
        betterText(renderer, "Press esc to go back", W / 2, 48, 255, 255, 255, 255, 48, "Baloo");
        betterText(renderer, "jump", W - 300 + 240, H / 2 + 120, 255, 255, 255, 255, 24, "Baloo");
        betterText(renderer, "shoot", W - 300, H / 2 + 120, 255, 255, 255, 255, 24, "Baloo");
        betterText(renderer, "speed", W - 300, H / 2 - 120, 255, 255, 255, 255, 24, "Baloo");
        betterText(renderer, "height", W - 300 + 240, H / 2 - 120, 255, 255, 255, 255, 24, "Baloo");
        SDL_Rect bodyrect = {100, 180, 300, 450};
        for (int i = 7; i < 14; i++)
        {
            buttons[i].animate();
            buttons[i].drawButton(renderer);
            buttons[i].checkPress(event);
            if (buttons[i].pressGet())
            {
                k = i - 7;
                tbody = IMG_LoadTexture(renderer, ("./fullBody/" + to_string(k) + ".png").c_str());
                tpowerup = IMG_LoadTexture(renderer, ("./powerups/" + to_string(k) + ".png").c_str());
            }
        }
        SDL_QueryTexture(tbody, NULL, NULL, &bodyrect.w, &bodyrect.h);
        bodyrect.w /= 4;
        bodyrect.h /= 4;
        SDL_RenderCopy(renderer, tbody, NULL, &bodyrect);
        roundedBoxColor(renderer, poweruprect.x - 30, poweruprect.y - 30, poweruprect.x + poweruprect.w + 30, poweruprect.y + poweruprect.h + 100, 30, 0xAA444444);
        switch (k)
        {
        case 0:
            betterText(renderer, "Triples players!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 36, "Baloo");
            break;
        case 1:
            betterText(renderer, "Steals a random powerUp!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 36, "Baloo");
            break;
        case 2:
            betterText(renderer, "Makes the ball invisible!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 36, "Baloo");
            break;
        case 3:
            betterText(renderer, "Punches and stuns other player!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 36, "Baloo");
            break;
        case 4:
            betterText(renderer, "Makes charcter bigger!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 36, "Baloo");
            break;
        case 5:
            betterText(renderer, "Shoots a fireball that stuns other player!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 24, "Baloo");
            break;
        case 6:
            betterText(renderer, "Scores a head goal using a fireball that stuns!", W / 2, poweruprect.y + poweruprect.h + 50, 255, 255, 255, 255, 24, "Baloo");
            break;
        default:
            break;
        }
        SDL_RenderCopy(renderer, tpowerup, NULL, &poweruprect);
        nemudarbekesh(k);
        betterText(renderer, buttons[k + 7].name, bodyrect.x + bodyrect.w / 2, bodyrect.y + bodyrect.h + 10, 255, 255, 255, 255, 48, "Baloo");
        if (event->key.keysym.sym == SDLK_ESCAPE)
            return 1;
        return 0;
    }
};
class leaderBoard
{
public:
    string names[100];
    int goalzade[100];
    int goalkhorde[100];
    int score[100];
    int duration[100];
    int timesplayed[100];
    int gameswon[100];
    int gameslost[100];
    int n, pageMax, page;
    int animTime;
    SDL_Renderer *renderer;
    SDL_Texture *screens[4];
    button returnButton;
    leaderBoard(SDL_Renderer *Renderer)
    {
        renderer = Renderer;
        n = 0;
        page = 0;
        animTime = 0;
        load();
        setPageMax();
        for (int i = 0; i < 4; i++)
            screens[i] = IMG_LoadTexture(renderer, ("./Leaderboard/" + to_string(i + 1) + ".png").c_str());
        SDL_Texture *buttonT = IMG_LoadTexture(renderer, "./Leaderboard/homeButton.png");
        returnButton = button(W - 100, 100, 100, 100, buttonT, 0);
    }
    void load()
    {
        setarrnull(names, 100);
        setintarrnull(goalzade, 100);
        setintarrnull(goalkhorde, 100);
        setintarrnull(score, 100);
        setintarrnull(duration, 100);
        setintarrnull(timesplayed, 100);
        setintarrnull(gameswon, 100);
        setintarrnull(gameslost, 100);
        readfile(names, goalzade, goalkhorde, score, duration, timesplayed, gameswon, gameslost, n);
        quicksortscores(names, goalzade, goalkhorde, score, duration, timesplayed, gameswon, gameslost, n, 0);
        reversescores(names, goalzade, goalkhorde, score, duration, timesplayed, gameswon, gameslost, n);
    }
    void setPageMax()
    {
        pageMax = n / 7;
        if (double(pageMax) < (double(n) / 7.0))
            pageMax++;
        pageMax--;
    }
    int loadscores(SDL_Event *event)
    {
        if (!animTime)
        {
            cout << "yeahhh" << endl;
            n = 0;
            load();
        }
        if (animTime < 12)
            animTime++;
        double zarib = animTime / 12.0;
        setPageMax();
        SDL_Rect rect = {0, 0, W, H};
        SDL_RenderCopy(renderer, screens[min(page, 3)], NULL, &rect);
        betterText(renderer, "Leaderboard!", 640, 90, 255, 255, 255, 255 * zarib, 100, "Baloo");
        betterText(renderer, "Press Esc to exit to the main menu", 640, 140, 255, 255, 255, 255 * zarib, 20, "Baloo");
        betterText(renderer, "Name", 300, 205, 255, 255, 255, 255 * zarib, 32, "Baloo");
        betterText(renderer, "Goals", 500, 205, 255, 255, 255, 255 * zarib, 32, "Baloo");
        betterText(renderer, "Total time", 990, 205, 255, 255, 255, 255 * zarib, 32, "Baloo");
        betterText(renderer, "Games", 800, 205, 255, 255, 255, 255 * zarib, 32, "Baloo");
        betterText(renderer, "Score", 1140, 205, 255, 255, 255, 255 * zarib, 32, "Baloo");
        int i = page * 7;
        int a = min(7, (n - i));
        for (int j = 0; j < a; j++)
        {
            switch (page)
            {
            case 0:
                betterText(renderer, names[i + j], 300, 255 + 78 * j, 253, 191, 76, 255 * zarib, 22, "Baloo");
                betterText(renderer, to_string((i + j + 1)), 125, 255 + 78 * j, 253, 191, 76, 255 * zarib, 30, "Baloo");
                break;
            case 1:
                betterText(renderer, names[i + j], 300, 255 + 78 * j, 202, 195, 183, 255 * zarib, 22, "Baloo");
                betterText(renderer, to_string((i + j + 1)), 125, 255 + 78 * j, 202, 195, 183, 255 * zarib, 30, "Baloo");
                break;
            case 2:
                betterText(renderer, names[i + j], 300, 255 + 78 * j, 255, 130, 94, 255 * zarib, 22, "Baloo");
                betterText(renderer, to_string((i + j + 1)), 125, 255 + 78 * j, 255, 130, 94, 255 * zarib, 30, "Baloo");
                break;
            default:
                betterText(renderer, names[i + j], 300, 255 + 78 * j, 255, 255, 255, 255 * zarib, 22, "Baloo");
                betterText(renderer, to_string((i + j + 1)), 125, 255 + 78 * j, 255, 255, 255, 255 * zarib, 30, "Baloo");
                break;
            }
            roundedBoxRGBA(renderer, 400, 255 + 78 * j - 20, 600, 255 + 78 * j + 20, 20, 120, 15, 160, 255);
            betterText(renderer, "+" + to_string(goalzade[i + j]), 450, 255 + 78 * j, 74, 212, 0, 255 * zarib, 22, "Baloo");
            betterText(renderer, "-" + to_string(goalkhorde[i + j]), 500, 255 + 78 * j, 230, 0, 50, 255 * zarib, 22, "Baloo");
            betterText(renderer, to_string(goalzade[i + j] - goalkhorde[i + j]), 550, 255 + 78 * j, 240, 240, 0, 255 * zarib, 22, "Baloo");
            roundedBoxRGBA(renderer, 700, 255 + 78 * j - 20, 900, 255 + 78 * j + 20, 20, 120, 15, 160, 255);
            betterText(renderer, to_string(score[i + j]), 1145, 255 + 78 * j, 255, 255, 255, 255 * zarib, 35, "Baloo");
            betterText(renderer, timeplayed(duration[i + j]), 990, 255 + 78 * j, 255, 255, 255, 255 * zarib, 22, "Baloo");
            betterText(renderer, to_string(timesplayed[i + j]), 850, 255 + 78 * j, 255, 255, 255, 255 * zarib, 22, "Baloo");
            betterText(renderer, to_string(gameswon[i + j]), 750, 255 + 78 * j, 74, 212, 0, 255 * zarib, 22, "Baloo");
            betterText(renderer, to_string(gameslost[i + j]), 780, 255 + 78 * j, 230, 0, 50, 255 * zarib, 22, "Baloo");
            betterText(renderer, to_string(timesplayed[i + j] - gameslost[i + j] - gameswon[i + j]), 810, 255 + 78 * j, 240, 240, 0, 255 * zarib, 22, "Baloo");
        }
        roundedBoxColor(renderer, W - 150, 50, W - 50, 150, 20, 0xFFFFFFFF);
        returnButton.animate();
        returnButton.drawButton(renderer);
        returnButton.checkPress(event);
        if (returnButton.pressGet())
        {
            reset();
            return 1;
        }
        if (SDL_PollEvent(event))
        {
            if (event->type == SDL_KEYDOWN)
            {
                switch (event->key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    reset();
                    return 1;
                    break;
                case SDLK_RIGHT:
                    if (page != pageMax)
                        page++;
                    break;
                case SDLK_LEFT:
                    if (page != 0)
                        page--;
                    break;
                default:
                    SDL_PollEvent(event);
                    break;
                }
            }
        }
        return 0;
    }
    void reset()
    {
        page = 0;
        animTime = 0;
    }
};
// main game loop
void gameLoop(SDL_Renderer *renderer, SDL_Window *window)
{
    gameState status = START_MENU; // initialising state
    // inititial variables
    static int frame0, frame1;
    int currentSong = 1;
    Mix_Music **gameSongs;
    gameSongs[0] = Mix_LoadMUS("./music/rocketLeage.mp3");
    gameSongs[1] = Mix_LoadMUS("./music/Darude - Sandstorm.mp3");
    gameSongs[2] = Mix_LoadMUS("./music/OOOOO.mp3");
    gameSongs[3] = Mix_LoadMUS("./music/axelF.mp3");
    static SDL_Event *event = new SDL_Event();
    // character and player info
    static characterCard stats1, stats2;
    static player player1 = {"", 0, 0, &stats1};
    static player player2 = {"", 0, 0, &stats2};
    static character Character1 = character(300, H - 40, &stats1, renderer, false);
    static character Character2 = character(1000, H - 40, &stats2, renderer, true);
    // sound effects and music
    static alottaParticles clickConfetti = alottaParticles(renderer, "confetti", 30, 20, 30, 12);
    alottaParticles transitionBits = alottaParticles(renderer, "koobs", 300, W, 0, 1000, NULL, 0.9);
    transitionBits.randomize();
    transitionBits.draw = true;
    Mix_Chunk *tick = Mix_LoadWAV("./soundEffects /tick.wav");
    Mix_Chunk **ingameFx;
    ingameFxInit(ingameFx);
    // initial screens classes
    static bubble Bubble = bubble(time(NULL), renderer, &Character1, &Character2);
    ball m_ball = ball("football", 40, 0, 0, renderer, &Character1, &Character2, &Bubble);
    game m_game = game(renderer, "", &player1, &player2, &Character1, &Character2, 300, ingameFx, &m_ball);
    button selectionScreenButtons[17], startMenuButtons[6], endScreenButton, pauseMenuButtons[4], settingMenuButtons[3];
    selectionScreenButtonsInit(selectionScreenButtons, renderer);
    initStartMenuButtons(startMenuButtons, renderer);
    initPauseMenuButtons(pauseMenuButtons, renderer);
    endMenuButtonInit(endScreenButton, renderer);
    settingsMenuButtonsInit(settingMenuButtons, renderer);
    menuStart startMenu = menuStart(renderer, startMenuButtons, tick);
    selectionMenu selectMenu = selectionMenu(renderer, selectionScreenButtons, tick, &m_game, 0);
    menuPause pauseMenu = menuPause(renderer, pauseMenuButtons, tick, &m_game);
    EndScreen endMenu = EndScreen(renderer, &player1, &player2, &endScreenButton);
    menuSettings settingsMenu = menuSettings(renderer, settingMenuButtons, NULL, window);
    leaderBoard boardLeader = leaderBoard(renderer);
    static collections collectionmenu = collections(renderer, selectionScreenButtons, event);
    gameState prevState;
    // player1.name = "borna";
    // player2.name = "sep";
    // Character1.stats->name = "beyranvand";
    // Character2.stats->name = "moSaleh";
    // Character1.stats->team = "barcelona";
    // Character2.stats->team = "madrid";
    // m_game.map = "grass";
    static int startTime = time(NULL);
    static int transitionAnim = time(NULL);
    // game loop
    while (status != CLOSED)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_PollEvent(event);
        // music handler
        if (currentSong != settingsMenu.track)
        {
            currentSong = settingsMenu.track;
            (currentSong == 4) ? Mix_HaltMusic() : Mix_PlayMusic(gameSongs[currentSong], 20);
            SDL_Delay(100); // to chill a bit xd
        }
        // rest of the code xd
        if (status == START_MENU)
        {
            int x = startMenu.showMenu(event);
            if (x)
            {
                startMenu.reset();
                switch (x)
                {
                case 1:
                    status = SELECTION_MENU;
                    transitionBits.randomize();
                    transitionAnim = clock();
                    break;
                case 2:
                    screenShot(renderer);
                    status = SETTINGS_MENU;
                    prevState = START_MENU;
                    break;
                case 3:
                    status = LEADER_BOARD;
                    break;
                case 4:
                    status = CLOSED;
                    break;
                case 5:
                    status = COLLECTION_MENU;
                    break;
                default:
                    break;
                }
            }
        }
        else if (status == SETTINGS_MENU)
        {
            SDL_Rect screen = {0, 0, W, H};
            SDL_Texture *screenS = IMG_LoadTexture(renderer, "screenshot.bmp");
            SDL_RenderCopy(renderer, screenS, NULL, &screen);
            SDL_DestroyTexture(screenS);
            if (settingsMenu.showMenu(event))
            {
                status = prevState;
                settingsMenu.reset();
            }
        }
        else if (status == LEADER_BOARD)
        {
            if (boardLeader.loadscores(event))
                status = START_MENU;
        }
        else if (status == POWERUPS_MENU)
        {
            /* code */
        }
        else if (status == COLLECTION_MENU)
        {
            if (collectionmenu.bekesh())
                status = START_MENU;
        }
        else if (status == SELECTION_MENU)
        {
            int x = selectMenu.showSelectionMenu(event);
            if (x == 1)
            {
                setDefaultStats(&stats1);
                setDefaultStats(&stats2);
                setDefaultAttributes(stats1, renderer);
                setDefaultAttributes(stats2, renderer);
                selectMenu.reset();
                m_game.setBackground();
                status = GAME_START;
            }
            else if (x == 2)
                status = START_MENU;
        }
        else if (status == GAME_START)
        {
            settingsMenu.track = 1;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            m_game.showIntro();
            status = INGAME;
            startTime = time(NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(200); // prevents bugs
        }
        else if (status == INGAME)
        {
            frame0 = clock();
            m_game.gameTime = (time(NULL) - startTime);
            if (m_game.showGameScreen(event))
            {
                status = INGAME_PAUSED;
                screenShot(renderer);
            }
            if (m_game.endGame())
            {
                status = END_MENU;
                screenShot(renderer);
            }
            frame1 = clock();
            if (frame1 - frame0 < 32)
                SDL_Delay(32 - (frame1 - frame0));
        }
        else if (status == INGAME_PAUSED)
        {
            SDL_Rect screen = {0, 0, W, H};
            SDL_Texture *screenS = IMG_LoadTexture(renderer, "screenshot.bmp");
            SDL_RenderCopy(renderer, screenS, NULL, &screen);
            SDL_DestroyTexture(screenS);
            int x = pauseMenu.showMenu(event);
            if (x)
            {
                pauseMenu.reset();
                switch (x)
                {
                case 1:
                    status = INGAME;
                    break;
                case 2:
                    status = INGAME;
                    m_game.resetGame();
                    m_game.showIntro();
                    startTime = time(NULL);
                    break;
                case 3:
                    screenShot(renderer);
                    status = SETTINGS_MENU;
                    prevState = INGAME_PAUSED;
                    break;
                case 4:
                    status = END_MENU;
                    break;
                default:
                    break;
                }
                SDL_Delay(200); // prevents errors
            }
        }
        else if (status == END_MENU)
        {
            SDL_Rect screen = {0, 0, W, H};
            SDL_Texture *screenS = IMG_LoadTexture(renderer, "screenshot.bmp");
            SDL_RenderCopy(renderer, screenS, NULL, &screen);
            SDL_DestroyTexture(screenS);
            if (endMenu.showEndScreen(event))
            {
                status = START_MENU;
                savescore(player1.name, m_game.gameTime, player1.goal, player2.goal, player1.score);
                savescore(player2.name, m_game.gameTime, player2.goal, player1.goal, player2.score);
                player1 = {"", 0, 0, &stats1};
                player2 = {"", 0, 0, &stats2};
            }
        }
        if (clock() - transitionAnim < 2000 && clock() - transitionAnim > 0)
        {
            cout << "titties" << endl;
            transitionBits.UpdateAndRender(0, 0);
        }
        if (status != INGAME && status != LEADER_BOARD)
            clickConfetti.clickStream(event);
        SDL_RenderPresent(renderer);
    }
    return;
}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS;//SDL_WINDOW_FULLSCREEN; 
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    SDL_Init(SDL_flags);
    TTF_Init();
    SDL_CreateWindowAndRenderer(1280, 800, WND_flags, &m_window, &m_renderer);
    SDL_RaiseWindow(m_window);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
    // the game
    gameLoop(m_renderer, m_window); //E=mc2 F=ma and ... dU=TdS-PdV+miudN S=-kB(sigma(pi*ln(pi)))
    // Finalize and free resources
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
// functions and classes
float fastSine(float x)
{
    const float PI = 3.1416;
    short int sign = 2 * (int(x / PI) % 2) - 1;
    x = x - PI * (int(x / PI));
    return sign * (16 * x * (PI - x)) / (5 * PI * PI - 4 * x * (PI - x));
}
float easeBackinOut(float p1, float p2, int time, int totalTime, float overshoot /*def = 1*/)
{
    // this function implements an animation which
    // Starts the animation slowly as it overshoots the initial keyframe’s value, then accelerates quickly before it slows and overshoots the ending keyframes value.
    // quick motion with a rebounding motion in the end
    // By Borna Khodabandeh
    float d = p2 - p1;
    float s = overshoot * 1.525;
    double t = 2 * time / (double)totalTime;
    if (t < 1)
        return (d / 2) * (t * t * ((s + 1) * t - s)) + p1;
    t -= 2;
    return (d / 2) * ((t * t * ((s + 1) * t + s)) + 2) + p1;
}
float parabola(float P1, float Pmax, int time, int totalTime)
{
    return P1 + (Pmax - P1) * 4 * (time * (totalTime - time)) / (float)(totalTime * totalTime);
}
float easeJump(float P1, float Pmax, int &time, int totalTime) // might change
{
    if (time < totalTime)
    {
        time++;
        return parabola(P1, Pmax, time, totalTime);
    }
    else
    {
        time = 0;
        return easeJump(P1, Pmax, time, totalTime);
    }
}
Uint32 ColourToUint(int R, int G, int B)
{
    return (Uint32)((R << 16) + (G << 8) + (B << 0));
}
void drawZoozanaghe(int x[4], int y[4], SDL_Color *mainColor, SDL_Color *borderColor, SDL_Renderer *renderer) // 0 , 1 : bala / 2 , 3 : payin
{
    for (int i = 0; i <= y[2] - y[0]; i++)
        hlineColor(renderer, x[0] + (i * (x[2] - x[0]) / (y[2] - y[0])), x[1] + (i * (x[3] - x[1]) / (y[2] - y[0])), y[0] + i, 0xFF000000 + ColourToUint(mainColor->r, mainColor->g, mainColor->b));
    for (int i = -2; i < 2 && borderColor != NULL; i++)
    {
        aalineColor(renderer, x[0] - i, y[0] - i, x[1] + i, y[1] - i, 0xFF000000 + ColourToUint(borderColor->r, borderColor->g, borderColor->b));
        aalineColor(renderer, x[0] - i, y[0] - i, x[2] - i, y[2] + i, 0xFF000000 + ColourToUint(borderColor->r, borderColor->g, borderColor->b));
        aalineColor(renderer, x[1] + i, y[1] - i, x[3] + i, y[3] + i, 0xFF000000 + ColourToUint(borderColor->r, borderColor->g, borderColor->b));
        aalineColor(renderer, x[2] - i, y[2] + i, x[3] + i, y[3] + i, 0xFF000000 + ColourToUint(borderColor->r, borderColor->g, borderColor->b));
    }
}
void drawShadedZoozanaghe(int x[4], int y[4], SDL_Color *mainColor, SDL_Renderer *renderer) // 0 , 1 : bala / 2 , 3 : payin
{
    for (int i = 0; i <= y[2] - y[0]; i++)
        hlineColor(renderer, x[0] + (i * (x[2] - x[0]) / (y[2] - y[0])), x[1] + (i * (x[3] - x[1]) / (y[2] - y[0])), y[0] + i, Uint32((i * 150 / (y[2] - y[0])) << 24) + ColourToUint(mainColor->r, mainColor->g, mainColor->b));
}
void selectionMenuText(string S1, string S2, int &animTime, SDL_Renderer *renderer, bool which, string S3)
{
    if (which)
    {
        if (animTime == 0)
        {
            betterText(renderer, S1, W / 2, 100, 255, 255, 255, 255, 72, "Baloo");
            betterText(renderer, S2, W / 2, H - 100, 255, 255, 255, 255, 36, "Baloo");
            betterText(renderer, S3, W / 2, H - 50, 255, 255, 255, 255, 24, "Baloo");
        }
        else
        {
            betterText(renderer, S1, W / 2, 100, 255, 255, 255, 250 * (72 - animTime) / 72 + 2, 72, "Baloo");
            betterText(renderer, S2, W / 2, easeBackinOut(H - 100, 100, animTime, 72), 255, 255, 255, 255, 36, "Baloo");
            betterText(renderer, S3, W / 2, easeBackinOut(H - 50, 50, animTime, 72), 255, 255, 255, 255, 24, "Baloo");
            animTime--;
        }
    }
    else
    {
        if (animTime < 72)
        {
            betterText(renderer, "Press enter to go next", W / 2, easeBackinOut(500, 100, animTime, 72), 255, 255, 255, 255, 36, "Baloo");
            betterText(renderer, "Press tab to go back", W / 2, easeBackinOut(550, 50, animTime, 72), 255, 255, 255, 255, 24, "Baloo");
            animTime++;
        }
        else
        {
            betterText(renderer, "Press enter to go next", W / 2, 100, 255, 255, 255, 255, 36, "Baloo");
            betterText(renderer, "Press tab to go back", W / 2, 50, 255, 255, 255, 255, 24, "Baloo");
        }
    }
}
int *faseleReact(int x, int y)
{
    int *arr;
    arr = new int[2];
    int s = x * x + y * y;
    if (s > 6400)
    {
        arr[0] = 20 * 20 * 5 * x / s;
        arr[1] = 20 * 20 * 5 * y / s;
    }
    else
    {
        arr[0] = x / 4;
        arr[1] = y / 4;
    }
    return arr;
}
void getSlider(int &slide, SDL_Rect rect, SDL_Event *event)
{
    SDL_Point point;
    SDL_GetMouseState(&point.x, &point.y);
    if (SDL_PointInRect(&point, &rect) == SDL_TRUE && mousePress(event))
        slide = 100 * (point.x - rect.x) / (double)rect.w;
}
void getTypingInput(string &s, SDL_Event *event)
{
    SDL_PumpEvents();
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if (event->type == SDL_KEYDOWN)
    {
        for (size_t i = 4; i < 30; i++)
        {
            if (keyboardState[i])
            {
                if (keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT])
                    s += (char)i + 61;
                else
                    s += (char)i + 93;
                SDL_Delay(90);
            }
        }
        for (size_t i = 30; i < 39; i++)
        {
            if (keyboardState[i])
            {
                s += (char)i + 19;
                SDL_Delay(90);
            }
        }
        if (keyboardState[39])
        {
            s += '0';
            SDL_Delay(90);
        }
        if (keyboardState[SDL_SCANCODE_SPACE])
        {
            s += ' ';
            SDL_Delay(90);
        }
        if (keyboardState[SDL_SCANCODE_BACKSPACE])
        {
            s = s.substr(0, s.size() - 1);
            SDL_Delay(90);
        }
    }
}
void getInputIngame(SDL_Event *Event, character &Character1, character &Character2)
{
    SDL_PumpEvents();
    const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
    if (Event->type == SDL_KEYDOWN || Event->type == SDL_KEYUP)
    {
        Character1.vx = Character1.stats->speed * ((int)keyboard_state_array[SDL_SCANCODE_D] - (int)keyboard_state_array[SDL_SCANCODE_A]) * !Character1.stunned;
        Character2.vx = Character2.stats->speed * ((int)keyboard_state_array[SDL_SCANCODE_RIGHT] - (int)keyboard_state_array[SDL_SCANCODE_LEFT]) * !Character2.stunned;
        Character1.jump = (bool)keyboard_state_array[SDL_SCANCODE_W];
        Character2.jump = (bool)keyboard_state_array[SDL_SCANCODE_UP];
        if (keyboard_state_array[SDL_SCANCODE_LCTRL] && !Character1.powerUp && Character1.getPower)
            Character1.powerUpAct = clock();
        if (keyboard_state_array[SDL_SCANCODE_RCTRL] && !Character2.powerUp && Character2.getPower)
            Character2.powerUpAct = clock();
        Character1.variant = 2 * keyboard_state_array[SDL_SCANCODE_LSHIFT];
        Character2.variant = 2 * keyboard_state_array[SDL_SCANCODE_RSHIFT];
    }
}
void setDefaultAttributes(characterCard &stats, SDL_Renderer *renderer)
{
    stats.Thead = IMG_LoadTexture(renderer, ("./textures/" + stats.name + "Head.png").c_str());
    stats.Tbody = IMG_LoadTexture(renderer, ("./textures/" + stats.team + "Shirt.png").c_str());
    stats.Tfoot = IMG_LoadTexture(renderer, "./textures/shoe3.png");
    stats.Tcard = IMG_LoadTexture(renderer, ("./textures/" + stats.team + "Card.png").c_str());
    stats.Tfan = IMG_LoadTexture(renderer, ("./textures/" + stats.team + "Fan.png").c_str());
    stats.Thand = IMG_LoadTexture(renderer, "./textures/hand.png");
}
void setDefaultStats(characterCard *stats) // should change
{
    fstream input;
    input.open(("./stats/" + stats->name + ".txt").c_str(), ios::in);
    if (input.is_open())
        input >> stats->jump >> stats->scale >> stats->speed >> stats->powerUp >> stats->shootingSpeed;
    input.close();
    stats->scale0 = stats->scale;
    stats->powerUp0 = stats->powerUp;
    stats->maxJump = stats->jump;
}
void betterText(SDL_Renderer *renderer, string S, int x, int y, int r, int g, int b, int a, int size, string Font /*def = arial */) // better function to render ttf text
{
    string fontDir = "./fonts/" + Font + ".ttf";
    TTF_Font *font = TTF_OpenFont(fontDir.c_str(), size);
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, S.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    int h, w;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    rect.h = h;
    rect.w = w;
    rect.x = x - w / 2;
    rect.y = y - h / 2;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
bool mousePress(SDL_Event *event)
{
    return event->type == SDL_MOUSEBUTTONUP;
}
void initStartMenuButtons(button *startButtons, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("./fonts/Baloo.ttf", 144);
    SDL_Color color = {255, 255, 255};
    SDL_Texture *tPlayButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Play", color));
    SDL_Texture *tSettingsButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Settings", color));
    SDL_Texture *tScoresButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Scores", color));
    SDL_Texture *tQuitButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Quit", color));
    SDL_Texture *tCollectionButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Collection", color));
    SDL_Texture *tPowerUpsButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "PowerUps", color));
    int hButton, wButton;
    SDL_QueryTexture(tPlayButton, NULL, NULL, &wButton, &hButton);
    startButtons[0] = button(W - 75 * (3 - 4 / double(H)) / 2, H / 2 - 100, 100, wButton * 100 / hButton, tPlayButton, 1);
    SDL_QueryTexture(tSettingsButton, NULL, NULL, &wButton, &hButton);
    startButtons[1] = button(W - 75 * (3 - 4 / double(H)) / 2, H / 2 + 20, 100, 200, tSettingsButton, 1);
    SDL_QueryTexture(tScoresButton, NULL, NULL, &wButton, &hButton);
    startButtons[2] = button(W - 75 * (3 - 4 / double(H)) / 2 - 20, H / 2 + 140, 100, wButton * 100 / hButton, tScoresButton, 1);
    SDL_QueryTexture(tQuitButton, NULL, NULL, &wButton, &hButton);
    startButtons[3] = button(W - 75 * (5 - 4 / double(H)) / 2, H / 2 + 260, 100, wButton * 100 / hButton, tQuitButton, 1);
    SDL_QueryTexture(tCollectionButton, NULL, NULL, &wButton, &hButton);
    startButtons[4] = button(75 * (3 - 4 / double(H)) / 2 + 15, H / 4 + 50, 80, 200, tCollectionButton, 1);
    SDL_QueryTexture(tPowerUpsButton, NULL, NULL, &wButton, &hButton);
    startButtons[5] = button(75 * (3 - 4 / double(H)) / 2, H / 4 + 200, 80, 180, tPowerUpsButton, 1);
    Mix_Chunk *tick = Mix_LoadWAV("./soundEffects/tick.wav");
}
void initPauseMenuButtons(button *startButtons, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("./fonts/Baloo.ttf", 144);
    SDL_Color color = {255, 255, 255};
    SDL_Texture *tPlayButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "resume", color));
    SDL_Texture *tSettingsButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "restart", color));
    SDL_Texture *tScoresButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "settings", color));
    SDL_Texture *tQuitButton = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "main menu", color));
    int hButton, wButton;
    SDL_QueryTexture(tPlayButton, NULL, NULL, &wButton, &hButton);
    startButtons[0] = button(W - 175 * (3 - 4 / double(H)) / 2, H / 2 - 100 + 40, 100, wButton * 100 / hButton, tPlayButton, 1);
    SDL_QueryTexture(tSettingsButton, NULL, NULL, &wButton, &hButton);
    startButtons[1] = button(W - 175 * (3 - 4 / double(H)) / 2, H / 2 + 20 + 40, 100, 200, tSettingsButton, 1);
    SDL_QueryTexture(tScoresButton, NULL, NULL, &wButton, &hButton);
    startButtons[2] = button(W - 175 * (3 - 4 / double(H)) / 2, H / 2 + 140 + 40, 100, wButton * 100 / hButton, tScoresButton, 1);
    SDL_QueryTexture(tQuitButton, NULL, NULL, &wButton, &hButton);
    startButtons[3] = button(W - 175 * (3 - 4 / double(H)) / 2, H / 2 + 260 + 40, 100, wButton * 100 / hButton, tQuitButton, 1);
    Mix_Chunk *tick = Mix_LoadWAV("./soundEffects/tick.wav");
}
void selectionScreenButtonsInit(button *buttons, SDL_Renderer *renderer)
{
    SDL_Texture *tMadrid = IMG_LoadTexture(renderer, "./textures/madridCard.png");
    SDL_Texture *tMessiHead = IMG_LoadTexture(renderer, "./textures/messiHead.png");
    SDL_Texture *tGrassButton = IMG_LoadTexture(renderer, "./textures/grassButton.png");
    int hCard, wCard, wHead, hHead, wButton, hButton;
    SDL_QueryTexture(tMadrid, NULL, NULL, &wCard, &hCard);
    SDL_QueryTexture(tMessiHead, NULL, NULL, &wHead, &hHead);
    SDL_QueryTexture(tGrassButton, NULL, NULL, &wButton, &hButton);
    hCard /= 1.5;
    wCard /= 1.5;
    hHead /= 3;
    wHead /= 3;
    buttons[0] = button(1 * W / 8, H - hCard / 2 - 10, hCard, wCard, tMadrid, 0, "madrid");
    buttons[1] = button(2 * W / 8, H - hCard / 2 - 10, hCard, wCard, IMG_LoadTexture(renderer, "./textures/barcelonaCard.png"), 0, "barcelona");
    buttons[2] = button(3 * W / 8, H - hCard / 2 - 10, hCard, wCard, IMG_LoadTexture(renderer, "./textures/chelseaCard.png"), 0, "chelsea");
    buttons[3] = button(4 * W / 8, H - hCard / 2 - 10, hCard, wCard, IMG_LoadTexture(renderer, "./textures/liverpoolCard.png"), 0, "liverpool");
    buttons[4] = button(5 * W / 8, H - hCard / 2 - 10, hCard, wCard, IMG_LoadTexture(renderer, "./textures/manchesterCard.png"), 0, "manchester");
    buttons[5] = button(6 * W / 8, H - hCard / 2 - 10, hCard, wCard, IMG_LoadTexture(renderer, "./textures/perspolisCard.png"), 0, "perspolis");
    buttons[6] = button(7 * W / 8, H - hCard / 2 - 10, hCard, wCard, IMG_LoadTexture(renderer, "./textures/esteghlalCard.png"), 0, "esteghlal");
    buttons[7] = button(1 * W / 8, H - hCard / 2 - 10, hHead, wHead, tMessiHead, 0, "messi");
    buttons[8] = button(2 * W / 8, H - hCard / 2 - 10, hHead, wHead, IMG_LoadTexture(renderer, "./textures/ronaldoHead.png"), 0, "ronaldo");
    buttons[9] = button(3 * W / 8, H - hCard / 2 - 10, hHead, wHead, IMG_LoadTexture(renderer, "./textures/neymarHead.png"), 0, "neymar");
    buttons[10] = button(4 * W / 8, H - hCard / 2 - 10, hHead, wHead, IMG_LoadTexture(renderer, "./textures/suarezHead.png"), 0, "suarez");
    buttons[11] = button(5 * W / 8, H - hCard / 2 - 10, hHead, wHead, IMG_LoadTexture(renderer, "./textures/ramosHead.png"), 0, "ramos");
    buttons[12] = button(6 * W / 8, H - hCard / 2 - 10, hHead, wHead, IMG_LoadTexture(renderer, "./textures/moSalehHead.png"), 0, "moSaleh");
    buttons[13] = button(7 * W / 8, H - hCard / 2 - 10, hHead, wHead, IMG_LoadTexture(renderer, "./textures/beyranvandHead.png"), 0, "beyranvand");
    buttons[14] = button(1 * W / 4 - 50, H - hButton / 2 - 10, hButton, wButton, tGrassButton, 0, "grass");
    buttons[15] = button(2 * W / 4, H - hButton / 2 - 10, hButton, wButton, IMG_LoadTexture(renderer, "./textures/sandButton.png"), 0, "sand");
    buttons[16] = button(3 * W / 4 + 50, H - hButton / 2 - 10, hButton, wButton, IMG_LoadTexture(renderer, "./textures/streetButton.png"), 0, "street");
}
void endMenuButtonInit(button &homeButton, SDL_Renderer *renderer)
{
    SDL_Texture *ThomeButton = IMG_LoadTexture(renderer, "./textures/mainMenu.png");
    int wButton, hButton;
    SDL_QueryTexture(ThomeButton, NULL, NULL, &wButton, &hButton);
    homeButton = button(W / 2, 0.9 * H, hButton, wButton, ThomeButton, 0);
}
void settingsMenuButtonsInit(button *settingButtons, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("./fonts/Baloo.ttf", 72);
    SDL_Color color = {255, 255, 255};
    SDL_Texture *tRArrow = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, ">", color));
    SDL_Texture *tLArrow = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "<", color));
    SDL_Texture *tButton = IMG_LoadTexture(renderer, "./textures/backButton.png");
    int hButton, wButton;
    SDL_QueryTexture(tRArrow, NULL, NULL, &wButton, &hButton);
    settingButtons[0] = button(W / 2 + 400 - 60 - 90 + 100, H / 2 - 300 + 410, hButton, wButton, tRArrow, 1);
    settingButtons[1] = button(W / 2 + 400 - 60 - 90 - 100, H / 2 - 300 + 410, hButton, wButton, tLArrow, 1);
    SDL_QueryTexture(tButton, NULL, NULL, &wButton, &hButton);
    settingButtons[2] = button(W / 2, H / 2 + 300 - 80, hButton, wButton, tButton, 0);
}
void ingameFxInit(Mix_Chunk **chunks)
{
    chunks[0] = Mix_LoadWAV("./soundEffects/whistle.wav");
    chunks[1] = Mix_LoadWAV("./soundEffects/ding2.wav");
    chunks[2] = Mix_LoadWAV("./soundEffects/betterCheer.wav");
}
void screenShot(SDL_Renderer *renderer)
{
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, W, H, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, "screenshot.bmp");
    SDL_FreeSurface(sshot);
}
string tdigitnum(int num)
{
    if (num < 10)
        return ("0" + to_string(num));
    else
        return (to_string(num));
}
string timeplayed(int t)
{
    return (tdigitnum(t / 3600) + ":" + tdigitnum(((t % 3600) / 60)) + ":" + tdigitnum(t % 60));
}
string timeplaying(int t)
{
    return (tdigitnum((t / 60)) + ":" + tdigitnum(t % 60));
}
bool lowercasehast(char a)
{
    if ((int(a) <= int('z')) && (int(a) >= int('a')))
        return true;
    return false;
}
bool uppercasehast(char a)
{
    if ((int(a) <= int('Z')) && (int(a) >= int('A')))
        return true;
    return false;
}
char lowercasee(char a)
{
    if (uppercasehast(a))
        return (('a' - 'A') + a);
    return a;
}
bool yekian(string a, string b)
{
    int la = a.length(), lb = b.length();
    if (la != lb)
        return false;
    for (int i = 0; i < la; i++)
        if (lowercasee(a[i]) != lowercasee(b[i]))
            return false;
    return true;
}
bool issmaller(string a, string b)
{
    int la = a.length(), lb = b.length(), lmin = min(la, lb), tulcheck = min(5, lmin);
    for (int i = 0; i < tulcheck; i++)
        if (lowercasee(a[i]) != lowercasee(b[i]))
        {
            if (int(lowercasee(a[i])) > int(lowercasee(b[i])))
                return true;
            else if (int(lowercasee(a[i])) < int(lowercasee(b[i])))
                return false;
        }
        else
        {
            if (int(a[i]) > int(b[i]))
                return true;
            else if (int(a[i]) < int(b[i]))
                return false;
        }
    if (lmin >= 5)
        return false;
    else if (la > lb)
        return true;
    else if (la <= lb)
        return false;
}
void quicksort(string arr[], int n, int s)
{
    if ((n == 1) || (n == 0))
        return;
    int k = 0;
    for (int i = (s + n - 1); i >= (s + 1); i--)
        if (!issmaller(arr[s], arr[i]))
        {
            swap(arr[i], arr[s + n - k - 1]);
            k++;
        }
    swap(arr[s], arr[s + n - k - 1]);
    quicksort(arr, (n - k - 1), s);
    quicksort(arr, k, (s + n - k));
}
void reversescores(string names[], int goalzade[], int goalkhorde[], int score[], int duration[], int timesplayed[], int gameswon[], int gameslost[], int n)
{
    for (int i = 0; i < n / 2; i++)
    {
        swap(names[i], names[n - i - 1]);
        swap(goalzade[i], goalzade[n - i - 1]);
        swap(goalkhorde[i], goalkhorde[n - i - 1]);
        swap(score[i], score[n - i - 1]);
        swap(duration[i], duration[n - i - 1]);
        swap(timesplayed[i], timesplayed[n - i - 1]);
        swap(gameswon[i], gameswon[n - i - 1]);
        swap(gameslost[i], gameslost[n - i - 1]);
    }
}
void quicksortscores(string names[], int goalzade[], int goalkhorde[], int score[], int duration[], int timesplayed[], int gameswon[], int gameslost[], int n, int s)
{
    if ((n == 1) || (n == 0))
        return;
    int k = 0;
    for (int i = (s + n - 1); i >= (s + 1); i--)
    {
        if (score[i] > score[s])
        {
            swap(names[i], names[s + n - k - 1]);
            swap(goalzade[i], goalzade[s + n - k - 1]);
            swap(goalkhorde[i], goalkhorde[s + n - k - 1]);
            swap(score[i], score[s + n - k - 1]);
            swap(duration[i], duration[s + n - k - 1]);
            swap(timesplayed[i], timesplayed[s + n - k - 1]);
            swap(gameswon[i], gameswon[s + n - k - 1]);
            swap(gameslost[i], gameslost[s + n - k - 1]);
            k++;
        }
    }
    swap(names[s], names[s + n - k - 1]);
    swap(goalzade[s], goalzade[s + n - k - 1]);
    swap(goalkhorde[s], goalkhorde[s + n - k - 1]);
    swap(score[s], score[s + n - k - 1]);
    swap(duration[s], duration[s + n - k - 1]);
    swap(timesplayed[s], timesplayed[s + n - k - 1]);
    swap(gameswon[s], gameswon[s + n - k - 1]);
    swap(gameslost[s], gameslost[s + n - k - 1]);
    quicksortscores(names, goalzade, goalkhorde, score, duration, timesplayed, gameswon, gameslost, (n - k - 1), s);
    quicksortscores(names, goalzade, goalkhorde, score, duration, timesplayed, gameswon, gameslost, k, (s + n - k));
}
bool hast(string a, string b)
{
    int lb = b.length(), la = a.length();
    if (lb < la)
        return false;
    bool oke = true;
    for (int i = 0; i < la; i++)
        if (lowercasee(a[i]) != lowercasee(b[i]))
            return false;
    return true;
}
void savescore(string playername, double duration, int goalzade, int goalkhorde, int score)
{
    ofstream o;
    o.open("scores.txt", ios_base::app);
    o << playername << " " << goalzade << " " << goalkhorde << " " << score << " " << int(duration) << endl;
    o.close();
}
int afterspace(int n, string line)
{
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        while (line[j] != ' ')
            j++;
        j++;
    }
    return j;
}
string namefromline(string line)
{
    string b = "";
    for (int i = 0; line[i] != ' '; i++)
        b += line[i];
    return b;
}
int goalzadefromline(string line)
{
    string b = "";
    for (int i = afterspace(1, line); line[i] != ' '; i++)
        b += line[i];
    return stoi(b);
}
int goalkhordefromline(string line)
{
    string b = "";
    for (int i = afterspace(2, line); line[i] != ' '; i++)
        b += line[i];
    return stoi(b);
}
int scorefromline(string line)
{
    string b = "";
    for (int i = afterspace(3, line); line[i] != ' '; i++)
        b += line[i];
    return stoi(b);
}
int durationfromline(string line)
{
    string b = "";
    for (int i = afterspace(4, line), l = line.length(); i < l; i++)
        b += line[i];
    return stoi(b);
}
void setarrnull(string arr[], int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = "";
}
void setintarrnull(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = 0;
}
void readfile(string names[], int goalzade[], int goalkhorde[], int score[], int duration[], int timesplayed[], int gameswon[], int gameslost[], int &n)
{
    ifstream in;
    in.open("scores.txt");
    string line, name;
    n = 0;
    bool tekrari = false;
    while (in.peek() != EOF)
    {
        getline(in, line);
        name = namefromline(line);
        int i = 0;
        for (; (i < n) && (!tekrari); i++)
            if (yekian(names[i], name))
                tekrari = true;
        if (tekrari)
        {
            i--;
            goalzade[i] += goalzadefromline(line);
            goalkhorde[i] += goalkhordefromline(line);
            score[i] += scorefromline(line);
            duration[i] += durationfromline(line);
            timesplayed[i] += 1;
            if (goalzadefromline(line) > goalkhordefromline(line))
                gameswon[i] += 1;
            else if (goalzadefromline(line) < goalkhordefromline(line))
                gameslost[i] += 1;
        }
        else
        {
            names[n] = namefromline(line);
            goalzade[n] = goalzadefromline(line);
            goalkhorde[n] = goalkhordefromline(line);
            score[n] = scorefromline(line);
            duration[n] = durationfromline(line);
            timesplayed[n] = 1;
            if (goalzadefromline(line) > goalkhordefromline(line))
                gameswon[n] = 1;
            else if (goalzadefromline(line) < goalkhordefromline(line))
                gameslost[n] = 1;
            n++;
        }
        tekrari = false;
    }
    in.close();
}
void readnames(string names[], int &n)
{
    ifstream in;
    in.open("scores.txt");
    string line;
    string name;
    n = 0;
    bool tekrari = false;
    while (in.peek() != EOF)
    {
        getline(in, line);
        name = namefromline(line);
        for (int i = 0; (i < n) && (!tekrari); i++)
        {
            if (yekian(names[i], name))
                tekrari = true;
        }
        if (!tekrari)
        {
            names[n] = namefromline(line);
            n++;
        }
        tekrari = false;
    }
    in.close();
}
void namesuggestion(string arrr[], string s, int &j)
{
    if (s == "")
    {
        j = 0;
        return;
    }
    string names[100];
    setarrnull(names, 100);
    int n;
    readnames(names, n);
    string arr[100];
    setarrnull(arr, 100);
    j = 0;
    for (int i = 0; i < n; i++)
    {
        if (hast(s, names[i]))
        {
            arr[j] = names[i];
            j++;
        }
    }
    quicksort(arr, j, 0);
    for (int i = 0; i < min(5, j); i++)
        arrr[i] = arr[i];
}

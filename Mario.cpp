#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 10;

vector<string> level = {
"########################################",
"# P      C      E      C       F       #",
"#                                      #",
"#     #######          #####           #",
"#                                      #",
"#         C                E           #",
"#   ######                     ####    #",
"#                                      #",
"#                                      #",
"########################################"
};

int playerX = 2;
int playerY = 1;

int score = 0;
int lives = 3;
bool jumping = false;
int jumpCount = 0;

void draw()
{
    system("cls");

    for(int y=0;y<HEIGHT;y++)
    {
        for(int x=0;x<WIDTH;x++)
        {
            if(x==playerX && y==playerY)
                cout<<"M";
            else
                cout<<level[y][x];
        }
        cout<<endl;
    }

    cout<<"\nScore : "<<score;
    cout<<"\nLives : "<<lives;
    cout<<"\nControls : A D W";
}

void gravity()
{
    if(level[playerY+1][playerX]==' ')
        playerY++;
}

void jump()
{
    if(jumping)
    {
        if(jumpCount<2)
        {
            if(level[playerY-1][playerX]==' ')
                playerY--;

            jumpCount++;
        }
        else
        {
            jumping=false;
            jumpCount=0;
        }
    }
}

void checkObjects()
{
    char &cell = level[playerY][playerX];

    if(cell=='C')
    {
        score+=10;
        cell=' ';
    }

    if(cell=='E')
    {
        lives--;
        playerX=2;
        playerY=1;

        if(lives==0)
        {
            system("cls");
            cout<<"GAME OVER\n";
            cout<<"Final Score : "<<score;
            exit(0);
        }
    }

    if(cell=='F')
    {
        system("cls");
        cout<<"YOU WIN!\n";
        cout<<"Final Score : "<<score<<endl;
        exit(0);
    }
}

int main()
{
    while(true)
    {
        draw();

        if(_kbhit())
        {
            char key=_getch();

            if(key=='a' || key=='A')
            {
                if(level[playerY][playerX-1]!='#')
                    playerX--;
            }

            if(key=='d' || key=='D')
            {
                if(level[playerY][playerX+1]!='#')
                    playerX++;
            }

            if((key=='w' || key=='W') && !jumping)
            {
                jumping=true;
            }
        }

        jump();
        gravity();
        checkObjects();

        Sleep(120);
    }

    return 0;
}

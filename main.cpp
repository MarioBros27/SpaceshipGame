#include<stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
#include<iostream>

using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80



void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon,dwPos);
}

void ocultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon,&cci);
}

class NAVE{
    int x, y;
    int vidas;
public:
    NAVE(int _x, int _y, int _vidas): x(_x),y(_y),vidas(_vidas){}
    int xNave(){return x;};
    int yNave(){return y;};
    void pintar();
    int getVidas(){return vidas;};
    void borrar();
    void mover();
    void contar_vidas();
    void explotar();
    void bajarVida(){vidas--;};

};


void NAVE:: pintar(){
    gotoxy(x,y);printf("  ^");
    gotoxy(x,y+1);printf("|/A%c|",92);
}

void NAVE::borrar(){
    gotoxy(x,y);printf("     ");
    gotoxy(x,y+1);printf("     ");
}
void NAVE:: mover(){
    if(kbhit()){
        char tecla = getch();
            borrar();
            if((tecla== IZQUIERDA||tecla == 's')&&x>4)x--;
            if((tecla== DERECHA||tecla == 'f')&&x<75)x++;
            if((tecla==ARRIBA||tecla == 'e')&&y>2)y--;
            if((tecla== ABAJO||tecla == 'd')&&y<27)y++;
            pintar();
            contar_vidas();
            //if(tecla == 'l')explotar();

        }
}
void NAVE:: contar_vidas(){
    gotoxy(64,0);printf("vidas");
    gotoxy(70,0);printf("    ");

    for(int c = 0; c<vidas; c++){
        gotoxy(70+c,0);printf("%c",43);
    }
}
void NAVE:: explotar(){
    borrar();
    gotoxy(x,y);printf("  %c",92);
    gotoxy(x,y+1);printf("%c%c%c%c%c",92,92,92,92,92);
    Sleep(250);
    gotoxy(x,y);printf("  _");
    gotoxy(x,y+1);printf("__-__");
    Sleep(150);
    vidas -=1;;
    contar_vidas();
    borrar();
    pintar();
}

void pintar_limites(){
    for(int c = 4; c< 80; c++){
        gotoxy(c,1);printf("%c",205);
        gotoxy(c,29);printf("%c",205);
    }
    for(int c = 2; c< 29; c++){
        gotoxy(3,c);printf("%c",186);
        gotoxy(80,c);printf("%c",186);
    }
    gotoxy(3,1);printf("%c",201);
    gotoxy(80,1);printf("%c",187);
    gotoxy(3,29);printf("%c",200);
    gotoxy(80,29);printf("%c",188);
}

class AST{
    int x, y;
public:
    AST(int _x, int _y):x(_x),y(_y){}
    int xAst(){return x;}
    int yAst(){return y;}
    void pintar();
    void mover();
    void choque(struct NAVE &N);
};
void AST::pintar(){
    gotoxy(x,y);printf("O");
}
void AST::mover(){
    gotoxy(x,y);printf("  ");
    y++;
    if(y>28){
        x = rand()%75 +4;
        y=2;
    }
    pintar();

}
void AST::choque(struct NAVE &N){
    if(x>= N.xNave()&& x< N.xNave()+5&& y>=N.yNave()&&y<=N.yNave()+1){
    N.bajarVida();
    N.explotar();
    N.borrar();
    N.pintar();
    N.contar_vidas();
    x = rand()%76 +4;
    y=2;

    }
}

class LASER{
    int x, y;
public:
    LASER(int _x, int _y): x(_x),y(_y){}
    int xLaser(){return x;};
    int yLaser(){return y;};
    void mover();
    bool fuera();
};

void LASER::mover(){
    gotoxy(x,y);printf(" ");
    if(y> 2)y--;
    gotoxy(x,y);printf("l");

}

bool LASER::fuera(){
    if(y<=2)return true;
    return false;
}
int main()
{
    ocultarCursor();
    pintar_limites();
    NAVE N(38,26,3);
    N.pintar();
    N.contar_vidas();

    //AST A(15,4), B(25,6), C(64,18),D(34,18),E(6,6);
    list<AST*> A;
    list<AST*>::iterator itA;
    for(int c = 0; c<5; c++){
        A.push_back(new AST(rand()%73+4,rand()%20+2));
    }

    list<LASER*> L;
    list<LASER*>::iterator it;

    bool game_over = false;
    int puntos = 0;

    while(!game_over){

            gotoxy(10,0);printf("Puntos: %d",puntos);
        if(kbhit()){
            //char tecla = getch();
            if(GetAsyncKeyState(0x20))
                L.push_back(new LASER(N.xNave()+2,N.yNave()-1));
        }
        for(it= L.begin(); it!=L.end();it++){
            (*it) -> mover();
            if((*it)->fuera()){
                gotoxy((*it)->xLaser(),(*it)->yLaser());printf(" ");
                delete(*it);
                it = L.erase(it);
            }
        }
        for(itA = A.begin(); itA != A.end(); itA++){
            (*itA) -> mover();
            (*itA) -> choque(N);
        }
        for(itA = A.begin(); itA!= A.end(); itA++){
            for(it = L.begin(); it!= L.end(); it++){
                if((*itA)->xAst()==(*it)->xLaser()&&((*itA)->yAst()+1 ==(*it)->yLaser()|| (*itA)->yAst() ==(*it)->yLaser())){
                    gotoxy((*it)-> xLaser(),(*it)->yLaser()); printf(" ");
                    delete(*it);
                    it = L.erase(it);


                    A.push_back(new AST(rand()%73 +3,6));
                    gotoxy((*itA)-> xAst(),(*itA)->yAst()); printf(" ");
                    delete(*itA);
                    itA = A.erase(itA);
                    puntos +=5;

                }
            }
        }

        if(N.getVidas() <= 0){
          game_over = true;
        }
        N.mover();
        Sleep(50);
    }

    return 0;
}

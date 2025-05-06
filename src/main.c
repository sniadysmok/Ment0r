#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/chess_eng_util.h"
#include "../include/input.h"
#include "../include/vector.h"
#include "../include/graphic.h"
#include "../include/board.h"
#include "../include/eval.h"
int temp3[BRD_SIZE] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};
int main(int argc, char *argv[])
{
    init_tables();
    brd br;
    vector_pos v;
    io_manager mg,mg2;
    init_io(&mg);
    init_io(&mg2);
    init(&v);
    init_table_h();

    init_board(&br,temp3,temp3);
    int my,op;
   // init_board(&br,temp,temp2);
    int if_flipped = 0;
    char * fen = "8/8/8/8/8/8/8/8";
    my = WH;
    op = BL;
    if((argc > 1) && argv[1][0] == 'B'){
        if_flipped=1;
        my = BL;
        op = WH;
    }


    if(argc == 2 || argc == 1){
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        br.LB_castle = 1;
        br.LW_castle = 1;
        br.RB_castle = 1;
        br.RW_castle = 1;
    }
    if(argc >= 4){
        fen = argv[2];
        if(argv[3][0] == 'b'){
            br.mv = BL;
        }
        for(int i = 0; i < 4; i++){
            char cstl = argv[4][i];
            if(cstl == EOF){
                break;
            }
            switch(cstl){
                case 'K':
                    br.RW_castle=1;
                    break;
                case 'k':
                    br.RB_castle=1;
                    break;
                case 'Q':
                    br.LW_castle=1;
                    break;
                case 'q':
                    br.LB_castle=1;
                    break;
            }
        }
    }


    read_fen_to_board(&br,fen);
    pos_int p = {10,10};
    push_back((pos_int){10,10},&v);
    push_back((pos_int){11,12},&v);
    push_back((pos_int){11,11},&v);
    printf("EWALUACJA : %d\n", eval(&br));
    for(int i = 0; i < v.size; ++i){
        //printf("x: %d y: %d\n", v.dat[i].x, v.dat[i].y);
    }
    clear(&v);

    vector_pos end_g, end_g_to;
    int want_to_mate = 0;
    int END = 0;
    mv_info mi;
    int m_dp ;

    printf("podaj glebie silnika (zalecane 6) :\n");
    scanf("%d",&m_dp);
    printf("==================================\n\n");
  //  sleep(1);
    InitWindow(512, 512, "Ment0r");


    Image board = LoadImage("res/Chessboard480.png");
    Image pic = LoadImage("res/Chess_Pieces_Sprite.png");
    Rectangle src = {0,0,256,256};
    Texture2D tex = LoadTextureFromImage(board);
    Texture2D sprite = LoadTextureFromImage(pic);
    UnloadImage(board);
    UnloadImage(pic);
    SetTargetFPS(60);
    //------------
    int MOVES = 0;
    while (!WindowShouldClose())
    {
        fflush(stdout);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 mpos = GetMousePosition();
        if(br.mv == op && !END){
            pos_int bf, bt;
            mv_info min;


            printf("",find_best_move(&br,&bf,&bt,m_dp,m_dp,-(1e8+6),(1e8+6)));
            printf(" NAJLEPSZY : %d %d -> %d %d \n",bf.x,bf.y,bt.x,bt.y);
            make_move(bf,bt,&br,&min);//MOVES+=2;

        }
        if(!END){
            mouseio(if_flipped,&mg,&mg2,temp3,&br,&want_to_mate);
         draw_board(tex,sprite,br,temp3,if_flipped);

        }

        //printf("en pesant (x,y) = ( %d , %d )\n",br.en_passant.x, br.en_passant.y);
        if(want_to_mate||END){

            if(END){
                //init_board(&br,temp3,temp3);
                if(br.checks > 0){
                    if(br.mv == WH){
                        DrawText("Mat - Zwyciestwo Czarnych", 20, 20, 30, RED);
                    }else{
                        DrawText("Mat - Zwyciestwo Bialych", 20, 20, 30, RED);
                    }
                }else{
                    DrawText("Pat - Remis", 20, 20, 30, RED);
                }
                END = 1;

                //sleep(10);
               // exit(0);
            }
            want_to_mate = 0;

        }
      //  printf("\nEWALUACJA : %d\n", eval(&br));
        EndDrawing();
        init(&end_g); init(&end_g_to);
        generate_moves(&br,&end_g,&end_g_to);
        if(end_g.size == 0){
            printf("==============================================================\n");
            printf("SZACH MAT \n");
            printf("==============================================================\n");
           // pause();
            //    END = 1;
        }
        clear(&end_g); clear(&end_g_to);
    }

    CloseWindow();

    return 0;
}

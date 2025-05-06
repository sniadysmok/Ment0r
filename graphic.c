#include "../include/graphic.h"
#include <stdio.h>
void draw_board(Texture2D brd_base, Texture2D spri, brd br,int clr_mask[64], int flip){
    DrawTexture(brd_base, 0,0,WHITE);
    Color cl_ar[] = {BLANK,RED,SKYBLUE};
    for(int i = 0; i < BRD_SIZE; ++i){
        int in = i;
        if(flip){
            in = BRD_SIZE - i - 1;
        }
        int x = 0; int y = 0;
        x += (5 - br.pieces[in])*BRD_SIZE;
        y += (br.piece_colors[in])*BRD_SIZE;
        Vector2 position = {(i - (i/8 * 8))*BRD_SIZE, (i/8)*BRD_SIZE};
        DrawRectangle(position.x, position.y, BRD_SIZE, BRD_SIZE, cl_ar[clr_mask[in]]);
        if(br.pieces[in] == EMPTY){
            continue;
        }
        Rectangle src = {x,y,BRD_SIZE,BRD_SIZE};

        DrawTextureRec(spri,src,position,WHITE);

    }
   // //printf("\n");
}

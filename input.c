#include "../include/input.h"
#include "stdlib.h"
void init_io(io_manager *mg){
    mg->selct.x = -1;
    mg->selct.y = -1;
}
void slct(io_manager *mg, int flip){
    if(mg->selct.x == -1){
    int X = (int)(GetMousePosition().x) / BRD_SIZE;
    int Y = (int)(GetMousePosition().y) / BRD_SIZE;
    mg->selct.x = X;
    mg->selct.y = Y;
    if(flip){
    mg->selct.x = 7-X;
    mg->selct.y = 7-Y;
    }
   // //printf("\nwybrano X: %d, Y: %d\n", 7-X, 7-Y);
    }
}
void dslct(io_manager *mg){
    mg->selct.x = -1;
    mg->selct.y = -1;
}
void paint(int x, int y, int clr_mask[BRD_SIZE], int val){
  //  //printf("\n%d, %d\n",x,y);
    clr_mask[x + y*8] = val;
}
void reset_paint(int clr[BRD_SIZE]){
    for(int i = 0; i < BRD_SIZE; ++i){
        clr[i]=0;
    }
}
void move(brd *br, pos_int from, pos_int to, int col_mask[BRD_SIZE]){
    int i1,i2;
    if(from.x < 0 || to.x<0){return;}
    i1=pos_to_index(from);
    i2=pos_to_index(to);
   // //printf("ruszek z (%d,%d) do (%d,%d)",from.x,from.y,to.x,to.y);
    if(br->pieces[i1] == EMPTY){return ;}
    if(i1==i2){return ;}
    vector_pos Mypos, Mypos_to;
    init(&Mypos); init(&Mypos_to);
    generate_moves(br,&Mypos,&Mypos_to);
   //
       // //printf("NZ\n");
       // clear(&Mypos);
       // clear(&Mypos_to);
      //  return;
   // }

    if(contains(from,to,Mypos,Mypos_to)){
        mv_info m1;
        make_move(from,to,br,&m1);
    }

    clear(&Mypos);
    clear(&Mypos_to);
}
void mouseio(int is_flipped, io_manager *mf, io_manager *mt, int col_mask[64], brd *br, int *m_c){
    if(mf->selct.x!=-1&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
        slct(mt,is_flipped);
        move(br,mf->selct,mt->selct,col_mask);
        (*m_c) = 1;
        ////printf("\nruch z (%d, %d) do (%d,%d)\n",mg.selct.x,mg.selct.y,mg2.selct.x,mg2.selct.y);
        reset_paint(col_mask);
        dslct(mf);
        dslct(mt);

    }else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        dslct(mf);
        slct(mf,is_flipped);
        vector_pos Mypos, Mypos_to;
        init(&Mypos); init(&Mypos_to);
        generate_moves(br,&Mypos,&Mypos_to);
        paint_possible_moves(mf->selct,Mypos,Mypos_to,col_mask);
        clear(&Mypos);
        clear(&Mypos_to);
        if(br->pieces[mf->selct.x + mf->selct.y*8] == EMPTY){
            dslct(mf);
        }else{
            paint(mf->selct.x, mf->selct.y, col_mask,1);
        }
    }else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
        reset_paint(col_mask);
        // dslct(&mg);
        dslct(mf);
        dslct(mt);
    }
}

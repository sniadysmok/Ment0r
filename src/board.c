#include "../include/board.h"
#include "../include/eval.h"
#include <stdio.h>
#include <stdlib.h>
static const pos_int masks[] = {
    {-1,-1},{1,1},{-1,1},{1,-1},{0,1},{0,-1},{1,0},{-1,0},
    {1,2},{-1,2},{-1,-2},{1,-2},{2,1},{-2,1},{2,-1},{-2,-1}
};
static const pos_int p_mask_c[] = {
    {1,-1}, {-1,-1},{1,1}, {-1,1}
};
// ===================================
// ==          INIT / SETUP         ==
// ===================================
// -> init_board
// -> read_fen_to_board
void init_board(brd * br, int pic[BRD_SIZE], int col[BRD_SIZE]){
    // 0 - pawn | 1 - rook | 2 - knight | 3 - bishop | 4 - queen | 5 - king
    // br->pieces = malloc(sizeof(int) * 4 * 64)
    br->en_passant = (pos_int){-1,-1};
    for(int i = 0; i < BRD_SIZE; ++i){
        br->pieces[i] = pic[i];
        br->piece_colors[i] = col[i];
    }
    // br->LB_castle=1;
    // br->LW_castle=1;
    // br->RB_castle=1;
    // br->RW_castle=1;
}
pos_int add_pos(pos_int p1, pos_int p2){
    return (pos_int){p1.x+p2.x,p1.y+p2.y};
}

void read_fen_to_board(brd *br, const char *c){
    for(int i = 0; i < BRD_SIZE; i++){
        br->pieces[i] = EMPTY;
        br->piece_colors[i] = EM;
    }
    /*

     */
    int sl = 0;
    for(int i = 0; i < BRD_SIZE; i++){
        //  //printf("sl : %d, i : %d c : %c \n ",sl,i,c[sl]);
        if(c[sl] == '/'){
            i--;
            sl++;
        }else if(c[sl] >= '0' && c[sl] <= '9'){
            i += (int)(c[sl] - '0');
            i--;
            sl++;
        }else{
            char cur_c = c[sl];
            if(c[sl] >= 'A' && c[sl] <= 'Z'){
                cur_c += 32;
            }
            switch(cur_c){
            case 'p':
                br->pieces[i] = PAWN;
                break;
            case 'n':
                br->pieces[i] = KNIGHT;
                break;
            case 'b':
                br->pieces[i] = BISHOP;
                break;
            case 'r':
                br->pieces[i] = ROOK;
                break;
            case 'q':
                br->pieces[i] = QUEEN;
                break;
            case 'k':
                br->pieces[i] = KING;
                break;
            }
            if(c[sl] >= 'A' && c[sl] <= 'Z'){
                br->piece_colors[i] = WH;
            }else{
                br->piece_colors[i] = BL;
            }
            sl++;
        }


    }
}
// ===================================
// ==         BOARD HELPERS         ==
// ===================================
// -> delete_piece
// -> summon_piece
// -> make_move
// -> undo_move
void delete_piece(pos_int pos, brd *br){
    br->pieces[pos_to_index(pos)] = EMPTY;
    br->piece_colors[pos_to_index(pos)]=EM;
}
void summon_piece(pos_int pos, brd *br, int piece,int color){
    br->pieces[pos_to_index(pos)] = piece;
    br->piece_colors[pos_to_index(pos)] = color;
}
void make_move(pos_int from, pos_int to, brd *br, mv_info *info){
    int i1 = pos_to_index(from);
    int i2 = pos_to_index(to);

    info->sec_from = to;
    info->sec_to = to;
    info->sec_type = br->pieces[i2];
    info->sec_color = br->piece_colors[i2];
    info->prom = 0;
    if(br->pieces[i1] == KING && abs(i2-i1) == 2){
        if(i2 - i1 < 0){
            info->sec_color = br->piece_colors[i1-4];
            info->sec_type = br->pieces[i1-4];
            info->sec_to = index_to_pos(i1-1);
            info->sec_from = index_to_pos(i1-4);
            br->pieces[i1-1] = br->pieces[i1-4];
            br->piece_colors[i1-1] = br->piece_colors[i1-4];
            br->pieces[i1-4] = EMPTY;
            br->piece_colors[i1-4] = EM;
        }else{
            info->sec_color = br->piece_colors[i1+3];
            info->sec_type = br->pieces[i1+3];
            info->sec_to = index_to_pos(i1+1);
            info->sec_from = index_to_pos(i1+3);
            br->pieces[i1+1] = br->pieces[i1+3];
            br->piece_colors[i1+1] = br->piece_colors[i1+3];
            br->pieces[i1+3] = EMPTY;
            br->piece_colors[i1+3] = EM;
        }
    }
    info->LB_castle = br->LB_castle;
    info->LW_castle = br->LW_castle;
    info->RB_castle = br->RB_castle;
    info->RW_castle = br->RW_castle;
    if(br->pieces[i1] == KING){
        if(br->mv == BL){
            br->RB_castle = 0;
            br->LB_castle = 0;
        }else{
            br->RW_castle = 0;
            br->LW_castle = 0;
        }
    }
    if(i1 == 0){
        br->LB_castle=0;
    }
    if(i1 == 7){

        br->RB_castle=0;
    }
    if(i1 == 56){

        br->LW_castle=0;
    }
    if(i1 == 63){

        br->RW_castle=0;
    }
    if(i2 == 0){

        br->LB_castle=0;
    }
    if(i2 == 7){

        br->RB_castle=0;
    }
    if(i2 == 56){

        br->LW_castle=0;
    }
    if(i2 == 63){

        br->RW_castle=0;
    }
    int passant = 0;
    //printf("EN PESANET ( %d , %d ) \n",index_to_pos(i1).y,index_to_pos(i2).y);
    info->en_pas = br->en_passant;
    if(i2 == pos_to_index(br->en_passant) && br->pieces[i1] == PAWN){
        if(br->mv == WH){
            info->sec_color = BL;
            info->sec_type = PAWN;
            info->sec_from = index_to_pos(i2+8);
            info->sec_to = index_to_pos(i2+8);
            br->piece_colors[i2 + 8] = EM;
            br->pieces[i2 + 8] = EMPTY;
            //i2 += 8;
        }else{
           // i2 -= 8;
            info->sec_color = WH;
            info->sec_type = PAWN;
            info->sec_from = index_to_pos(i2-8);
            info->sec_to = index_to_pos(i2-8);
            br->piece_colors[i2 - 8] = EM;
            br->pieces[i2 - 8] = EMPTY;
        }
        passant = 1;
    }
    br->en_passant = (pos_int){-1,-1};

    if(abs(index_to_pos(i1).y - index_to_pos(i2).y) == 2 && br->pieces[i1] == PAWN)
    {
        //printf("ENENENENE\n");
        br->en_passant.x = index_to_pos(i2).x;
        if(br->mv == WH){
            br->en_passant.y = index_to_pos(i2).y+1;
        }else{
            br->en_passant.y = index_to_pos(i2).y-1;
        }
    }
    info->from = from;
    info->to = to;
    br->pieces[i2] = br->pieces[i1];
    br->piece_colors[i2] = br->piece_colors[i1];
    br->pieces[i1] = EMPTY;
    br->piece_colors[i1] = EM;
    info->mv = br->mv;
    if(br->mv == WH){
        br->mv = BL;
    }else{
        br->mv = WH;
    }
    for(int i = 0; i < BRD_SIZE; i++){
        if(i == 8){
            i = BRD_SIZE - 8;
        }
        if(br->pieces[i] == PAWN){br->pieces[i]=QUEEN; info->prom=1;}
    }
    br->mvs++;
    br->history[br->mvs] = (zbr_hash(br) % (int)(1e6 + 3));
}
void undo_move(mv_info info, brd *br){
    int i1 = pos_to_index(info.from);
    int i2 = pos_to_index(info.to);
    br->history[br->mvs] = 0;
    br->mvs--;
    br->LB_castle = info.LB_castle;
    br->LW_castle = info.LW_castle;
    br->RB_castle = info.RB_castle;
    br->RW_castle = info.RW_castle;
    br->en_passant = info.en_pas;
    if(info.prom == 1){
        br->pieces[i2] = PAWN;
    }
    if(i1 < 0 && i2 < 0){
        return;
    }
    br->pieces[i1] = br->pieces[i2];
    br->piece_colors[i1] = br->piece_colors[i2];
    br->pieces[i2] = EMPTY;
    br->piece_colors[i2] = EM;
    int i1_s = pos_to_index(info.sec_from);
    int i2_s = pos_to_index(info.sec_to);
    if(is_in_bounds(info.sec_from) && is_in_bounds(info.sec_to) && info.sec_type != EMPTY){
        br->pieces[i2_s] = EMPTY;
        br->piece_colors[i2_s] = EM;
    br->pieces[i1_s] = info.sec_type;
    br->piece_colors[i1_s] = info.sec_color;

    }
    br->mv = info.mv;
}
// ===================================
// ==    CHECKS, PINS & THREATS     ==
// ===================================
// -> is_tile_under_attack
// -> detect_pins_and_pinners
// -> detect_checks
// -> checks_and_pins

int is_tile_under_attack(int atk_color,int my_color, brd *br, pos_int tile){



    for(int k = 0; k < 16; k++){
        int dist = 0;
        for(pos_int p = add_pos(tile,masks[k]); is_in_bounds(p); p = add_pos(p,masks[k])){
            // //printf("dist : %d \n", dist);
            dist++;

            if(br->piece_colors[pos_to_index(p)] == my_color && br->pieces[pos_to_index(p)] == KING){continue;}
            if(br->piece_colors[pos_to_index(p)] == my_color){break;}
            if(br->piece_colors[pos_to_index(p)] == atk_color){
                if(k >= 0 && k < 4){
                    if(br->pieces[pos_to_index(p)] == QUEEN || br->pieces[pos_to_index(p)] == BISHOP){
                        return 1;
                    }else if(br->pieces[pos_to_index(p)] == KING && dist == 1){
                        return 1;
                    }else if(br->pieces[pos_to_index(p)] != EMPTY){
                        break;
                    }


                }else if(k >= 4 && k < 8){

                    if(br->pieces[pos_to_index(p)] == QUEEN || br->pieces[pos_to_index(p)] == ROOK){
                        if(tile.x == 0 && tile.y == 5){
                            //printf("UWAGA : ( %d , %d )\n", p.x,p.y);
                        }
                        return 1;
                    }else if(br->pieces[pos_to_index(p)] == KING && dist == 1){
                        if(tile.x == 0 && tile.y == 5){
                            //printf("UWAGA : ( %d , %d )\n", p.x,p.y);
                        }
                        return 1;
                    }else if(br->pieces[pos_to_index(p)] != EMPTY){
                        break;
                    }


                }else{
                    if(br->pieces[pos_to_index(p)] == KNIGHT){
                        return 1;
                    }
                }
            }

            if(k >= 8){break;}
        }
        int stk_p = 0;
        if(my_color == BL){
            stk_p += 2;
        }
        for(int k = stk_p; k < stk_p + 2; k++){
            for(pos_int p = add_pos(tile,p_mask_c[k]); is_in_bounds(p); p = add_pos(p,p_mask_c[k])){
                // //printf("dist : %d \n", dist);

                if(br->piece_colors[pos_to_index(p)] == my_color){break;}
                if(br->piece_colors[pos_to_index(p)] == atk_color){
                    if(br->pieces[pos_to_index(p)] == PAWN){
                        return 1;
                    }
                }
                break;
            }
        }
    }

    return 0;
        //magic :)
}

void detect_pins_and_pinners(brd *br, vector_pos * from, vector_pos *to,int my, int op, pos_int * where_1_check, int * where_1_k, int *checks, int my_king, int pins[BRD_SIZE], int pinners[BRD_SIZE], int k){



        for(pos_int p = add_pos(index_to_pos(my_king),masks[k]); is_in_bounds(p); p = add_pos(p,masks[k])){
            if(br->piece_colors[pos_to_index(p)] == my){
                for(pos_int pj = add_pos(p,masks[k]); is_in_bounds(pj); pj = add_pos(pj,masks[k])){
                    if(br->piece_colors[pos_to_index(pj)] == my){break;}
                    if(br->piece_colors[pos_to_index(pj)] == op){
                        //printf("pj %d %d \n",pj.x,pj.y);
                        if(k >= 0 && k < 4 && (br->pieces[pos_to_index(pj)] == QUEEN || br->pieces[pos_to_index(pj)] == BISHOP)){
                            pins[pos_to_index(p)] = 1;
                            pinners[pos_to_index(p)] = pos_to_index(pj);break;
                        }else if(k >= 4 && k < 8 && (br->pieces[pos_to_index(pj)] == QUEEN || br->pieces[pos_to_index(pj)] == ROOK)){
                            pins[pos_to_index(p)] = 1;
                            pinners[pos_to_index(p)] = pos_to_index(pj);break;
                        }
                        break;
                    }
                }//syf
                break;
            }else if(br->pieces[pos_to_index(p)] != EMPTY && br->piece_colors[pos_to_index(p)] == op){
                break;
            }
        }

}
void detect_checks(brd *br, vector_pos * from, vector_pos *to,int my, int op, pos_int * where_1_check, int * where_1_k, int *checks, int my_king, int pins[BRD_SIZE], int pinners[BRD_SIZE], int k){

    for(pos_int p = add_pos(index_to_pos(my_king),masks[k]); is_in_bounds(p); p = add_pos(p,masks[k])){
        if(br->piece_colors[pos_to_index(p)] == my){break;}
        if(br->piece_colors[pos_to_index(p)] == op){
            if(k >= 0 && k < 4){
                if(br->pieces[pos_to_index(p)] == QUEEN || br->pieces[pos_to_index(p)] == BISHOP){
                    (*checks)++; (*where_1_k) = k; (*where_1_check) = p; break;
                }else if(br->pieces[pos_to_index(p)] != EMPTY){
                    break;
                }
            }else if(k >= 4 && k < 8){
                if(br->pieces[pos_to_index(p)] == QUEEN || br->pieces[pos_to_index(p)] == ROOK){
                    (*checks)++; (*where_1_k) = k; (*where_1_check) = p; break;
                }else if(br->pieces[pos_to_index(p)] != EMPTY){
                    break;
                }
            }else{
                if(br->pieces[pos_to_index(p)] == KNIGHT){
                    (*checks)++; (*where_1_k) = k; (*where_1_check) = p; break;
                }
            }
            break;
        }
        if(k >= 8){break;}
    }
}
void checks_and_pins(brd *br, vector_pos * from, vector_pos *to,int my, int op, pos_int * where_1_check, int * where_1_k, int *checks, int my_king, int pins[BRD_SIZE], int pinners[BRD_SIZE]){




    for(int k = 0; k < 16; k++){
        detect_pins_and_pinners(br,from,to,my,op,where_1_check,where_1_k,checks,my_king,pins,pinners,k);
        detect_checks(br,from,to,my,op,where_1_check,where_1_k,checks,my_king,pins,pinners,k);
    }

    int stk_p = 0;
    if(my == BL){
        stk_p += 2;
    }
    for(int k = stk_p; k < stk_p + 2; k++){
        for(pos_int p = add_pos(index_to_pos(my_king),p_mask_c[k]); p.x > -1 && p.y > -1 && p.x < 8 && p.y < 8; p = add_pos(p,p_mask_c[k])){
            // //printf("dist : %d \n", dist);

            if(br->piece_colors[pos_to_index(p)] == my){break;}
            if(br->piece_colors[pos_to_index(p)] == op){
                if(br->pieces[pos_to_index(p)] == PAWN){
                    (*where_1_check) = p;
                    for(int i = 0 ; i < 16; i++){
                        if(masks[i].x == p_mask_c[k].x && masks[i].y == p_mask_c[k].y){
                            (*where_1_k) = i;
                        }
                    }
                    (*checks)++;
                }
            }
            break;
        }
    }
}
// ===================================
// ==        SPECIAL MOVES          ==
// ===================================
// -> try_en_passant
void try_en_passant(brd *br, vector_pos *from, vector_pos *to, pos_int p, int cur_index, int my, int op, int my_king, pos_int cur_pos,pos_int cur_dir, int pins[BRD_SIZE], int pinners[BRD_SIZE]){
    if(br->en_passant.x == p.x && br->en_passant.y == p.y){
        int pinok = 0;

        for(pos_int pp = add_pos(cur_pos,(pos_int){-cur_dir.x,-cur_dir.y}); is_in_bounds(pp); pp = add_pos(pp,(pos_int){-cur_dir.x,-cur_dir.y})){
           //  printf(" %d %d \n", pp.x, pp.y);
            if(pinners[pos_to_index(cur_pos)] == pos_to_index(pp)){
                pinok=1;
                break;
            }
            if(br->pieces[pos_to_index(pp)] != EMPTY){
                break;
            }
        }

        if(!pins[cur_index] || pinok){

            br->pieces[pos_to_index(br->en_passant)] = PAWN;
            br->piece_colors[pos_to_index(br->en_passant)] = my;
            br->pieces[cur_index] = EMPTY; br->piece_colors[cur_index] = EM;
            if(my == WH){
                delete_piece(index_to_pos(pos_to_index(br->en_passant) + 8),br);
            }else{
                delete_piece(index_to_pos(pos_to_index(br->en_passant) - 8),br);
            }
            //printf("ohoho\n");
            if(!is_tile_under_attack(op,my,br,index_to_pos(my_king))){
                push_back(cur_pos,from);
                push_back(p,to);
            }
            delete_piece(br->en_passant, br);
            br->pieces[cur_index] = PAWN; br->piece_colors[cur_index] = my;
            if(my == WH){
                summon_piece(index_to_pos(pos_to_index(br->en_passant) + 8),br,PAWN,op);

            }else{
                summon_piece(index_to_pos(pos_to_index(br->en_passant) - 8),br,PAWN,op);
            }
        }
    }
}
// ===================================
// ==        PIECE MOVEMENT         ==
// ===================================
// -> pawn_moves
// -> king_moves
// -> castling
void pawn_moves(brd *br, vector_pos *from, vector_pos *to, int checks, int my, int op,int my_king, int pins[BRD_SIZE], int pinners[BRD_SIZE]){
    pos_int p_mask[] = {
        {0,-1}, {0,-2}, {1,-1}, {-1,-1},{0,1}, {0,2}, {1,1}, {-1,1}
    };
    // brd, to ,from ,
    for(int cur_index = 0; cur_index < BRD_SIZE; cur_index++){
        if(checks>1){break;}
        pos_int cur_pos = {cur_index%8, cur_index/8};
        if(br->pieces[cur_index] == EMPTY || br->piece_colors[cur_index] == op){
            continue;
        }
        if(br->pieces[cur_index] == PAWN){
            int st = 0;
            if(my == BL){
                st+=4;
            }
            for(int k = st; k < st+4; k++){
                pos_int cur_dir = p_mask[k];
                for(pos_int p = add_pos(cur_pos,cur_dir); is_in_bounds(p); p = add_pos(p,cur_dir)){
                    if(k%4 == 0){
                        if(br->pieces[pos_to_index(p)] == EMPTY){
                            if(!pins[cur_index]){
                                push_back(cur_pos,from);
                                push_back(p,to);
                            }else{
                               // printf("spr\n");
                                for(pos_int pp = add_pos(p,cur_dir); is_in_bounds(pp); pp = add_pos(pp,cur_dir)){
                                   // printf(" %d %d \n", pp.x, pp.y);
                                    if(pinners[pos_to_index(cur_pos)] == pos_to_index(pp)){

                                        push_back(cur_pos,from);
                                        push_back(p,to);
                                        break;
                                    }
                                    if(br->pieces[pos_to_index(pp)] != EMPTY){
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(k%4 == 1 && (cur_pos.y == 1 || cur_pos.y == 6)){
                        if(br->pieces[pos_to_index(p)] == EMPTY && br->pieces[pos_to_index(add_pos(cur_pos,p_mask[k-1]))] == EMPTY){
                            if(!pins[cur_index] ){
                                push_back(cur_pos,from);
                                push_back(p,to);
                            }else{
                                for(pos_int pp = add_pos(p,p_mask[k-1]); is_in_bounds(pp); pp = add_pos(pp,p_mask[k-1])){
                                   // printf(" %d %d \n", pp.x, pp.y);
                                    if(pinners[pos_to_index(cur_pos)] == pos_to_index(pp)){

                                        push_back(cur_pos,from);
                                        push_back(p,to);
                                        break;
                                    }
                                    if(br->pieces[pos_to_index(pp)] != EMPTY){
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(k%4 > 1){
                        if(br->piece_colors[pos_to_index(p)] == op){
                            if(!pins[cur_index] || pinners[cur_index] == pos_to_index(p)){
                                push_back(cur_pos,from);
                                push_back(p,to);
                            }else{

                            }
                        }
                        try_en_passant(br,from,to,p,cur_index,my,op,my_king,cur_pos,cur_dir,pins,pinners);
                    }
                    break;
                }
            }

        }
    }
}
void king_moves(brd *br, vector_pos * from, vector_pos *to,int op, int my, int my_king){

    for(int k = 0; k < 8; k++){
        pos_int til = add_pos(masks[k],index_to_pos(my_king));
        if(is_tile_under_attack(op,my,br,til) == 0 && is_in_bounds(til) && br->piece_colors[pos_to_index(til)] != my){
            //printf("tils : ( %d , %d )\n", til.x,til.y);
            push_back(index_to_pos(my_king),from);
            push_back(til,to);
        }
    }
}
void castling(brd *br,vector_pos *from, vector_pos *to, int checks, int op, int my, int my_king){
    if(my_king != 60 && my == WH){
        return;
    }
    if(my_king != 4 && my == BL){
        return;
    }
    if(checks == 0){
        if(my == WH){
            int allow_r = 1;
            int allow_l = 1;
            for(int i = my_king+1; i < BRD_SIZE-1; i++){
                if(br->pieces[i] != EMPTY){
                    //printf("skibi : %d \n", i);
                    allow_r = 0;
                }
            }
            for(int i = my_king-1; i >= BRD_SIZE-7; i--){
                if(br->pieces[i] != EMPTY){
                    allow_l = 0;
                }
            }
            if(allow_r){
                if(br->RW_castle == 1 && is_tile_under_attack(op,my,br,add_pos((pos_int){1,0},index_to_pos(my_king))) == 0 && is_tile_under_attack(op,my,br,add_pos((pos_int){2,0},index_to_pos(my_king))) == 0){
                    push_back(index_to_pos(my_king),from);
                    push_back(add_pos((pos_int){2,0},index_to_pos(my_king)),to);
                }
            }
            if(allow_l){
             if(br->LW_castle == 1 && is_tile_under_attack(op,my,br,add_pos((pos_int){-1,0},index_to_pos(my_king))) == 0 && is_tile_under_attack(op,my,br,add_pos((pos_int){-2,0},index_to_pos(my_king))) == 0){
                    push_back(index_to_pos(my_king),from);
                    push_back(add_pos((pos_int){-2,0},index_to_pos(my_king)),to);
               }
            }
        }else{
            int allow_r = 1;
            int allow_l = 1;
            for(int i = my_king+1; i < 7; i++){
                if(br->pieces[i] != EMPTY){
                    allow_r = 0;
                }
            }
            for(int i = my_king-1; i > 0; i--){
                if(br->pieces[i] != EMPTY){
                    allow_l = 0;
                }
            }
            if(allow_r){
                if(br->RB_castle && (is_tile_under_attack(op,my,br,add_pos((pos_int){1,0},index_to_pos(my_king))) == 0 && is_tile_under_attack(op,my,br,add_pos((pos_int){2,0},index_to_pos(my_king))) == 0)){
                    push_back(index_to_pos(my_king),from);
                    push_back(add_pos((pos_int){2,0},index_to_pos(my_king)),to);
                }
            }
            if(allow_l){
                if(br->LB_castle && (is_tile_under_attack(op,my,br,add_pos((pos_int){-1,0},index_to_pos(my_king))) == 0 && is_tile_under_attack(op,my,br,add_pos((pos_int){-2,0},index_to_pos(my_king))) == 0)){
                     push_back(index_to_pos(my_king),from);
                     push_back(add_pos((pos_int){-2,0},index_to_pos(my_king)),to);
                }
            }
        }
    }
}
// ===================================
// ==        MOVE GENERATION        ==
// ===================================
// -> generate_moves
// -> get_sliding
void get_sliding(brd *br, vector_pos *from, vector_pos *to, int my, int op, int checks, int pins[BRD_SIZE], int pinners[BRD_SIZE]){
    for(int cur_index = 0; cur_index < BRD_SIZE; cur_index++){
        if(checks > 1){break;}
        //magic :)
        if(br->pieces[cur_index] == EMPTY || br->piece_colors[cur_index] == op){
            continue;
        }

        pos_int cur_pos = {cur_index%8, cur_index/8};
        if(br->pieces[cur_index] == BISHOP || br -> pieces[cur_index] == ROOK || br->pieces[cur_index] == QUEEN || br->pieces[cur_index] == KNIGHT){
            int sl_index = 0;
            int sl_lim = 8;
            if(br->pieces[cur_index] == BISHOP){
                sl_lim = 4;
            }else if(br->pieces[cur_index] == ROOK){
                sl_index = 4;
            }else if(br->pieces[cur_index] == KNIGHT){
                sl_index = 8;
                sl_lim = 16;
            }
            for(int k = sl_index; k < sl_lim; k++){
                pos_int cur_dir = masks[k];
                for(pos_int p = add_pos(cur_pos,cur_dir);  is_in_bounds(p); p = add_pos(p,cur_dir)){
                    // //printf("( %d , %d )\n",p.x,p.y);
                    if(br->piece_colors[pos_to_index(p)] == my){
                        break;
                    }
                    //  //
                    if(!pins[cur_index] || pinners[cur_index] == pos_to_index(p)){
                        push_back(cur_pos,from);
                        push_back(p,to);
                        if(pinners[cur_index] == pos_to_index(p) && pins[cur_index] == 1){
                            // printf("( %d , %d ) | ( %d , %d )\n",cur_pos.x,cur_pos.y,add_pos(cur_pos,cur_dir).x,add_pos(cur_pos,cur_dir).y);
                            for(pos_int pp = add_pos(cur_pos,cur_dir);  is_in_bounds(pp) && !(pp.x == p.x && pp.y == p.y); pp = add_pos(pp,cur_dir)){
                               // printf("( %d , %d ) | ( %d , %d )\n",cur_pos.x,cur_pos.y,pp.x,pp.y);
                               // if( pp.x != p.x && pp.y != p.y){
                                push_back(cur_pos,from);
                                push_back(pp,to);
                               // }
                            }
                        }
                    }
                    if(br->piece_colors[pos_to_index(p)] == op){
                        break;
                    }
                    if(br->pieces[cur_index] == KNIGHT){break;}
                }
                //    //printf("---------------\n");
            }
            //   //printf("---------------\n");
        }
    }
}
void manage_1_check(brd *br, vector_pos * from, vector_pos *to, int checks, int my_king, int where_1_k, pos_int where_1_check){
    if(checks == 1){
        vector_pos can_move_1,from_1, to_1;
        init(&can_move_1); init(&from_1); init(&to_1);
        for(pos_int p = add_pos(index_to_pos(my_king),masks[where_1_k]); is_in_bounds(p); p = add_pos(p,masks[where_1_k])){
            if(pos_to_index(p) != pos_to_index(where_1_check)){
                push_back(p,&can_move_1);

            }else{push_back(p,&can_move_1);break;}
            if(where_1_k > 7){break;}
        }
        for(int i = 0; i < from->size; ++i){
            for(int j = 0; j < can_move_1.size; ++j){
                if((pos_to_index(from->dat[i]) != my_king && pos_to_index(to->dat[i]) == pos_to_index(can_move_1.dat[j])) || (pos_to_index(from->dat[i]) == my_king)){
                    push_back(from->dat[i],&from_1);
                    push_back(to->dat[i],&to_1);
                    // //printf("skibidi %d %d\n", to->dat[i].x,to->dat[i].y);
                    break;
                }
            }
        }
        br->checks = checks;
        clear(from); clear(to);
        from->size = from_1.size; from->rel_size = from_1.rel_size; from->dat = from_1.dat;
        to->size = to_1.size; to->rel_size = to_1.rel_size; to->dat = to_1.dat;
        clear(&can_move_1);
    }
}
void generate_moves(brd *br, vector_pos * from, vector_pos *to){
    //two vectors, from and to, the indexes match so its ok :->


    enum piece_c my, op;
    my = br->mv;
    if(my==WH){
        op = BL;
    }else{
        op = WH;
    }
    int my_king, op_king;
    for(int i = 0; i < BRD_SIZE; ++i){
        if(br->pieces[i] == KING && br->piece_colors[i] == my){
            my_king = i;
            break;
        }
    }
    //checks :
    int checks = 0;
    int pins[BRD_SIZE] = {0};
    int pinners[BRD_SIZE] = {0};
    int where_1_k; pos_int where_1_check;
    checks_and_pins(br,from,to,my,op,&where_1_check,&where_1_k,&checks,my_king,pins,pinners);
    get_sliding(br,from,to,my,op,checks,pins,pinners);
    castling(br,from,to,checks,op,my,my_king);
    pawn_moves(br,from,to,checks,my,op,my_king,pins,pinners);
    king_moves(br,from,to,op,my,my_king);
 //   manage_1_check(br,from,to,checks,my_king,where_1_k,where_1_check);
    br->checks = checks; // TODO: FIX
    if(checks == 1){
        vector_pos can_move_1,from_1, to_1;
        init(&can_move_1); init(&from_1); init(&to_1);
        for(pos_int p = add_pos(index_to_pos(my_king),masks[where_1_k]); p.x > -1 && p.y > -1 && p.x < 8 && p.y < 8; p = add_pos(p,masks[where_1_k])){
            if(pos_to_index(p) != pos_to_index(where_1_check)){
                push_back(p,&can_move_1);

            }else{push_back(p,&can_move_1);break;}
            if(where_1_k > 7){break;}
        }
        for(int i = 0; i < from->size; ++i){
            for(int j = 0; j < can_move_1.size; ++j){
                if((pos_to_index(from->dat[i]) != my_king && pos_to_index(to->dat[i]) == pos_to_index(can_move_1.dat[j])) || (pos_to_index(from->dat[i]) == my_king)){
                    push_back(from->dat[i],&from_1);
                    push_back(to->dat[i],&to_1);
                    // printf("skibidi %d %d\n", to->dat[i].x,to->dat[i].y);
                    break;
                }
            }
        }
        br->checks = checks;
        clear(from); clear(to);
      //  init(from); init(to);
        from->size = from_1.size; from->rel_size = from_1.rel_size; from->dat = from_1.dat;
        to->size = to_1.size; to->rel_size = to_1.rel_size; to->dat = to_1.dat;
        clear(&can_move_1);
    }
    //printf("SZACHY : %d \n",checks);
    //end



    //pawn captures:





    //printf("LEGALNE RUCHY W POZYCJI :   %d \n", from->size);

}
// ===================================
// ==      VISUAL / DEBUG TOOLS     ==
// ===================================
// -> paint_possible_moves
void paint_possible_moves(pos_int pos, const vector_pos from, const vector_pos to, int col[BRD_SIZE]){
    // //printf("\nP %d %d\n",pos.x,pos.y);
    for(int i = 0; i < from.size; i++){
        //printf("\nP %d %d\n",to.dat[i].x,to.dat[i].y);
        if(pos.x == from.dat[i].x && pos.y == from.dat[i].y){
            col[pos_to_index(to.dat[i])] = 2;
            //printf("adaa : %d \n", pos_to_index(to.dat[i]));
        }
    }
}


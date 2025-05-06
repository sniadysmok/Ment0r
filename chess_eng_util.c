#include "../include/chess_eng_util.h"
#include "../include/eval.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
unsigned long long int perft(brd *br, int dph, int m_dph){
    unsigned long long int mvs = 0; int nds = 0;

    vector_pos from,to;


    init(&from); init(&to);

    generate_moves(br,&from,&to);
    if(dph == 0){
        return 1;
    }
    if(dph == 1){
        return from.size;
    }

    for(int i = 0; i < from.size; i++){
        mv_info mi;
        unsigned long long int cr_mv;
        make_move(from.dat[i],to.dat[i],br,&mi);

        //printf("\n====================================\n");
       // sleep(1);
        cr_mv = perft(br,dph-1,m_dph);
       // hashes_val[hash(br)] = cr_mv;
        mvs += cr_mv;
        undo_move(mi,br);
        if(dph == m_dph){
          //  printf("%c%d%c%d: %lld\n",(char)(from.dat[i].x + 'a'), 8 - from.dat[i].y , (char)(to.dat[i].x + 'a'), 8 - to.dat[i].y,cr_mv);
          //

        }

    }
    clear(&from); clear(&to);
    return mvs;
}
int cnt_util[1000004];
int find_best_move(brd *br, pos_int *from, pos_int *to, int m_dph, int dph, int alpha, int beta){
    // if(hashes_val[hash(br)] > 0){
    //     return hashes_val[hash(br)];
    // }
    int draw = 0;
    for(int i = br->mvs; i>=0;i--){
        cnt_util[br->history[i]]++;
        if(cnt_util[br->history[i]] == 3){
            draw = 1;
        }
    }
    for(int i = br->mvs; i>=0;i--){
        cnt_util[br->history[i]]--;
    }
    if(draw){return 0;}
    vector_pos from_v,to_v;
    init(&from_v); init(&to_v);
    generate_moves(br,&from_v,&to_v);
    if(from_v.size == 0){
        clear(&from_v); clear(&to_v);
       // printf("dp dp %d\n", dph);
        if(br->checks > 0 && br->mv == BL){
            return -(1e6+7 + dph);
        }else if(br->checks > 0 && br->mv == WH){
            return -(1e6+7 + dph);
        }

        return 0;
    }
    if(dph == 0){

        clear(&from_v); clear(&to_v);

        return eval(br);
    }


    int max_ev = -(1e7+6);
    for(int i = 0; i < from_v.size; i++){
        mv_info mi;
        make_move(from_v.dat[i],to_v.dat[i],br,&mi);
        int ev = -find_best_move(br,to,from,m_dph,dph-1,-beta,-alpha);
        undo_move(mi,br);
        //printf("ev : %d\n",ev);
        if(ev > max_ev){
            max_ev = ev;
            if(ev > alpha){
                if(dph == m_dph){
                    if(is_in_bounds(from_v.dat[i]) && is_in_bounds(to_v.dat[i])){
                        (*from) = from_v.dat[i];
                        (*to) = to_v.dat[i];
                    }
                }
                alpha = ev;
            }
        }
        if(ev >= beta){
           // undo_move(mi,br);
            max_ev = ev;
            clear(&from_v); clear(&to_v);

            return max_ev;
        }

    }
    clear(&from_v); clear(&to_v);

    return max_ev;
}

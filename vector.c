#include "../include/vector.h"
#include <stdlib.h>
#include <stdio.h>
void init(  vector_pos * const vect){
    vect->rel_size = 1;
    vect->size = 0;
    //printf("\nmemory allocated!\n");
    vect->dat = malloc(sizeof(pos_int) * 1);
    if(vect->dat == NULL){
        printf("nie udało się zalokować pamięcci \n");


    }
}
void push_back(pos_int elem, vector_pos * const vect){
    if(vect == NULL){return;}
    if(vect->size == vect->rel_size){
        int new_cap = vect->rel_size*2;

      //  //printf("stinky\n");
        pos_int *new_ptr = realloc(vect->dat, new_cap * sizeof(pos_int));
        if(new_ptr != NULL){
            vect->dat = new_ptr;
            vect->rel_size=new_cap;
        }else{
            //printf("BŁAD TODO: dodaj jego obsługę\n");
            return;
        }
    }
    vect->dat[vect->size++] = elem;

}
void clear(vector_pos * const vect){
    if(vect->dat != NULL){
        //printf("\nmemory freed!\n");
        free(vect->dat); // fuck
        vect->dat = NULL;
    }
    vect->size = -1;
    vect->rel_size = -1;
}
int contains(pos_int elem, pos_int elem2, const vector_pos vect, const vector_pos vect2){
    if(vect.size != vect2.size){return 0;}
    if(!vect.dat || !vect2.dat){return 0;}
    for(int i = 0; i < vect.size; ++i){
        if(vect.dat[i].x == elem.x && vect.dat[i].y == elem.y && vect2.dat[i].x == elem2.x && vect2.dat[i].y == elem2.y){
            return 1;
        }
    }
    return 0;
}
// #include "../include/vector.h"
// #include <stdlib.h>
// #include <stdio.h>
// void init(  vector_pos * const vect){
//     vect->rel_size = 1;
//     vect->size = 0;
//   //  printf("\nmemory allocated!\n");
//     vect->dat = malloc(sizeof(pos_int) * 1);
// }
// void push_back(pos_int elem, vector_pos * const vect){
//     if(vect == NULL){return;}
//     if(vect->size++ == vect->rel_size){
//         vect->rel_size = vect->rel_size*2;
//         //  printf("stinky\n");
//         vect->dat = realloc(vect->dat, vect->rel_size * sizeof(pos_int));
//     }
//     vect->dat[vect->size-1] = elem;
// }
// void clear(vector_pos * const vect){
//     if(vect->dat != 0){
//        // printf("\nmemory freed!\n");
//         free(vect->dat); // fuck
//         vect->dat = 0;
//     }
//     vect->size = -1;
//     vect->rel_size = -1;
// }
// int contains(pos_int elem, pos_int elem2, const vector_pos vect, const vector_pos vect2){
//     for(int i = 0; i < vect.size; ++i){
//         if(vect.dat[i].x == elem.x && vect.dat[i].y == elem.y && vect2.dat[i].x == elem2.x && vect2.dat[i].y == elem2.y){
//             return 1;
//         }
//     }
//     return 0;
// }

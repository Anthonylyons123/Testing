#include "geesespotter_lib.h"
#include <iostream>

char * create_board(std::size_t x_dim, std::size_t y_dim){
    char *p_arrays{ new char[x_dim*y_dim]{} };
    return p_arrays;
}
void clean_board(char * board){
    delete[] board;
    board = nullptr;
    return;
}
void print_board(char * board, std::size_t x_dim, std::size_t y_dim){
    
    std::size_t k{0};

    for (std::size_t y{0}; y < y_dim; ++y){
        for(std::size_t x{0}; x < x_dim; ++x){
                
                if ((board[k]& marked_mask()) != 0){
                    std::cout<<"M";
                } else if ((board[k]& hidden_mask()) != 0){
                    std::cout<<"*";
                } else {
                    std::cout<<(board[k]&value_mask());
                }
                
            k += 1;
        }
        std::cout<<std::endl;
        
    }
    return;
}
void hide_board(char * board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t k{0}; k < (x_dim*y_dim); ++k){
        board[k] ^= 0x20;
    } 
    return;
}
int mark(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    
    //I think there is a bug in this, it should use bitwise

        if ( board[x_loc+(x_dim*y_loc)]  <= 9 ) {
            return 0x02;
        } else {
            board[x_loc+(x_dim*y_loc)] ^= 0x10;
            return 0;
        }
   
    return 0;
}

void compute_neighbours(char * board, std::size_t x_dim, std::size_t y_dim){
    int count{0x00};
    std::size_t i{0}; //board[i]
 
    for (std::size_t y{0}; y < y_dim; ++y){
        for(std::size_t x{0}; x < x_dim; ++x){
            //If statement for each space surrounding index value
            //chech if there is a 9 there and ++count

            if ( (board[i]&value_mask()) != 9) {
                //Left side
                if (i == (x_dim*y)){

                    for (int t{0}; t < 2; ++t){

                            if ( (i >= x_dim) && ((board[i - (x_dim - t)]&value_mask()) == 9) )  {  
                                count++;
                            }

                            if ( (i < (x_dim*(y_dim-1))) && ((board[i + (x_dim + t)]&value_mask()) == 9) ) {
                                count++;
                            }

                            if ( (board[i] != board[i+t]) && ((board[i+t] &value_mask() ) == 9)){
                                count++;  
                            }
                    }
                    board[i] = count;
                //Right side
                } else if ( i == ((x_dim*y) + (x_dim-1)) ){
                   
                    for (int t{0}; t < 2; ++t){
                       
                        if ( (i >= x_dim) && ((board[i - (x_dim + t)]&value_mask()) == 9) )  {  
                                count++;
                            }

                            if ( (i <= (x_dim*(y_dim-1))) && ((board[i + (x_dim - t)]&value_mask()) == 9) ) {
                                count++;
                            }

                            if ( (board[i] != board[i-t]) && ((board[i-t] &value_mask() ) == 9)){
                                count++;  
                            }

                    }
                    board[i] = count;

                //Center 
                } else {
                    
                    for (int t {-1}; t < 2; ++t){

                        // checking if it is in the bottom row 
                        if ( (i < (x_dim*(y_dim-1))) && ((board[i + (x_dim + t)] &value_mask()) == 9) ){
                            count++;
                        }

                        //Checking to see if it is in the top row
                        if ( (i > (x_dim-1)) && ((board[i - (x_dim + t)]&value_mask()) == 9) ) {
                            count++;
                        }

                        //left and right
                        if ( (board[i] != board[i+t]) && ((board[i+t] &value_mask() ) == 9)){
                            count++;
                        }

                    }
                    board[i] = count;
        
                }

                
            }
            i++;
            count = 0;
        }
    }         

    return;
}
bool is_game_won(char * board, std::size_t x_dim, std::size_t y_dim){

    for (std::size_t i{0}; i < (x_dim*y_dim); ++i){
        if ( ((board[i]& value_mask()) == 9) && ((board[i]& hidden_mask()) == 0) ){
            return false;
        } 
        else if ( ((board[i]& value_mask()) != 9) && ((board[i]& hidden_mask()) != 0) ){
            return false;
        }
    }
    return true;
   
}
int reveal(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){

    if ((board[x_loc + (x_dim*y_loc)]& marked_mask()) == marked_mask()){
        return 1;

    } else if ((board[x_loc + (x_dim*y_loc)]& hidden_mask()) != hidden_mask()){
        return 2;

    //Not sure if this works properly
    } else if ((board[x_loc + (x_dim*y_loc)]& value_mask()) == 9) {
        board[x_loc + (x_dim*y_loc)] &= value_mask();
        return 9; 

    } else if ( ((board[x_loc + (x_dim*y_loc)]& value_mask()) == 0 ) && ((board[x_loc + (x_dim*y_loc)]& hidden_mask()) == hidden_mask()) ) {
        
        board[x_loc + (x_dim*y_loc)] &= value_mask();
       
        //Left side
        if (x_loc == 0){
            for (int t{0}; t < 2; ++t){

                //Top corner
                if ( ((x_loc + (x_dim*y_loc)) >= x_dim)  )  {  
                    board[(x_loc + (x_dim*y_loc)) - (x_dim - t)] &= value_mask();
                }

                if ( (x_loc + (x_dim*y_loc)) < (x_dim*(y_dim-1)) ) {
                    board[(x_loc + (x_dim*y_loc)) + (x_dim + t)] &= value_mask();
                }

                if ( board[x_loc + (x_dim*y_loc)] != board[(x_loc + (x_dim*y_loc)) + t] ){
                    board[(x_loc + (x_dim*y_loc)) +t] &= value_mask();
                }
            }

        //Right side
        } else if (x_loc == (x_dim-1)){
            
            for (int t{0}; t < 2; ++t){
                            //Top corner
                            if  ((x_loc + (x_dim*y_loc)) >= x_dim)   {  
                                
                                board[(x_loc + (x_dim*y_loc)) - (x_dim + t)] &= value_mask();
                            }
                            //Bottom corner
                            if ( (x_loc + (x_dim*y_loc)) <= (x_dim*(y_dim-1)) ) {
                                
                                board[(x_loc + (x_dim*y_loc)) + (x_dim - t)] &= value_mask();
                            }

                            if (board[x_loc + (x_dim*y_loc)] != board[(x_loc + (x_dim*y_loc)) - t] ) {
                                
                                board[(x_loc + (x_dim*y_loc)) - t] &= value_mask();
                            }

                    }

        //Center
        } else {
            for (int t {-1}; t < 2; ++t){
            
                // checking if it is in the bottom row 
                if ( (x_loc + (x_dim*y_loc)) < (x_dim*(y_dim-1)) ){
                    board[(x_loc + (x_dim*y_loc)) + (x_dim+t)] &= value_mask();
                        
                }

                //Checking to see if it is in the top row
                if ( (x_loc + (x_dim*y_loc )) > (x_dim-1) ) {
                            
                     board[(x_loc + (x_dim*y_loc)) - (x_dim+t)] &= value_mask();
                        
                }

                //left and right
                if ( (board[x_loc + (x_dim*y_loc)] != board[(x_loc + (x_dim*y_loc)) + t]) ){    
                    board[(x_loc + (x_dim*y_loc)) +t] &= value_mask();
                }

            }
          
        }

    return 0;

    } else {    
        board[x_loc + (x_dim*y_loc)] ^= hidden_mask(); 
        return 0;
    }
   
   std::cout<<"Hello World";
    // TEsting tesing 
}
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "elements.h"

// Input cell
struct cell{
    char content[1];
};
typedef struct cell Cell;

// Pipe characteristics
struct pipe{
    float length;
    float radius;
    int orientation;
};
typedef struct pipe Pipe;

struct elbow{
    int outputs;
    float length;
    float radius;
};
typedef struct elbow Elbow;

struct cap{
    float radius;
};

int main()
{
    //--------------------------------------------------------------------------
    //-------------------------------Variables----------------------------------
    //--------------------------------------------------------------------------
    // Number of cells required for a system consisting
    // of one v pipe an elbow of two or three outputs and one h pipe.
    int system_size=7;

    // Records which was the last type of elbow inserted.
    // Either 2 or 3 outputs.
    int last_elbow;

    // Number of nodes of the total system
    int numNodes = 0;

    // Number of nodes of the total system
    int numTriangles = 0;

    // Initialize the input tape
    Cell*  cell_position=calloc(system_size, sizeof(Cell));

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //--------------------------------Counters----------------------------------
    //--------------------------------------------------------------------------
    // Counter to move between the cells
    int ct         = 0;
    // Counter to check the total number of cells traversed
    int numCells   = 0;
    // Counter to check the level of the system, that is, a net horizontal pipe
    // after an elbow of 3
    int cur_i_size = 0;
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //------------------------------Input File---------------------------------
    //--------------------------------------------------------------------------
    FILE *fp;
    fp = fopen("input.txt", "r");
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    //-------------------------Processing the Input-----------------------------
    //--------------------------------------------------------------------------
    while(1) {

        char c = fgetc(fp);
        if(c == EOF) {
            break;
        }
        printf("Counter:%d\n",ct);

        printf("%c\n",c);
        (cell_position + cur_i_size + ct)->content[0] = c;

        // Check for water bomb
        if(ct==0){
            if(strcmp((cell_position+cur_i_size+ct)->content,"w") != 0){
                printf("The model has been rejected as there is no initial water bomb.\n");
                break;
            }
        }

        // Check for pipe
        if(ct==1){
            if((cell_position+cur_i_size+ct)->content[0] != 'p'){
//                printf("%c\n",(cell_position+cur_i_size+ct)->content[0]);
                printf("The model has been rejected as it must follow with a pipe.\n");
                break;
            }
        }

        // Check for horizontal pipe
        if(ct==2){
            if(strcmp((cell_position+cur_i_size+ct)->content,"h") != 0){
                printf("Replacing the orientation of element [%d] to be horizontal.\n",cur_i_size+ct-1);
                (cell_position+cur_i_size+ct)->content[0]='h';
            }
        }

        // Check for size of pipe
        if(ct==3){
            int cond = 0;
            if(strcmp((cell_position+cur_i_size+ct)->content,"s") != 0){
                cond++;
            }
            if(strcmp((cell_position+cur_i_size+ct)->content,"m") != 0){
                cond++;
            }
            if(strcmp((cell_position+cur_i_size+ct)->content,"l") != 0){
                cond++;
            }
            if(cond == 3){
                printf("The size of the pipe is not recognized.\n");
                break;
            }
        }

        // Check for elbow after a horizontal pipe
        if(ct==4){
            if(strcmp((cell_position+cur_i_size+ct)->content,"e") != 0) {
                printf("The model has been rejected as there is no elbow after horizontal pipe.\n");
                break;
            }
        }

        // Check for type of elbow
        if(ct==5){
            if((cell_position+cur_i_size+ct)->content[0] == '3') {
                last_elbow = 3;
            }
            else {
                last_elbow = 2;
            }
        }

        // Check for a pipe or cap after elbow
        if(ct==6){
            if(strcmp((cell_position+cur_i_size+ct)->content,"p") == 0){
                cell_position = realloc(cell_position, numCells+4);
            }
            else if(strcmp((cell_position+cur_i_size+ct)->content,"c") != 0) {
                printf("The model has been rejected as it must follow with a pipe or cap.\n");
                break;
            }
        }

        // Check for vertical orientation of pipe or cap after an elbow
        if(ct==7){
            if(strcmp((cell_position+cur_i_size+ct)->content,"v") != 0) {
                printf("Replacing the orientation of element [%d] to be vertical.\n",cur_i_size+ct-1);
                (cell_position+cur_i_size+ct)->content[0] = 'v';
            }

            if ((cell_position+cur_i_size+ct-1)->content[0] == 'c') {
                if (last_elbow == 2) {
                    printf("True\n");
                    numCells++;
                    break;
                }
                else{
                    printf("ncells:%d\n",numCells);
                    cell_position = realloc(cell_position, numCells + system_size);
                    cur_i_size = numCells;
                    numCells++;
                    ct = 1;
                    continue;
                }
            }

        }

        // check for length of vertical pipe after elbow or check if the system is completed
        if(ct==8){
            if((cell_position+cur_i_size+ct-2)->content[0] == 'p'){
                int cond = 0;
                if(strcmp((cell_position+cur_i_size+ct)->content,"s") != 0){
                    cond++;
                }
                if(strcmp((cell_position+cur_i_size+ct)->content,"m") != 0){
                    cond++;
                }
                if(strcmp((cell_position+cur_i_size+ct)->content,"l") != 0){
                    cond++;
                }
                if(cond == 3){
                    printf("The size of the pipe is not recognized.\n");
                    break;
                }
            }
            else if((cell_position+cur_i_size+ct-2)->content[0] == 'c'){
                printf("%d\n",numCells);
                cell_position = realloc(cell_position, numCells + system_size);
                cur_i_size = numCells-2;
                ct = 1;
                continue;
            }
        }

        // Check for cap to close the system after a vertical pipe coming out from an elbow
        if(ct==9){
            if (strcmp((cell_position+cur_i_size+ct)->content, "c") != 0) {
                printf("The vertical pipe after the elbow is not closed.\n");
                break;
            }
        }

        // Check for vertical orientation of closing cap after a vertical pipe
        if(ct==10){
            if (strcmp((cell_position+cur_i_size+ct)->content, "v") != 0) {
                printf("Replacing the orientation of element [%d] to be vertical\n.", cur_i_size + ct);
                (cell_position + cur_i_size + ct)->content[0] = 'v';
            }
            if(last_elbow == 2){
                numCells++;
                break;
            }
            cell_position = realloc(cell_position, numCells + system_size);
            cur_i_size = numCells;
            numCells++;
            ct = 1;
            continue;
        }

        numCells++;
        ct++;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // Processed Input
    for(int i=0; i<numCells; i++){
        printf("|%c",(cell_position+i)->content[0]);
    }
    puts("|\n");

    //--------------------------------------------------------------------------
    //---------------Determining the number of nodes & triangles----------------
    //--------------------------------------------------------------------------
    for(int i=0; i<numCells; i++){
        if((cell_position+i)->content[0] == 'p'){
            numNodes = numNodes + 2*n+2;
            numTriangles = numTriangles + 4*n;
        }

        if((cell_position+i)->content[0] == 'e'){
            numNodes = numNodes + 36*n;
            numTriangles = numTriangles + 70*n;
        }

        if((cell_position+i)->content[0] == 'c'){
            numNodes = numNodes + 2*n+1;;
            numTriangles = numTriangles + 3*n;
        }
    }

    int nodes[numNodes+4][3];
    int triangles[numTriangles+12][3];
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //-----------------Calculating the coordinates of elements------------------
    //--------------------------------------------------------------------------
//    NEP next_element_post;
//
//    for(int i=0; i<numCells; i++){
//        if((cell_position+i)->content[0] == 'p'){
//            next_element_post = makeCylinder()
//        }
//
//        if((cell_position+i)->content[0] == 'e'){
//            numNodes = numNodes + 36*n;
//            numTriangles = numTriangles + 70*n;
//        }
//
//        if((cell_position+i)->content[0] == 'c'){
//            numNodes = numNodes + 2*n+1;;
//            numTriangles = numTriangles + 3*n;
//        }
//    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    return 0;
}
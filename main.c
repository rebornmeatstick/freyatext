#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialization(char** txtBuffer, size_t* cap, size_t* len);
void handleInput(char** txtBuffer, size_t* cap, size_t* len);
void cleanup(char** txtBuffer, size_t* len);
void saveToFile(char* filename, char* txtBuffer, size_t len);


int main()
{
    char* txtBuffer;
    size_t cap, len;

    initialization(&txtBuffer, &cap, &len);
    handleInput(&txtBuffer, &cap, &len);
    cleanup(&txtBuffer, &len);
    return 0;
}

void initialization(char** txtBuffer, size_t* cap, size_t* len){
    *cap = 256;
    *len = 0;
    *txtBuffer = malloc(*cap);

    if(!*txtBuffer) {
        fprintf(stderr, "malloc couldnt allocate memory lol\n");
        exit(1);
    }

    printf("welcome to freyatext-0.2!! press ESC (or ctrl + [) to finish typing!\n");
}

void handleInput(char** txtBuffer, size_t* cap, size_t* len){
    int ch;
    while((ch = getchar()) != 27) { // starts taking in user input, stops program on ESC
            // increment handle, divides current length by the cap to decide how many times it needs to increment to realloc into new buffer
            // accounts for shit like if i paste in a 2 billion character essay and simply increasing the buffer by 256 is NOT enough
            if (*len + 1 >= *(cap)) {
                printf("len: %zu\n", *len);
                printf("pre-cap: %zu\n", *cap);
                double increment = ((*len) / (*cap));
                double incrementRounded = ceil(increment);
                double newCap = (*(cap) * incrementRounded);
                (*cap) += newCap;
                char* newBuffer = realloc(*txtBuffer, *cap);



                if (!newBuffer) {
                    free(*txtBuffer);
                    fprintf(stderr, "REALLOC FUCKING DIED ITS THE END OF THE WORLD ...jk!, safely killing everything !!!\n");
                    fprintf(stderr, "post-cap: %zu\n", *cap);
                    exit(2);
                }
                *txtBuffer = newBuffer;
            }

            if (ch == 8) { // backspace handle
                if (*len > 0) {
                    (*len) -= 1;
                    (*txtBuffer)[(*len)--] = '\0';
                }
            } else {
                (*txtBuffer)[(*len)++] = (char)ch;
            }
    }
}


void cleanup(char** txtBuffer, size_t* len){
    char save;
    char filename[255];
    int pass = 0;
    (*txtBuffer)[*len] = '\0'; // null terminate

    printf("\ndo you wanna save the modified buffer? [y/n] : ");
    scanf("%s", &save);


    while (pass != 1) {
        if(save == 'y'){
            pass = 1;
            printf("\nsave under what filename? : ");
            scanf("%255s", &filename);
            printf("%s", &filename);
            saveToFile(filename, *txtBuffer, *len);
            break;
        }else if(save == 'n') {
            pass = 1;
            printf("\n--- your unsaved buffer ---\n%s\n", *txtBuffer);
            exit(0);
        }else{
            printf("that's not a vaild argument! type in [y/n], please! : ");
            scanf("%s", &save);
        }
    }

    free(*txtBuffer);
}

void saveToFile(char* filename, char* txtBuffer, size_t len){
    FILE* savedFile = fopen(filename, "w"); // open file in pointer
    if(!savedFile){
        fprintf(stderr, "\ncouldn't open %s for writing", filename);
        exit(3);
    }

    size_t written = fwrite(txtBuffer, sizeof(char), len, savedFile); // writes the file and gets the written length as a size_t
    if(written < len){
        fprintf(stderr, "\nwarning: could only write %zu out of %zu bytes", written, len);
    }else{
        printf("\nsuccessfully saved file!");
    }

    fclose(savedFile); // close file in pointer
}

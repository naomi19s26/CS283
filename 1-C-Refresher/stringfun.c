#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *str_ptr;
    int temp_val;
    char prev_char = '\0';
    int str_len = 0;
	str_ptr = user_str;
	//updates buffer based on inputed user_string
    while(*str_ptr != '\0'){    	
		if(*str_ptr != ' ' || (*str_ptr == ' ' && prev_char != ' ') || (*str_ptr == ' ' && prev_char == '\0')){
			str_len++;
			if(str_len == 50 && *(str_ptr + 1) != '\0'){
				return -1;
			}
			*buff = *str_ptr;
			buff++;
		}
		prev_char = *str_ptr;
		str_ptr++;
    }
	
	//handles edge case where user's input is all white spaces and no characters
    if (str_len == 1 && *buff == '\0' && prev_char == ' '){
    	*(buff - 1) = '.';
        str_len = 0;         
    }

	//checks if user_string is larger than buffer size
    if(str_len > BUFFER_SZ){
        return -1;
    }

    //updates buffer with remaining dots if str_len less than buffer size
    temp_val = str_len;
    while(temp_val != len){
		*buff = '.';
		buff++;
		temp_val++;
    }
	*buff = '\0';
    return str_len; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
	if (str_len > len) {
        return -1;
    }
	int word_count = 0;
	//last char keeps track of the last character checked in buffer
	char last_char = ' ';
    while(str_len > 0){
		
        //checks if we just passed a word in the string and updates the word count
		if ((*buff != ' ' && last_char == ' ')) {
            word_count++;
        }
		//updates last_char to previously check buffer character
		last_char = *buff;
		buff++;
		str_len--;
    }
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
//implements reversed_string function: reverses string in buffer
char* reverse_string(char* buff, int str_len){
	char *reversed_string = (char *)malloc((str_len + 1) * sizeof(char));
    if (reversed_string == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    //defines pointers at the end of user string and in the beginning of reversed_string
    char *end_ptr = buff + str_len - 1;
    char *reversed_ptr = reversed_string; 
	
	//updates buffer with reversed characters
	while (str_len > 0) {
        *reversed_ptr = *end_ptr;
        reversed_ptr++;
        end_ptr--;
        str_len--;
    }

    *reversed_ptr = '\0';
	return reversed_string;
	
}

//implements word_print function: prints out words and character count in buffer
int word_print(char *buff, int len, int str_len) {
    if (str_len > len) {
        return -1;
    }
    int char_count = 0;
    int word_count = 0;
    printf("Word Print\n");
    printf("----------\n");

    while (str_len > 0) {
        if (*buff != ' ') {
        	//prints non space charcacters and updates char_count and word_count
            if (char_count == 0) {
                word_count++;
                printf("%d. ", word_count);
            }
            printf("%c", *buff);
            char_count++;
        } else {
        	//prints out char_count if space character is found in between words
            if (word_count != 0 && str_len != 1) {
                printf(" %d\n", char_count);
                char_count = 0;
            }
        }
        buff++;
        str_len--;
    }

    if (char_count > 0) { 
        printf(" %d\n", char_count);
    }

    printf("\nTotal Words: %d\n", word_count);
    return 0;
}

int main(int argc, char *argv[]){
    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string
    char *reversed_string;

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // argc is an integer that represents the number of elements given in command line
    //argv is a list that stores all command line arguments
    //this if statement checks if the user inputted less than 2 arguments or the first charcter fo the second argument is not "-"
    //argv[1] signifies the type command you want to be done on the string, this checks if the first character of this part of the argument is "-" is given 
    //in teh correct format
    //if the command isn't given in this format, we can't process it. Therefore, this is a safe and essential check to implement
    //This immediately causes the code in the if statement to execute causing the program to exit
    //This check is needed and safe because if makes sure that the second command line argument is in a valid format which can be read by the system
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // for the program to successfully execute we need at list 3 arguments in the command line
    //This program exits if that requirement is not met
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    
    buff = (char*)malloc((BUFFER_SZ+1));
    if(buff == NULL){
		printf("Memory allocation failed");
		exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);//see todos
    
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //
        //the case statement options
        //implements reverse string case
        case 'r':
        	reversed_string = reverse_string(buff, user_str_len);
        	printf("Reversed string:%s\n", reversed_string);
        	free(reversed_string);
        	break;
		//implements word print case
        case 'w':
        	rc = word_print(buff, BUFFER_SZ, user_str_len);
        	if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
                exit(2);
            }
            break;

        //implements replace word case (without extra credit)   
        case 'x':
        	if(argc < 5){
				usage(argv[0]);
				exit(1);
        	}
        	printf("Not implemented!\n");
        	break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
// Having both the pointer and the length is good practice because
// the pointer serves as a reference to loop through the string, and
// the length variable acts as a safety check to ensure that the user's 
// string doesn't exceed the buffer size. Even though we already check for this in setup_buff(),
//I think it is good to still check this for safety reasons.

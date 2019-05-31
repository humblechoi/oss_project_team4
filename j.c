#include <stdio.h>     
#include <stdlib.h>    
#include <stdbool.h>   
#include <string.h>    

// restrict the number of tokens to 1024
#define TOKEN_COUNT 1024

// struct TOKEN
// contains information about each tokens
// such as types { string, number, array }
typedef struct _TOKEN {
    int begin;
    int end;
    int stringLength;
    int tokenIndex;
    union {            
        char *string;     
        double number;
    };
    bool isString;
    bool isNumber;
    bool isObjectList;
    bool isArray;
} TOKEN;

// struct JSON
// contains token list
typedef struct _JSON {
    TOKEN tokens[TOKEN_COUNT];
} JSON;

// utility function to readfile
// store date into buffer
// then return buffer
char *readFile(char *filename, int *readSize)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) return NULL;
    int size;
    char *buffer;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);
    while(!feof(fp)){
    	fread(buffer, size, 1, fp);}
    *readSize = size;
    fclose(fp);   
    return buffer;
}

// utility function to parse 
// the contents of buffer into
// each tokens
void parseJSON(char *doc, int size, JSON *json)
{
    // points to the position of index in token list
    int tokenIndex = 0;  
    // points to position of item in buffer 
    int pos = 0;           
    if (doc[pos] != '{')   
        return;
    pos++;
    // applies to all ( key : value ) objects    
    while (pos < size)     
    {
        switch (doc[pos])    
        {
	// ignore quotation marks 
        case '"':            
        {
	    json->tokens[tokenIndex].tokenIndex = tokenIndex;
            char *begin = doc + pos + 1;
	    json->tokens[tokenIndex].begin = pos + 1;
            char *end = strchr(begin, '"');
            if (end == NULL) 
                break;       
            int stringLength = end - begin; 
	    json->tokens[tokenIndex].end = pos + stringLength;
	    json->tokens[tokenIndex].stringLength = stringLength;  
            json->tokens[tokenIndex].isString = true;
            json->tokens[tokenIndex].string = malloc(stringLength + 1);
            memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
            memcpy(json->tokens[tokenIndex].string, begin, stringLength);
            tokenIndex++;    
            pos = pos + stringLength + 1;   
        }
        break;
	// applies to object list
	// ex : { "ex1":"item1", "ex2":"item2", "ex3":"item3" },
        case '{':            
        {
	    pos++;
            while (doc[pos] != '}')    
            {
		json->tokens[tokenIndex].tokenIndex = tokenIndex;
                if (doc[pos] == '"')   
                {
                    char *begin = doc + pos + 1;
		    json->tokens[tokenIndex].begin = pos + 1;
                    char *end = strchr(begin, '"');
                    if (end == NULL)  
                        break;        
                    int stringLength = end - begin; 
		    json->tokens[tokenIndex].end = pos + stringLength;  
		    json->tokens[tokenIndex].stringLength = stringLength;
                    json->tokens[tokenIndex].isString = true;
                    json->tokens[tokenIndex].string = malloc(stringLength + 1);
                    json->tokens[tokenIndex].isObjectList = true;
                    memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
                    memcpy(json->tokens[tokenIndex].string, begin, stringLength);
                    tokenIndex++;    
                    pos = pos + stringLength + 1;    
		}
		if (doc[pos] == '0' || doc[pos] == '1'
			|| doc[pos] == '2'
			|| doc[pos] == '3'
			|| doc[pos] == '4'
			|| doc[pos] == '5'
			|| doc[pos] == '6'
			|| doc[pos] == '7'
			|| doc[pos] == '8'
			|| doc[pos] == '9'
			|| doc[pos] == '-')
		{
			char *begin = doc + pos;
			json->tokens[tokenIndex].begin = pos;
           	 	char *end;
            		char *buffer;
            		end = strchr(doc + pos, ',');
            		if (end == NULL)
            		{
                		end = strchr(doc + pos, '}');
                		if (end == NULL)   
                    		break;          
            		}	
            		int stringLength = end - begin;
		        json->tokens[tokenIndex].end = pos + stringLength;	
			json->tokens[tokenIndex].stringLength = stringLength;
            		buffer = malloc(stringLength + 1);
            		memset(buffer, 0, stringLength + 1);
            		memcpy(buffer, begin, stringLength);
            		json->tokens[tokenIndex].isNumber = true;
		        json->tokens[tokenIndex].isObjectList = true;	
            		json->tokens[tokenIndex].number = atof(buffer);
            		free(buffer);   
            		tokenIndex++;    
            		pos = pos + stringLength + 1;                    
	       	}
                pos++;   
	    }
	}
        break;
	// applies to array list
	// ex : [ "item1", "item2", "item3" ].
	case '[':           
        {
            pos++;
            while (doc[pos] != ']')    
            {
		json->tokens[tokenIndex].tokenIndex = tokenIndex;
                if (doc[pos] == '"')   
                {
                    char *begin = doc + pos + 1;
		    json->tokens[tokenIndex].begin = pos + 1;
                    char *end = strchr(begin, '"');
                    if (end == NULL) break;         
                    int stringLength = end - begin;   
		    json->tokens[tokenIndex].end = pos + stringLength;
		    json->tokens[tokenIndex].stringLength = stringLength;
                    json->tokens[tokenIndex].isString = true;
                    json->tokens[tokenIndex].string = malloc(stringLength + 1);
                    json->tokens[tokenIndex].isArray = true;
                    memset(json->tokens[tokenIndex].string, 0, stringLength + 1);
                    memcpy(json->tokens[tokenIndex].string, begin, stringLength);
                    tokenIndex++;    
                    pos = pos + stringLength + 1;  
                }
      
	        if (doc[pos] == '0' || doc[pos] == '1'
			|| doc[pos] == '2'
			|| doc[pos] == '3'
			|| doc[pos] == '4'
			|| doc[pos] == '5'
			|| doc[pos] == '6'
			|| doc[pos] == '7'
			|| doc[pos] == '8'
			|| doc[pos] == '9'
			|| doc[pos] == '-')
		{
			char *begin = doc + pos;
			json->tokens[tokenIndex].begin = pos;
           	 	char *end;
            		char *buffer;
            		end = strchr(doc + pos, ',');
            		if (end == NULL)
            		{
                		end = strchr(doc + pos, '}');
                		if (end == NULL)   
                    		break;          
            		}	
            		int stringLength = end - begin;   
			json->tokens[tokenIndex].end = pos + stringLength;
			json->tokens[tokenIndex].stringLength = stringLength;
            		buffer = malloc(stringLength + 1);
            		memset(buffer, 0, stringLength + 1);
            		memcpy(buffer, begin, stringLength);
            		json->tokens[tokenIndex].isNumber = true;
		        json->tokens[tokenIndex].isArray = true;	
            		json->tokens[tokenIndex].number = atof(buffer);
            		free(buffer);   
            		tokenIndex++;    
            		pos = pos + stringLength + 1;                    
	       	}
                pos++;
            }
        }
        break;
	// read numbers and 
	// store it as string
	// need to use atof()
	// to convert string -> integer 
        case '0': case '1': case '2': case '3': case '4': case '5':    
        case '6': case '7': case '8': case '9': case '-':              
        {
	    json->tokens[tokenIndex].tokenIndex = tokenIndex;
            char *begin = doc + pos;
	    json->tokens[tokenIndex].begin = pos;
            char *end;
            char *buffer;
            end = strchr(doc + pos, ',');
            if (end == NULL)
            {
                end = strchr(doc + pos, '}');
                if (end == NULL)   
                    break;          
            }
            int stringLength = end - begin;   
	    json->tokens[tokenIndex].end = pos + stringLength;
	    json->tokens[tokenIndex].stringLength = stringLength;
            buffer = malloc(stringLength + 1);
            memset(buffer, 0, stringLength + 1);
            memcpy(buffer, begin, stringLength);
            json->tokens[tokenIndex].isNumber = true;  
            json->tokens[tokenIndex].number = atof(buffer);
            free(buffer);   
            tokenIndex++;    
            pos = pos + stringLength + 1; 
        }
        break;
        }
        pos++;
    }
}

// utility function to free json memeory
void freeJSON(JSON *json) 
{
    for (int i = 0; i < TOKEN_COUNT; i++) 
    {
        if (json->tokens[i].isString == true) 
            free(json->tokens[i].string);  
    }
}

void printJson (JSON *json)
{
    int i;
//  while (json.tokens[i].string != NULL){
    for (i=0;i<TOKEN_COUNT;i++){ 
	// to stop at the end of token list
	if (json->tokens[i].string == NULL) {
//		printf("null point character\n");
		break;
	// if the token is object list 
	// then do the following
	} else if (json->tokens[i+1].isObjectList == true) { 
	    	printf("%s : \n",json->tokens[i].string);
	    	i++;
		// print out tokens after '{'
		while (1) {
			// if the end of token is met
			// don't print and just break loop
			if (json->tokens[i].isObjectList == false) break;
			else if (json->tokens[i+1].isNumber == true) {
//			else if (json.tokens[i+1].type == 1) {
				printf("\t%s : %d\n",json->tokens[i].string,(int)json->tokens[i+1].number);}
			else {
				printf("\t%s : %s\n",json->tokens[i].string,json->tokens[i+1].string);}
			i+=2;
		}
		if (json->tokens[i+1].isNumber == true) {
//		if (json.tokens[i+1].type == 1) {
			printf("%s : %d\n",json->tokens[i].string, (int)json->tokens[i+1].number);
		} else {
			printf("%s : %s\n",json->tokens[i].string, json->tokens[i+1].string);
		}
		i++;
	// if the token is array list
	// then do the following
	} else if (json->tokens[i+1].isArray == true) {
		printf("%s : \n",json->tokens[i].string);
		i++;
		// print out tokens after '['
		while (1) {
			// if the end of token is met
			// don't print and just break loop
			if (json->tokens[i].isArray == false) break;
			else if (json->tokens[i].isNumber == true) {
//			else if (json.tokens[i].type == 1) {
				printf("\t%d\n",(int)json->tokens[i].number);}
			else {
				printf("\t%s\n",json->tokens[i].string);}
			i++;
		}
		if (json->tokens[i+1].isNumber == true) {
//		if (json.tokens[i+1].type == 1) {
			printf("%s : %d\n",json->tokens[i].string, (int)json->tokens[i+1].number);
		} else {
			printf("%s : %s\n",json->tokens[i].string, json->tokens[i+1].string);
		}
		i++;
	// if token is number then print the following line
	} else if (json->tokens[i+1].isNumber == true) {
//      } else if (json.tokens[i+1].type == 1) {
		printf("%s : %d\n",json->tokens[i].string, (int)json->tokens[i+1].number);
		i++;
	// if just regular ( key : value ) then print the following line
	} else {
		printf("%s : %s\n",json->tokens[i].string, json->tokens[i+1].string);
		i++;
	}
    }
}

// the main function to carry out main tasks
int main(int argc, char **argv)
{
    // if the argv[1] is not specified at the beginning 
    // print out error message
    if (argv[1] == NULL) printf("please give json file name as argument\n");
    int size;
    // store file contents into buffer
    char *doc = readFile(argv[1], &size);
    if (doc == NULL) return -1;
    // initialize json token list
    JSON json = { 0, };    
    // separate buffer into list of strings
    parseJSON(doc, size, &json);
//    char input;
//    scanf("%c",&input);
//    printJson(&json);
    for (int j=0;j<TOKEN_COUNT;j++){
	if (json.tokens[j].string == NULL) break;
	else if (json.tokens[j].isString == true)
		printf("[%d]: %s ( Line Number : %d ~ %d, Object Type : String )\n",
				json.tokens[j].tokenIndex,
				json.tokens[j].string, 
				json.tokens[j].begin,
				json.tokens[j].end);
	else if (json.tokens[j].isNumber == true)
		printf("[%d]: %s ( Line Number : %d ~ %d, Object Type : Number )\n",
				json.tokens[j].tokenIndex,
				json.tokens[j].string, 
				json.tokens[j].begin,
				json.tokens[j].end);
	else break;
    }

    freeJSON(&json);
    free(doc);    
    return 0;
}

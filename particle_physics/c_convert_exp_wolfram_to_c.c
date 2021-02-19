#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

char *IN_FILE_PATH="trial_01.m";

char *VAR_1="Eh";
char *VAR_2="Eq";
char *VAR_3="I";
char *VAR_4="\\[Theta]";
char *VAR_5="\\[Phi]";
char *VAR_6="\\[Theta]p";
char *VAR_7="\\[Phi]p";
char *VAR_8="\\[Theta]pp";
char *VAR_9="\\[Phi]pp";

float VAR_1_VALUE=50;
float VAR_2_VALUE=10;
float VAR_3_VALUE=1;
float VAR_4_VALUE=M_PI;
float VAR_5_VALUE=M_PI;
float VAR_6_VALUE=M_PI;
float VAR_7_VALUE=M_PI;
float VAR_8_VALUE=M_PI;
float VAR_9_VALUE=M_PI;



struct term{
  char *operator;
  int is_binary;
  char *dtype;
  void *term1;
  void *term2;
  int modify;
};

struct list{
  char ch;
  struct list *next;
};

void append(struct list *, char);
int len(struct list *);
char *getWord(struct list *);
struct term *getNewTerm();
struct term *getUnaryTerm();
struct term *getTerm(FILE *);
char *chtostr(char );
void skipBlankSpace(FILE *);
void printTerm(struct term *);
float evaluate(struct term *);

int main(){
 
  FILE *infile=fopen(IN_FILE_PATH,"r");
  struct term *_term=getTerm(infile);
  fclose(infile);
  printTerm(_term);
  printf("\n");
  float term_value=evaluate(_term);
  printf("Evaluate = %E\n",term_value);
  return 0;
}

void append(struct list *head, char ch){
  struct list *ptr, *prev_ptr;
  ptr=head;
  if(head==NULL){
    printf("Cannot append to empty list");
    return;
  }
  while(ptr!=NULL){
    prev_ptr=ptr;
    ptr=ptr->next;
  }
  struct list *new_element=(struct list*)malloc(sizeof(struct list));
  new_element->ch=ch;
  new_element->next=NULL;
  prev_ptr->next=new_element;
}

int len(struct list *head){
  int size=0;
  struct list *ptr=head;
  while(ptr!=NULL){
    ptr=ptr->next;
    size++;
  }
  return size;
}

char *getWord(struct list *head){
  int size=len(head);
  if(size==0){
    printf("getWord: Error List is empty\n");
    return NULL;
  }
  char *word=(char *)malloc(sizeof(char)*size);
  int i;
  struct list *ptr=head;
  for(i=0;i<size;i++){
    word[i]=ptr->ch;
    ptr=ptr->next;
  }
  printf("curr_word returned  =  %s\n",word);
  return word;
}

struct term *getNewTerm(){
  struct term *new_term=(struct term *)malloc(sizeof(struct term));
  new_term->operator=NULL;
  new_term->is_binary=-1;
  new_term->dtype=NULL;
  new_term->term1=NULL;
  new_term->term2=NULL;
  new_term->modify=1;
  return new_term;
}

char curr_ch='\0';
struct list *curr_word=NULL;

struct term *getUnaryTerm(){
  struct term *unary_term=getNewTerm();
  unary_term->operator="i";
  unary_term->is_binary=0;
  
  char *word=getWord(curr_word);
  if(isdigit(word[0])!=0 || word[0]=='+' || word[0]=='-'){
    unary_term->dtype="NUMBER";
    float *data=(float *)malloc(sizeof(float));
    *data=atof(word);
    unary_term->term1=data;
  }else if(strcmp(word,VAR_1)){
    unary_term->dtype="VAR_1";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_2)){
    unary_term->dtype="VAR_2";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_3)){
    unary_term->dtype="VAR_3";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_4)){
    unary_term->dtype="VAR_4";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_5)){
    unary_term->dtype="VAR_5";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_6)){
    unary_term->dtype="VAR_6";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_7)){
    unary_term->dtype="VAR_7";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_8)){
    unary_term->dtype="VAR_8";
    unary_term->term1=word;
  }else if(strcmp(word,VAR_9)){
    unary_term->dtype="VAR_9";
    unary_term->term1=word;
  }else{
    printf("getUnaryTerm: Error No Match");
  }

  free(curr_word);
  curr_word=NULL;
  return unary_term;
}

char *chtostr(char ch){
  char *str=(char *)malloc(sizeof(char)*2);
  str[1]='\0';
  str[0]=ch;
  return str;
}

void skipBlankSpace(FILE *infile){
  curr_ch=fgetc(infile);
  while(curr_ch==' ' || curr_ch=='\n'){
    curr_ch=fgetc(infile);
  }
  fseek(infile,ftell(infile)-1,SEEK_SET ); 
}

struct term *getTerm(FILE *infile){
  struct term *this_term,*tmp_term,*new_tmp_term,*negative_tmp_term;
  char *this_word,*this_operator,*tmp_operator,*new_tmp_operator;
  int term_no=1,bra_count=0,is_negative;

  this_term=getNewTerm();
  tmp_term=NULL;
  new_tmp_term=NULL;
  tmp_operator=NULL;  

  while(1){
    curr_ch=fgetc(infile);
    printf("curr_ch = %c\n",curr_ch);
    if (curr_ch=='('){
      if (curr_word!=NULL && strcmp(getWord(curr_word),"-")==0){
        is_negative=1;
        free(curr_word);
        curr_word=NULL;
      }else{
        is_negative=0;
      }
      tmp_term=getTerm(infile);
      tmp_term->modify=0;
      printf("( ");
      printTerm(tmp_term);
      if(is_negative==1){
        negative_tmp_term=getNewTerm();
        negative_tmp_term->operator="*";
        negative_tmp_term->is_binary=1;
        free(curr_word);
        curr_word=(struct list *)malloc(sizeof(struct list));
        curr_word->ch='-';
        curr_word->next=NULL;
        append(curr_word,'1');
        negative_tmp_term->term1=getUnaryTerm();
        negative_tmp_term->term2=tmp_term;
        tmp_term=negative_tmp_term;
      }
      if(term_no==1){
        this_term->term1=tmp_term;
        term_no++;
      }else if(term_no>=2){
        this_term->term2=tmp_term;
        term_no++;
      }
    }
    else if(curr_ch==')' || curr_ch==']' || curr_ch==EOF){
      //printf("RRRRRRR\n");
      //if(curr_ch==EOF){printf("EOF cought\n");}
      if(this_term->term1==NULL){
        tmp_term=getUnaryTerm();
        this_term->operator="i";
        this_term->is_binary=0;
        this_term->dtype=tmp_term->dtype;
        this_term->term1=tmp_term->term1;
      }else if(this_term->term1!=NULL && this_term->term2==NULL){
        if(this_term->operator==NULL){
          if(((struct term *)this_term->term1)->is_binary!=1){
            this_term->operator=((struct term *)this_term->term1)->operator;
            this_term->is_binary=0;
            this_term->dtype=((struct term *)this_term->term1)->dtype;
            this_term->term1=((struct term *)this_term->term1)->term1;
          }else{
            this_term->operator=((struct term *)this_term->term1)->operator;
            this_term->is_binary=1;
            this_term->term2=((struct term *)this_term->term1)->term2;
            this_term->term1=((struct term *)this_term->term1)->term1;
          }
        }else{
          if((strcmp(this_term->operator,"*")==0 || strcmp(this_term->operator,"/")==0) && ((struct term *)this_term->term1)->is_binary==1 && ((struct term *)this_term->term1)->modify==1){
            tmp_term=getNewTerm();
            tmp_term->operator=this_term->operator;
            tmp_term->is_binary=1;
            tmp_term->term1=((struct term *)this_term->term1)->term2;
            tmp_term->term2=getUnaryTerm();
            this_term->operator=((struct term *)this_term->term1)->operator;
            this_term->is_binary=1;
            this_term->term1=((struct term *)this_term->term1)->term1;
            this_term->term2=tmp_term;
          }else{
            this_term->term2=getUnaryTerm();
          }

        }
      }else if(this_term->term1!=NULL && this_term->term2!=NULL){
        if((strcmp(this_term->operator,"*")==0 || strcmp(this_term->operator,"/")==0) && ((struct term *)this_term->term1)->is_binary==1 && curr_word==NULL && ((struct term *)this_term->term1)->modify==1){
          tmp_term=getNewTerm();
          tmp_term->operator=this_term->operator;
          tmp_term->is_binary=1;
          tmp_term->term1=((struct term *)this_term->term1)->term2;
          tmp_term->term2=this_term->term2;
          this_term->operator=((struct term *)this_term->term1)->operator;
          this_term->is_binary=1;
          this_term->term1=((struct term *)this_term->term1)->term1;
          this_term->term2=tmp_term;
        }else if(curr_word!=NULL){
          printf("UUUUUUUUUUUu\n");
          this_term->term2=getUnaryTerm();
        }
      }else{printf("confused");}
      return this_term;
    }
    else if(curr_ch=='+' || curr_ch=='-' || curr_ch=='*' || curr_ch=='/'){
      if(curr_word==NULL && this_term->term1==NULL && this_term->term2==NULL){
        curr_word=(struct list *)malloc(sizeof(struct list));
        curr_word->ch=curr_ch;
        curr_word->next=NULL; 
      }else if(this_term->term1==NULL){
        this_term->operator=chtostr(curr_ch);
        this_term->is_binary=1;
        this_term->term1=getUnaryTerm();
        term_no=2;
      }else if(this_term->term1!=NULL && this_term->term2==NULL){
        if(this_term->operator!=NULL){
          if((strcmp(this_term->operator,"*")==0 || strcmp(this_term->operator,"/")==0) && ((struct term *)this_term->term1)->is_binary==1 && ((struct term *)this_term->term1)->modify==1){
            tmp_term=getNewTerm();
            tmp_term->operator=this_term->operator;
            tmp_term->is_binary=1;
            tmp_term->term1=((struct term *)this_term->term1)->term2;
            tmp_term->term2=getUnaryTerm();

            new_tmp_term=getNewTerm();
            new_tmp_term->operator=((struct term *)this_term->term1)->operator;
            new_tmp_term->is_binary=1;
            new_tmp_term->term1=((struct term *)this_term->term1)->term1;
            new_tmp_term->term2=tmp_term;

            this_term->operator=chtostr(curr_ch);
            this_term->is_binary=1;
            this_term->term1=new_tmp_term; 
          }else{
            tmp_term=getNewTerm();
            tmp_term->operator=this_term->operator;
            tmp_term->is_binary=this_term->is_binary;
            tmp_term->term1=this_term->term1;
            tmp_term->term2=getUnaryTerm();
            this_term->term1=tmp_term;
            this_term->operator=chtostr(curr_ch);
            this_term->is_binary=1; 
          }
        }else{
          this_term->operator=chtostr(curr_ch);
          this_term->is_binary=1;
        }
        term_no=2;
      }else if(this_term->term1!=NULL && this_term->term2!=NULL){
        if((strcmp(this_term->operator,"*")==0 || strcmp(this_term->operator,"/")==0) && ((struct term *)this_term->term1)->is_binary==1 && ((struct term *)this_term->term1)->modify==1){
          tmp_term=getNewTerm();
          tmp_term->operator=this_term->operator;
          tmp_term->is_binary=1;
          tmp_term->term1=((struct term *)this_term->term1)->term2;
          tmp_term->term2=this_term->term2;

          new_tmp_term=getNewTerm();
          new_tmp_term->operator=((struct term *)this_term->term1)->operator;
          new_tmp_term->is_binary=1;
          new_tmp_term->term1=((struct term *)this_term->term1)->term1;
          new_tmp_term->term2=tmp_term;

          this_term->operator=chtostr(curr_ch);
          this_term->is_binary=1;
          this_term->term1=new_tmp_term;
          this_term->term2=NULL;
        }else{
          tmp_term=getNewTerm();
          tmp_term->operator=this_term->operator;
          tmp_term->is_binary=this_term->is_binary;
          tmp_term->term1=this_term->term1;
          tmp_term->term2=this_term->term2;
          this_term->operator=chtostr(curr_ch);
          this_term->is_binary=1;
          this_term->term1=tmp_term;
          this_term->term2=NULL;
        }
        term_no=2;
      }
    }
    else if(curr_ch=='^'){
      tmp_operator=chtostr(curr_ch);
      skipBlankSpace(infile);
      curr_ch=fgetc(infile);
      if(curr_ch=='-'){
        tmp_operator=strcat(tmp_operator,chtostr(curr_ch));
      }else{
        fseek(infile,ftell(infile)-1,SEEK_SET);
      }
      skipBlankSpace(infile);
      curr_ch=fgetc(infile);
      while(isdigit(curr_ch)!=0 || curr_ch=='.'){
        tmp_operator==strcat(tmp_operator,chtostr(curr_ch));
        curr_ch=fgetc(infile);
      }
      fseek(infile,ftell(infile)-1,SEEK_SET);
      printf("^ operator %s\n",tmp_operator);
      tmp_term=getNewTerm();
      tmp_term->operator=tmp_operator;
      tmp_term->is_binary=0;
       
      ///////////////////////// 
      if(curr_word==NULL && this_term->term2==NULL && this_term->operator!=NULL && strcmp(this_term->operator,"*")==0){
        tmp_term->operator=strcat("*",tmp_operator);
        if(((struct term *)this_term->term1)->is_binary!=1){
          tmp_term->term1=this_term->term1;
          this_term->term1=tmp_term;
        }else{
          tmp_term->term1=((struct term *)this_term->term1)->term2;
          ((struct term *)this_term->term1)->term2=tmp_term;
        }
        this_term->operator=NULL;
        term_no=2;
        continue;
      }
      //////////////////////////
      if(curr_word!=NULL){
        tmp_term->term1=getUnaryTerm();
      }else{
        if(this_term->term1!=NULL && this_term->term2==NULL){
          tmp_term->term1=this_term->term1;
          term_no=1;
        }else if(this_term->term1!=NULL && this_term->term2!=NULL){
          tmp_term->term1=this_term->term2;
        }
      }
      if(term_no==1){
        this_term->term1=tmp_term;
        term_no++;
      }else if(term_no>=2){
        this_term->term2=tmp_term;
        term_no++;
      }  
    }
    else if(isalnum(curr_ch)!=0 || curr_ch=='.'){
      if(curr_word==NULL){
        curr_word=(struct list *)malloc(sizeof(struct list));
        curr_word->ch=curr_ch;
        curr_word->next=NULL;
      }else{
        append(curr_word,curr_ch);
      }
    }
   
    this_word=getWord(curr_word);
    if(this_word!=NULL && (strcmp(this_word,"Sin")==0 || strcmp(this_word,"Cos")==0 || strcmp(this_word,"Sqrt")==0)){
      tmp_term=getNewTerm();
      tmp_term->operator=this_word;
      tmp_term->is_binary=0;
      free(curr_word);
      curr_word=NULL;

      curr_ch=fgetc(infile);
      skipBlankSpace(infile);
      curr_ch=fgetc(infile);
      if(isalnum(curr_ch)!=0){
        curr_word=(struct list *)malloc(sizeof(struct list));
        curr_word->ch=curr_ch;
        curr_word->next=NULL;
        tmp_term->term1=getTerm(infile);
      }else if(curr_ch='\\'){
        free(curr_word);
        curr_word=NULL;
        bra_count=0;
        while(bra_count<2){
          if(curr_word==NULL){
            curr_word=(struct list *)malloc(sizeof(struct list));
            curr_word->ch=curr_ch;
            curr_word->next=NULL;
          }else{
            append(curr_word,curr_ch);
          }
          curr_ch=fgetc(infile);
          if(curr_ch==']'){
            bra_count++;
          }
        }
        tmp_term->term1=getUnaryTerm();
      }
      if(term_no==1){
        this_term->term1=tmp_term;
        printf("%s\n",tmp_term->operator);
        printTerm(this_term->term1);
        term_no++;
      }else if(term_no>=2){
        this_term->term2=tmp_term;
        printf("%s\n",tmp_term->operator);
        printTerm(this_term->term2);
        term_no++;
      }

    } 
  }
  return this_term;
}

void printTerm(struct term *_term){
  //printf("-->%s\n",_term->operator);
  if(strcmp(_term->operator,"i")==0){
    if(strcmp(_term->dtype,"NUMBER")==0){
      printf("%f",*((float *)(_term->term1)));
    }else{
      printf("%s",_term->term1);
    }
  }
  else if(strcmp(_term->operator,"Sin")==0 || strcmp(_term->operator,"Cos")==0 || strcmp(_term->operator,"Sqrt")==0){
    printf("%s[",_term->operator);
    printTerm(_term->term1);
    printf("]");
  }
  else if(strcmp(_term->operator,"+")==0 || strcmp(_term->operator,"-")==0 || strcmp(_term->operator,"*")==0 || strcmp(_term->operator,"/")==0){
    printf("(");
    printTerm(_term->term1);
    printf(" %s ",_term->operator);
    printTerm(_term->term2);
    printf(")");
  }
  else if(strcmp(_term->operator,"^") || strcmp(_term->operator,"*^")){
    printTerm(_term->term1);
    printf("%s",_term->operator);
  }
  //printf("\n"); 
}

float evaluate(struct term *_term){
  printf("Evaluate operator = %s\n",_term->operator);
  if(strcmp(_term->operator,"i")==0){
    if(strcmp(_term->dtype,"NUMBER")==0){
      printf("num\n = %f\n",*((float *)_term->term1));
      return *((float *)_term->term1);
    }else if(strcmp(_term->dtype,"VAR_1")==0){
      return VAR_1_VALUE;
    }else if(strcmp(_term->dtype,"VAR_2")==0){
      return VAR_2_VALUE;
    }else if(strcmp(_term->dtype,"VAR_3")==0){
      return VAR_3_VALUE;
    }else if(strcmp(_term->dtype,"VAR_4")==0){
      return VAR_4_VALUE;
    }else if(strcmp(_term->dtype,"VAR_5")==0){
      return VAR_5_VALUE;
    }else if(strcmp(_term->dtype,"VAR_6")==0){
      return VAR_6_VALUE;
    }else if(strcmp(_term->dtype,"VAR_7")==0){
      return VAR_7_VALUE;
    }else if(strcmp(_term->dtype,"VAR_8")==0){
      return VAR_8_VALUE;
    }else if(strcmp(_term->dtype,"VAR_9")==0){
      return VAR_9_VALUE;
    }else{
      printf("Evaluate: Error check dtype\n");
    }
  }
  else if(strcmp(_term->operator,"Sin")==0){
    printf("1\n");
    return sin(evaluate(_term->term1));
  }
  else if(strcmp(_term->operator,"Cos")==0){
    printf("2\n");
    return cos(evaluate(_term->term1));
  }
  else if(strcmp(_term->operator,"Sqrt")==0){
    printf("3\n");
    return sqrt(evaluate(_term->term1));
  }
  else if(strcmp(_term->operator,"*^")==0){
    printf("4\n");
    //
  }
  else if(strstr(_term->operator,"^")!=NULL){
    printf("5\n");
    float exponent=atof(_term->operator+1);
    printf("exponents %f\n",exponent);
    return pow(evaluate(_term->term1),exponent);
  }
  else if(strcmp(_term->operator,"+")==0){
    printf("6\n");
    return evaluate(_term->term1) + evaluate(_term->term2);
  }
  else if(strcmp(_term->operator,"-")==0){
    printf("7\n");
    return evaluate(_term->term1) - evaluate(_term->term2);
  }
  else if(strcmp(_term->operator,"*")==0){
    printf("****\n");
    return (evaluate(_term->term1) * evaluate(_term->term2));
  }
  else if(strcmp(_term->operator,"/")==0){
    printf("8\n");
    return evaluate(_term->term1) / evaluate(_term->term2);
  }
  else{
    printf("Evaluate: Error check operator\n");
    printf("Operator = %s\n",_term->operator);
  }
}

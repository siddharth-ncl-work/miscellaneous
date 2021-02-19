#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(){
  char s[100]="hello world!";
  printf("%s\n",s);

  FILE *infile=fopen("test.txt","r");
  char *text[1000];

  fscanf(infile,"%s",text);
  printf("Text from file:\n%s\n",text);

  //char text[1000];
  size_t elements_read;
  elements_read=fread(text,1,1000,infile);
  printf("-->Text from file:\n%s\n",text);
  printf("SIZE=%d\n",elements_read);

  int pos;
  pos=ftell(infile);
  printf("CURR POS: %d\n",pos);

  float fp=atof(" +60.25 ");
  printf("FLOAT = %f\n",fp);

  float fn=atof(" -2.25 ");
  printf("FLOAT = %f\n",fn);

  printf("-->%f\n",M_PI);

  int *p=NULL;
  if(p==NULL){
    printf("p is NULL\n");
  }
}

/*
int main(){

  int BUFFER_SIZE=100;
  long int approx_total_chars;
  FILE *infile=fopen(IN_FILE_PATH,"r");
  int init_pos,end_pos;
  init_pos=ftell(infile);
  fseek(infile,0,SEEK_END);
  end_pos=ftell(infile);
  printf("INIT POS=%d\nEND POS=%d\n",init_pos,end_pos);

  approx_total_chars=end_pos;

  fseek(infile,0,SEEK_SET);
  char line[BUFFER_SIZE],*return_value;

  return_value=fgets(line,BUFFER_SIZE,infile);
  printf("return value: %s",return_value);
  printf("LINE 1: %s\n",line);

  return_value=fgets(line,BUFFER_SIZE,infile);
  printf("return value: %s",return_value);
  printf("LINE 2: %s\n",line);

  return_value=fgets(line,BUFFER_SIZE,infile);
  printf("return value: %s",return_value);
  printf("LINE 3: %s\n",line);

  return_value=fgets(line,BUFFER_SIZE,infile);
  printf("return value: %s",return_value);
  printf("LINE 4: %s\n",line);

  fseek(infile,0,SEEK_SET);
  getTerm(infile);
  printf("%d %d\n,'\0'",EOF);
  return 0;
}

*/

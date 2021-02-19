import math

IN_FILE_PATH="test.txt"

VAR_1="Eq"
VAR_2="Eh"
VAR_3="I"
VAR_4="\\[Theta]"
VAR_5="\\[Phi]"
VAR_6="\\[Theta]p"
VAR_7="\\[Phi]p"
VAR_8="\\[Theta]pp"
VAR_9="\\[Phi]pp"

VALUE_MAP={
"VAR_1":50,
"VAR_2":10,
"VAR_3":1,
"VAR_4":math.pi,
"VAR_5":math.pi,
"VAR_6":math.pi,
"VAR_7":math.pi,
"VAR_8":math.pi,
"VAR_9":math.pi
}


class term:
  
  def __init__(self):
    self.operator=None
    self.is_binary=False
    self.dtype=None
    self.term1=None
    self.term2=None
    self.modify=True

  def __str__(self):
    #print(f"-->{self.operator}")
    if self.operator=="i":
      return str(self.term1)
    elif self.operator=="Sin" or self.operator=="Cos" or self.operator=="Sqrt":
      return f"{self.operator}[{self.term1}]"
    elif self.operator=="+" or self.operator=="-" or self.operator=="*" or self.operator=="/":
      if self.operator=="/":
        return f"({self.term1}) {self.operator} ({self.term2})"
      else:
        return f"({self.term1} {self.operator} {self.term2})"
    elif "*^" in self.operator:
      return f"{self.term1}{self.operator}"
    elif '^' in self.operator:
      return f"({self.term1}){self.operator}" 
    else:
      return "what"
 
  def evaluate(self,value_map):
    if self.operator=="i":
      if self.dtype=="NUMBER":
        return float(self.term1)
      elif "VAR" in self.dtype:
        return value_map[self.dtype]
      else:
        print("Evaluate: Error check dtypes")
    elif self.operator=="Sin":
      return math.sin(self.term1.evaluate(value_map))
    elif self.operator=="Cos":
      return math.cos(self.term1.evaluate(value_map))
    elif self.operator=="Sqrt":
      return math.sqrt(self.term1.evaluate(value_map))
    elif "*^" in self.operator:
      exponent=float(self.operator[2:])
      return self.term1.evaluate(value_map)*(10**exponent)
    elif "^" in self.operator:
      exponent=float(self.operator[1:])
      return math.pow(self.term1.evaluate(value_map),exponent)
    elif self.operator in "+":
      return self.term1.evaluate(value_map) + self.term2.evaluate(value_map)
    elif self.operator in "-":
      return self.term1.evaluate(value_map) - self.term2.evaluate(value_map)
    elif self.operator in "*":
      return self.term1.evaluate(value_map) * self.term2.evaluate(value_map)
    elif self.operator in "/":
      return self.term1.evaluate(value_map) / self.term2.evaluate(value_map)
    else:
      print("Evaluate: Error check operators")
    
    
curr_ch=None
curr_word=None

def getUnaryTerm():
  global curr_word
  print(f"Unary1 = {curr_word}")
  word=curr_word.strip()
  unary_term=term()
  unary_term.operator="i"
  unary_term.is_binary=False
  if word[0].isnumeric() or word[0]=='-' or word[0]=='+':
    unary_term.dtype="NUMBER"
    unary_term.term1=float(word)
  elif word==VAR_1:
    unary_term.dtype="VAR_1"
    unary_term.term1=word
  elif word==VAR_2:
    unary_term.dtype="VAR_2"
    unary_term.term1=word
  elif word==VAR_3:
    unary_term.dtype="VAR_3"
    unary_term.term1=word
  elif word==VAR_4:
    unary_term.dtype="VAR_4"
    unary_term.term1=word
  elif word==VAR_5:
    unary_term.dtype="VAR_5"
    unary_term.term1=word
  elif word==VAR_6:
    unary_term.dtype="VAR_6"
    unary_term.term1=word
  elif word==VAR_7:
    unary_term.dtype="VAR_7"
    unary_term.term1=word
  elif word==VAR_8:
    unary_term.dtype="VAR_8"
    unary_term.term1=word
  elif word==VAR_9:
    unary_term.dtype="VAR_9"
    unary_term.term1=word
  else:
    print("No Match")
  curr_word=None
  return unary_term

def skipBlankSpace(infile):
  curr_ch=infile.read(1)
  while curr_ch==' ' or curr_ch=='\n':
    curr_ch=infile.read(1) 
  infile.seek(infile.tell()-1)


def getTerm(infile):
  global curr_ch,curr_word,total_lines
  this_term=term()
  term_no=1

  while curr_ch is not '':
    curr_ch=infile.read(1)
    #print(curr_ch)
    
    debug_file.write(curr_ch)    

    if curr_ch=='(':
      is_negative=None
      if curr_word=="-":
        print('Success'+'='*50)
        print(this_term.operator)
        is_negative=True
        curr_word=None
      else:
        is_negative=False
      tmp_term=getTerm(infile)
      tmp_term.modify=False
      print(f" ( = {tmp_term}")
      if is_negative:
        negative_tmp_term=term()
        negative_tmp_term.operator="*"
        negative_tmp_term.is_binary=True
        curr_word="-1"
        negative_tmp_term.term1=getUnaryTerm()
        negative_tmp_term.term2=tmp_term       
        tmp_term=negative_tmp_term
      if term_no==1:
        this_term.term1=tmp_term
        term_no+=1
      elif term_no>=2:
        this_term.term2=tmp_term
        term_no+=1
    elif curr_ch==')' or curr_ch==']' or curr_ch=='':
      #print(f"return {curr_ch} = {curr_word}  {this_term.operator} {this_term.term1}")
      if this_term.term1 is None:
        tmp_term=getUnaryTerm()
        this_term.operator="i"
        this_term.is_binary=False
        this_term.dtype=tmp_term.dtype
        this_term.term1=tmp_term.term1
      elif this_term.term1 is not None and this_term.term2 is None:
        if this_term.operator is None:
          if not this_term.term1.is_binary:
            this_term.operator=this_term.term1.operator
            this_term.is_binary=False
            this_term.dtype=this_term.term1.dtype
            this_term.term1=this_term.term1.term1
          else:
            this_term.operator=this_term.term1.operator
            this_term.is_binary=True
            this_term.term2=this_term.term1.term2
            this_term.term1=this_term.term1.term1
        else:
          print(f"return2 = {this_term.operator} -- {this_term.term1} -- {curr_ch}")
          if this_term.operator in "*/" and this_term.term1.is_binary and this_term.term1.modify:
            tmp_term=term()
            tmp_term.operator=this_term.operator
            tmp_term.is_binary=True
            tmp_term.term1=this_term.term1.term2
            tmp_term.term2=getUnaryTerm()
            #print(f"return3 = {tmp_term.term1}")
            this_term.operator=this_term.term1.operator
            this_term.is_binary=True
            this_term.term1=this_term.term1.term1
            this_term.term2=tmp_term
          else:
            this_term.term2=getUnaryTerm()
      elif this_term.term1 is not None and this_term.term2 is not None:
        print(f"return 4 = {this_term.term1} {this_term.operator} {this_term.term2}")
       #################
        if this_term.operator in "*/" and this_term.term1.is_binary and curr_word is None and this_term.term1.modify:
          tmp_term=term()
          tmp_term.operator=this_term.operator
          tmp_term.is_binary=True
          tmp_term.term1=this_term.term1.term2
          tmp_term.term2=this_term.term2
          #print(f"return3 = {tmp_term.term1}")
          this_term.operator=this_term.term1.operator
          this_term.is_binary=True
          this_term.term1=this_term.term1.term1
          this_term.term2=tmp_term
        elif curr_word is not None:
          print(f"Return {curr_ch} = is this possible?")
          this_term.term2=getUnaryTerm()
       ############### 
      else:
        print("Unexpedcted")
      return this_term
    elif curr_ch=='+' or curr_ch=='-' or curr_ch=='*' or curr_ch=='/':
      if curr_word is None and this_term.term1 is None and this_term.term2 is None:
        curr_word=curr_ch
      elif this_term.term1 is None:
        this_term.operator=str(curr_ch)
        this_term.is_binary=True
        this_term.term1=getUnaryTerm()
        term_no=2
      elif this_term.term1 is not None and this_term.term2 is None:
        if this_term.operator is not None:
          if this_term.operator in "*/" and this_term.term1.is_binary and this_term.term1.modify:
            tmp_term=term()
            tmp_term.operator=this_term.operator
            tmp_term.is_binary=True
            tmp_term.term1=this_term.term1.term2
            tmp_term.term2=getUnaryTerm()

            new_tmp_term=term()
            new_tmp_term.operator=this_term.term1.operator
            new_tmp_term.is_binary=True
            new_tmp_term.term1=this_term.term1.term1
            new_tmp_term.term2=tmp_term
          
            this_term.operator=str(curr_ch)
            this_term.is_binary=True
            this_term.term1=new_tmp_term
          else:   
            tmp_term=term()
            tmp_term.operator=this_term.operator
            tmp_term.is_binary=this_term.is_binary
            tmp_term.term1=this_term.term1
            tmp_term.term2=getUnaryTerm()
            this_term.term1=tmp_term
            this_term.operator=str(curr_ch)
            this_term.is_binary=True
        else:
          this_term.operator=str(curr_ch)
          this_term.is_binary=True
        term_no=2
      elif this_term.term1 is not None and this_term.term2 is not None:
        if this_term.operator in "*/" and this_term.term1.is_binary and this_term.term1.modify:
          tmp_term=term()
          tmp_term.operator=this_term.operator
          tmp_term.is_binary=True
          tmp_term.term1=this_term.term1.term2
          tmp_term.term2=this_term.term2

          new_tmp_term=term()
          new_tmp_term.operator=this_term.term1.operator
          new_tmp_term.is_binary=True
          new_tmp_term.term1=this_term.term1.term1
          new_tmp_term.term2=tmp_term

          this_term.operator=str(curr_ch)
          this_term.is_binary=True
          this_term.term1=new_tmp_term
          this_term.term2=None
        else:
          tmp_term=term()
          tmp_term.operator=this_term.operator
          tmp_term.is_binary=this_term.is_binary
          tmp_term.term1=this_term.term1
          tmp_term.term2=this_term.term2
          this_term.operator=str(curr_ch)
          this_term.is_binary=True
          this_term.term1=tmp_term
          this_term.term2=None
        term_no=2
    elif curr_ch=='^':
      #print(f'^ = {curr_word} {this_term.term1}')
      tmp_operator=str(curr_ch)
      skipBlankSpace(infile)
      curr_ch=infile.read(1)
      if curr_ch=='-':
        tmp_operator+=curr_ch
      else:
        infile.seek(infile.tell()-1)
      skipBlankSpace(infile)
      curr_ch=infile.read(1)
      while curr_ch.isdigit() or curr_ch=='.':
        tmp_operator+=curr_ch
        curr_ch=infile.read(1)
      infile.seek(infile.tell()-1)

      tmp_term=term()
      tmp_term.operator=tmp_operator
      tmp_term.is_binary=False
  
      #############################
      if curr_word is None and this_term.term2 is None and this_term.operator=="*":
        tmp_term.operator="*"+tmp_operator
        if not this_term.term1.is_binary:
          tmp_term.term1=this_term.term1
          this_term.term1=tmp_term
        else:
          tmp_term.term1=this_term.term1.term2
          this_term.term1.term2=tmp_term
        this_term.operator=None
        term_no=2
        continue
      ##############################

      if curr_word is not None:
        tmp_term.term1=getUnaryTerm()        
      else:
        if this_term.term1 is not None and this_term.term2 is None:
          print(f"^ term1 = {this_term.term1}")
          tmp_term.term1=this_term.term1
          term_no=1
        elif this_term.term1 is not None and this_term.term2 is not None:
          tmp_term.term1=this_term.term2
      print(f"^ term_no = {term_no}")
      if term_no==1:
        this_term.term1=tmp_term
        term_no+=1
      elif term_no>=2:
        this_term.term2=tmp_term
        term_no+=1
    elif curr_ch.isalnum() or curr_ch=='.':
      if curr_word is None:
        curr_word=str(curr_ch)
      else:
        curr_word+=curr_ch
    
    this_word=curr_word
    if this_word is not None and (this_word=='Sin' or this_word=='Cos' or this_word=='Sqrt'):
      #print(this_word)
      tmp_term=term()
      tmp_term.operator=this_word
      tmp_term.is_binary=False

      #skipBlankSpace(infile)
      curr_ch=infile.read(1)
      skipBlankSpace(infile)
      curr_ch=infile.read(1)
      if curr_ch.isalnum():
        curr_word=curr_ch
        tmp_term.term1=getTerm(infile)
      elif curr_ch=='\\':
        curr_word=''
        bra_count=0
        while bra_count<2:
          curr_word+=curr_ch
          curr_ch=infile.read(1)
          if curr_ch==']':
            bra_count+=1
        tmp_term.term1=getUnaryTerm()
      if term_no==1:
        this_term.term1=tmp_term
        term_no+=1
      elif term_no>=2:
        this_term.term2=tmp_term
        term_no+=1

  return this_term

infile=open(IN_FILE_PATH,'r',encoding="UTF-8")
debug_file=open('debug.txt','w')
test_term=getTerm(infile)
debug_file.close()
print(test_term)
print(test_term.evaluate(VALUE_MAP))

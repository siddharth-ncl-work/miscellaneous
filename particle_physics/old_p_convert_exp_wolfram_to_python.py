IN_FILE_PATH="test.txt"

VAR_1="Eh"
VAR_2="Eq"
VAR_3="I"
VAR_4="\\[Theta]"
VAR_5="\\[Theta]p"
VAR_6="\\[Theta]pp"
VAR_7="\\[Phi]"
VAR_8="\\[Phi]p"
VAR_9="\\[Phi]pp"

class term:
  
  def __init__(self):
    self.operator=None
    self.is_binary=False
    self.dtype=None
    self.term1=None
    self.term2=None

  def __str__(self):
    print(f"-->{self.operator}")
    if self.operator=="i":
      return str(self.term1)
    elif self.operator=="Sin" or self.operator=="Cos" or self.operator=="Sqrt":
      return f"{self.operator}[{self.term1}]"
    elif self.operator=="+" or self.operator=="-" or self.operator=="*" or self.operator=="/":
      return f"({self.term1} {self.operator} {self.term2})"
    elif '^' in self.operator:
      return f"({self.term1}){self.operator}" 
    else:
      return "what"
 
    
curr_ch=None
curr_word=None

def getUnaryTerm():
  global curr_word
  print(f"Unary1 = {curr_word}")
  word=curr_word
  unary_term=term()
  unary_term.operator="i"
  unary_term.is_binary=False
  if word[0].isnumeric() or word[0]=='-':
    print("unary number")
    unary_term.dtype="number"
    unary_term.term1=float(word)
  elif word==VAR_1:
    unary_term.dtype="var_1"
    unary_term.term1=word
  elif word==VAR_2:
    unary_term.dtype="var_2"
    unary_term.term1=word
  elif word==VAR_3:
    unary_term.dtype="var_3"
    unary_term.term1=word
  elif word==VAR_4:
    unary_term.dtype="var_4"
    unary_term.term1=word
  elif word==VAR_5:
    unary_term.dtype="var_5"
    unary_term.term1=word
  elif word==VAR_6:
    unary_term.dtype="var_6"
    unary_term.term1=word
  elif word==VAR_7:
    unary_term.dtype="var_7"
    unary_term.term1=word
  elif word==VAR_8:
    unary_term.dtype="var_8"
    unary_term.term1=word
  elif word==VAR_9:
    unary_term.dtype="var_9"
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
  global curr_ch,curr_word 
  this_term=term()

  while curr_ch is not '':
    curr_ch=infile.read(1)
    print(curr_ch)
    
    if curr_ch=='(':
      tmp_term=getTerm(infile)
      print(f" ( = {tmp_term}")
      if not tmp_term.is_binary:
        this_term.operator=tmp_term.operator
        this_term.is_binary=False
        this_term.term1=tmp_term.term1
      else:
        this_term.term1=tmp_term
    elif curr_ch==')' or curr_ch==']' or curr_ch=='':
      print(f"return {curr_ch} = {curr_word}  {this_term.operator} {this_term.term1}")
      if this_term.term1 is None:
        tmp_term=getUnaryTerm()
        this_term.operator="i"
        this_term.dtype=tmp_term.dtype
        this_term.term1=tmp_term.term1
      elif this_term.term1 is not None and this_term.operator is None:
        if not this_term.term1.is_binary:
          this_term.operator=this_term.term1.operator
          this_term.is_binary=False
          this_term.term1=this_term.term1.term1
        else:
          print(f"coming == {this_term.term1}" )
          this_term.operator=this_term.term1.operator
          this_term.is_binary=True
          this_term.term2=this_term.term1.term2
          this_term.term1=this_term.term1.term1
      else:
        print("Unexpedcted")
      return this_term
    elif curr_ch=='+' or curr_ch=='-' or curr_ch=='*' or curr_ch=='/':
      if curr_word is None and this_term.term1 is None and this_term.term2 is None:
        curr_word=curr_ch
      elif this_term.term1 is not None and this_term.operator is None:
        print(f"binary = {this_term.term1}")
        this_term.operator=str(curr_ch)
        this_term.is_binary=True
        this_term.term2=getTerm(infile)
        return this_term
      elif this_term.term1 is not None and this_term.operator is not None:
        print(f"binary = {this_term.term1}")
        prev_term1=term()
        prev_term1.operator=this_term.operator
        prev_term1.is_binary=False
        prev_term1.term1=this_term.term1
        this_term.term1=prev_term1
        this_term.operator=str(curr_ch)
        this_term.is_binary=True
        this_term.term2=getTerm(infile)
        return this_term
      elif this_term.term1 is None and this_term.term2 is None:
        this_term.operator=str(curr_ch)
        this_term.is_binary=True
        this_term.term1=getUnaryTerm()
        this_term.term2=getTerm(infile)
        print(this_term.term2)
        return this_term  
    elif curr_ch=='^':
      print(f'^ = {curr_word} {this_term.term1}')
      tmp_operator=str(curr_ch)
      skipBlankSpace(infile)
      curr_ch=infile.read(1)
      if curr_ch=='-':
        tmp_operator+=curr_ch
      else:
        infile.seek(infile.tell()-1)
      skipBlankSpace(infile)
      curr_ch=infile.read(1)
      tmp_operator+=curr_ch
      if this_term.term1 is None:
        tmp_term=term()
        this_term.operator=tmp_operator
        this_term.is_binary=False
        this_term.term1=getUnaryTerm()
      else:
        print(f"^ term1 = {this_term.term1}")
        this_term.operator=tmp_operator
        this_term.is_binary=False       
    elif curr_ch.isalnum() or curr_ch=='.':
      if curr_word is None:
        curr_word=str(curr_ch)
      else:
        curr_word+=curr_ch

    this_word=curr_word
    if this_word is not None and (this_word=='Sin' or this_word=='Cos' or this_word=='Sqrt'):
      print(this_word)
      this_term.operator=this_word
      this_term.is_binary=False

      curr_ch=infile.read(1)
      curr_ch=infile.read(1)
      if curr_ch.isalnum():
        curr_word=curr_ch
        this_term.term1=getTerm(infile)
      elif curr_ch=='\\':
        curr_word=''
        bra_count=0
        while bra_count<2:
          curr_word+=curr_ch
          curr_ch=infile.read(1)
          if curr_ch==']':
            bra_count+=1
        this_term.term1=getUnaryTerm()
  return this_term


infile=open(IN_FILE_PATH,'r',encoding="UTF-8")
test_term=getTerm(infile)
print(test_term)

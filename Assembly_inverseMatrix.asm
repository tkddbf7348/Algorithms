#developer : SangYul Kim, CSAI, PuKyong National University
      

.data
   NewLine : .asciiz "\n"
   Empty : .asciiz " "
   Error : .asciiz "No determinant"
   Array : .word 1, 2, -1   #acyually [9], but i mean [3][3]
           .word 0, -1, 0
           .word -1, 3, 2   
      #{{1, 2, -1},
      # {0, -1, 0},
      # {-1, 3, 2}}

            #answer :   {{2, 7, 1},   
                 # {0, -1, 0},
                   # {1, 5, 1}}
      
   size : .word 3   #3x3 matrix size
   
   .eqv DATA_SIZE 4   #Define DATA_SIZE 4 (int size)
   
   
#main function startㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ   
.text   
   main :
      
      la $a0, Array   #a0 = address of Array     #load address
      lw $a1, size   #a1 = 3(size)         #load word
      jal determinant   #call determinant function      #jump and link
      
#check if determinant == 0ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      li $t0, 0      #load immediate
      mtc1 $t0, $f0   #move to coprocessor1  : move the value to coprocessor to calculate floating point  
      c.eq.s $f20, $f0 #if(determinant == 0)   #coprocessor.equal.single-precision
      bc1f exit #if determinant != 0 ,     goto exit
      
      
#if determinant == 0,    exitㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         li $v0, 4   #4 : print Stirng
         la $a0, Error   #if determinant == 0, print Error
         syscall
         li $v0,10   #10 : exit   
         syscall
         
         
#print determinant valueㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ          
      exit :         
         
      mov.s $f12, $f20 #move to f12 and print 
      li $v0, 2   #v0 to system code (1: print integer 2 : print float) 
      syscall      #print determinant
      
      li $v0, 4   #\n
      la $a0, NewLine   
      syscall
      
      
      
      
#inverse Matrixㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ      
      jal Inverse_matrix #Inverse Matrix print function
      #f12 : determinant value
      
      
      
#quit programㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ      
      
      li $v0, 10 # quit main
      syscall
      

      
#start determinant functionㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#determinant formula : a[0][0]a[1][1]a[2][2] - a[0][0]a[1][2]a[2][1]
#                    + a[0][1]a[1][2]a[2][0] -a[0][1]a[1][0]a[2][2]
#                   + a[0][2]a[1][0]a[2][1] - a[0][2]a[1][1]a[2][0]   #alias Array = a


   determinant :
      li $v1, 0   #return value
      li $t0, 0     #i
      li $t3, 1   #use to reset f2
#first calculationㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ  
      sumLoop_1 :  #a[0][0]a[1][1]a[2][2]
      
         li $t1, 0   #t1 = address of Array
      
      
#if i == 0,  reset f2 register(return value store)ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         beq $t0, 0, reset_f2_1 #if i==0
         j f2_1
            reset_f2_1 : mtc1 $t3 $f2   # reset f2 (if i == 0)
            cvt.s.w $f2 $f2 #integer -> float
         f2_1 :
 
         
         
         mul $t1, $t0, $a1   #t1 = rowIndex * colSize
         add $t1, $t1, $t0   #t1 = rowIndex * colSize + colindex
         mul $t1, $t1, DATA_SIZE #t1 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t1, $t1, $a0   #t1 = (rowIndex * colSize + colindex) * DATA_SIZE + base address(Array)
         
         #t1= adderss of Array[0][0]
         
         lw $t2, ($t1)   #t2= Array[0][0]   (value of t1(pointer) -> t2)
         mtc1 $t2 $f0   # f0 = (float)Array[0][0], (float)Array[1][1], (float)Array[2][2] (each loop)
         cvt.s.w $f0 $f0 #integer to float
      

         mul.s $f2, $f2, $f0   #result value multiply to f2
                
         
         addi $t0, $t0, 1   #i = i + 1  
         blt $t0, $a1, sumLoop_1   #if i < size, Loop again.   
        
      add.s $f20, $f20, $f2 #floating point return value
      #return += Array[0][0]Array[1][1]Array[2][2]
      
      li $t0, 0   #reset i == 0    (for next loop)
         
#second calculationㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      
      sumLoop_2 :  #Array[0][0]Array[1][2]Array[2][1]
         li $t1, 0   #t1 = address of Array
      
      
#if i == 0,  reset f2 register(return value store)ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         beq $t0, 0, reset_f2_2 #if i == 0
         j f2_2 
            reset_f2_2 : mtc1 $t3 $f2   # reset f2 (if i == 0)
            cvt.s.w $f2 $f2 #integer -> float
         f2_2 :
      
         mul $t1, $t0, $a1   #t1 = rowIndex * colSize
         add $t1, $t1, $t0   #t1 = rowIndex * colSize + colindex
         mul $t1, $t1, DATA_SIZE #t1 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t1, $t1, $a0   #t1 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         beq $t0, 1, add_21 #if i == 1
         j else_21 #if i != 1, goto else_21
            add_21 : add $t1, $t1, 4      #if i == 1,  Array[1][1] -> Array[1][2]
         else_21:   
         
         beq $t0, 2, sub_22 #if i == 2
         j else_22 #if i != 2, goto else_22
            sub_22 : sub $t1, $t1, 4      #if i == 2, Array[2][2] -> Array[2][1]
         else_22:
         
         
         lw $t2, ($t1)   #insert the value of Array[0][0], Array[1][2], Array[2][1] to $t2 (each loop)
         
         mtc1 $t2 $f0   # f0 = (float)Array[0][0], (float)Array[1][2], (float)Array[2][1]
         cvt.s.w $f0 $f0 #integer to float

         mul.s $f2, $f2, $f0   
         
         addi $t0, $t0, 1   #i = i + 1
         blt $t0, $a1, sumLoop_2   #if i < size, Loop again.
      
      
      sub.s $f20, $f20, $f2 #floating point return value
      #return -= Array[0][0]Array[1][2]Array[2][1]
      
      li $t0, 0   #reset i == 0    (for next loop)
      
      
#third calculationㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      sumLoop_3 :  #Array[0][1]Array[1][2]Array[2][0]
         li $t1, 0   #t1 = address of Array
      
         
         #각 루프 처음(i == 0)마다 f2(결과값 저장 레지스터)를 초기화시켜줌ㅡㅡㅡㅡㅡㅡㅡㅡ
         beq $t0, 0, reset_f2_3 #같은지 비교
         j f2_3
            reset_f2_3 : mtc1 $t3 $f2   # f2 = 결과 저장하는 칸은 루프 처음에만 초기화해줌
            cvt.s.w $f2 $f2 #integer를 float으로 변환
         f2_3 :
      
         mul $t1, $t0, $a1   #t1 = rowIndex * colSize
         add $t1, $t1, $t0   #t1 = rowIndex * colSize + colindex
         mul $t1, $t1, DATA_SIZE #t1 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t1, $t1, $a0   #t1 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         #if-else 구문 
         beq $t0, 0, add_30 #같은지 비교
         j else_30 #다르면 뛰어넘음
            add_30 : add $t1, $t1, 4      #i=0일 때 Array[0][0]에서 Array[0][1]로 만들어줌
         else_30:
            
         beq $t0, 1, add_31 #같은지 비교
         j else_31 #다르면 뛰어넘음
            add_31 : add $t1, $t1, 4      #i=1일 때 Array[1][1]에서 Array[1][2]로 만들어줌
         else_31:
         
         beq $t0, 2, sub_32 #같은지 비교
         j else_32 #다르면 뛰어넘음
            sub_32 : sub $t1, $t1, 8      #i=2일 때 Array[2][2]에서 Array[2][0]로 만들어줌
         else_32:
         
         
         lw $t2, ($t1)   #t2에 루프마다 Array[0][1], Array[1][2], Array[2][0]의 값을 넣음 
         
         mtc1 $t2 $f0   # f0 = (float)Array[0][1], (float)Array[1][2], (float)Array[2][0]
         cvt.s.w $f0 $f0 #integer를 float으로 변환
         

         mul.s $f2, $f2, $f0   
         
         addi $t0, $t0, 1   #i = i + 1
         blt $t0, $a1, sumLoop_3   #if i < size, Loop again.
      
      
      add.s $f20, $f20, $f2 #floating point 연산 리턴값
       #return += Array[0][1]Array[1][2]Array[2][0]
      
      li $t0, 0   #i = 0으로 초기화(다음 루프를 위해)
      
#공식 네번째 계산ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      sumLoop_4 :  #Array[0][1]Array[1][0]Array[2][2]
         li $t1, 0   #t1 = address of Array
      
         #각 루프 처음(i == 0)마다 f2(결과값 저장 레지스터)를 초기화시켜줌ㅡㅡㅡㅡㅡㅡㅡㅡ
         beq $t0, 0, reset_f2_4 #같은지 비교
         j f2_4
            reset_f2_4 : mtc1 $t3 $f2   # f2 = 결과 저장하는 칸은 루프 처음에만 초기화해줌
            cvt.s.w $f2 $f2 #integer를 float으로 변환
         f2_4 :
      
         mul $t1, $t0, $a1   #t1 = rowIndex * colSize
         add $t1, $t1, $t0   #t1 = rowIndex * colSize + colindex
         mul $t1, $t1, DATA_SIZE #t1 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t1, $t1, $a0   #t1 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         #if-else 구문 
         beq $t0, 0, add_40 #같은지 비교
         j else_40 #다르면 뛰어넘음
            add_40 : add $t1, $t1, 4      #i=0일 때 Array[0][0]에서 Array[0][1]로 만들어줌
         else_40:
            
         beq $t0, 1, sub_41 #같은지 비교
         j else_41 #다르면 뛰어넘음
            sub_41 : sub $t1, $t1, 4      #i=1일 때 Array[1][1]에서 Array[1][0]로 만들어줌
         else_41:
         
         
         lw $t2, ($t1)   #t2에 루프마다 Array[0][1], Array[1][0], Array[2][2]의 값을 넣음 
         
         mtc1 $t2 $f0   # f0 = (float)Array[0][1], (float)Array[1][0], (float)Array[2][2]
         cvt.s.w $f0 $f0 #integer를 float으로 변환

         mul.s $f2, $f2, $f0   
         
         addi $t0, $t0, 1   #i = i + 1
         blt $t0, $a1, sumLoop_4   #if i < size, Loop again.
      
      
      sub.s $f20, $f20, $f2 #floating point 연산 리턴값
      #return -= Array[0][1]Array[1][2]Array[2][0]
      
      li $t0, 0   #i = 0으로 초기화(다음 루프를 위해)
      
      
#공식 다섯번째 계산ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      sumLoop_5 :  #Array[0][2]Array[1][0]Array[2][1]
         li $t1, 0   #t1 = address of Array
      
         #각 루프 처음(i == 0)마다 f2(결과값 저장 레지스터)를 초기화시켜줌ㅡㅡㅡㅡㅡㅡㅡㅡ
         beq $t0, 0, reset_f2_5 #같은지 비교
         j f2_5
            reset_f2_5 : mtc1 $t3 $f2   # f2 = 결과 저장하는 칸은 루프 처음에만 초기화해줌
            cvt.s.w $f2 $f2 #integer를 float으로 변환
         f2_5 :
      
         mul $t1, $t0, $a1   #t1 = rowIndex * colSize
         add $t1, $t1, $t0   #t1 = rowIndex * colSize + colindex
         mul $t1, $t1, DATA_SIZE #t1 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t1, $t1, $a0   #t1 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         #if-else 구문 
         beq $t0, 0, add_50 #같은지 비교
         j else_50 #다르면 뛰어넘음
            add_50 : add $t1, $t1, 8      #i=0일 때 Array[0][0]에서 Array[0][2]로 만들어줌
         else_50:   
         
         beq $t0, 1, sub_51 #같은지 비교
         j else_51 #다르면 뛰어넘음
            sub_51 : sub $t1, $t1, 4      #i=1일 때 Array[1][1]에서 Array[1][0]로 만들어줌
         else_51:
         
         beq $t0, 2, sub_52 #같은지 비교
         j else_52 #다르면 뛰어넘음
            sub_52 : sub $t1, $t1, 4      #i=1일 때 Array[2][2]에서 Array[2][1]로 만들어줌
         else_52:
         
         
         lw $t2, ($t1)   #t2에 루프마다 Array[0][2], Array[1][0], Array[2][1]의 값을 넣음 
         
         mtc1 $t2 $f0   # f0 = (float)Array[0][2], (float)Array[1][0], (float)Array[2][1]
         cvt.s.w $f0 $f0 #integer를 float으로 변환
         

         mul.s $f2, $f2, $f0   
         
         addi $t0, $t0, 1   #i = i + 1
         blt $t0, $a1, sumLoop_5   #if i < size, Loop again.
      
      
      add.s $f20, $f20, $f2 #floating point 연산 리턴값
      #return += Array[0][2]Array[1][0]Array[2][1]
      
      li $t0, 0   #i = 0으로 초기화(다음 루프를 위해)
         
         
         
#공식 여섯번째 계산ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      sumLoop_6 :  #Array[0][2]Array[1][1]Array[2][0]
         li $t1, 0   #t1 = address of Array
      
         #각 루프 처음(i == 0)마다 f2(결과값 저장 레지스터)를 초기화시켜줌ㅡㅡㅡㅡㅡㅡㅡㅡ
         beq $t0, 0, reset_f2_6 #같은지 비교
         j f2_6
            reset_f2_6 : mtc1 $t3 $f2   # f2 = 결과 저장하는 칸은 루프 처음에만 초기화해줌
            cvt.s.w $f2 $f2 #integer를 float으로 변환
         f2_6 :
      
         mul $t1, $t0, $a1   #t1 = rowIndex * colSize
         add $t1, $t1, $t0   #t1 = rowIndex * colSize + colindex
         mul $t1, $t1, DATA_SIZE #t1 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t1, $t1, $a0   #t1 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         #if-else 구문 
         beq $t0, 0, add_60 #같은지 비교
         j else_60 #다르면 뛰어넘음
            add_60 : add $t1, $t1, 8      #i=0일 때 Array[0][0]에서 Array[0][2]로 만들어줌
         else_60:
         
         beq $t0, 2, sub_62 #같은지 비교
         j else_62 #다르면 뛰어넘음
            sub_62 : sub $t1, $t1, 8      #i=1일 때 Array[2][2]에서 Array[2][0]로 만들어줌
         else_62:
         
         
         lw $t2, ($t1)   #t2에 루프마다 Array[0][2], Array[1][1], Array[2][0]의 값을 넣음 
         
         mtc1 $t2 $f0   # f0 = (float)Array[0][2], (float)Array[1][1], (float)Array[2][0]
         cvt.s.w $f0 $f0 #integer를 float으로 변환
         

         mul.s $f2, $f2, $f0   
         
         addi $t0, $t0, 1   #i = i + 1
         blt $t0, $a1, sumLoop_6   #if i < size, Loop again.
      
      sub.s $f20, $f20, $f2 #floating point 연산 리턴값 
      #return -= Array[0][2]Array[1][1]Array[2][0]
      
      li $t0, 0   #i = 0으로 초기화(다음 루프를 위해)
      
   jr $ra #함수 나가기
#행렬식 함수 끝ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


#역행렬 함수ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
#inv[i][j] = 1.0 / determinant(a) *
            #(a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3] - 
               #a[(i + 1) % 3][(j + 2) % 3] * a[(i + 2) % 3][(j + 1) % 3]);
   Inverse_matrix :
      li $t8, 0      # : 행 인덱스 i
      li $t9, 0      # : 열 인덱스 j
      li $t4, 3      #mod연산을 위한 3
      
      li $t7, 1   # 1 / detminant(a) 를 하기위한 1
      mtc1 $t7, $f4   #  1 / detminant(a) 를 하기위한 1을 coprocessor에 전달
      cvt.s.w $f4, $f4 #float으로 바꿔줌
      
      I_loop:
         la $a0, Array   #a0 = Array
         li $t2, 0      # : 배열의 주소 저장
         
         li $t6, 0      #(a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3] - 의 결과값 
         li $t7, 0      # a[(i + 1) % 3][(j + 2) % 3] * a[(i + 2) % 3][(j + 1) % 3]);의 결과값      
      
       #a[(i + 1) % 3][(j + 1) % 3]ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         #(i + 1) % 3
         move $t0, $t8   #i 나머지 연산을 위해 따로 저장
         move $t1, $t9   #j 나머지 연산을 위해 따로 저장
         
         add $t0, $t0, 1   # i + 1  : 나머지 연산을 하기 위함
         div $t0, $t4        # (i + 1)을 3으로 나눈 나머지
         mfhi $t5           # 나머지값 저장
         move $t0 $t5   #i는  (i + 1) % 3의 값을 가짐
             
         #(j + 1) % 3
         add $t1, $t1, 1   # j + 1  : 나머지 연산을 하기 위함
         div $t1, $t4        # (j + 1)을 3으로 나눈 나머지
         mfhi $t5           # 나머지값 저장
         move $t1 $t5   #j는  (j + 1) % 3의 값을 가짐
             
         #t2는 Array[i][j]의 주소를 가지게 되는 공식.
         mul $t2, $t0, $a1   #t2 = rowIndex * colSize
         add $t2, $t2, $t1   #t2 = rowIndex * colSize + colindex
         mul $t2, $t2, DATA_SIZE #t2 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t2, $t2, $a0   #t2 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         lw $t3, ($t2)   #t3 : Array[i][j]의 값
         add $t6, $t6, $t3   #a[(i + 1) % 3][(j + 1) % 3]
         
         
         
         #a[(i + 2) % 3][(j + 2) % 3]ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         #(i + 2) % 3
         
         move $t0, $t8   #i 나머지 연산을 위해 따로 저장
         move $t1, $t9   #j 나머지 연산을 위해 따로 저장
         
         add $t0, $t0, 2   # i + 2  : 나머지 연산을 하기 위함
         div $t0, $t4        # (i + 2)을 3으로 나눈 나머지
         mfhi $t5           # temp for the mod 나머지값 저장
         move $t0 $t5   #i는  (i + 2) % 3의 값을 가짐
             
         #(j + 2) % 3
         add $t1, $t1, 2   # j + 2  : 나머지 연산을 하기 위함
         div $t1, $t4        # (j + 2)을 3으로 나눈 나머지
         mfhi $t5           # temp for the mod 나머지값 저장
         move $t1 $t5   #j는  (j + 2) % 3의 값을 가짐
             
         #t2는 Array[i][j]의 주소를 가지게 되는 공식.
         mul $t2, $t0, $a1   #t2 = rowIndex * colSize
         add $t2, $t2, $t1   #t2 = rowIndex * colSize + colindex
         mul $t2, $t2, DATA_SIZE #t2 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t2, $t2, $a0   #t2 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         lw $t3, ($t2)   #t3 : Array[i][j]의 값
         mul $t6, $t6, $t3   #a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3]
         
         
         #a[(i + 1) % 3][(j + 2) % 3]ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         #(i + 1) % 3
         
         move $t0, $t8   #i 나머지 연산을 위해 따로 저장
         move $t1, $t9   #j 나머지 연산을 위해 따로 저장
         
         add $t0, $t0, 1   # i + 1  : 나머지 연산을 하기 위함
         div $t0, $t4        # (i + 1)을 3으로 나눈 나머지
         mfhi $t5           # temp for the mod 나머지값 저장
         move $t0 $t5   #i는  (i + 1) % 3의 값을 가짐
             
         #(j + 2) % 3
         add $t1, $t1, 2   # j + 2  : 나머지 연산을 하기 위함
         div $t1, $t4        # (j + 2)을 3으로 나눈 나머지
         mfhi $t5           # temp for the mod 나머지값 저장
         move $t1 $t5   #j는  (j + 2) % 3의 값을 가짐
             
         #t2는 Array[i][j]의 주소를 가지게 되는 공식.
         mul $t2, $t0, $a1   #t2 = rowIndex * colSize
         add $t2, $t2, $t1   #t2 = rowIndex * colSize + colindex
         mul $t2, $t2, DATA_SIZE #t2 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t2, $t2, $a0   #t2 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         lw $t3, ($t2)   #t3 : Array[i][j]의 값
         add $t7, $t7, $t3   #a[(i + 1) % 3][(j + 2) % 3]
                  
                  
         #a[(i + 2) % 3][(j + 1) % 3]ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
         #(i + 2) % 3
         
         move $t0, $t8   #i 나머지 연산을 위해 따로 저장
         move $t1, $t9   #j 나머지 연산을 위해 따로 저장
         
         add $t0, $t0, 2   # i + 2  : 나머지 연산을 하기 위함
         div $t0, $t4        # (i + 2)을 3으로 나눈 나머지
         mfhi $t5           # temp for the mod 나머지값 저장
         move $t0 $t5   #i는  (i + 2) % 3의 값을 가짐
             
         #(j + 1) % 3
         add $t1, $t1, 1   # j + 1  : 나머지 연산을 하기 위함
         div $t1, $t4        # (j + 1)을 3으로 나눈 나머지
         mfhi $t5           # temp for the mod 나머지값 저장
         move $t1 $t5   #j는  (j + 1) % 3의 값을 가짐
             
         #t2는 Array[i][j]의 주소를 가지게 되는 공식.
         mul $t2, $t0, $a1   #t2 = rowIndex * colSize
         add $t2, $t2, $t1   #t2 = rowIndex * colSize + colindex
         mul $t2, $t2, DATA_SIZE #t2 = (rowIndex * colSize + colindex) * DATA_SIZE
         add $t2, $t2, $a0   #t2 = (rowIndex * colSize + colindex) * DATA_SIZE + base address
         
         lw $t3, ($t2)   #t3 : Array[i][j]의 값
         mul $t7, $t7, $t3   #a[(i + 1) % 3][(j + 2) % 3] * a[(i + 2) % 3][(j + 1) % 3]
         
         sub $t6, $t6, $t7   #      (a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3] - 
                       #     a[(i + 1) % 3][(j + 2) % 3] * a[(i + 2) % 3][(j + 1) % 3]);
                       
         mtc1 $t6, $f13      #t6의 값을 f13으로
         cvt.s.w $f13, $f13   #floating 연산              f13 = (float)t6
         
         div.s $f4, $f4, $f20   #f4 = 1 / determinant(a)
         
         mul.s $f14, $f4, $f13   #  f14 = 1.0 / determinant(a) {* (a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3] - a[(i + 1) % 3][(j + 2) % 3] * a[(i + 2) % 3][(j + 1) % 3]) };
         
         #여인수 확인  : (-1)을 i+j번 곱해서 각 요소에 곱해줘야함     -> i+j가 짝수면 결국 1을 곱하는거기 때문에 똑같음.
         li $t0, -1   #여인수를 만들기위한 -1
         li $t1, 2   #여인수를 위해 나머지연산을 하기위한 2
         mtc1 $t0, $f8
         cvt.s.w $f8, $f8
         add $t2, $t8, $t9   #t2 = i + j
         div $t2, $t1   #(i + j) % 2
         mfhi $t5   #t5 = 나머지
         
         bne $t5, $zero, yes   #(i + j) % 2의 나머지가 1이면 -1을 홀수 번 곱하기 때문에 결국 -임.     -1을 곱함
         j no#1을 곱함
            yes : #-1을 곱함
               mul.s $f14, $f14, $f8   #-1을 곱한 역행렬 요소 값
            
         
         no :   #나머지가 0이면 -1을 곱하지 않음(1을 곱하기때문에 결국 그대로)
         li $v0, 2   #출력
         mov.s $f12, $f14
         syscall                  
         
         li $v0, 4   #띄어쓰기
         la $a0, Empty   
         syscall        
            
         
           
         #for문 루프 판별구문 ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ    
         #역행렬은 전치행렬로 프린트해야하기때문에 i와 j를 반대로 해서 for문을 돌림
         bne $t8, 2, i_not_2   #i != 2
         j i_2   #i = 2
            
            
            i_not_2 :   #i != 2
               add $t8, $t8, 1   #i++
               j I_loop   #다음loop 반복
               
         i_2 :   #i = 2
            beq $t9, 2, ji_2   #j, i = 2
            j j_not_2_and_i_2
               
               
               ji_2 :   #j, i = 2
                  li $v0, 10   #함수종료
                  syscall
                  
            j_not_2_and_i_2 :   #j != 2, i = 2
               add $t9, $t9, 1   #j++
               li $t8, 0   #i=0
               
               li $v0, 4   #다음 행 출력을 위한 개행
               la $a0, NewLine   
               syscall
               j I_loop

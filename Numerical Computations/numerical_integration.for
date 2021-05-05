      PROGRAM MAIN
        IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      
        COMMON /part/ lines
        COMMON /interval/ a, b, step 
        COMMON /results/ result_1, result_2
    
        DIMENSION memory(100000000)
        
        CALL readData()
        CALL makePart(memory(1))
        
        CALL rectangle(memory(1))
        CALL newton_cotes(memory(1))
                
        PRINT *, 'Rectangle: ', result_1
        PRINT *, 'Newton-Cotes (n=4): ', result_2

      END
      



      DOUBLE PRECISION FUNCTION fun(x)
        IMPLICIT DOUBLE PRECISION(A-H, O-Z)

        fun = x**2
        !fun = 4*x**4
        !fun = 5*x**5
        !fun = 6*x**6
        !fun = 7*x**7
        !fun = 8*x**8
        !fun = 9*x**9
        !fun = 10*x**10

      END
      

      SUBROUTINE readData()
        IMPLICIT DOUBLE PRECISION(A-H, O-Z)
        COMMON /part/ lines
        COMMON /interval/ a, b, step 
        
        OPEN(1, FILE = 'input.txt', STATUS = 'OLD', ERR = 1)
        READ(1, *) a, b, lines
        CLOSE(1)
        
        IF (b .LT. a .OR. a .EQ. b) GOTO 2
        IF (lines .EQ. 0.0) GOTO 3
        
        step = (b - a) / lines
      
        PRINT *, 'Reading success.' 
        GOTO 4

    1   PRINT *, 'input.txt does not exist!'
        STOP
    2   PRINT *, 'Invalid segment of integration!'
        STOP
    3   PRINT *, 'Zero step!'
        STOP
    4  END
      

      SUBROUTINE makePart(part)
        IMPLICIT DOUBLE PRECISION(A-H, O-Z)
        COMMON /part/ lines
        COMMON /interval/ a, b, step 
        
        DIMENSION part(*)
        
        DO i = 0, lines
          part(i + 1) = a + i * step
        END DO

        IF (part(lines) .NE. b) THEN
          lines = lines + 1
          part(lines) = b
        END IF
        
        PRINT *, 'Partition created.'
        RETURN  
      END 


      SUBROUTINE rectangle(part)
        IMPLICIT DOUBLE PRECISION(A-H, O-Z)
        COMMON /part/ lines
        COMMON /interval/ a, b, step 
        COMMON /results/ result_1, result_2
        
        DIMENSION part(*)
        REAL rect
        rect = 0
        DO i = 1, lines - 1
          rect = rect+fun((part(i+1)-part(i))/2)*(part(i+1)-part(i))
        END DO
        result_1 = rect
      END 


      SUBROUTINE newton_cotes(part)
        IMPLICIT DOUBLE PRECISION(A-H, O-Z)
        COMMON /part/ lines
        COMMON /interval/ a, b, step 
        COMMON /results/ result_1, result_2
        
        DIMENSION part(*)
        REAL n_cotes, q
        n_cotes = 0
        q = fun(part(1))
        DO i = 1, lines - 1
          n_cotes = n_cotes+((part(i+1)-part(i))/8)*(q+3*
     & fun((2*part(i)+part(i+1))/3)+3*
     & fun((part(i)+2*part(i+1))/3))
          q = fun(part(i+1))
          n_cotes = n_cotes+((part(i+1)-part(i))/8)*q
        END DO
        result_2 = n_cotes
      END


      PROGRAM MAIN
        IMPLICIT NONE

        REAL x_min, x_max, x_step
        REAL y_min, y_max, y_step 
        COMMON /x_var/ x_min, x_max, x_step
        COMMON /y_var/ y_min, y_max, y_step
        LOGICAL input, makeTable, checkInput
        REAL round

        IF (input() .AND. makeTable()) THEN
          PRINT *, 'Done.'
          pause
        ELSE
          PRINT *, 'Something went wrong.'
          pause
        END IF
      END 



      LOGICAL FUNCTION input()
        IMPLICIT NONE
        REAL x_min, x_max, x_step
        REAL y_min, y_max, y_step
        COMMON /x_var/ x_min, x_max, x_step
        COMMON /y_var/ y_min, y_max, y_step
        LOGICAL checkInput
        
        OPEN(1, FILE = 'input.txt', STATUS = 'old', ERR = 1)
        READ(1, *, ERR = 2) x_min, x_max, x_step, 
     & y_min, y_max, y_step
        CLOSE(1)
        
        input = .TRUE.
        IF (.NOT. checkInput(x_min, x_max, x_step)) GOTO 3
        IF (.NOT. checkInput(y_min, y_max, y_step)) GOTO 4
        GOTO 5


    1   PRINT *, 'Input file does not exist!'
        PAUSE 
        STOP
    2   PRINT *, 'Invalid input!' 
        PAUSE 
        STOP
    3   PRINT *, 'Incorrect range or step size for x!'
        PAUSE 
        STOP
    4   PRINT *, 'Incorrect range or step size for y!'
        PAUSE 
        STOP

    5   END


      LOGICAL FUNCTION checkInput(r_min, r_max, step)
        IMPLICIT NONE
        REAL r_min, r_max, step, limit, round
        
         limit = 1E-4
        checkInput = .FALSE.
        IF (r_min .LE. r_max .AND. 
     & limit .LE. step .AND.ABS(r_min-round(r_min+step)).GE.limit)
!     (ABS(step/r_max)
!     *.GT.limit).OR.(ABS(step/r_min).GT.limit))       
     & checkInput = .TRUE.
      END
      

      REAL FUNCTION ff(x, y)
        IMPLICIT NONE
        REAL x, y, toRad
        IF ((MOD(ABS(y), 90.0) .LT. 1E-4) 
     &.AND. (MOD(ABS(y), 180.0) .GE. 1E-4)) THEN
          ff = ( 1 / (y - y))
        ELSE
          ff = sin(toRad(x))/cos(toRad(y))
        END IF
      END


      LOGICAL FUNCTION makeTable()
        IMPLICIT NONE
        REAL x_min, x_max, x_step
        REAL y_min, y_max, y_step 
        COMMON /x_var/ x_min, x_max, x_step
        COMMON /y_var/ y_min, y_max, y_step
        REAL i, j, i_tmp, j_tmp, i_sum, j_sum, round, f, toRad, ff
        INTEGER n, n_max, m, m_max
        
        OPEN(2, FILE = 'output.txt', STATUS = 'NEW', ERR = 1)
          WRITE(2, 9)

          i = x_min
          n = 0
          DO WHILE (i .LT. x_max)
            i = round(i)
            WRITE(2, 11) i
            n = n + 1 
            i = x_min + n * x_step
          END DO
          n_max = n 
          
          WRITE(2, 11) x_max
          WRITE(2, 6)
          WRITE(2, 7)

          n = 0
          DO WHILE (n .LE. n_max)
            WRITE(2, 8)
            n = n + 1
          END DO
          
          
          j = y_min
          m = 0
          DO WHILE (j .LT. y_max)
            WRITE(2, 6)
            j = round(j)
            WRITE(2, 10) j
            n = 0
            i = x_min
            DO WHILE (i .LT. x_max)
                f = ff(i, j)
                WRITE(2, 11) f
                n = n + 1 
                i = x_min + n * x_step
            END DO
            f = ff(i, j)
            WRITE(2, 11) f
            m = m + 1
            j = y_min + m * y_step
          END DO
          WRITE(2, 6)
          j = round(j)
          WRITE(2, 10) j
          n = 0
          i = x_min
          DO WHILE (i .LT. x_max)
            f = ff(i, j)
            WRITE(2, 11) f
            n = n + 1 
            i = x_min + n * x_step
          END DO
          f = ff(i, j)
          WRITE(2, 11) f
           
        CLOSE(2)
        makeTable = .TRUE.
        GOTO 2
        
    1   PRINT *, 'Output file already exists!'
        makeTable = .FALSE.
        GOTO 2  
  
    6   FORMAT(/1x)
    7   FORMAT(13('-'))
    8   FORMAT('|', 13('-'))
    9   FORMAT('     y\x     ')
   10   FORMAT(E12.4, ' ')
   11   FORMAT('|', E12.4, ' ')
    2 END 
       

      REAL FUNCTION toRad(degree)
        IMPLICIT NONE
        REAL degree, pi
        pi = 3.14159265
        toRad = degree * pi / 180.0
      END
      

      REAL FUNCTION round(degree)
        IMPLICIT NONE
        REAL degree
        CHARACTER str_value*11
      
        WRITE(str_value, 6) degree
        READ(str_value, *) round
        
  6   FORMAT(E11.4)  
      END
      
 

      PROGRAM MAIN
        IMPLICIT NONE

        REAL x_min, x_max, x_step
        REAL y_min, y_max, y_step 
        COMMON x_min, x_max, x_step
        COMMON y_min, y_max, y_step
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
        COMMON x_min, x_max, x_step
        COMMON y_min, y_max, y_step
        LOGICAL checkInput
        
        OPEN(1, FILE = 'input.txt', STATUS = 'old', ERR = 1)
        READ(1, *, ERR = 2) x_min, x_max, x_step, 
     & y_min, y_max, y_step
        CLOSE(1)
        
        input = .TRUE.
        IF (checkInput(x_min, x_max, x_step) .AND.
     & checkInput(y_min, y_max, y_step)) THEN 
          GOTO 5
        ELSE 
          GOTO 3
        END IF


    1   PRINT *, 'Input file does not exist!' 
        GOTO 4
    2   PRINT *, 'Invalid input!' 
        GOTO 4
    3   PRINT *, 'Incorrect range or step size!'
    4   input = .FALSE.
    5   END



      LOGICAL FUNCTION checkInput(min, max, step)
        IMPLICIT NONE
        REAL min, max, step
        
        checkInput = .FALSE.
        IF (min .LE. max .AND. 
     & step .GE. 0 .AND.
     & min + step .LE. max) 
     & checkInput = .TRUE.
      END
      

      REAL FUNCTION f(x, y)
        IMPLICIT NONE
        REAL x, y, toRad
        
        IF ((AMOD(y, 90.0) .GT. 0.0) .OR. (AMOD(y, 180.0) .EQ. 0.0))THEN
          IF (AMOD(x, 180.0) .EQ. 0.0) THEN 
            f = 0
          ELSE
            f = sin(toRad(x)) / cos(toRad(y))
          END IF
        ELSE 
          f = ( 1 / (y - y))
        END IF
      END


      LOGICAL FUNCTION makeTable()
        IMPLICIT NONE
        REAL x_min, x_max, x_step
        REAL y_min, y_max, y_step 
        COMMON x_min, x_max, x_step
        COMMON y_min, y_max, y_step
        REAL i, j, i_tmp, j_tmp, i_sum, j_sum, round, f
        
        OPEN(2, FILE = 'output.txt', STATUS = 'new', ERR = 1)

          WRITE(2, 9)

          i = x_min
          i_tmp = i

          DO WHILE (i .LT. x_max)
            i_sum = i + x_step

            IF (i_tmp .NE. round(i_sum)) THEN 
              WRITE(2, 11) i_tmp
              i_sum = round(i_sum)
              i_tmp = i_sum
            END IF 

            i = i_sum
          END DO

          WRITE(2, 11) x_max
          WRITE(2, 6)

          i = x_min
          i_tmp = i

          WRITE(2, 7)
          DO WHILE (i .LT. x_max)
            i_sum = i + x_step

            IF (i_tmp .NE. round(i_sum)) THEN 
              WRITE(2, 8)
              i_sum = round(i_sum)
              i_tmp = i_sum
            END IF

            i = i_sum
          END DO

          WRITE(2, 8)
          i = y_min
          i_tmp = i

          DO WHILE (i .LT. y_max)
            i_sum = i + y_step

            IF (i_tmp .NE. round(i_sum)) THEN      
              WRITE(2, 6)
              WRITE(2, 10) i_tmp
              j = x_min
              j_tmp = j

              DO WHILE (j .LT. x_max)
                j_sum = j + x_step

                IF (j_tmp .NE. round(j_sum)) THEN
                  WRITE(2, 11) f(j_tmp, i_tmp)
                  j_sum = round(j_sum)
                  j_tmp = j_sum
                END IF

                j = j_sum
              END DO

              WRITE(2, 11) f(x_max, i_tmp)
              
              i_sum = round(i_sum)
              i_tmp = i_sum
            END IF
            
              i = i_sum
          END DO

          WRITE(2, 6)
          WRITE(2, 10) y_max

          i = x_min
          i_tmp = i

          DO WHILE (i .LT. x_max)
            i_sum = i + x_step

            IF (i_tmp .NE. round(i_sum)) THEN
              WRITE(2, 11) f(i_tmp, y_max)
              i_sum = round(i_sum)
              i_tmp = i_sum
            END IF
            
            i = i_sum            
          END DO

          WRITE(2, 11) f(x_max, y_max) 
           
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

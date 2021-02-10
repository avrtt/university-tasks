      PROGRAM MAIN
        IMPLICIT NONE

        REAL a, a1, a2, a3, S, min_cos, min_angle, an1, an2, an3
        COMMON a, a1, a2, a3, S, min_cos, min_angle, an1, an2, an3
        INTEGER i



c     MAIN_BODY  

        DO WHILE (i .NE. 5)
          CALL menu()
          CALL input(i)
     
          SELECT CASE (i)

            CASE (1) 
              CALL setTriangle

            CASE (2)
              CALL calculateS
              IF (S .NE. 0) THEN
                PRINT *, 'S = '
                PRINT *, S
              ELSE 
                PRINT *, 'Enter the coordinates of the triangle!'
              END IF

            CASE (3)
              IF (S .NE. 0) THEN
                PRINT *, 'Min angle = '
                CALL minAngle
              ELSE 
                PRINT *, 'Enter the coordinates of the triangle!'
              END IF

            CASE (4)
              IF (S .NE. 0) THEN
                PRINT *, 'Min cos = '
                CALL minCos
              ELSE 
                PRINT *, 'Enter the coordinates of the triangle!'
              END IF

            CASE ( : 0, 6 : )
              PRINT *, 'Wrong menu item!'     

          END SELECT

        END DO
      END
      




      SUBROUTINE menu()
        IMPLICIT NONE

        PRINT *, ''
        PRINT *, 'MENU'
        PRINT *, '1. Create a new triangle;'
        PRINT *, '2. Calculate the S of the triangle;'
        PRINT *, '3. Calculate the minimum angle;'
        PRINT *, '4. Calculate the cosine of the minimum angle;'
        PRINT *, '5. Exit.'
      END
     

      SUBROUTINE input(item)
        IMPLICIT NONE
        INTEGER item

        PRINT *, ''
        PRINT *, 'Select menu item: '
        READ *, item
        PRINT *, ''
      END
       

      SUBROUTINE setTriangle()
         IMPLICIT NONE
         REAL a, a1, a2, a3, S
         COMMON a, a1, a2, a3, S
         
         DO WHILE (.TRUE.)
            PRINT '(A, $)', 'Enter the lenght of side A: '
            READ *, a

            PRINT '(A, $)', 'Enter the angles: '
            PRINT '(A, $)', 'a1, a2: '
            READ *, a1, a2
            
            
            IF ((a1+a2.GT.0) .AND. (a1+a2.LT.180) .AND. 
     *(a.GT.0) .AND. (a1.GT.0) .AND. (a2.GT.0)) THEN    

               PRINT *, ''
               PRINT *, 'Done.'
               RETURN
            END IF
            
            PRINT *, ''
            PRINT *, 'Not a triangle. Try again.'
            PRINT *, ''
            
         END DO ! WHILE (.TRUE.)
      END


      SUBROUTINE calculateS()
        IMPLICIT NONE
        REAL a, a1, a2, a3, S
        COMMON a, a1, a2, a3, S
                
        a3 = 180 - (a1 + a2)
        CALL toRad
        S = ( (a**2)/2 ) * ( (sin(a1)*sin(a2)) / (sin(a3)) )
      END


      SUBROUTINE toRad()
        IMPLICIT NONE
        REAL a, a1, a2, a3, S, pi, an1, an2, an3
        COMMON a, a1, a2, a3, S, an1, an2, an3
        PARAMETER (pi = 3.141592652)
        
        an1 = a1
        an2 = a2
        an3 = a3

        a1 = a1 * pi / 180
        a2 = a2 * pi / 180
        a3 = a3 * pi / 180        
      END


      SUBROUTINE minAngle()
        IMPLICIT NONE
        REAL a, a1, a2, a3, pi, min_angle, an1, an2, an3
        COMMON a, a1, a2, a3, pi, an1, an2, an3

        min_angle = min(an1, an2, an3)

        PRINT *, min_angle
      END


      SUBROUTINE minCos()
        IMPLICIT NONE
        REAL a1, a2, a3, min_cos, an1, an2, an3, min_angle
        COMMON a1, a2, a3, an1, an2, an3, min_angle

        min_cos = cos(min(an1, an2, an3))

        PRINT *, min_cos
      END
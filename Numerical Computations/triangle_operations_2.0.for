      PROGRAM MAIN
        IMPLICIT NONE
      
c     COMMON_VARIABLES
        REAL a, S, min_cos, min_angle, one_degree, pi
        REAL a1, a2, a3
        REAL an1, an2, an3
        COMMON a, S, min_cos, min_angle, one_degree, pi
        COMMON /angles_deg/ a1, a2, a3
        COMMON /angles_rad/ an1, an2, an3

c     LOCAL_VARIABLES
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
      


c     DEFINITION_OF_FUNCTIONS

      SUBROUTINE menu()
        IMPLICIT NONE
        PRINT *, '_'
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
        PRINT *, '_'
        PRINT *, 'Select menu item: '
        READ *, item
        PRINT *, '_'
      END
       

      SUBROUTINE setTriangle()
         IMPLICIT NONE
         
        REAL a, S, min_cos, min_angle, one_degree, pi
        REAL a1, a2, a3
        REAL an1, an2, an3
        COMMON a, S, min_cos, min_angle, one_degree, pi
        COMMON /angles_deg/ a1, a2, a3
        COMMON /angles_rad/ an1, an2, an3
         
         DO WHILE (.TRUE.)
            PRINT '(A, $)', 'Enter the lenght of side A: '
            READ *, a

            PRINT '(A, $)', 'Enter the angles: '
            PRINT '(A, $)', 'a1, a2: '
            READ *, a1, a2
            
            
            IF ((a1+a2.GT.0) .AND. (a1+a2.LT.180) .AND. 
     *(a.GT.0) .AND. (a1.GT.0) .AND. (a2.GT.0)) THEN    

               PRINT *, '_'
               PRINT *, 'Done.'
               RETURN
            END IF
            
            PRINT *, '_'
            PRINT *, 'Not a triangle. Try again.'
            PRINT *, '_'
            
         END DO ! WHILE (.TRUE.)
      END ! SUBROUTINE SetTriangle


      SUBROUTINE calculateS()
        IMPLICIT NONE
        REAL a, S, min_cos, min_angle, one_degree, pi
        REAL a1, a2, a3
        REAL an1, an2, an3
        COMMON a, S, min_cos, min_angle, one_degree, pi
        COMMON /angles_deg/ a1, a2, a3
        COMMON /angles_rad/ an1, an2, an3
                
        a3 = 180 - (a1 + a2)
        CALL toRad
        S = ( (a**2)/2 ) * ( (sin(a1)*sin(a2)) / (sin(a3)) )

      END


      SUBROUTINE toRad()
        IMPLICIT NONE
        REAL a, S, min_cos, min_angle, one_degree, pi
        REAL a1, a2, a3
        REAL an1, an2, an3
        COMMON a, S, min_cos, min_angle, one_degree
        COMMON /angles_deg/ a1, a2, a3
        COMMON /angles_rad/ an1, an2, an3
        PARAMETER (pi = 3.141592652)
        
        one_degree = pi/180

        an1 = a1
        an2 = a2
        an3 = a3

        a1 = a1 * one_degree
        a2 = a2 * one_degree
        a3 = a3 * one_degree
        
      END


      SUBROUTINE minAngle()
        IMPLICIT NONE
        REAL a, S, min_cos, min_angle, one_degree, pi
        REAL a1, a2, a3
        REAL an1, an2, an3
        COMMON a, S, min_cos, min_angle, one_degree, pi
        COMMON /angles_deg/ a1, a2, a3
        COMMON /angles_rad/ an1, an2, an3

        min_angle = min(an1, an2, an3)

        PRINT *, min_angle

      END


      SUBROUTINE minCos()
        IMPLICIT NONE
        REAL a, S, min_cos, min_angle, one_degree, pi
        REAL a1, a2, a3
        REAL an1, an2, an3
        COMMON a, S, min_cos, min_angle, one_degree, pi
        COMMON /angles_deg/ a1, a2, a3
        COMMON /angles_rad/ an1, an2, an3

        min_cos = cos(min(a1, a2, a3))

        PRINT *, min_cos

      END
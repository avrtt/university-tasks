      PROGRAM MAIN
        COMMON /size/ N, nodes 
        DIMENSION mem(1000000)

!        matrix_size = 2
!        CALL makeTest(matrix_size)
        CALL toDirect(iA, Di, Al, V)
!        CALL toSequential()
  

!        CALL readSizes(1000000)
        CALL readSizesDirect(1000000)

!        CALL readMatrix(mem(1), mem(N + 2), mem(2 * N + 3),
!     & mem(2 * N + nodes + 4))
        CALL readMatrixDirect(iA, Di, Al)
     
!        CALL readVector(mem(2 * N + 2 * nodes + 5))
        CALL readVectorDirect(V)
           
        CALL mult(mem(1), mem(N + 2), mem(2 * N + 3),
     & mem(2 * N + nodes + 4), mem(2 * N + 2 * nodes + 5),
     & mem(3 * N + 2 * nodes + 6))

      END



! MAIN FUNCTIONS

      SUBROUTINE readSizes(mem_size)
        COMMON /size/ N, nodes 
        
        OPEN(1, FILE = 'matrix_size.txt', STATUS = 'OLD', ERR=1)
        READ(1, *) N
        CLOSE(1)
        
        OPEN(1, FILE = 'vector_size.txt', STATUS = 'OLD', ERR=2)
        READ(1, *) Nv
        CLOSE(1)
        
        IF (N .NE. Nv) GOTO 3
        OPEN(1, FILE = 'ia.txt', STATUS = 'OLD', ERR = 4)
        READ(1, *) (values, i = 1, N + 1)
        nodes = INT(values) - 1
        CLOSE(1)  
          
        IF (mem_size .LT. (4 * N + 2 * nodes + 2 + 100)) GOTO 5
        PRINT *, 'Sizes reading success.' 
        RETURN

    1   PRINT *, 'matrix_size.txt file does not exist!'
    2   PRINT *, 'vector_size.txt does not exist!'
    3   PRINT *, 'Incorrect size of matrix/vector!'
    4   PRINT *, 'ia.txt does not exist!'
    5   PRINT *, 'Out of memory!'
      END
      

      SUBROUTINE readMatrix(iA, Di, Al)
        COMMON /size/ N, nodes 
        DIMENSION iA(*), Di(*), Al(*)
        
        OPEN(1, FILE = 'ia.txt', STATUS = 'OLD', ERR = 1)
        READ(1, *) (iA(i), i = 1, N + 1)
        CLOSE(1)

        OPEN(1, FILE = 'di.txt', STATUS = 'OLD', ERR = 2)
        READ(1, *) (Di(i), i = 1, N)
        CLOSE(1)

        OPEN(1, FILE = 'al.txt', STATUS = 'OLD', ERR = 3)
        READ(1, *) (Al(i), i = 1, nodes)
        CLOSE(1)

        PRINT *, 'Matrix reading success.'
        RETURN 
        
    1   PRINT *, 'ia.txt does not exist!'
    2   PRINT *, 'di.txt does not exist!'
    3   PRINT *, 'al.txt does not exist!' 
      END      


      SUBROUTINE readVector(V)
        COMMON /size/ N, nodes 
        
        DIMENSION V(*)
        
        OPEN(1, FILE = 'vector.txt', STATUS = 'OLD', ERR = 1)
        READ(1, *) (V(i), i = 1, N)
        CLOSE(1)
        PRINT *, 'Vector reading success.'
        RETURN 
        
    1   PRINT *, 'vector.txt does not exist!'
      END
      

      SUBROUTINE mult(iA, Di, Al, Au, V, V_res)
        COMMON /size/ N, nodes 
        DIMENSION iA(*), Di(*), Al(*), Au(*), V(*), V_res(*)
        INTEGER z, l 
        
        DO i = 1, N
          V_res(i) = Di(i)*V(i)
        END DO
        
       
        DO i = 2, N
          l = 0
          k = iA(i + 1) - iA(i)
          DO j = i - k, i - 1 
            V_res(i) = V_res(i) + Al(ia(i) + l)*V(j)
            V_res(j) = V_res(j) + Al(iA(i) + l)*V(i)
            l = l + 1 
          END DO 
        END DO
        
!        DO i = 2, N
!          l = 0
!          k = iA(i + 1)- iA(i)
!          DO z = i - k, i - 1
!            V_res(z) = V_res(z) + Al(iA(i) + l)*V(i)
!            l = l + 1
!          END DO
!        END DO 
        
        CALL multResult(V_res)
      END
      

      SUBROUTINE multResult(V_res)
        COMMON /size/ N, nodes 
        DIMENSION V_res(*)
        
        OPEN(1, FILE = 'mult_result.txt')
        WRITE(1, 100) (V_res(i), i = 1, N)
        CLOSE(1)
        PRINT *, 'Multiplication success.'
        RETURN
        
  100   FORMAT(E11.4, ' ')        
      END



! DIRECT ACCESS FUNCIONS

      SUBROUTINE readSizesDirect(mem_size)
        COMMON /size/ N, nodes 
        
        OPEN(1, FILE = 'matrix_size.txt', STATUS = 'OLD', ERR=1)
        READ(1, *) N
        CLOSE(1)
        
        OPEN(1, FILE = 'vector_size.txt', STATUS = 'OLD', ERR=2)
        READ(1, *) Nv
        CLOSE(1)
        
        IF (N .NE. Nv) GOTO 3
        OPEN(1, FILE = 'ia.dat', ACCESS='DIRECT', RECL=4, ERR=4)
        READ(1, REC = N + 1) values
        nodes = INT(values) - 1
        CLOSE(1)  
          
        IF (mem_size .LT. (4 * N + 2 * nodes + 2 + 100)) GOTO 5
        PRINT *, 'Sizes reading success.' 
        RETURN

    1   PRINT *, 'matrix_size.txt file does not exist!'
        PAUSE
        STOP
    2   PRINT *, 'vector_size.txt does not exist!'
        PAUSE
        STOP
    3   PRINT *, 'Incorrect size of matrix/vector!'
        PAUSE
        STOP
    4   PRINT *, 'ia.dat does not exist!'
        PAUSE
        STOP
    5   PRINT *, 'Out of memory!'
        PAUSE
        STOP
      END
      

      SUBROUTINE readMatrixDirect(iA, Di, Al)
        COMMON /size/ N, nodes 
        DIMENSION iA(*), Di(*), Al(*)
        
        OPEN(1, FILE='ia.dat', ACCESS='DIRECT', RECL=4, ERR=1)
        DO i = 1, N + 1
          READ(1, REC = i) iA(i)
        END DO
        CLOSE(1)

        OPEN(1, FILE='di.dat', ACCESS='DIRECT', RECL=4, ERR=2)
        DO i = 1, N
          READ(1, REC = i) Di(i)
        END DO
        CLOSE(1)

        OPEN(1,FILE='al.dat',ACCESS='DIRECT',RECL=4,ERR=3)
        DO i = 1, nodes
          READ(1, REC = i) Al(i)
        END DO
        CLOSE(1)

        PRINT *, 'Matrix reading success.'
        RETURN 
        
    1   PRINT *, 'ia.txt does not exist!'
        PAUSE
        STOP
    2   PRINT *, 'di.txt does not exist!'
        PAUSE
        STOP
    3   PRINT *, 'al.txt does not exist!'
        PAUSE
        STOP
      END


      SUBROUTINE readVectorDirect(V)
        COMMON /size/ N, nodes 
        
        DIMENSION V(*)
        
        OPEN(1, FILE='vector.dat', ACCESS='DIRECT', RECL=4, ERR=1)
        DO i = 1, N
          READ(1, REC = i) V(i)
        END DO
        CLOSE(1)

        PRINT *, 'Vector reading success.'
        RETURN 
        
    1   PRINT *, 'vector.dat does not exist!'
        PAUSE
        STOP
      END



! GENERATOR

      SUBROUTINE makeTest(matrix_size)
        CALL createMatrix(matrix_size)
        CALL createVector(matrix_size)
      END

      SUBROUTINE createMatrix(matrix_size) 
        
      END
      
      SUBROUTINE createVector(matrix_size)
        
      END



! CONVERSION FUNCTIONS

      SUBROUTINE toDirect(iA, Di, Al, V)
        COMMON /size/ N, nodes 
        DIMENSION iA(*), Di(*), Al(*)
        
        CALL matrixToDir(iA, Di, Al)
        CALL vectorToDir(V)
      END
      
      SUBROUTINE toSequential() 
        CALL matrixToSeq()
        CALL vectorToSeq()
      END

      
      SUBROUTINE matrixToDir(iA, Di, Al)
        COMMON /size/ N, nodes 
        
        DIMENSION iA(*), Di(*), Al(*)
        
        OPEN(1, FILE = 'ia.txt', STATUS = 'OLD', ERR = 1)
        OPEN(2, FILE='ia.dat', ACCESS='DIRECT', RECL=4)
        READ(1, *) (iA(i), i = 1, N + 1)
        DO i = 1, N + 1
          WRITE(2, REC = i) iA(i)
        END DO 
        CLOSE(1)
        CLOSE(2)
        
        OPEN(1, FILE = 'di.txt', STATUS = 'OLD', ERR = 2)
        OPEN(2, FILE='di.dat', ACCESS='DIRECT', RECL=4)
        READ(1, *) (Di(i), i = 1, N)
        DO i = 1, N
          WRITE(2, REC = i) Di(i)  
        END DO 
        CLOSE(1)
        CLOSE(2)
        
        OPEN(1, FILE = 'al.txt', STATUS = 'OLD', ERR = 3)
        OPEN(2, FILE='al.dat', ACCESS='DIRECT', RECL=4)
        READ(1, *) (Al(i), i = 1, nodes)
        DO i = 1, nodes
          WRITE(2, REC = i) Al(i) 
        END DO 
        CLOSE(1)
        CLOSE(2)

        PRINT *, 'Sequential to direct MATRIX convertion success.'
        RETURN  

    1   PRINT *, 'ia.txt does not exist!'
        PAUSE
        STOP
    2   PRINT *, 'di.txt does not exist!'
        PAUSE
        STOP
    3   PRINT *, 'al.txt does not exist!'
        PAUSE
        STOP

      END

      
      SUBROUTINE matrixToSeq()
        COMMON /size/ N, nodes

        OPEN(1, FILE='ia.dat', ACCESS='DIRECT', RECL=4)
        OPEN(2, FILE = 'ia_1.txt')
        DO i = 1, N + 1
          READ(1, REC = i) tmp
          WRITE(2, 100) tmp 
        END DO 
        CLOSE(1)
        CLOSE(2)
        
        OPEN(1, FILE='di.dat', ACCESS='DIRECT', RECL=4)
        OPEN(2, FILE = 'di_1.txt')
        DO i = 1, N
          READ(1, REC = i) tmp
          WRITE(2, 100) tmp 
        END DO 
        CLOSE(1)
        CLOSE(2)  
      
        OPEN(1, FILE='al.dat', ACCESS='DIRECT', RECL=4)
        OPEN(2, FILE = 'matrix_Al_1.txt')
        DO i = 1, nodes
          READ(1, REC = i) tmp
          WRITE(2, 100) tmp 
        END DO 
        CLOSE(1)
        CLOSE(2)

        PRINT *, 'Direct to sequential MATRIX convertion success.'
        RETURN
        
  100   FORMAT(E11.4, ' ')
      END


      SUBROUTINE vectorToDir(V)
        COMMON /size/ N, nodes 
        
        DIMENSION V(*)
      
        OPEN(1, FILE = 'vector.txt', STATUS = 'OLD', ERR = 1)
        OPEN(2, FILE = 'vector.dat', ACCESS = 'DIRECT', RECL = 4)
        READ(1, *) (V(i), i = 1, N)
        DO i = 1, N
          WRITE(2, REC = i) V(i) 
        END DO 
        CLOSE(1)
        CLOSE(2)
        PRINT *, 'Sequential to direct VECTOR convertion success.'
        RETURN
        
    1   PRINT *, 'vector.txt does not exist!'
        PAUSE
        STOP
      END
      

      SUBROUTINE vectorToSeq()
        COMMON /size/ N, nodes

        OPEN(1, FILE = 'vector.dat', ACCESS = 'DIRECT', RECL = 4)
        OPEN(2, FILE = 'vector_1.txt')
        DO i = 1, N
          READ(1, REC = i) tmp
          WRITE(2, 100) tmp
        END DO 
        CLOSE(1)    
        CLOSE(2)
        PRINT *, 'Direct to sequential VECTOR convertion success.'
        RETURN

  100   FORMAT(E11.4, ' ')
      END
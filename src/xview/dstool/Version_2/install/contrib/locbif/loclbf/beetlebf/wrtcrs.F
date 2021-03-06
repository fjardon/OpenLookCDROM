C  -------------------------------------------------------------------
C
C	Copyright (1990-1995) by Alexander Khibnik, Yuri Kuznetsov, and 
C	Eugene Nikolaev.
C
C The Locbif computation code in DsTool is distributed in the hope that it  
C will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty  
C of FITNESS FOR A PARTICULAR PURPOSE.  The software is provided as is without 
C any obligation on the part of Locbif authors, Cornell faculty, 
C Cornell staff or Cornell students to assist in its use, correction, 
C modification or enhancement. 
C
C  -------------------------------------------------------------------

      SUBROUTINE WRTCRS (IPRTCL,IWRT,IPRCRS,ITER,ITNWTZ,ITEND,NCAN,NX,
     A                   NDIM,EPSCRS,EPSR,DETZ,CONDZ,XCRS,XABD,DFUN)
C     3.05.90�.
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA WRTCRS �PE�HA�HA�EHA ��� B�BO�A �POTOKO�A �TO�HEH��    *
C*    TO�K� CAMO�EPECE�EH�� KP�BO�.                                    *
C*                                                                     *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2(I-N)
      DIMENSION XCRS(1), XABD(1), DFUN(NDIM,NX), ITNWTZ(1)
C
#if defined(hib)
      IF (IPRCRS .LT. 0) RETURN
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2020)
          WRITE (NCAN,2010)
          WRITE (NCAN,2030)      EPSR
          IF (IPRTCL .NE. 1)     WRITE (NCAN,2040) EPSCRS
          IF (IPRTCL .EQ. 2)     WRITE (NCAN,2050)
          IF (IPRTCL .EQ. 3)     WRITE (NCAN,2060)
          IF (IPRCRS .LT. 1)     GOTO 200
C
C.... CTAH�APTHOE COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���.
C
              IDIV = 1
              IPR  = 2
              IF (IPRTCL .EQ. 3) CALL WRTOUT(IWRT,ISTEP,IPR,ITEND,IDIV,
     A                                       NCAN,NX,NDIM,XABD,DFUN)
              IF (IPRTCL .GT. 1) WRITE (NCAN,3050)
              WRITE (NCAN,2010)
              WRITE (NCAN,2010)
              WRITE (NCAN,2080)  DETZ
              WRITE (NCAN,2090)  CONDZ
              WRITE (NCAN,2010)
              WRITE (NCAN,3000)  ITER
              WRITE (NCAN,3010)  (ITNWTZ(I), I=1,ITER)
 200      CONTINUE
          WRITE (NCAN,2010)
          WRITE (NCAN,3020)
          WRITE (NCAN,2010)
          WRITE (NCAN,3030)      (XCRS(I),I=1,NX)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          WRITE (NCAN,3040)
#endif
      RETURN
C
#if defined(hib)
 2000 FORMAT
     A(1X,120('-'))
C
 2010 FORMAT
     A(1X)
C
 2020 FORMAT
     A('   �PO��EHA TO�KA CAMO�EPCE�EH��.'/
     B '   ------------------------------')
C
 2030 FORMAT
     A('   �OCT��H�TA TO�HOCT�:              ',G14.5)
C
 2040 FORMAT
     A('   �A�AHHA� TO�HOCT� HE �OCT��H�TA:  ',G14.5)
C
 2050 FORMAT
     A('   �PO�E�AHO MAKC�MA��HOE ��C�O �A�OB- 10.')
C
 2060 FORMAT
     A('   HE �TO�HEHO O�EPE�HOE �P�����EH�E K   '
     B/'   TO�KE CAMO�EPECE�EH��.')
C
 2080 FORMAT
     A('   �HA�EH�E O�PE�E��TE��  (DET):     ',G14.6)
C
 2090 FORMAT
     A('   O�EHKA O��C�OB�EHHOCT� (COND):    ',G14.6)
C
 3000 FORMAT
     A('   �PO�E�AHO �A�OB �TO�HEH��:        ',I4)
C
 3010 FORMAT
     A('   �PO�E�AHO H��TOHOBCK�X �TEPA���   '
     B/'   HA BCEX �A�AX:                    ',2X,10(I3,1X))
C
 3020 FORMAT
     A('   KOOP��HAT� TO�K� HA KP�BO�:       ')
C
 3030 FORMAT
     A(10(2X,7(G14.5,1X)/))
C
 3040 FORMAT
     A(1X,120('.'))
C
 3050 FORMAT
     A(1X,120('-'))
C
#endif
      END

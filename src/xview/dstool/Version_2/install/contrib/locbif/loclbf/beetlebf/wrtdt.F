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

C
      SUBROUTINE WRTDT (NX,NG,IADD,X0,HV0,EPS,FUN,IER,ISTOP)
C     3.05.90�.
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  WRTDT �PE�HA�HA�EHA ��� �POBEPK� �PAB���HOCT�         *
C*    �A�AH�� �OPMA��H�X �APAMETPOB �PO�PAMM�  BEETLE � B�BO�A �X      *
C*    HA �E�AT�.                                                       *
C*                                                                     *
C*  �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X HEO�XO��MO:           *
C*      1) ��MEH�T� PA�MEPHOCT� MACC�BOB:FX0(NXC-1) � IVARS(NXC);      *
C*      2) ��MEH�T� �HA�EH�E �APAMETPA NXC;                            *
C*  �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �CC�E��EM�X ��HK��� HEO�XO��MO:  *
C*      1) ��MEH�T� PA�MEPHOCT� MACC�BA IFUNS(NUFC);                   *
C*      2) ��MEH�T� �HA�EH�E �APAMETPA  NUFC;                          *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      EXTERNAL FUN
      DIMENSION X0(1), FX0(NDIM1), IADD(1)
C
      COMMON /ADD1A/ IPR, NSTEP, NCAN
      COMMON /ADD1B/ DX, HVMN, HVMX
      COMMON /ADD2A/ ICRS, IPRCRS
      COMMON /ADD2B/ EPSCRS
      COMMON /ADD3A/ NUF, IPRZER, IFUNS(IFMAX)
      COMMON /ADD3B/ EPSZER
      COMMON /ADD4A/ NUE, IPREXT, IVARS(NDIM)
      COMMON /ADD4B/ EPSEXT
      COMMON /ADD5A/ PXGMN, PXGMX
      COMMON /ADD6A/  XGMN,  XGMX
      COMMON /ADD7A/ EPSNWT
      COMMON /ADD7B/ ITNWT
C
C.... HA�HA�EH�E �HA�EH�� �APAMETPOB, �A�AHH�X �O �MO��AH��.
C
      ISTOP= 0
      NXC  = NDIM
      NUFC = IFMAX
C.... 1) �APAMETP�, OTHOC���EC� K �B��EH�� �O KP�BO�:
C
      IPR   = 0
      NSTEP = 1000
      NCAN  = 6
      DX    = 1.D-07
      HVMN  = DABS(HV0)*1.D-02
      HVMX  = DABS(HV0)*1.D+02
C
C.... 2) �APAMETP�, OTHOC���EC� K �TO�HEH�� TO�EK CAMO�EPECE�EH��:
C
      ICRS   = 1
      IPRCRS = 0
      EPSCRS = 10.D0*EPS
C
C.... 3) �APAMETP�, OTHOC���EC� K �TO�HEH�� H��E� �A�AHH�X ��HK���:
C
      NUF    = 0
      IPRZER = 0
      EPSZER = 10.D0*EPS
      DO 220 I=1,NUFC
         IFUNS(I) = I
 220  CONTINUE
C
C.... 4) �APAMETP�, OTHOC���EC� K �TO�HEH�� �KCTPEM�MOB �A�AHH�X
C        KOOP��HAT:
C
      NUE    = 0
      IPREXT = 0
      EPSEXT = 10.D0*EPS
      DO 260 I=1,NXC
         IVARS(I) = I
 260  CONTINUE
C
C.... 5) �APAMETP�, OTHOC���EC� K �PO�E��PE �TO�HEH�� �EPBO� TO�K�
C        HA KP�BO�:
C
      PXGMN = X0(NG) 
      PXGMX = X0(NG) 
C
C.... 6). �APAMETP�, OTHOC���EC� K �A�AH�� �O��CT�MO�O �HTEPBA�A
C         ��MEHEH�� ��� B��E�EHHO� �EPEMEHHO�.
C
      XGMN = X0(NG) - 100.D0
      XGMX = X0(NG) + 100.D0
C
C.... 7). �APAMETP�, OTHOC���EC� K �PO�E��PE H��TOHA:
C
      EPSNWT = EPS
      ITNWT  = 10
C
C.... KOPPEKT�POBKA �APAMETPOB, �A�AHH�X �O �MO��AH��, �O���OBATE�EM.
C
      CALL BDATA
C
      IF (IADD(3) .EQ. 0) NUF = 0
      IF (IADD(4) .EQ. 0) NUE = 0
C
C.... �E�AT� �A�AHH�X �APAMETPOB.
C
      IF (IPR .EQ. -2) RETURN
C
C..1. �APAMETP�, OTHOC���EC� K �TO�HEH�� TO�EK HA KP�BO�.
C
      IER    =-1
#if defined(hib)
      WRITE (NCAN,1000)
      WRITE (NCAN,1001) NX
#endif
         IF (NX.LT.1 .OR. NX.GT.NXC)                     RETURN
#if defined(hib)
      WRITE (NCAN,1002) NG
#endif
         IF (NG.LT.1 .OR. NG.GT.NXC)                     RETURN
#if defined(hib)
      WRITE (NCAN,1004) (X0(I),I=1,NX)
#endif
      IERR = 0
      CALL FUN (X0,FX0,IERR,ISTOP)
      IF (ISTOP .NE. 0) RETURN
      IF (IERR  .EQ. 0) GOTO 500
#if defined(hib)
          WRITE (NCAN,3090)
#endif
          RETURN
 500  CONTINUE
      NFUN = NX - 1
#if defined(hib)
      WRITE (NCAN,1005) (FX0(I),I=1,NFUN)
      WRITE (NCAN,1007) HV0
#endif
         IF (DABS(HV0) .LE. 1.D-14)                      RETURN
#if defined(hib)
      WRITE (NCAN,1010) EPS
#endif
         IF (     EPS  .LE. 1.D-14)                      RETURN
      IF (IADD(1) .EQ. 0) GOTO 502
#if defined(hib)
          WRITE (NCAN,2000)
          WRITE (NCAN,2020)
          WRITE (NCAN,2000)
          WRITE (NCAN,1012) NSTEP
#endif
             IF (NSTEP .GE. 10000)                       RETURN
#if defined(hib)
          WRITE (NCAN,2030) NCAN
          WRITE (NCAN,1011) DX
#endif
             IF (DABS(DX) .LE. 1.D-14)                   RETURN
#if defined(hib)
          WRITE (NCAN,1008) HVMN
          WRITE (NCAN,1009) HVMX
#endif
          AHV0   = DABS(HV0)
             IF (.NOT.(HVMN.LE.AHV0 .AND. AHV0.LE.HVMX)) RETURN
 502  CONTINUE
C
C..2. �APAMETP� OTHOC���EC� K �TO�HEH�� TO�EK CAM��EPECE�EH��.
C
          IF (ICRS .EQ. 0) GOTO 510
#if defined(hib)
              WRITE (NCAN,2010)
              WRITE (NCAN,2080) EPSCRS
#endif
              IF (EPSCRS .LE. 1.D-14)                    RETURN
 510  CONTINUE
C
C..3. �APAMETP�, OTHOC���EC� K �TO�HEH�� H��E� ��HK���.
C
      IF (IADD(3) .EQ. 0) GOTO 520
#if defined(hib)
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          WRITE (NCAN,2040) NUF
#endif
             IF (NUF .GT. NUFC)                          RETURN
#if defined(hib)
          WRITE (NCAN,2090) EPSZER
#endif
             IF (EPSZER .LE. 1.D-14)                     RETURN
#if defined(hib)
          IF (NUF .NE. 0) WRITE (NCAN,2050) (IFUNS(I),I=1,NUF)
#endif
          DO 518 I=1,NUFC
            IF (IFUNS(I) .LE. 0) RETURN
 518      CONTINUE
 520  CONTINUE
C
C..4. �APAMETP�, OTHOC���EC� K �TO�HEH�� �KCTPEM�MOB.
C
      IF (IADD(4) .EQ. 0) GOTO 530
#if defined(hib)
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          WRITE (NCAN,2060) NUE
#endif
             IF (NUE .GT. NXC)                           RETURN
#if defined(hib)
          WRITE (NCAN,3000) EPSEXT
#endif
             IF (EPSEXT .LE. 1.D-14)                     RETURN
#if defined(hib)
          IF (NUE .NE. 0) WRITE (NCAN,2070) (IVARS(I),I=1,NUE)
#endif
          DO 528 I=1,NXC
             IF (IVARS(I) .LE. 0) RETURN
 528      CONTINUE
 530  CONTINUE
C
C..5. �APAMETP�, OTHOC���EC� K �TO�HEH�� �EPBO� TO�K� HA KP�BO�.
C
      IF (IADD(5) .EQ. 0) GOTO 540
#if defined(hib)
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          WRITE (NCAN,3010) PXGMN, PXGMX
#endif
          IF (.NOT.(PXGMN.LE.X0(NG) .AND. X0(NG).LE.PXGMX)) RETURN
 540  CONTINUE
C
C..6. �APAMETP�, OTHOC���EC� K �A�AH�� �O��CT�MO�O �HTEPBA�A
C     ��MEHEH�� ��� B��E�EHHO� �EPEMEHHO�.
C
      IF (IADD(6) .EQ. 0) GOTO 550
#if defined(hib)
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          WRITE (NCAN,1006) XGMN, XGMX
#endif
          IF (.NOT.(XGMN.LE.X0(NG) .AND. X0(NG).LE.XGMX)) RETURN
 550  CONTINUE
C
C..7. �APAMETP�, OTHOC���EC� K �PO�E��PE H��TOHA:
C
      IF (IADD(7) .EQ. 0) GOTO 560
#if defined(hib)
          WRITE (NCAN,2000)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          WRITE (NCAN,3020) EPSNWT, ITNWT
#endif
 560  CONTINUE
#if defined(hib)
      WRITE (NCAN,1014)
#endif
      IER    = 0
      RETURN
C
C.... �OPMAT� COO��EH��.
#if defined(hib)
C
 1000 FORMAT
     A(////' ',119('=')//
     B '   �HA�EH�� �APAMETPOB �PO�PAMM�  BEETLE :'/
     C '   -------------------------------------  ' )
C
 1001 FORMAT
     A('   ��C�O �EPEMEHH�X:                      ',I5/)
C
 1002 FORMAT
     A('   HOMEP B��E�EHHO� �EPEMEHHO�:           ',I5/)
C
 1004 FORMAT
     A('   KOOP��HAT� HA�A��HO�O �P�����EH�� X0:  '/
     B10(2X,7(G14.5,1X)/))
C
 1005 FORMAT
     A(/'   �HA�EH�� ��HK��� B TO�KE X0:          '/
     B10(2X,7(G14.5,1X)/))
C
 3090 FORMAT
     A('   HE B���C�EH� �HA�EH�� ��HK��� B TO�KE X0')
C
 1006 FORMAT
     A(/'   �HTEPBA� ��MEHEH�� B��E�EHHO� �EPEMEHHO�:'/
     B  '   M�H�MA��HOE �HA�EH�E:                 ',G14.5/
     C  '   MAKC�MA��HOE �HA�EH�E:                ',G14.5/)
C
 1007 FORMAT
     A('   HA�A��H�� �A� �B��EH�� �O KP�BO�:      ',G14.5/)
C
 1008 FORMAT
     A('   M�H�MA��H�� �A� �B��EH�� �O KP�BO�:    ',G14.5/)
C
 1009 FORMAT
     A('   MAKC�MA��H�� �A� �B��EH�� �O KP�BO�:   ',G14.5)
C
 1010 FORMAT
     A('   TO�HOCT� O�PE�E�EH�� TO�K� HA KP�BO�:  ',G14.5)
C
 1011 FORMAT
     A('   �A� ��C�EHHO�O ����EPEH��POBAH��:      ',G14.2/)
C
 1012 FORMAT
     A('   MAKC�MA��HOE ��C�O �A�OB B�O�� KP�BO�: ',I5/)
C
 1014 FORMAT
     A(//1X,119('=')//)
C
 2000 FORMAT
     A(1X)
C
 2010 FORMAT
     A(1X,120('.'))
C
 2020 FORMAT
     A('   �HA�EH�� �APAMETPOB �O�O�H�TE��H�X BO�MO�HOCTE�: '/
     B '   ------------------------------------------------ ')
C
 2030 FORMAT
     A('   HOMEP KAHA�A ��� B�BO�A PE����TATOB:   ',I5/)
C
 2040 FORMAT
     A('   ��C�O �CC�E��EM�X ��HK��� HA KP�BO�:   ',I5/)
C
 2050 FORMAT
     A('   �OP��OK �TO�HEH�� H��E�:      '/2(30(1X,I3))/)
C
 2060 FORMAT
     A('   ��C�O �CC�E��EM�X  KOOP��HAT:          ',I5/)
C
 2070 FORMAT
     A('   �OP��OK �TO�HEH�� �KCTPEM�MOB:'/2(30(1X,I3))/)
C
 2080 FORMAT
     A('   TO�HOCT� �TO�HEH�� TO�EK CAMO�EPECE�EH��:',G14.5)
C
 2090 FORMAT
     A('   TO�HOCT� �TO�HEH�� KOOP��HAT H��E�:    ',G14.5)
C
 3000 FORMAT
     A('   TO�HOCT� �TO�HEH�� KOOP��HAT �KCTPEM�MOB:',G14.5)
C
 3010 FORMAT
     A('   �P� �TO�HEH�� �EPBO� TO�K� HA KP�BO� - TECT�P�ETC�'/
     B '   �HTEPBA�:  ',G14.5,'< X0(NG)<',G14.5)
C
 3020 FORMAT
     A( '   MATP��A �ACTH�X �PO��BO�H�X HE B���C��ETC�, EC��:'
     B /'   �OCT����TA TO�HOCT� :           ',G14.5
     C /'   �PO�E�AHO H��TOHOBCK�X �TEPA���:',I5)
#endif
      END

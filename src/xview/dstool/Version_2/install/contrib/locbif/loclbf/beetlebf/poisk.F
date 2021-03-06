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

      SUBROUTINE POISK (NX,NFIX,NCAN,IPR,X0,EPS,FUN,DFDX,IER,ISTOP)
C     3.05.90�.
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  POISK  �PE�HA�HA�EHA ��� �TO�HEH�� �EPBO� TO�K� HA    *
C* KP�BO� �O �A�AHHOM� HA�A��HOM� �P�����EH��. CTPATE��� �TO�HEH��     *
C* O�EH� �POCTA:O��ACT� ��MEHEH�� B��E�EHHO� �EPEMEHHO� X(NG)          *
C* �OKP�BAETC� CETKO� C HEKOTOP�M �A�OM. ��CT� X0<1>,X0<2>,...,X0<M>-  *
C* ���� CETK�. HA��HA� C HEKOTOPO�O ���A X0<K>=X0(NG) �EPE��PA�TC�     *
C* BCE ���� B C�E����EM �OP��KE:X0<K>,X0<K-1>,X0<K+1>,X0<K-2> � T.�.   *
C* �P��EM B KA��OM "���E" �PO��BO��TC� �EPE�OP BCEBO�MO�H�X            *
C* ���EP��OCKOCTE� C��CKA �O TEX �OP �OKA TO�KA HA KP�BO� HE ���ET     *
C* HA��EHA, ���O HE �POBEP�TC� BCE "����" CETK� �O �EPEMEHHO� X(NG).   *
C* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
C*                                                                     *
C*    C��COK �OPMA��H�X �APAMETPOB:                                    *
C*                                                                     *
C*    NX   -PA�MEPHOCT� �POCTPAHCTBA �EPEMEHH�X.                       *
C*    NFIX -HA BXO�E B �PO�PAMM� - HOMEP B��E�EHO� �EPEMEHHO�;         *
C*          HA B�XO�E-HOMEP ��KC�POBAHHO� �EPEMEHHO� �TO�HEHHO� TO�K�. *
C*    X0   -HA�A��HOE �P�����EHH�E K �EPBO� TO�KE HA KP�BO�.           *
C*          (HA B�XO�E �� �PO�PAMM� KOOP��HAT� �TO�HEHHO� TO�K�)       *
C*    EPS  -TO�HOCT� O�PE�E�EH�� TO�K� HA KP�BO�.                      *
C*    FUN  -�M� �O��PO�PAMM� ��HK���.                                  *
C*    DFDX -�M� �PO�PAMM� B���C�EH�� MATP��� �ACTH�X   �P��BO�H�X;     *
C*    IER  -KO� BO�BPATA (IER=0 -COOTBETCTB�ET HOPMA��HOM�             *
C*                        �ABEP�EH�� PA�OT� �PO�PAMM�).                *
C*    ISTOP-                                                           *
C* ................................................................... *
C*    B���BAEM�E �O��PO�PAMM�: NEWTN.                                  *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C* �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X NDIM -HEO�XO��MO ��ME- *
C* H�T� PA�MEPHOCT� MACC�BOB XOLD(NDIM),DFUN(NDIM,NDIM),FUNS(NDIM-1) � *
C* �HA�EH�E �APAMETPA NDIM.                                            *
C***********************************************************************
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      DIMENSION X0(1), XOLD(NDIM), FUNS(NDIM1)
      EXTERNAL FUN, DFDX
      COMMON /ADD5A/ PXGMN, PXGMX
      COMMON /ADD6A/  XGMN,  XGMX
      COMMON /ALL2/ IPOISK
      COMMON /ALL3/  IBND
      COMMON /CNWT1/ IPNT1
C
      COMMON /WORKBE/ DFUN(NDIM,NDIM), ARR2(NDIM,NDIM)
C
      ISTOP = 0
      IPNT1 = 1
C
C.... O�PE�E�EH�E BC�OMO�ATE��H�X �APAMETPOB.
C
      NG   = NFIX
      NFUN = NX - 1
      DO 100 I=1,NX
        XOLD(I)=X0(I)
 100  CONTINUE
      DELTA = 0.1D0*(PXGMX - PXGMN)
      XC    = X0(NG)
C
      IF (DELTA.NE.0.D0) GOTO 105  
        MEND = 1
        IND  = -1
        GOTO 108
 105  CONTINUE
C                
      ML    = (XC - PXGMN)/DELTA + 2
      MR    = (PXGMX - XC)/DELTA + 2
      MEND  = MAX0 (ML,MR)
      IND   = -1
      IF (ML .GT. MR) IND = 1
C
 108  CONTINUE
C
      NDR   = 2
      IEND  = 0
C
C.... O���� ��K� ��� �POXO��EH�� �HTEPBA�A (PXGMN,PXGMX) C �A�OM DELTA.
C
#if defined(hib)
      IF (IPR .GE. 1) WRITE (NCAN,2020)
#endif
      DO 170 I=1,MEND
        NEND = NDR
        IF (I .EQ. 1) NEND = 1
        DX0 = (I - 1)*DELTA
C
C.... ��K� �TO�HEH�� TO�K� HA KP�BO� �P� PA����H�X �HA�EH��X X0(NG)
C     �� �HTEPBA�A (PXGMN,PXGMX) (�O PA����H�M HA�PAB�EH��M C��CKA �P�
C     KA��OM �HA�EH�� X0(NG)).
C
        DO 160 J=1,NEND
          X0NEW = XC + DX0*IND
          IF (IBND .EQ. 0) GOTO 110
C
C.... �POBEPKA B�XO�A �A �PAH���.
C
          IF (XGMN .LT. X0NEW .AND. X0NEW .LT. XGMX) GOTO 110
C
C.... �O�CK TO�K� B����� �PAH���.
C
              XBOUND = XGMN
              IF (X0NEW .GT. XGMX) XBOUND = XGMX
              IEND = IEND + 1
              NDR  = 1
              IND  =-1*IND
              X0NEW= XBOUND
 110      CONTINUE
C
C.... �TO�HEH�E TO�K� B ���EP��OCKOCT� X(NG)=CONSTANATA.
C
          NFIX   = NG
          X0(NG) = X0NEW
#if defined(hib)
          IF (IPR .GE. 1) WRITE (NCAN,2000)
          IF (IPR .GE. 1) WRITE (NCAN,2030) X0NEW
          IF (IPR .GE. 1) WRITE (NCAN,2040) NFIX
          IF (IPR .GE. 1) WRITE (NCAN,2050) X0(NFIX)
#endif
          IT     = 10
C
C.... O�PA�EH�E K �PO�E��PE H��TOHA.
C
          IERN = 0
          CALL NEWTN (NX,NDIM,NG,NFIX,NCAN,X0,EPS,EPSN,IT,IPR,
     A                FUN,DFDX,DFUN,IERN,ISTOP)
          IF (ISTOP .NE. 0) RETURN
          IF (IERN  .EQ. 0) GOTO 180
C
C.... TO�KA HA KP�BO� HE HA��EHA.
C
#if defined(hib)
              IF (IPR .GE. 1) WRITE (NCAN,2000)
              IF (IPR .GE. 1) WRITE (NCAN,2060)
              IF (IPR .GE. 1) WRITE (NCAN,2070)
              IF (IPR .GE. 1) WRITE (NCAN,2080)  (X0(IW),IW=1,NX)
              IF (IPR .GE. 1) WRITE (NCAN,2090)
#endif
C
C.... B���C��M �HA�EH�� ��HK��� B �OC�E�HE� HA��EH�O� TO�KE.
C
              IERR = 0
              CALL FUN (X0,FUNS,IERR,ISTOP)
              IF (ISTOP .NE. 0) RETURN
              IF (IERR  .EQ. 0) GOTO 115
#if defined(hib)
                  IF (IPR .GE. 1) WRITE (NCAN,3000)
#endif
 115          CONTINUE
#if defined(hib)
              IF (IPR .GE. 1) WRITE (NCAN,2000)
              IF (IPR .GE. 1) WRITE (NCAN,2080)  (FUNS(IW),IW=1,NFUN)
              IF (IPR .GE. 1) WRITE (NCAN,2010)
#endif
              DO 120 K=1,NX
                 X0(K) = XOLD(K)
 120          CONTINUE
              X0(NG) = X0NEW
              IF (IPOISK .EQ. 0) GOTO 175
C
C.... ��K� �EPE�OPA BCEBO�MO�H�X ���EP��OCKOCTE� C��CKA HA KP�B��.
C
              DO 150 L=1,NX
                 IF (L .EQ. NG) GOTO 140
C
C.... �TO�HEH�� TO�K� B ���EP��OCKOCT� X0(NFIX)=CONSTANTA.
C
                     NFIX = L
                     IT   = 10
#if defined(hib)
                     IF (IPR .GE. 1) WRITE (NCAN,2000)
                     IF (IPR .GE. 1) WRITE (NCAN,2030) X0NEW
                     IF (IPR .GE. 1) WRITE (NCAN,2040) NFIX
                     IF (IPR .GE. 1) WRITE (NCAN,2050) X0(NFIX)
#endif
C
C.... O�PA�EH�E K �PO�E��PE H��TOHA.
C
                     IERN = 0
                     CALL NEWTN (NX,NDIM,NG,NFIX,NCAN,X0,EPS,EPSN,IT,
     A                           IPR,FUN,DFDX,DFUN,IERN,ISTOP)
                     IF (ISTOP .NE. 0) RETURN
                     IF (IERN  .EQ. 0) GOTO 180
C
C.... TO�KA HA KP�BO� HE HA��EHA.
C
#if defined(hib)
                         IF (IPR .GE. 1) WRITE (NCAN,2000)
                         IF (IPR .GE. 1) WRITE (NCAN,2060)
                         IF (IPR .GE. 1) WRITE (NCAN,2070)
                         IF (IPR .GE. 1) WRITE (NCAN,2080)
     A                                         (X0(IW),IW=1,NX)
                         WRITE (NCAN,2090)
#endif
C
C.... B���C��M �HA�EH�� ��HK��� B �OC�E�HE� HA��EHO� TO�KE.
C
                         IERR = 0
                         CALL FUN (X0,FUNS,IERR,ISTOP)
                         IF (ISTOP .NE. 0) RETURN
                         IF (IERR  .EQ. 0) GOTO 125
#if defined(hib)
                             IF (IPR .GE. 1) WRITE (NCAN,3000)
#endif
 125                     CONTINUE
#if defined(hib)
                         IF (IPR.GE.1) WRITE (NCAN,2000)
                         IF (IPR.GE.1 .AND. IERR.EQ.0) WRITE (NCAN,2080)
     A                                         (FUNS(IW),IW=1,NFUN)
                         IF (IPR .GE. 1) WRITE (NCAN,2000)
                         IF (IPR .GE. 1) WRITE (NCAN,2010)
#endif
                         DO 130 K=1,NX
                            X0(K) = XOLD(K)
 130                     CONTINUE
                         X0(NG) = X0NEW
 140             CONTINUE
 150          CONTINUE
C
C.... �EPBA� TO�KA HA KP�BO� HE HA��EHA.
C
              IER   = -1
              IPNT1 = 0
              IF (IEND .EQ. 2) RETURN
                  IF (IEND .EQ. 0) IND = -1*IND
 160    CONTINUE
 170  CONTINUE
 175  CONTINUE
      IER   = -1
      IPNT1 = 0
      RETURN
 180  CONTINUE
C
C.... �EPBA� TO�KA HA KP�BO� �C�E�HO HA��EHA.
C
#if defined(hib)
      IF (IPR .GE. 1) WRITE (NCAN,2000)
      IF (IPR .GE. 1) WRITE (NCAN,3010)
      IF (IPR .GE. 1) WRITE (NCAN,2000)
      IF (IPR .GE. 1) WRITE (NCAN,2080)  (X0(IW),IW=1,NX)
      IF (IPR .GE. 1) WRITE (NCAN,2000)
      IF (IPR .GE. 1) WRITE (NCAN,3020)
#endif
      IER   = 0
      IPNT1 = 0
      RETURN
#if defined(hib)
 2000 FORMAT
     A(1X)
C
 2010 FORMAT
     A(1X,120('-'))
C
 2020 FORMAT
     A('   �TO�H�ETC� �EPBA� TO�KA HA KP�BO�.'/
     B '   --------------------------------- ')
C
 2030 FORMAT
     A('   �HA�EH�E B��E�EHHO� �EPEMEHHO�:        ',G14.5)
C
 2040 FORMAT
     A('   HOMEP ��KC�POBAHHO� �EPEMEHHO�:        ',I6)
C
 2050 FORMAT
     A('   �HA�EH�E ��KC�POBAHHO� �EPEMEHHO�:     ',G14.5)
C
 2060 FORMAT
     A('   TO�KA HA KP�BO� HE HA��EHA.            '/
     B '   ---------------------------            ')
C
 2070 FORMAT
     A('   KOOP��HAT� �OC�E�HE� B���C�EHHO� TO�K�:')
C
 2080 FORMAT
     A(10(2X,7(G14.5,1X)/))
C
 2090 FORMAT
     A('   �HA�EH�� �YHK��� B �OC�E�HE� TO�KE:    ')
C
 3000 FORMAT
     A('   �HA�EH�� �YHK��� HE B���C�EH�.        ')
C
 3010 FORMAT
     A('   KOOP��HAT� �EPBO� TO�K� HA KP�BO�:    '
     B/'   ----------------------------------    ')
C
 3020 FORMAT
     A(1X,120('='))
#endif
      END

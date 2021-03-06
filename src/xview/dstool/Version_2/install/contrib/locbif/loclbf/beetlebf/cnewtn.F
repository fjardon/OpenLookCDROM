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

      SUBROUTINE CNEWTN (NX,NDIMM,NFIX,NFIXC,X,F,VTNG,EPS,
     A                  DFUN,IER,ISTOP)
C     3.08.1991
c Rev.26.07.92
C     18.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*        							       *
C*    CHECKING CURRENT POINT AGAINST ADDITIONAL                        *
C*         NEWTON ITERATION, USING VARIOUS DIRECTIONS		       *
C*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*
C*    �APAMETP�:                                                       *
C*    =========                                                        *
C*    NX   - ��C�O �EPEMEHH�X;                                         *
C*    NDIM - �A�B�EHHA� CTPO�HA� PA�MEPHOCT� MATP��� DFUN;             *
C*    NFIX - NUMBER OF FIXED VARIABLE                                  *
C*    NFIXC- NUMBER OF NEW FIXED VARIABLE                              *
C*    X    - CURRENT POINT                                             *
C*    F    - FUNCTION VALUES AT CURRENT POINT			       *
C*    VTNG - TANGENT VECTOR AT CURRENT POINT			       *
C*    EPS  - TO�HOCT� METO�A;                                          *
C*    DFUN - �B�MEPH�� MACC�B PA�MEPHOCT� NDIM*NX, CO�EP�A��� MATP���  *
C*           �EPB�X �ACTH�X �PO��BO�H�X.                               *
C*    IER  - �APAMETP O���K�:                                          *
C*             0-�TEPA��� �C�E�HO CO���C� C TO�HOCT�� EPS;             *
C*            -1-TPE��EMA� TO�HOCT� HE �OCT��H�TA �A ITC �TEPA���.     *
C*            -2-HA O�EPE�HO� �TEPA��� O�PE�E��TE�� ��HE�HO� C�CTEM� =0;
C*            -3-HE B���C�EH� �HA�EH�� ��HK��� B H��TOHOBCKOM �PO�HO�E;*
C*            -4-HE B���C�EHA MATP��A �ACTH�X �PO��BO�H�X;             *
C*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*
C*           BMAS - BC�OMO�ATE��H�� PA�O��� MACC�B.                    *
C*                                                                     *
C*           B���BAEM�E �O��PO�PAMM�:                                  *
C*              LEQV.                                                  *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A (NDIM) �EPEMEHH�X HEO�XO��MO   *
C*    ��MEH�T� PA�MEPHOCT� MACC�BOB:   A(NDIM,NDIM), BMAS(NDIM)        *
C*                                     ARR1(NDIM,NDIM)                 *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      DIMENSION X(1), F(1), VTNG(1), DFUN(NDIM,NX), BMAS(NDIM)
C
      COMMON /CHECKN/ EPSNAD,EPSNNF
C
      COMMON /WORKBE/ ARR1(NDIM,NDIM), A(NDIM,NDIM)
      COMMON /LEQV1/  DETL,CONDL
      COMMON /CORR1/  DET, COND,SGNDET
C
      ISTOP = 0
      IER = 0
      NFUN  = NX - 1
      EPSNAD=0.D0
      EPSNNF=0.D0
      DETLR=DETL
      CONDLR=CONDL
      DETR=DET
      CONDR=COND
C   
#if defined(hib)
      PRINT 1023,(X(I),I=1,NX)
 1023 FORMAT(' CNEWTN:   Initial point (X) =',
     *     /('         ',3G23.15)) 
#endif
      DO 20 I=1,NX
         A(NX,I)=0.D0
         IF (I.EQ.NFIX) A(NX,I)=1.D0
	 IF (I.EQ.NX) GOTO 20
         BMAS(I) =-F(I)
         DO 10 J=1,NX
            A(I,J)=DFUN(I,J)
 10      CONTINUE
 20   CONTINUE
      BMAS(NX)  = 0.D0
C
C.... SOLVING LINEAR SYSTEM  A*Y = BMAS;
C     SOLUTION VECTOR WILL BE IN ARRAY BMAS.
C
      IER = 0
      CALL LEQV (A,BMAS,NDIMM,NX,IER)
#if defined(hib)
      PRINT 1032,IER,ISTOP
 1032 FORMAT(/' CNEWTN:   Jacobian evaluation completed with',
     *           ' IER=',I3,'  ISTOP=',I3)
      PRINT 1033, ((DFUN(I,J),J=1,NX),I=1,NX-1)
 1033 FORMAT('     Jacobian (DFUN)=',
     *     /('         ',3G23.15)) 
#endif
      CONDAD = CONDL
C
#if defined(hib)
      PRINT 1042,IER,CONDL
 1042 FORMAT(/'    Solving completed with IER=',I3,'  CONDL=',G11.4)
      PRINT 1043,(BMAS(I),I=1,NX)
 1043 FORMAT('     Solution vector=',
     *     /('         ',3G23.15)) 
#endif
C
C.... TESTING ACCURACY
C
      EPSNAD = 0.D0
      DO 30 I = 1,NX
         RR=X(I)
         RR1   = BMAS(I)/(1.D0+DABS(RR))
 	 IF(EPSNAD.LT.DABS(RR1)) EPSNAD = DABS(RR1)
 30   CONTINUE
C
#if defined(hib)
      PRINT 1053,EPSNAD
 1053 FORMAT('     Accuracy achieved (EPSNAD)=',G11.4)
#endif
C
      IF (EPSNAD.GT.EPS) GOTO 180

      DVMAX=DABS(VTNG(NFIX))
      DVMAXN=0.D0
      NFIXN=0
      DO 40 I=1,NX
         IF (I.EQ.NFIX) GOTO 40
         IF (DABS(VTNG(I)).LE.DVMAXN) GOTO 40
	 DVMAXN=DABS(VTNG(I))
         NFIXN=I
 40   CONTINUE 
#if defined(hib)
      PRINT *
      PRINT *,'  NFIX=',NFIX,'  NFIXN=',NFIXN
      PRINT *,'  DVMAX=',DVMAX,'  DVMAXN=',DVMAXN
#endif
C
      IF (DVMAXN.LT.0.1D0*DVMAX) THEN
          GOTO 80
      ENDIF  
C
      DO 60 I=1,NX
         A(NX,I)=0.D0
         IF (I.EQ.NFIXN) A(NX,I)=1.D0
	 IF (I.EQ.NX) GOTO 60
         BMAS(I) =-F(I)
         DO 50 J=1,NX
            A(I,J)=DFUN(I,J)
 50      CONTINUE
 60   CONTINUE
      BMAS(NX)  = 0.D0
C
C.... SOLVING LINEAR SYSTEM  A*Y = BMAS;
C     SOLUTION VECTOR WILL BE IN ARRAY BMAS.
C
      IER = 0
C
      CALL LEQV (A,BMAS,NDIMM,NX,IER)
      CONDNF = CONDL
C
#if defined(hib)
      PRINT 1042,IER,CONDL
      PRINT 1043,(BMAS(I),I=1,NX)
#endif
C
C.... TESTING ACCURACY
C
      EPSNNF = 0.D0
      DO 70 I = 1,NX
         RR=X(I)
         RR1   = BMAS(I)/(1.D0+DABS(RR))
 	 IF(EPSNNF.LT.DABS(RR1)) EPSNNF = DABS(RR1)
 70   CONTINUE
#if defined(hib)
      PRINT 1153,EPSNNF
 1153 FORMAT('     Accuracy achieved (EPSNNF)=',G11.4)
#endif
C
 80   CONTINUE
C
C
C     CHOOSING LEADING VARIABLE
C
      NFIXC=NFIX
      RNFIX=DVMAX
      RNFIXN=DVMAXN
C
      IF (RNFIX.LT.RNFIXN) NFIXC=NFIXN
C
#if defined(hib)
      PRINT *,'  RNFIX=',RNFIX,'  RNFIXN=',RNFIXN,'  NFIXC=',NFIXC
#endif
C
      IF (EPSNNF.GT.EPS .AND. NFIXC.NE.NFIX) GOTO 180
      GOTO 200
C
C
C.... BAD ACCURACY.
C
 180  IER =-1
C
C     RESTORING AND RETURN
C
 200  CONTINUE
C
      DETL=DETLR
      CONDL=CONDLR
      DET=DETR
      COND=CONDR
C
      RETURN
      END

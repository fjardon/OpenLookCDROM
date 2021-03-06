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

      SUBROUTINE DECOMP (NDIM,N,A,COND,IPVT,WORK)
C     3.05.90�.
C     �PO�PAMMA B���C��ET PA��O�EH�E BE�ECTBEHHO�
C     MATP��� �OCPE�CTBOM �A�CCOBA �CK���EH�� ��
C     O�EH�BAET O��C�OB�EHHOCT� MATP���.
C
C     (�PO�PAMMA O��CAHA B KH��E:
C           ��.�OPCA�T, M.MA��KO��M, K.MO��EP
C          "MA��HH�E METO�� MATEMAT��ECK�X B���C�EH��".
C           M.:M�P,1980)
C
C     OHA �C�O����ETC� ��� B���C�EH�� PE�EH�� ��HE�H�X
C     C�CTEM.
C
C     BXO�HA� �H�OPMA���...
C
C       NDIM=�A�B�EHHA� CTPO�HA� PA�MEPHOCT� MACC�BA,
C       CO�EP�A�E�O A.
C
C       N=�OP��OK MATP���.
C
C       A=MATP��A, KOTOP�� H��HO PA��O��T�.
C
C     B�XO�HA� �H�OPMA���...
C
C       A CO�EP��T BEPXH�� TPE��O��H�� MATP��� U
C         � ���T�BA���� �EPECTAHOBK� BEPC��
C         H��HE� TPE��O��HO� MATP��� 1-L, TAK�E,
C         �TO (MATP��A �EPECTAHOBOK) *A=L*U
C
C     COND=O�EHKA O��C�OB�EHHOCT� A.
C        ��� ��HE�HO� C�CTEM� A*X=B ��MEHEH��
C        B A � B MO��T B��BAT� ��MEHEH�� B X, �O����E
C        B COND PA�. EC�� COND+1.0 .EQ. COND, TO A
C        B �PE�E�AX MA��HHO� TO�HOCT� �B��ETC�
C        B�PO��EHHO� MATP��E�. COND �O�A�AETC�
C        PABH�M 1.0E+32, EC�� O�HAP��EHA TO�HA�
C        B�PO��EHHOCT�.
C
C     IPVT=BEKTOP BE����X ��EMEHTOB.
C        IPVT(K)=�H�EKC K-�BE���E� CTPOK�
C        IPVT(N)=(-1)**(��C�O �EPECTAHOBOK)
C
C     PA�O�EE �O�E..BEKTOP WORK �O��EH ��T� O��CAH
C             � BK���EH B B���B. E�O BXO�HOE CO�EP-
C             �AH�E O���HO HE �AET BA�HO� �H�OP-
C             MA���.
C
C     O�PE�E��TE�� MATP��� A MO�ET ��T� �O���EH
C     HA B�XO�E �O �OPM��E
C        DET(A)=IPVT(N)*A(1,1)*A(2,2)*...*A(N,N).
C
C
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      DIMENSION A(NDIM,N), WORK(N), IPVT(N)
C
      IPVT(N) = 1
      IF (N .EQ. 1) GOTO 80
      NM1 = N - 1
C
C     B���C��T� 1-HOPM� MATP��� A.
C
      ANORM = 0.D0
      DO 10 J=1,N
         T = 0.D0
         DO 5 I=1,N
            T = T + DABS(A(I,J))
 5       CONTINUE
         IF (T .GT. ANORM) ANORM = T
 10   CONTINUE
C
C     �A�CCOBO �CK���EH�E C �ACT��H�M B��OPOM BE-
C     ���E�O ��EMEHTA.
C
      DO 35 K=1,NM1
         KP1 = K + 1
C
C     HA�T� BE����� ��EMEHT.
C
         M = K
         DO 15 I=KP1,N
            IF (DABS(A(I,K)) .GT. DABS(A(M,K))) M = I
 15      CONTINUE
         IPVT(K) = M
         IF (M .NE. K) IPVT(N) = -IPVT(N)
         T = A(M,K)
         A(M,K) = A(K,K)
         A(K,K) = T
C
C     �PO��CT�T� �TOT �A�, EC�� BE����� ��EMEHT
C     PABEH H���.
C
         IF (T .EQ. 0.D0) GOTO 35
C
C     B���C��T� MHO��TE��.
C
         DO 20 I=KP1,N
            A(I,K) = -A(I,K)/T
 20      CONTINUE
C
C     �EPECTAB��T� � �CK���AT� �O CTO���AM.
C
         DO 30 J=KP1,N
            T = A(M,J)
            A(M,J)=A(K,J)
            A(K,J)=T
            IF (T .EQ. 0.D0) GOTO 30
            DO 25 I=KP1,N
               A(I,J)=A(I,J) + A(I,K)*T
 25         CONTINUE
 30      CONTINUE
 35   CONTINUE
C
C     COND=(1-HOPMA MATP��� A)*(O�EHKA ��� 1-HOPM�
C           MATP���, O�PATHO� K A)
C     O�EHKA �O���AETC� �OCPE�CTBOM O�HO�O �A�A ME-
C     TO�A O�PATH�X �TEPA��� ��� HA�MEH��E�O C�H-
C     ����PHO�O BEKTOPA. �TO TPE��ET PE�EH�� �B�X
C     C�CTEM �PABHEH��, (TPAHC�OH�POBAHHA� ��� A)*Y=E
C     I A*Z=Y, ��E E-BEKTOP �� +1 � -1, B��PAHH�� TAK,
C     �TO�� MAKC�M���POBAT� BE����H� Y.
C     ESTIMATE=(1-HOPMA Z)/(1-HOPMA Y)
C
C     PE��T� C�CTEM� (TPAHC�OH�POBAHHA� ��� A)*Y=E
C
      DO 50 K=1,N
         T = 0.D0
         IF (K .EQ. 1) GOTO 45
         KM1 = K - 1
         DO 40 I=1,KM1
            T = T + A(I,K)*WORK(I)
 40      CONTINUE
 45      EK = 1.D0
         IF (T .LT. 0.D0) EK = -1.D0
         IF (A(K,K) .EQ. 0.D0) GOTO 90
         WORK(K) = -(EK + T)/A(K,K)
 50   CONTINUE
      DO 60 KB=1,NM1
         K = N - KB
         T = 0.D0
         KP1 = K + 1
         DO 55 I=KP1,N
            T = T + A(I,K)*WORK(K)
 55      CONTINUE
         WORK(K) = T
         M = IPVT(K)
         IF (M .EQ. K) GOTO 60
         T = WORK(M)
         WORK(M) = WORK(K)
         WORK(K) = T
 60   CONTINUE
C
      YNORM = 0.D0
      DO 65 I=1,N
         YNORM = YNORM + DABS(WORK(I))
 65   CONTINUE
C
C     PE��T� C�CTEM� A*Z=Y
C
      CALL SOLVE (NDIM,N,A,WORK,IPVT)
C
      ZNORM = 0.D0
      DO 70 I=1,N
         ZNORM = ZNORM + DABS(WORK(I))
 70   CONTINUE
C
C     O�EH�T� O��C�OB�EHHOCT�
C
      COND = ANORM*ZNORM/YNORM
      IF (COND .LT. 1.D0) COND = 1.D0
      RETURN
C
C     C���A� MATP��� 1:1.
C
 80   COND = 1.D0
      IF (A(1,1) .NE. 0.D0) RETURN
C
C     TO�HA� B�PO��EHHOCT�.
C
 90   COND = 1.0D+64
      RETURN
      END

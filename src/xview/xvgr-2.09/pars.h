
typedef union  {
    double val;
    int ival;
    double *ptr;
    int func;
    int pset;
    char *str;
} YYSTYPE;
extern YYSTYPE yylval;
# define ABS 257
# define ACOS 258
# define ASIN 259
# define ATAN 260
# define ATAN2 261
# define CEIL 262
# define COS 263
# define DEG 264
# define DX 265
# define DY 266
# define ERF 267
# define ERFC 268
# define EXP 269
# define FLOOR 270
# define HYPOT 271
# define INDEX 272
# define INT 273
# define INVN 274
# define INVT 275
# define IRAND 276
# define LGAMMA 277
# define LN 278
# define LOG 279
# define LOGISTIC 280
# define MAXP 281
# define MINP 282
# define MOD 283
# define NORM 284
# define NORMP 285
# define PI 286
# define RAD 287
# define RAND 288
# define RNORM 289
# define SETNO 290
# define SIN 291
# define SQR 292
# define SQRT 293
# define TAN 294
# define INUM 295
# define VX1 296
# define VX2 297
# define VY1 298
# define VY2 299
# define WX1 300
# define WX2 301
# define WY1 302
# define WY2 303
# define DELAYP 304
# define DOUBLEBUFFER 305
# define DOWN 306
# define ABSOLUTE 307
# define ABORT 308
# define ACTIVATE 309
# define ACTIVE 310
# define ALT 311
# define ALTERNATE 312
# define ALTXAXIS 313
# define ALTYAXIS 314
# define ANGLE 315
# define ANNOTATE 316
# define APPEND 317
# define AREA 318
# define ARROW 319
# define AUTO 320
# define AUTOSCALE 321
# define AUTOTICKS 322
# define AVG 323
# define AXIS 324
# define AXES 325
# define BACKBUFFER 326
# define BACKGROUND 327
# define BAR 328
# define BATCH 329
# define BIN 330
# define BOTH 331
# define BOTTOM 332
# define BOX 333
# define CELLS 334
# define CENTER 335
# define CHAR 336
# define CHRSTR 337
# define CLEAR 338
# define CLICK 339
# define CMAP 340
# define COLOR 341
# define COMMENT 342
# define COPY 343
# define CYCLE 344
# define DECIMAL 345
# define DEF 346
# define DEFAULT 347
# define DELETE 348
# define DEVICE 349
# define DFT 350
# define DIFFERENCE 351
# define DISK 352
# define DRAW2 353
# define DXDX 354
# define DXP 355
# define DYDY 356
# define DYP 357
# define ECHO 358
# define EDIT 359
# define ELSE 360
# define END 361
# define ERRORBAR 362
# define EXIT 363
# define EXPONENTIAL 364
# define FALSEP 365
# define FFT 366
# define FILEP 367
# define FILL 368
# define FIND 369
# define FIXEDPOINT 370
# define FLUSH 371
# define FOCUS 372
# define FOLLOWS 373
# define FONTP 374
# define FOREGROUND 375
# define FORMAT 376
# define FRONTBUFFER 377
# define FRAMEP 378
# define GETP 379
# define GRAPH 380
# define GRAPHNO 381
# define GRAPHS 382
# define GRAPHTYPE 383
# define GRID 384
# define HARDCOPY 385
# define HBAR 386
# define HGAP 387
# define HIDDEN 388
# define HORIZONTAL 389
# define HPGLL 390
# define HPGLP 391
# define HISTO 392
# define IF 393
# define IHL 394
# define IN 395
# define INIT 396
# define INITGRAPHICS 397
# define INOUT 398
# define INTEGRATE 399
# define INTERP 400
# define INVDFT 401
# define INVFFT 402
# define JUST 403
# define KILL 404
# define LABEL 405
# define LAYOUT 406
# define LEAVE 407
# define LEAVEGRAPHICS 408
# define LEFT 409
# define LEGEND 410
# define LENGTH 411
# define LEVEL 412
# define LEVELS 413
# define LINE 414
# define LINESTYLE 415
# define LINETO 416
# define LINEWIDTH 417
# define LINK 418
# define LOAD 419
# define LOCATOR 420
# define LOCTYPE 421
# define LOGX 422
# define LOGY 423
# define LOGXY 424
# define MAJOR 425
# define MIFL 426
# define MIFP 427
# define MINOR 428
# define MISSINGP 429
# define MOVE 430
# define MOVE2 431
# define MOVETO 432
# define NEGATE 433
# define NO 434
# define NONE 435
# define NORMAL 436
# define NXY 437
# define OFF 438
# define OFFSETX 439
# define OFFSETY 440
# define ON 441
# define OP 442
# define ORIENT 443
# define OUT 444
# define PAGE 445
# define PARA 446
# define PARALLEL 447
# define PARAMETERS 448
# define PARAMS 449
# define PATTERN 450
# define PERIMETER 451
# define PERP 452
# define PERPENDICULAR 453
# define PIE 454
# define PIPE 455
# define PLACE 456
# define POINT 457
# define POLAR 458
# define POWER 459
# define PREC 460
# define PREPEND 461
# define PRINT 462
# define PS 463
# define PSCOLORP 464
# define PSMONOP 465
# define PSCOLORL 466
# define PSMONOL 467
# define PUSH 468
# define POP 469
# define PUTP 470
# define READ 471
# define REDRAW 472
# define REGRESS 473
# define REGNUM 474
# define REGIONS 475
# define RENDER 476
# define REVERSE 477
# define RIGHT 478
# define RISER 479
# define ROT 480
# define RUNAVG 481
# define RUNMED 482
# define RUNSTD 483
# define RUNMIN 484
# define RUNMAX 485
# define SAMPLE 486
# define SCALE 487
# define SCIENTIFIC 488
# define SET 489
# define SETNUM 490
# define SETS 491
# define SIGN 492
# define SIZE 493
# define SKIP 494
# define SLEEP 495
# define SLICE 496
# define SOURCE 497
# define SPEC 498
# define SPECIFIED 499
# define SPECTRUM 500
# define STACK 501
# define STACKEDBAR 502
# define STACKEDHBAR 503
# define STACKEDLINE 504
# define STAGGER 505
# define START 506
# define STARTTYPE 507
# define STATUS 508
# define STOP 509
# define STRING 510
# define SUBTITLE 511
# define SWAPBUFFER 512
# define SYMBOL 513
# define TICK 514
# define TICKLABEL 515
# define TICKMARKS 516
# define TITLE 517
# define TO 518
# define TOP 519
# define TRUEP 520
# define TYPE 521
# define UP 522
# define VELOCITY 523
# define VERTICAL 524
# define VGAP 525
# define VIEW 526
# define WITH 527
# define WORLD 528
# define WRITE 529
# define X1 530
# define X2 531
# define X3 532
# define X4 533
# define X5 534
# define XAXES 535
# define XAXIS 536
# define XCOR 537
# define XMAX 538
# define XMIN 539
# define XY 540
# define XYARC 541
# define XYBOX 542
# define XYFIXED 543
# define XYHILO 544
# define XYRT 545
# define XYSEG 546
# define XYSTRING 547
# define XYDX 548
# define XYDY 549
# define XYDXDX 550
# define XYDYDY 551
# define XYDXDY 552
# define XYX2Y2 553
# define XYXX 554
# define XYYY 555
# define XYZ 556
# define XYZW 557
# define Y1 558
# define Y2 559
# define Y3 560
# define Y4 561
# define Y5 562
# define YAXES 563
# define YAXIS 564
# define YES 565
# define YMAX 566
# define YMIN 567
# define ZEROXAXIS 568
# define ZEROYAXIS 569
# define ABOVE 570
# define BELOW 571
# define POLYI 572
# define POLYO 573
# define GENERAL 574
# define DDMMYY 575
# define MMDDYY 576
# define MMYY 577
# define MMDD 578
# define MONTHDAY 579
# define DAYMONTH 580
# define MONTHS 581
# define MONTHL 582
# define DAYOFWEEKS 583
# define DAYOFWEEKL 584
# define DAYOFYEAR 585
# define HMS 586
# define MMDDHMS 587
# define MMDDYYHMS 588
# define DEGREESLON 589
# define DEGREESMMLON 590
# define DEGREESMMSSLON 591
# define MMSSLON 592
# define DEGREESLAT 593
# define DEGREESMMLAT 594
# define DEGREESMMSSLAT 595
# define MMSSLAT 596
# define DOT 597
# define STAR 598
# define PLUS 599
# define CROSS 600
# define CIRCLE 601
# define SQUARE 602
# define DIAMOND 603
# define TRIANGLE1 604
# define TRIANGLE2 605
# define TRIANGLE3 606
# define TRIANGLE4 607
# define SVAR 608
# define VAR 609
# define X 610
# define Y 611
# define NUMBER 612
# define FITPARM 613
# define OR 614
# define AND 615
# define GT 616
# define LT 617
# define LE 618
# define GE 619
# define EQ 620
# define NE 621
# define UMINUS 622
# define NOT 623
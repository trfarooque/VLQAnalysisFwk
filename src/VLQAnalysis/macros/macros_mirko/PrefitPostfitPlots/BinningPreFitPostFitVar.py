import os, sys

metbinning=[

"0,20,40,60,80,100,140,200,300",#preselection
"0,20,40,60,80,100,140,200,300",#0h5j2b
"0,20,40,60,80,100,140,300",#0h5j3b
"0,40,100,300",#0h5j4b
"0,20,40,60,80,100,140,200,300",#1h5j2b
"0,40,100,300",#1h5j3b
"0,300",#1h5j4b
"0,100,300",#2h5j2b
"0,300",#2h5j3b
"0,300",#2h5j4b

"0,20,40,60,80,100,140,200,300",#0h6j2b
"0,20,40,60,80,100,140,300",#0h6j3b
"0,40,100,140,200,300",#0h6j4b
"0,20,40,60,80,100,140,200,300",#1h6j2b
"0,40,100,300",#1h6j3blm
"0,300",#1h6j4blm
"0,40,100,300",#1h6j3bhm
"0,300",#1h6j4bhm
"0,100,300",#2h6j2b
"0,300",#2h6j3b
"0,300",#2h6j4b

]

metbinwidth=[

"20",#preselection
"20",#0h5j2b
"20",#0h5j3b
"40",#0h5j4b
"20",#1h5j2b
"40",#1h5j3b
"300",#1h5j4b
"100",#2h5j2b
"300",#2h5j3b
"300",#2h5j4b

"20",#0h6j2b
"20",#0h6j3b
"40",#0h6j4b
"20",#1h6j2b
"40",#1h6j3blm
"300",#1h6j4blm
"40",#1h6j3bhm
"300",#1h6j4bhm
"100",#2h6j2b
"300",#2h6j3b
"300",#2h6j4b


]








lepptbinning=[

"0,20,40,60,80,100,120,160,200,500",#preselection
"0,20,40,60,80,100,120,160,200,500",#0h5j2b
"0,20,40,60,80,100,120,160,200,500",#0h5j3b
"0,20,40,60,80,100,200",#0h5j4b
"0,20,40,60,80,100,120,160,200,500",#1h5j2b
"0,20,40,60,80,100,120,160,200,500",#1h5j3b
"0,100,200",#1h5j4b
"0,100,200",#2h5j2b
"0,200",#2h5j3b
"0,200",#2h5j4b


"0,20,40,60,80,100,120,160,200,500",#0h5j2b
"0,20,40,60,80,100,120,160,200,500",#0h5j3b
"0,20,40,60,80,100,200",#0h5j4b
"0,20,40,60,80,100,120,160,200,500",#0h5j2b
"0,20,40,60,80,100,120,160,200,500",#0h5j3b
"0,20,40,60,80,100,200",#0h5j4b
"0,100,200",#1h6j3bhm
"0,200",#1h6j4bhm
"0,40,100,200",#2h6j2b
"0,60,100,200",#2h6j3b
"0,200",#2h6j4b

]

lepptbinwidth=[

"20",#preselection
"20",#0h5j2b
"20",#0h5j3b
"20",#0h5j4b
"20",#1h5j2b
"20",#1h5j3b
"100",#1h5j4b
"100",#2h5j2b
"100",#2h5j3b
"100",#2h5j4b

"20",#0h6j2b
"20",#0h6j3b
"20",#0h6j4b
"20",#1h6j2b
"20",#1h6j3blm
"20",#1h6j4blm
"100",#1h6j3bhm
"100",#1h6j4bhm
"40",#2h6j2b
"60",#2h6j3b
"200",#2h6j4b




]




htbinning=[

"0,200,300,400,500,600,800,1000,1500,3000",#preselection
"0,200,300,400,500,600,800,1000,1500,3000",#0h5j2b
"0,200,300,400,500,600,800,1000,2000",#0h5j3b
"0,200,300,400,600,2000",#0h5j4b
"0,400,500,600,800,1000,1500,3000",#0h5j2b
"0,400,600,800,1000,2000",#0h5j2b
"0,400,2000",#0h5j2b
"0,600,1000,2000",
"0,2000",
"0,2000",

"0,200,300,400,500,600,800,1000,1500,3000",#0h5j2b
"0,200,300,400,500,600,800,1000,2000",#0h5j3b
"0,200,300,400,600,2000",#0h5j4b

"0,400,500,600,800,1000,1500,3000",#0h5j2b
"0,400,600,800,1000,2000",#0h5j2b
"0,400,600,800,1000,2000",#0h5j2b
"0,400,2000",#0h5j2b
"0,400,2000",#0h5j2b

"0,600,1000,2000",
"0,2000",
"0,2000",


]

htbinwidth=[

"200",#preselection
"200",#0h5j2b
"200",#0h5j3b
"200",#0h5j4b
"400",#1h5j2b
"400",#1h5j3b
"400",#1h5j4b
"500",#2h5j2b
"500",#2h5j3b
"500",#2h5j4b

"200",#0h6j2b
"200",#0h6j3b
"200",#0h6j4b
"200",#1h6j2b
"400",#1h6j3blm
"400",#1h6j4blm
"400",#1h6j3bhm
"400",#1h6j4bhm
"500",#2h6j2b
"500",#2h6j3b
"500",#2h6j4b




]






jetbinning=[

"0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#preselection
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b
"4.5,5.5",#0h5j2b


"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b

"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b

"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b
"5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5",#0h5j2b



]


hotjetbinning=[

"-0.5,0.5,1.5,2.5,3.5,4.5",#preselection
"-0.5,0.5",#0h5j2b
"-0.5,0.5",#0h5j2b
"-0.5,0.5",#0h5j2b
"0.5,1.5",#0h5j2b
"0.5,1.5",#0h5j2b
"0.5,1.5",#0h5j2b
"1.5,2.5,3.5,4.5",#0h5j2b
"1.5,2.5,3.5,4.5",#0h5j2b
"1.5,2.5,3.5,4.5",#0h5j2b




"-0.5,0.5",#0h5j2b
"-0.5,0.5",#0h5j2b
"-0.5,0.5",#0h5j2b
"0.5,1.5",#0h5j2b
"0.5,1.5",#0h5j2b
"0.5,1.5",#0h5j2b
"0.5,1.5",#0h5j2b
"0.5,1.5",#0h5j2b

"1.5,2.5,3.5,4.5",#0h5j2b
"1.5,2.5,3.5,4.5",#0h5j2b
"1.5,2.5,3.5,4.5",#0h5j2b



]





bjetbinning=[

"-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5",#preselection

"1.5,2.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b
"1.5,2.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b

"1.5,2.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b



"1.5,2.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b
"1.5,2.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b
"1.5,2.5",#0h5j2b
"2.5,3.5",#0h5j2b
"3.5,4.5,5.5,6.5",#0h5j2b



]




jetbinwidth=[

"1",#preselection
"1",#0h5j2b
"1",#0h5j3b
"1",#0h5j4b
"1",#1h5j2b
"1",#1h5j3b
"1",#1h5j4b
"1",#2h5j2b
"1",#2h5j3b
"1",#2h5j4b

"1",#0h6j2b
"1",#0h6j3b
"1",#0h6j4b
"1",#1h6j2b
"1",#1h6j3blm
"1",#1h6j4blm
"1",#1h6j3bhm
"1",#1h6j4bhm
"1",#2h6j2b
"1",#2h6j3b
"1",#2h6j4b




]

mbbbinning=[
"0,40,50,60,70,80,90,100,110,120,130,140,150,170,200,300",#Preselection
"0,40,50,60,70,80,90,100,110,120,130,140,150,170,200,300",#0h6j2b
"0,40,50,60,70,80,90,100,110,120,130,140,150,170,200,300",#1h6j2b
"0,50,100,150,300",#2h6j2b
"0,40,50,60,70,80,90,100,110,120,130,140,150,170,200,300",#0h6j3b
"0,40,50,60,70,80,90,100,120,150,300",#0h6j4b
"0,40,50,60,70,80,90,100,120,150,300",#1h6j3b
"0,50,100,150,300",#1h6j4b
]

mbbbinwidth=[
"10",#Preselection
"10",#0h6j2b
"10",#1h6j2b
"50",#2h6j2b
"10",#0h6j3b
"10",#0h6j4b
"10",#1h6j3b
"50",#1h6j4b
]

fjptbinning=[
"300,350,400,450,500,600,800,1000",#preselection
"0,1000",#0h6j2b
"0,1000",#0h6j3b
"0,1000",#0h6j4b
"300,350,400,450,500,600,800,1000",#1h6j2b
"300,350,400,450,500,1000",#1h6j3b
"300,400,500,1000",#1h6j4b
"300,350,400,450,500,600,800,1000",#2h6j2b
"300,350,400,450,500,600,800,1000",#2h6j2b
"300,350,400,450,500,600,800,1000",#2h6j2b

"0,1000",#0h7j2b
"0,1000",#0h7j3b
"0,1000",#0h7j4b
"300,350,400,450,500,600,800,1000",#1h6j2b
"300,350,400,450,500,1000",#1h6j3b
"300,400,500,1000",#1h6j4b
"300,350,400,450,500,1000",#1h6j3b
"300,400,500,1000",#1h6j4b
"300,350,400,450,500,600,800,1000",#2h6j2b
"300,350,400,450,500,600,800,1000",#2h6j2b
"300,350,400,450,500,600,800,1000",#2h6j2b

]

fjptbinwidth=[
"50",#1h6j2b
"50",#1h6j2b
"50",#1h6j3b
"50",#2h6j2b
"50",#1h6j2b
"50",#1h6j2b
"100",#1h6j3b
"50",#2h6j2b
"50",#1h6j2b
"50",#1h6j2b

"50",#1h6j3b
"50",#2h6j2b
"50",#1h6j2b
"50",#1h6j2b
"50",#1h6j3b
"100",#2h6j2b
"50",#1h6j2b
"100",#1h6j2b
"50",#1h6j3b
"50",#2h6j2b
"50",#1h6j2b
]


fjmbinning=[
"0,20,40,60,80,100,110,120,130,140,150,160,170,180,190,200,250,400",#preselection
"0,400",
"0,400",
"0,400",
"100,110,120,130,140,150,160,170,180,190,200,250,400",#1h6j2b
"100,120,140,160,180,200,400",#1h6j3b
"100,150,200,400",#1h6j4b
"100,120,140,160,180,200,400",#2h6j2b
"100,120,140,160,180,200,400",#2h6j2b
"100,120,140,160,180,200,400",#2h6j2b

"0,400",
"0,400",
"0,400",
"100,110,120,130,140,150,160,170,180,190,200,250,400",#1h6j2b
"100,120,140,160,180,200,400",#1h6j3b
"100,150,200,400",#1h6j4b
"100,120,140,160,180,200,400",#1h6j3b
"100,150,200,400",#1h6j4b
"100,120,140,160,180,200,400",#2h6j2b
"100,120,140,160,180,200,400",#2h6j2b
"100,120,140,160,180,200,400",#2h6j2b
]

fjmbinwidth=[
"20",#1h6j2b
"10",#1h6j2b
"10",#1h6j2b
"10",#1h6j2b
"10",#1h6j2b
"20",#1h6j3b
"50",#1h6j4b
"20",#2h6j2b
"20",#2h6j2b
"20",#2h6j2b


"10",#1h6j2b
"10",#1h6j2b
"10",#1h6j2b
"10",#1h6j2b
"20",#1h6j3b
"50",#1h6j4b
"20",#1h6j3b
"50",#1h6j4b
"20",#2h6j2b
"20",#2h6j2b
"20",#2h6j2b
]

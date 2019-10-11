EESchema Schematic File Version 4
LIBS:CEC2Usb-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4150 1350 4300 1350
Wire Wire Line
	4150 1550 4300 1550
Wire Wire Line
	4750 1650 4300 1650
Wire Wire Line
	4150 1750 4300 1750
Wire Wire Line
	4750 1850 4300 1850
Wire Wire Line
	1950 1650 1900 1650
Wire Wire Line
	1900 1650 1900 1000
Wire Wire Line
	1900 1000 4600 1000
Wire Wire Line
	7000 1000 7000 1650
Wire Wire Line
	7000 1650 6950 1650
Wire Wire Line
	3750 2050 3750 2100
Wire Wire Line
	3750 2100 4625 2100
Wire Wire Line
	5150 2100 5150 2050
Wire Wire Line
	5250 2050 5250 2150
Wire Wire Line
	5250 2150 3650 2150
Wire Wire Line
	3650 2150 3650 2050
Wire Wire Line
	3450 2050 3450 2200
Wire Wire Line
	3450 2200 4100 2200
Wire Wire Line
	5450 2200 5450 2050
Wire Wire Line
	3350 2050 3350 2250
Wire Wire Line
	5550 2250 5550 2050
Wire Wire Line
	3150 2050 3150 2300
Wire Wire Line
	5750 2300 5750 2050
Wire Wire Line
	2950 2050 2950 2350
Wire Wire Line
	5950 2350 5950 2050
Wire Wire Line
	6050 2050 6050 2400
Wire Wire Line
	6050 2400 2850 2400
Wire Wire Line
	2850 2400 2850 2050
Wire Wire Line
	2750 2050 2750 2450
Wire Wire Line
	2750 2450 6150 2450
Wire Wire Line
	6150 2450 6150 2050
Wire Wire Line
	6250 2050 6250 2500
Wire Wire Line
	6250 2500 2650 2500
Wire Wire Line
	2650 2500 2650 2050
Wire Wire Line
	2550 2050 2550 2550
Wire Wire Line
	2550 2550 6350 2550
Wire Wire Line
	6350 2550 6350 2050
Wire Wire Line
	6450 2050 6450 2600
Wire Wire Line
	6450 2600 2450 2600
Wire Wire Line
	2450 2600 2450 2050
Wire Wire Line
	2350 2050 2350 2650
Wire Wire Line
	2350 2650 6550 2650
Wire Wire Line
	6550 2650 6550 2050
Wire Wire Line
	6650 2050 6650 2700
Wire Wire Line
	6650 2700 2250 2700
Wire Wire Line
	2250 2700 2250 2050
$Comp
L power:GND #PWR015
U 1 1 5BD233AD
P 4300 1300
F 0 "#PWR015" H 4300 1050 50  0001 C CNN
F 1 "GND" H 4305 1127 50  0000 C CNN
F 2 "" H 4300 1300 50  0001 C CNN
F 3 "" H 4300 1300 50  0001 C CNN
	1    4300 1300
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR016
U 1 1 5BD233EC
P 4600 1050
F 0 "#PWR016" H 4600 900 50  0001 C CNN
F 1 "+5V" H 4615 1223 50  0000 C CNN
F 2 "" H 4600 1050 50  0001 C CNN
F 3 "" H 4600 1050 50  0001 C CNN
	1    4600 1050
	-1   0    0    1   
$EndComp
Wire Wire Line
	4600 1050 4600 1000
Connection ~ 4600 1000
Wire Wire Line
	4600 1000 7000 1000
Wire Wire Line
	4300 1300 4300 1350
Connection ~ 4300 1350
Text Label 4100 2800 0    50   ~ 0
SDA
Text Label 4350 2800 0    50   ~ 0
SCL
Wire Wire Line
	4300 1350 4750 1350
Wire Wire Line
	4750 1450 4300 1450
Wire Wire Line
	4300 1350 4300 1450
Connection ~ 4300 1450
Wire Wire Line
	4300 1450 4150 1450
Wire Wire Line
	4300 1550 4300 1450
Connection ~ 4300 1550
Wire Wire Line
	4300 1550 4750 1550
Wire Wire Line
	4300 1650 4300 1550
Connection ~ 4300 1650
Wire Wire Line
	4300 1650 4150 1650
Wire Wire Line
	4300 1750 4300 1650
Connection ~ 4300 1750
Wire Wire Line
	4300 1750 4750 1750
Wire Wire Line
	4300 1850 4300 1750
Connection ~ 4300 1850
Wire Wire Line
	4300 1850 4150 1850
Wire Wire Line
	4800 2100 4800 2800
Connection ~ 4800 2100
Wire Wire Line
	4800 2100 5150 2100
Text Label 4800 2800 0    50   ~ 0
HPD
$Comp
L power:+3.3V #PWR019
U 1 1 5D422099
P 7050 4775
F 0 "#PWR019" H 7050 4625 50  0001 C CNN
F 1 "+3.3V" H 7065 4948 50  0000 C CNN
F 2 "" H 7050 4775 50  0001 C CNN
F 3 "" H 7050 4775 50  0001 C CNN
	1    7050 4775
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R8
U 1 1 5D42382B
P 7050 4925
F 0 "R8" H 7109 4971 50  0000 L CNN
F 1 "27k" H 7109 4880 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7050 4925 50  0001 C CNN
F 3 "~" H 7050 4925 50  0001 C CNN
	1    7050 4925
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R9
U 1 1 5D424186
P 7050 5975
F 0 "R9" H 7109 6021 50  0000 L CNN
F 1 "220R" H 7109 5930 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7050 5975 50  0001 C CNN
F 3 "~" H 7050 5975 50  0001 C CNN
	1    7050 5975
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5D424324
P 7050 6125
F 0 "#PWR020" H 7050 5875 50  0001 C CNN
F 1 "GND" H 7055 5952 50  0000 C CNN
F 2 "" H 7050 6125 50  0001 C CNN
F 3 "" H 7050 6125 50  0001 C CNN
	1    7050 6125
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Small D2
U 1 1 5D424535
P 7050 5225
F 0 "D2" V 7125 5325 50  0000 R CNN
F 1 "1N4148" V 7125 5200 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123" V 7050 5225 50  0001 C CNN
F 3 "~" V 7050 5225 50  0001 C CNN
	1    7050 5225
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:BC817 Q1
U 1 1 5D424B3D
P 6950 5625
F 0 "Q1" H 7141 5671 50  0000 L CNN
F 1 "BC817" H 7141 5580 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7150 5550 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC817.pdf" H 6950 5625 50  0001 L CNN
	1    6950 5625
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 6075 7050 6125
Wire Wire Line
	7050 5875 7050 5825
Wire Wire Line
	7050 5125 7050 5075
$Comp
L Device:R_Small R7
U 1 1 5D42C4C7
P 6800 5075
F 0 "R7" V 6604 5075 50  0000 C CNN
F 1 "47k" V 6695 5075 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6800 5075 50  0001 C CNN
F 3 "~" H 6800 5075 50  0001 C CNN
	1    6800 5075
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R6
U 1 1 5D42C6CD
P 6500 5625
F 0 "R6" V 6575 5625 50  0000 C CNN
F 1 "100k" V 6425 5625 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6500 5625 50  0001 C CNN
F 3 "~" H 6500 5625 50  0001 C CNN
	1    6500 5625
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R5
U 1 1 5D42CEAB
P 6100 5400
F 0 "R5" H 6159 5446 50  0000 L CNN
F 1 "18k" H 6159 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6100 5400 50  0001 C CNN
F 3 "~" H 6100 5400 50  0001 C CNN
	1    6100 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C5
U 1 1 5D42E12D
P 6450 5200
F 0 "C5" H 6475 5275 50  0000 L CNN
F 1 "220pF" H 6475 5125 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6450 5200 50  0001 C CNN
F 3 "~" H 6450 5200 50  0001 C CNN
	1    6450 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 5D42D3C9
P 6800 5375
F 0 "C6" V 6900 5375 50  0000 C CNN
F 1 "220pF" V 6700 5425 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6800 5375 50  0001 C CNN
F 3 "~" H 6800 5375 50  0001 C CNN
	1    6800 5375
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 5325 7050 5375
Wire Wire Line
	7050 5375 6900 5375
Connection ~ 7050 5375
Wire Wire Line
	7050 5375 7050 5425
Wire Wire Line
	6700 5375 6700 5625
Wire Wire Line
	6700 5625 6750 5625
Wire Wire Line
	6600 5625 6700 5625
Connection ~ 6700 5625
Wire Wire Line
	7050 4825 7050 4775
Wire Wire Line
	6900 5075 7050 5075
Connection ~ 7050 5075
Wire Wire Line
	7050 5075 7050 5025
$Comp
L power:GND #PWR018
U 1 1 5D43CF04
P 6450 5350
F 0 "#PWR018" H 6450 5100 50  0001 C CNN
F 1 "GND" H 6550 5250 50  0000 C CNN
F 2 "" H 6450 5350 50  0001 C CNN
F 3 "" H 6450 5350 50  0001 C CNN
	1    6450 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 5350 6450 5300
Wire Wire Line
	6700 5075 6450 5075
Wire Wire Line
	6450 5075 6450 5100
Wire Wire Line
	6100 5500 6100 5625
Wire Wire Line
	6100 5625 6400 5625
Wire Wire Line
	6100 5300 6100 5075
Wire Wire Line
	6100 5075 6450 5075
Connection ~ 6450 5075
Wire Wire Line
	6100 5075 6050 5075
Connection ~ 6100 5075
Text Label 6000 5075 2    50   ~ 0
CEC_IN
Wire Wire Line
	6100 5625 6025 5625
Connection ~ 6100 5625
Text Label 6000 5625 2    50   ~ 0
CEC_OUT
Wire Wire Line
	7050 5375 7200 5375
Text Label 7300 5375 0    50   ~ 0
CEC
Wire Wire Line
	3975 6625 3975 6675
Wire Wire Line
	3975 6675 4025 6675
Wire Wire Line
	4075 6675 4075 6625
$Comp
L power:GND #PWR013
U 1 1 5D45FC71
P 4025 6675
F 0 "#PWR013" H 4025 6425 50  0001 C CNN
F 1 "GND" H 4030 6502 50  0000 C CNN
F 2 "" H 4025 6675 50  0001 C CNN
F 3 "" H 4025 6675 50  0001 C CNN
	1    4025 6675
	1    0    0    -1  
$EndComp
Connection ~ 4025 6675
Wire Wire Line
	4025 6675 4075 6675
$Comp
L Connector:USB_B_Mini J1
U 1 1 5D460831
P 1175 3800
F 0 "J1" H 925 3800 50  0000 C CNN
F 1 "USB_B_Mini" H 1175 4150 50  0000 C CNN
F 2 "Connector_USB:USB_Mini-B_Lumberg_2486_01_Horizontal" H 1325 3750 50  0001 C CNN
F 3 "~" H 1325 3750 50  0001 C CNN
	1    1175 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1075 4200 1075 4225
Wire Wire Line
	1075 4225 1125 4225
Wire Wire Line
	1175 4225 1175 4200
$Comp
L power:GND #PWR03
U 1 1 5D466D2A
P 1125 4225
F 0 "#PWR03" H 1125 3975 50  0001 C CNN
F 1 "GND" H 1130 4052 50  0000 C CNN
F 2 "" H 1125 4225 50  0001 C CNN
F 3 "" H 1125 4225 50  0001 C CNN
	1    1125 4225
	1    0    0    -1  
$EndComp
Connection ~ 1125 4225
Wire Wire Line
	1125 4225 1175 4225
Wire Wire Line
	1175 4225 1475 4225
Wire Wire Line
	1475 4225 1475 4000
Connection ~ 1175 4225
$Comp
L power:VCC #PWR04
U 1 1 5D469FA6
P 1525 3550
F 0 "#PWR04" H 1525 3400 50  0001 C CNN
F 1 "VCC" H 1542 3723 50  0000 C CNN
F 2 "" H 1525 3550 50  0001 C CNN
F 3 "" H 1525 3550 50  0001 C CNN
	1    1525 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1525 3550 1525 3600
Wire Wire Line
	1525 3600 1475 3600
$Comp
L power:VCC #PWR012
U 1 1 5D46D8C4
P 3975 3775
F 0 "#PWR012" H 3975 3625 50  0001 C CNN
F 1 "VCC" H 3900 3925 50  0000 C CNN
F 2 "" H 3975 3775 50  0001 C CNN
F 3 "" H 3975 3775 50  0001 C CNN
	1    3975 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	3975 3775 3975 3825
Wire Wire Line
	4075 3825 4075 3800
Wire Wire Line
	4075 3800 4125 3800
Wire Wire Line
	4175 3800 4175 3825
$Comp
L power:+3.3V #PWR014
U 1 1 5D477C0E
P 4125 3775
F 0 "#PWR014" H 4125 3625 50  0001 C CNN
F 1 "+3.3V" H 4175 3925 50  0000 C CNN
F 2 "" H 4125 3775 50  0001 C CNN
F 3 "" H 4125 3775 50  0001 C CNN
	1    4125 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 3775 4125 3800
Connection ~ 4125 3800
$Comp
L Device:R_Small R1
U 1 1 5D47BBD7
P 1625 3800
F 0 "R1" V 1550 3725 50  0000 C CNN
F 1 "22R" V 1550 3875 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1625 3800 50  0001 C CNN
F 3 "~" H 1625 3800 50  0001 C CNN
	1    1625 3800
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R2
U 1 1 5D47C470
P 1625 3900
F 0 "R2" V 1700 3800 50  0000 C CNN
F 1 "22R" V 1700 3975 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1625 3900 50  0001 C CNN
F 3 "~" H 1625 3900 50  0001 C CNN
	1    1625 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	1475 3900 1525 3900
Wire Wire Line
	1475 3800 1525 3800
Wire Wire Line
	1725 3800 1800 3800
Text Label 1800 3800 0    50   ~ 0
D+
Wire Wire Line
	1725 3900 1800 3900
Text Label 1800 3900 0    50   ~ 0
D-
Text Label 3375 4725 2    50   ~ 0
D+
Text Label 3375 4825 2    50   ~ 0
D-
$Comp
L Device:C_Small C4
U 1 1 5D48C73F
P 3300 5200
F 0 "C4" H 3125 5275 50  0000 L CNN
F 1 "1uF" H 3125 5125 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3300 5200 50  0001 C CNN
F 3 "~" H 3300 5200 50  0001 C CNN
	1    3300 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 3800 4175 3800
Wire Wire Line
	3300 5100 3300 5025
Wire Wire Line
	3300 5025 3375 5025
$Comp
L power:GND #PWR011
U 1 1 5D492223
P 3300 5300
F 0 "#PWR011" H 3300 5050 50  0001 C CNN
F 1 "GND" H 3305 5127 50  0000 C CNN
F 2 "" H 3300 5300 50  0001 C CNN
F 3 "" H 3300 5300 50  0001 C CNN
	1    3300 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5D492532
P 1800 3375
F 0 "C1" H 1892 3421 50  0000 L CNN
F 1 "10uF" H 1892 3330 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1800 3375 50  0001 C CNN
F 3 "~" H 1800 3375 50  0001 C CNN
	1    1800 3375
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5D493516
P 1800 3475
F 0 "#PWR07" H 1800 3225 50  0001 C CNN
F 1 "GND" H 1900 3375 50  0000 C CNN
F 2 "" H 1800 3475 50  0001 C CNN
F 3 "" H 1800 3475 50  0001 C CNN
	1    1800 3475
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R3
U 1 1 5D494BCA
P 3100 3975
F 0 "R3" H 3159 4021 50  0000 L CNN
F 1 "10k" H 3159 3930 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3100 3975 50  0001 C CNN
F 3 "~" H 3100 3975 50  0001 C CNN
	1    3100 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 4125 3100 4075
$Comp
L power:+3.3V #PWR010
U 1 1 5D49A016
P 3100 3875
F 0 "#PWR010" H 3100 3725 50  0001 C CNN
F 1 "+3.3V" H 3115 4048 50  0000 C CNN
F 2 "" H 3100 3875 50  0001 C CNN
F 3 "" H 3100 3875 50  0001 C CNN
	1    3100 3875
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5D4A0C25
P 2700 4525
F 0 "#PWR09" H 2700 4275 50  0001 C CNN
F 1 "GND" H 2705 4352 50  0000 C CNN
F 2 "" H 2700 4525 50  0001 C CNN
F 3 "" H 2700 4525 50  0001 C CNN
	1    2700 4525
	1    0    0    -1  
$EndComp
Text Label 4775 5825 0    50   ~ 0
CEC_IN
Text Label 4775 5925 0    50   ~ 0
CEC_OUT
Text Label 4775 6025 0    50   ~ 0
HPD
$Comp
L Connector:TestPoint_Alt TP3
U 1 1 5D59FF4D
P 6050 5075
F 0 "TP3" H 5950 5000 50  0000 L CNN
F 1 "CEC_IN" H 5600 5000 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 6250 5075 50  0001 C CNN
F 3 "~" H 6250 5075 50  0001 C CNN
	1    6050 5075
	1    0    0    -1  
$EndComp
Connection ~ 6050 5075
Wire Wire Line
	6050 5075 6000 5075
$Comp
L Connector:TestPoint_Alt TP2
U 1 1 5D5A0AB8
P 6025 5625
F 0 "TP2" H 5950 5550 50  0000 L CNN
F 1 "CEC_OUT" H 5825 5550 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 6225 5625 50  0001 C CNN
F 3 "~" H 6225 5625 50  0001 C CNN
	1    6025 5625
	1    0    0    -1  
$EndComp
Connection ~ 6025 5625
Wire Wire Line
	6025 5625 6000 5625
$Comp
L Connector:TestPoint_Alt TP4
U 1 1 5D5A1F3B
P 7200 5375
F 0 "TP4" H 7225 5500 50  0000 L CNN
F 1 "CEC" H 7258 5402 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 7400 5375 50  0001 C CNN
F 3 "~" H 7400 5375 50  0001 C CNN
	1    7200 5375
	1    0    0    -1  
$EndComp
Connection ~ 7200 5375
Wire Wire Line
	7200 5375 7300 5375
$Comp
L Connector:TestPoint_Alt TP1
U 1 1 5D5BF410
P 4625 2100
F 0 "TP1" H 4650 2225 50  0000 L CNN
F 1 "HPD" H 4683 2127 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 4825 2100 50  0001 C CNN
F 3 "~" H 4825 2100 50  0001 C CNN
	1    4625 2100
	1    0    0    -1  
$EndComp
Connection ~ 4625 2100
Wire Wire Line
	4625 2100 4800 2100
$Comp
L Device:C_Small C3
U 1 1 5D614F12
P 2875 4525
F 0 "C3" V 2825 4450 50  0000 C CNN
F 1 "10pF" V 2975 4525 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2875 4525 50  0001 C CNN
F 3 "~" H 2875 4525 50  0001 C CNN
	1    2875 4525
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5D616449
P 2875 4325
F 0 "C2" V 2825 4225 50  0000 C CNN
F 1 "10pF" V 2800 4450 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2875 4325 50  0001 C CNN
F 3 "~" H 2875 4325 50  0001 C CNN
	1    2875 4325
	0    1    1    0   
$EndComp
Wire Wire Line
	2775 4325 2700 4325
Wire Wire Line
	2700 4325 2700 4425
Wire Wire Line
	2700 4525 2775 4525
Connection ~ 2700 4525
$Comp
L Regulator_Linear:TLV70233_SOT23-5 U1
U 1 1 5D64DD26
P 1575 5075
F 0 "U1" H 1575 5300 50  0000 C CNN
F 1 "TLV70233_SOT23-5" H 1975 4825 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 1575 5400 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tlv702.pdf" H 1575 5125 50  0001 C CNN
	1    1575 5075
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR01
U 1 1 5D64F595
P 975 4775
F 0 "#PWR01" H 975 4625 50  0001 C CNN
F 1 "VCC" H 992 4948 50  0000 C CNN
F 2 "" H 975 4775 50  0001 C CNN
F 3 "" H 975 4775 50  0001 C CNN
	1    975  4775
	1    0    0    -1  
$EndComp
Wire Wire Line
	1275 5075 1225 5075
Wire Wire Line
	1225 5075 1225 4975
Connection ~ 1225 4975
Wire Wire Line
	1225 4975 1275 4975
$Comp
L power:GND #PWR05
U 1 1 5D660428
P 1575 5375
F 0 "#PWR05" H 1575 5125 50  0001 C CNN
F 1 "GND" H 1580 5202 50  0000 C CNN
F 2 "" H 1575 5375 50  0001 C CNN
F 3 "" H 1575 5375 50  0001 C CNN
	1    1575 5375
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Bridged12 JP1
U 1 1 5D660F90
P 975 4975
F 0 "JP1" V 975 5042 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 1020 5043 50  0001 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_RoundedPad1.0x1.5mm" H 975 4975 50  0001 C CNN
F 3 "~" H 975 4975 50  0001 C CNN
	1    975  4975
	0    -1   1    0   
$EndComp
Wire Wire Line
	1125 4975 1225 4975
$Comp
L power:+5V #PWR02
U 1 1 5D66BA28
P 975 5175
F 0 "#PWR02" H 975 5025 50  0001 C CNN
F 1 "+5V" H 990 5348 50  0000 C CNN
F 2 "" H 975 5175 50  0001 C CNN
F 3 "" H 975 5175 50  0001 C CNN
	1    975  5175
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 5D66C068
P 1950 4925
F 0 "#PWR08" H 1950 4775 50  0001 C CNN
F 1 "+3.3V" H 1965 5098 50  0000 C CNN
F 2 "" H 1950 4925 50  0001 C CNN
F 3 "" H 1950 4925 50  0001 C CNN
	1    1950 4925
	1    0    0    -1  
$EndComp
Wire Wire Line
	1875 4975 1950 4975
Wire Wire Line
	1950 4975 1950 4925
$Comp
L Connector:HDMI_A J2
U 1 1 5D478B52
P 3050 1650
F 0 "J2" V 3617 1650 50  0000 C CNN
F 1 "HDMI_A" V 3526 1650 50  0000 C CNN
F 2 "hyde_parts:HDMI-19P-SND13x" H 3075 1650 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/HDMI" H 3075 1650 50  0001 C CNN
	1    3050 1650
	0    -1   -1   0   
$EndComp
$Comp
L Connector:HDMI_A J3
U 1 1 5D478F71
P 5850 1650
F 0 "J3" V 6417 1650 50  0000 C CNN
F 1 "HDMI_A" V 6326 1650 50  0000 C CNN
F 2 "hyde_parts:HDMI-19P-SNDx" H 5875 1650 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/HDMI" H 5875 1650 50  0001 C CNN
	1    5850 1650
	0    1    -1   0   
$EndComp
Connection ~ 2700 4425
Wire Wire Line
	2700 4425 2700 4525
$Comp
L MCU_Microchip_ATmega:ATmega32U2-AU U2
U 1 1 5D4E43FF
P 4075 5225
F 0 "U2" H 3550 3850 50  0000 C CNN
F 1 "ATmega32U2-AU" H 4475 3850 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 4075 5225 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc7799.pdf" H 4075 5225 50  0001 C CNN
	1    4075 5225
	1    0    0    -1  
$EndComp
Wire Wire Line
	3375 4125 3350 4125
NoConn ~ 4775 6225
NoConn ~ 4775 6125
NoConn ~ 4775 5725
NoConn ~ 4775 5625
NoConn ~ 4775 5225
NoConn ~ 4775 5125
NoConn ~ 4775 5025
NoConn ~ 4775 4725
NoConn ~ 4775 4625
NoConn ~ 4775 4525
NoConn ~ 4775 4125
$Comp
L Device:R_Small R4
U 1 1 5D5DE5A7
P 1300 6025
F 0 "R4" V 1225 6025 50  0000 C CNN
F 1 "R_Small" V 1375 6025 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1300 6025 50  0001 C CNN
F 3 "~" H 1300 6025 50  0001 C CNN
	1    1300 6025
	0    1    1    0   
$EndComp
$Comp
L Device:LED_Small D1
U 1 1 5D5DE715
P 1550 6025
F 0 "D1" H 1550 5900 50  0000 C CNN
F 1 "LED" H 1450 5975 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1550 6025 50  0001 C CNN
F 3 "~" V 1550 6025 50  0001 C CNN
	1    1550 6025
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5D5DE85C
P 1725 6050
F 0 "#PWR017" H 1725 5800 50  0001 C CNN
F 1 "GND" H 1730 5877 50  0000 C CNN
F 2 "" H 1725 6050 50  0001 C CNN
F 3 "" H 1725 6050 50  0001 C CNN
	1    1725 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 6025 1450 6025
Wire Wire Line
	1650 6025 1725 6025
Wire Wire Line
	1725 6025 1725 6050
$Comp
L Connector:TestPoint TP5
U 1 1 5D60262E
P 1225 4975
F 0 "TP5" H 1283 5095 50  0000 L CNN
F 1 "Vcc" H 1283 5004 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 1425 4975 50  0001 C CNN
F 3 "~" H 1425 4975 50  0001 C CNN
	1    1225 4975
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP7
U 1 1 5D602904
P 2700 4325
F 0 "TP7" H 2758 4445 50  0000 L CNN
F 1 "GND" H 2758 4354 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 2900 4325 50  0001 C CNN
F 3 "~" H 2900 4325 50  0001 C CNN
	1    2700 4325
	1    0    0    -1  
$EndComp
Connection ~ 2700 4325
$Comp
L Connector:TestPoint TP6
U 1 1 5D602989
P 2050 4975
F 0 "TP6" H 2108 5095 50  0000 L CNN
F 1 "3.3v" H 2108 5004 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 2250 4975 50  0001 C CNN
F 3 "~" H 2250 4975 50  0001 C CNN
	1    2050 4975
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4975 2050 4975
Connection ~ 1950 4975
$Comp
L power:+3.3V #PWR0101
U 1 1 5D609449
P 1800 3275
F 0 "#PWR0101" H 1800 3125 50  0001 C CNN
F 1 "+3.3V" H 1815 3448 50  0000 C CNN
F 2 "" H 1800 3275 50  0001 C CNN
F 3 "" H 1800 3275 50  0001 C CNN
	1    1800 3275
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0102
U 1 1 5D59BE5B
P 5375 3900
F 0 "#PWR0102" H 5375 3750 50  0001 C CNN
F 1 "+3.3V" H 5390 4073 50  0000 C CNN
F 2 "" H 5375 3900 50  0001 C CNN
F 3 "" H 5375 3900 50  0001 C CNN
	1    5375 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5375 3900 5375 3925
Wire Wire Line
	5375 3925 5400 3925
$Comp
L power:GND #PWR0103
U 1 1 5D5AE4F5
P 5275 4025
F 0 "#PWR0103" H 5275 3775 50  0001 C CNN
F 1 "GND" H 5280 3852 50  0000 C CNN
F 2 "" H 5275 4025 50  0001 C CNN
F 3 "" H 5275 4025 50  0001 C CNN
	1    5275 4025
	-1   0    0    1   
$EndComp
Wire Wire Line
	5275 4025 5400 4025
Wire Wire Line
	5400 4125 5175 4125
Wire Wire Line
	5175 4125 5175 3525
Wire Wire Line
	5175 3525 3350 3525
Wire Wire Line
	3350 3525 3350 4125
Connection ~ 3350 4125
Wire Wire Line
	3350 4125 3100 4125
$Comp
L Device:Crystal_GND24_Small Y1
U 1 1 5D5D7924
P 3150 4425
F 0 "Y1" V 3300 4400 50  0000 L CNN
F 1 "16MHz" V 2975 4325 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 3150 4425 50  0001 C CNN
F 3 "~" H 3150 4425 50  0001 C CNN
	1    3150 4425
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 4525 3375 4525
Wire Wire Line
	3375 4325 3150 4325
Wire Wire Line
	2975 4325 3150 4325
Connection ~ 3150 4325
Connection ~ 3150 4525
Wire Wire Line
	2975 4525 3150 4525
Text Label 4600 2800 0    50   ~ 0
CEC
Wire Wire Line
	3150 2300 4600 2300
Wire Wire Line
	4350 2800 4350 2250
Connection ~ 4350 2250
Wire Wire Line
	4350 2250 5550 2250
Wire Wire Line
	3350 2250 4350 2250
Wire Wire Line
	4100 2200 4100 2800
Connection ~ 4100 2200
Wire Wire Line
	4100 2200 5450 2200
Wire Wire Line
	2950 2350 5950 2350
Wire Wire Line
	4600 2800 4600 2300
Connection ~ 4600 2300
Wire Wire Line
	4600 2300 5750 2300
Wire Wire Line
	1100 6025 1200 6025
Wire Wire Line
	2700 4425 3000 4425
Wire Wire Line
	3250 4425 3250 4625
Wire Wire Line
	3250 4625 3000 4625
Wire Wire Line
	3000 4625 3000 4425
Connection ~ 3000 4425
Wire Wire Line
	3000 4425 3050 4425
$Comp
L Device:R_Small R11
U 1 1 5DA87D80
P 5000 5175
F 0 "R11" H 4875 5225 50  0000 L CNN
F 1 "4.7k" H 4825 5300 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5000 5175 50  0001 C CNN
F 3 "~" H 5000 5175 50  0001 C CNN
	1    5000 5175
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R12
U 1 1 5DA88DFB
P 5125 5175
F 0 "R12" H 5175 5200 50  0000 L CNN
F 1 "4.7k" H 5125 5275 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5125 5175 50  0001 C CNN
F 3 "~" H 5125 5175 50  0001 C CNN
	1    5125 5175
	1    0    0    -1  
$EndComp
Text Label 5425 5425 0    50   ~ 0
SCL
Wire Wire Line
	5125 5275 5125 5425
Connection ~ 5125 5425
Wire Wire Line
	5000 5275 5000 5325
Connection ~ 5000 5325
Wire Wire Line
	5000 5075 5000 5050
Wire Wire Line
	5125 5050 5125 5075
$Comp
L power:+3.3V #PWR0106
U 1 1 5DABD971
P 5050 5050
F 0 "#PWR0106" H 5050 4900 50  0001 C CNN
F 1 "+3.3V" H 5065 5223 50  0000 C CNN
F 2 "" H 5050 5050 50  0001 C CNN
F 3 "" H 5050 5050 50  0001 C CNN
	1    5050 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5050 5050 5050
Connection ~ 5050 5050
Wire Wire Line
	5050 5050 5125 5050
Text Label 4775 4825 0    50   ~ 0
LED
Text Label 5425 5325 0    50   ~ 0
SDA
Text Label 1100 6025 2    50   ~ 0
LED
$Comp
L Connector:TestPoint_Alt TP8
U 1 1 5DAF6782
P 5300 5300
F 0 "TP8" H 5300 5500 50  0000 L CNN
F 1 "SDA" H 4850 5225 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 5500 5300 50  0001 C CNN
F 3 "~" H 5500 5300 50  0001 C CNN
	1    5300 5300
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Alt TP9
U 1 1 5DAF8B9A
P 5400 5300
F 0 "TP9" H 5450 5425 50  0000 L CNN
F 1 "SCL" H 4950 5225 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 5600 5300 50  0001 C CNN
F 3 "~" H 5600 5300 50  0001 C CNN
	1    5400 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5325 5300 5325
Wire Wire Line
	5125 5425 5400 5425
Wire Wire Line
	5400 5300 5400 5425
Connection ~ 5400 5425
Wire Wire Line
	5400 5425 5425 5425
Wire Wire Line
	5300 5300 5300 5325
Wire Wire Line
	5300 5325 5425 5325
Connection ~ 5300 5325
Wire Wire Line
	4775 5325 5000 5325
Wire Wire Line
	4775 5425 5125 5425
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 5DA6713B
P 2700 5825
F 0 "J5" H 2780 5817 50  0000 L CNN
F 1 "LEDs" H 2780 5726 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2700 5825 50  0001 C CNN
F 3 "~" H 2700 5825 50  0001 C CNN
	1    2700 5825
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5DA67E0F
P 2450 6075
F 0 "#PWR0107" H 2450 5825 50  0001 C CNN
F 1 "GND" H 2455 5902 50  0000 C CNN
F 2 "" H 2450 6075 50  0001 C CNN
F 3 "" H 2450 6075 50  0001 C CNN
	1    2450 6075
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 6075 2450 6025
Wire Wire Line
	2450 6025 2500 6025
$Comp
L power:VCC #PWR0108
U 1 1 5DA71F68
P 2450 5675
F 0 "#PWR0108" H 2450 5525 50  0001 C CNN
F 1 "VCC" H 2467 5848 50  0000 C CNN
F 2 "" H 2450 5675 50  0001 C CNN
F 3 "" H 2450 5675 50  0001 C CNN
	1    2450 5675
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5675 2450 5725
Wire Wire Line
	2450 5725 2500 5725
Text Label 2500 5825 2    50   ~ 0
SPI_CLK
Text Label 2500 5925 2    50   ~ 0
SPI_MOSI
$Comp
L Connector_Generic:Conn_01x06 J4
U 1 1 5D59BBBF
P 5600 4125
F 0 "J4" H 5680 4117 50  0000 L CNN
F 1 "ICSP" H 5680 4026 50  0000 L CNN
F 2 "Connector_PinHeader_1.27mm:PinHeader_1x06_P1.27mm_Vertical" H 5600 4125 50  0001 C CNN
F 3 "~" H 5600 4125 50  0001 C CNN
	1    5600 4125
	1    0    0    -1  
$EndComp
Wire Wire Line
	4775 4225 5400 4225
Wire Wire Line
	4775 4325 5400 4325
Wire Wire Line
	4775 4425 5400 4425
Text Label 4925 4225 0    50   ~ 0
SPI_CLK
Text Label 4925 4325 0    50   ~ 0
SPI_MOSI
Wire Wire Line
	5125 6325 5225 6325
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 5DAFD626
P 4975 6325
F 0 "JP2" H 4900 6400 50  0000 C CNN
F 1 "DFU" H 5100 6375 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 4975 6325 50  0001 C CNN
F 3 "~" H 4975 6325 50  0001 C CNN
	1    4975 6325
	1    0    0    -1  
$EndComp
Wire Wire Line
	5225 6325 5225 6375
$Comp
L power:GND #PWR0104
U 1 1 5DA2D01E
P 5225 6375
F 0 "#PWR0104" H 5225 6125 50  0001 C CNN
F 1 "GND" H 5230 6202 50  0000 C CNN
F 2 "" H 5225 6375 50  0001 C CNN
F 3 "" H 5225 6375 50  0001 C CNN
	1    5225 6375
	1    0    0    -1  
$EndComp
Wire Wire Line
	4775 6325 4825 6325
$EndSCHEMATC

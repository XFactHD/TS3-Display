EESchema Schematic File Version 4
EELAYER 30 0
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
$Comp
L Custom:ATSAMD21G18A-AU U3
U 1 1 5FB923A8
P 5350 3950
F 0 "U3" H 5350 5817 50  0000 C CNN
F 1 "ATSAMD21G18A-AU" H 5350 5726 50  0000 C CNN
F 2 "Housings_DFN_QFN:QFN-48-1EP_7x7mm_Pitch0.5mm" H 5350 3950 50  0001 L BNN
F 3 "TQFP-48 Microchip" H 5350 3950 50  0001 L BNN
F 4 "ATSAMD21G18A-AU" H 5350 3950 50  0001 L BNN "Field4"
F 5 "SAMD21G Series 256 kB Flash 32 kB SRAM 48 MHz 32-Bit Microcontroller - TQFP-48" H 5350 3950 50  0001 L BNN "Field5"
F 6 "None" H 5350 3950 50  0001 L BNN "Field6"
F 7 "Microchip" H 5350 3950 50  0001 L BNN "Field7"
F 8 "Unavailable" H 5350 3950 50  0001 L BNN "Field8"
	1    5350 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3250 4650 3250
Wire Wire Line
	4600 3350 4650 3350
Wire Wire Line
	4600 3450 4650 3450
Wire Wire Line
	4600 3550 4650 3550
Wire Wire Line
	4600 3650 4650 3650
Wire Wire Line
	4600 3750 4650 3750
Wire Wire Line
	4600 3850 4650 3850
Wire Wire Line
	4600 3950 4650 3950
Wire Wire Line
	4600 4050 4650 4050
Wire Wire Line
	4600 4150 4650 4150
Wire Wire Line
	4600 4250 4650 4250
Wire Wire Line
	4600 4350 4650 4350
Wire Wire Line
	4600 4450 4650 4450
Wire Wire Line
	4600 4550 4650 4550
Wire Wire Line
	4600 4650 4650 4650
Wire Wire Line
	4600 4750 4650 4750
Text GLabel 4600 3150 0    50   Input ~ 0
~TFT_WR
Text GLabel 6100 3550 2    50   Input ~ 0
TFT_DC
Text GLabel 6100 3450 2    50   Input ~ 0
~TFT_CS
Text GLabel 6100 3950 2    50   Input ~ 0
~TFT_RST
Text GLabel 4600 5050 0    50   Input ~ 0
USB_D-
Text GLabel 4600 5150 0    50   Input ~ 0
USB_D+
Wire Wire Line
	4600 5050 4650 5050
Wire Wire Line
	4600 5150 4650 5150
Wire Wire Line
	6100 3950 6050 3950
Wire Wire Line
	6100 3450 6050 3450
Wire Wire Line
	6100 3550 6050 3550
Wire Wire Line
	4600 3150 4650 3150
$Comp
L Device:C C11
U 1 1 5FB9D982
P 6300 2750
F 0 "C11" V 6048 2750 50  0000 C CNN
F 1 "1u" V 6139 2750 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6338 2600 50  0001 C CNN
F 3 "~" H 6300 2750 50  0001 C CNN
	1    6300 2750
	0    1    1    0   
$EndComp
Text GLabel 6500 2800 3    50   Input ~ 0
GND
Wire Wire Line
	6450 2750 6500 2750
Wire Wire Line
	6500 2750 6500 2800
Text GLabel 6150 2350 2    50   Input ~ 0
3.3V
Wire Wire Line
	6050 2350 6100 2350
Wire Wire Line
	6100 2350 6100 2450
Wire Wire Line
	6100 2450 6050 2450
Connection ~ 6100 2350
Wire Wire Line
	6100 2350 6150 2350
Wire Wire Line
	6100 2450 6100 2550
Wire Wire Line
	6100 2550 6050 2550
Connection ~ 6100 2450
Wire Wire Line
	6100 2550 6100 2650
Wire Wire Line
	6100 2650 6050 2650
Connection ~ 6100 2550
Text GLabel 6150 5550 2    50   Input ~ 0
GND
Wire Wire Line
	6050 5050 6100 5050
Wire Wire Line
	6100 5050 6100 5150
Wire Wire Line
	6100 5150 6050 5150
Wire Wire Line
	6100 5150 6100 5250
Wire Wire Line
	6100 5250 6050 5250
Connection ~ 6100 5150
Wire Wire Line
	6100 5250 6100 5350
Wire Wire Line
	6100 5350 6050 5350
Connection ~ 6100 5250
Text GLabel 6100 3850 2    50   Input ~ 0
TFT_BL
Wire Wire Line
	6050 3850 6100 3850
Wire Wire Line
	4600 2750 4650 2750
Wire Wire Line
	4600 2650 4650 2650
$Comp
L Device:C C4
U 1 1 5FBAC4E5
P 3750 2900
F 0 "C4" V 3800 2800 50  0000 C CNN
F 1 "9p" V 3800 3000 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 3788 2750 50  0001 C CNN
F 3 "~" H 3750 2900 50  0001 C CNN
	1    3750 2900
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 5FBADD6A
P 3750 2500
F 0 "C5" V 3700 2400 50  0000 C CNN
F 1 "9p" V 3700 2600 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 3788 2350 50  0001 C CNN
F 3 "~" H 3750 2500 50  0001 C CNN
	1    3750 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	4600 2500 4600 2650
Wire Wire Line
	4600 2750 4600 2900
Wire Wire Line
	3600 2500 3550 2500
Text GLabel 3550 2950 3    50   Input ~ 0
GND
Wire Wire Line
	6050 2750 6150 2750
$Comp
L Custom:NHD-2.8-240320AF-CSXP-F U4
U 1 1 5FBBECF6
P 9250 2500
F 0 "U4" H 9250 2550 50  0000 C CNN
F 1 "NHD-2.8-240320AF-CSXP-F" H 9250 2450 50  0000 C CNN
F 2 "Custom:NHD-2.8-240320AF-CSXP-F" H 9250 2500 50  0001 C CNN
F 3 "" H 9250 2500 50  0001 C CNN
	1    9250 2500
	1    0    0    -1  
$EndComp
Text GLabel 9250 3600 3    50   Input ~ 0
GND
Text GLabel 9250 1400 1    50   Input ~ 0
3.3V
Wire Wire Line
	9250 1400 9250 1450
Wire Wire Line
	9200 1450 9250 1450
Wire Wire Line
	9300 1450 9300 1500
Wire Wire Line
	9200 1450 9200 1500
Wire Wire Line
	9150 3500 9150 3550
Wire Wire Line
	9150 3550 9250 3550
Wire Wire Line
	9350 3550 9350 3500
Wire Wire Line
	9250 3500 9250 3550
Connection ~ 9250 3550
Wire Wire Line
	9250 3550 9350 3550
Wire Wire Line
	9250 3600 9250 3550
Text GLabel 8200 1950 0    50   Input ~ 0
~TFT_CS
Text GLabel 8200 2050 0    50   Input ~ 0
TFT_DC
Text GLabel 8200 2350 0    50   Input ~ 0
~TFT_RST
Text GLabel 8200 2150 0    50   Input ~ 0
~TFT_WR
Text GLabel 8200 2250 0    50   Input ~ 0
3.3V
Wire Wire Line
	8200 1950 8250 1950
Wire Wire Line
	8200 2050 8250 2050
Wire Wire Line
	8200 2150 8250 2150
Wire Wire Line
	8200 2250 8250 2250
Wire Wire Line
	8200 2350 8250 2350
Connection ~ 9250 1450
Wire Wire Line
	9250 1450 9300 1450
Wire Wire Line
	10300 1750 10250 1750
Wire Wire Line
	10300 1850 10250 1850
Wire Wire Line
	10300 1950 10250 1950
Wire Wire Line
	10300 2050 10250 2050
Wire Wire Line
	10300 2150 10250 2150
Wire Wire Line
	10300 2250 10250 2250
Wire Wire Line
	10300 2350 10250 2350
Wire Wire Line
	10300 2450 10250 2450
Wire Wire Line
	10300 2550 10250 2550
Wire Wire Line
	10300 2650 10250 2650
Wire Wire Line
	10300 2750 10250 2750
Wire Wire Line
	10300 2850 10250 2850
Wire Wire Line
	10300 2950 10250 2950
Wire Wire Line
	10300 3050 10250 3050
Wire Wire Line
	10300 3150 10250 3150
Wire Wire Line
	10300 3250 10250 3250
Text GLabel 8150 3050 0    50   Input ~ 0
LEDA
Text GLabel 8150 2950 0    50   Input ~ 0
LEDK
Wire Wire Line
	8150 3050 8250 3050
Wire Wire Line
	8150 2950 8200 2950
Wire Wire Line
	8200 2950 8200 2850
Wire Wire Line
	8200 2850 8250 2850
Connection ~ 8200 2950
Wire Wire Line
	8200 2950 8250 2950
Wire Wire Line
	8200 2850 8200 2750
Wire Wire Line
	8200 2750 8250 2750
Connection ~ 8200 2850
Wire Wire Line
	8200 2750 8200 2650
Wire Wire Line
	8200 2650 8250 2650
Connection ~ 8200 2750
$Comp
L Device:C C7
U 1 1 5FBEC9E7
P 4650 1600
F 0 "C7" H 4765 1646 50  0000 L CNN
F 1 "100n" H 4765 1555 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4688 1450 50  0001 C CNN
F 3 "~" H 4650 1600 50  0001 C CNN
	1    4650 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 5FBED905
P 5100 1600
F 0 "C8" H 5215 1646 50  0000 L CNN
F 1 "100n" H 5215 1555 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5138 1450 50  0001 C CNN
F 3 "~" H 5100 1600 50  0001 C CNN
	1    5100 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 5FBEE295
P 5550 1600
F 0 "C9" H 5665 1646 50  0000 L CNN
F 1 "100n" H 5665 1555 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5588 1450 50  0001 C CNN
F 3 "~" H 5550 1600 50  0001 C CNN
	1    5550 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 5FBEE74B
P 6000 1600
F 0 "C10" H 6115 1646 50  0000 L CNN
F 1 "100n" H 6115 1555 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6038 1450 50  0001 C CNN
F 3 "~" H 6000 1600 50  0001 C CNN
	1    6000 1600
	1    0    0    -1  
$EndComp
Text GLabel 6000 1400 1    50   Input ~ 0
3.3V
Text GLabel 5550 1400 1    50   Input ~ 0
3.3V
Text GLabel 5100 1400 1    50   Input ~ 0
3.3V
Text GLabel 4650 1400 1    50   Input ~ 0
3.3V
Text GLabel 4650 1800 3    50   Input ~ 0
GND
Text GLabel 5100 1800 3    50   Input ~ 0
GND
Text GLabel 5550 1800 3    50   Input ~ 0
GND
Text GLabel 6000 1800 3    50   Input ~ 0
GND
Wire Wire Line
	4650 1400 4650 1450
Wire Wire Line
	5100 1400 5100 1450
Wire Wire Line
	5550 1400 5550 1450
Wire Wire Line
	6000 1400 6000 1450
Wire Wire Line
	4650 1750 4650 1800
Wire Wire Line
	5100 1750 5100 1800
Wire Wire Line
	5550 1750 5550 1800
Wire Wire Line
	6000 1750 6000 1800
Text GLabel 9550 4850 1    50   Input ~ 0
5V
Text GLabel 9550 6000 3    50   Input ~ 0
GND
$Comp
L Device:C C13
U 1 1 5FC14730
P 8300 5400
F 0 "C13" H 8185 5354 50  0000 R CNN
F 1 "10u" H 8185 5445 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805" H 8338 5250 50  0001 C CNN
F 3 "~" H 8300 5400 50  0001 C CNN
	1    8300 5400
	-1   0    0    1   
$EndComp
Text GLabel 8300 5200 1    50   Input ~ 0
5V
Text GLabel 8300 5600 3    50   Input ~ 0
GND
Text GLabel 10200 5500 2    50   Input ~ 0
LEDK
Text GLabel 10700 5300 2    50   Input ~ 0
LEDA
Text GLabel 8950 5400 0    50   Input ~ 0
TFT_BL
Wire Wire Line
	8300 5200 8300 5250
Wire Wire Line
	8300 5550 8300 5600
$Comp
L Device:R R1
U 1 1 5FC5F943
P 10150 5700
F 0 "R1" H 10220 5746 50  0000 L CNN
F 1 "1" H 10220 5655 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 10080 5700 50  0001 C CNN
F 3 "~" H 10150 5700 50  0001 C CNN
	1    10150 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 5FC81EBC
P 10300 5300
F 0 "L1" V 10490 5300 50  0000 C CNN
F 1 "10u" V 10399 5300 50  0000 C CNN
F 2 "Inductors_SMD:L_Taiyo-Yuden_NR-40xx" H 10300 5300 50  0001 C CNN
F 3 "~" H 10300 5300 50  0001 C CNN
	1    10300 5300
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C6
U 1 1 5FC8B666
P 4250 1600
F 0 "C6" H 4365 1646 50  0000 L CNN
F 1 "1u" H 4365 1555 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4288 1450 50  0001 C CNN
F 3 "~" H 4250 1600 50  0001 C CNN
	1    4250 1600
	1    0    0    -1  
$EndComp
Text GLabel 4250 1800 3    50   Input ~ 0
GND
Text GLabel 4250 1400 1    50   Input ~ 0
3.3V
Wire Wire Line
	4250 1400 4250 1450
Wire Wire Line
	4250 1750 4250 1800
$Comp
L Regulator_Linear:SPX3819M5-L-3-3 U1
U 1 1 5FC94A55
P 1700 2750
F 0 "U1" H 1700 3092 50  0000 C CNN
F 1 "SPX3819M5-L-3-3" H 1700 3001 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 1700 3075 50  0001 C CNN
F 3 "https://www.exar.com/content/document.ashx?id=22106&languageid=1033&type=Datasheet&partnumber=SPX3819&filename=SPX3819.pdf&part=SPX3819" H 1700 2750 50  0001 C CNN
	1    1700 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 2750 1350 2750
Wire Wire Line
	1350 2750 1350 2650
Wire Wire Line
	1350 2650 1400 2650
Wire Wire Line
	1350 2650 1300 2650
Connection ~ 1350 2650
Text GLabel 1300 2650 0    50   Input ~ 0
5V
$Comp
L Device:C C2
U 1 1 5FCAFE5E
P 2050 2950
F 0 "C2" H 1935 2904 50  0000 R CNN
F 1 "10n" H 1935 2995 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805" H 2088 2800 50  0001 C CNN
F 3 "~" H 2050 2950 50  0001 C CNN
	1    2050 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2000 2750 2050 2750
Wire Wire Line
	2050 2750 2050 2800
Wire Wire Line
	1350 2750 1350 2800
Connection ~ 1350 2750
$Comp
L Device:C C3
U 1 1 5FCC5AB9
P 2450 2950
F 0 "C3" H 2565 2996 50  0000 L CNN
F 1 "10u" H 2565 2905 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2488 2800 50  0001 C CNN
F 3 "~" H 2450 2950 50  0001 C CNN
	1    2450 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 3150 1700 3150
$Comp
L Device:C C1
U 1 1 5FCBA599
P 1350 2950
F 0 "C1" H 1100 3000 50  0000 L CNN
F 1 "1u" H 1100 2900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1388 2800 50  0001 C CNN
F 3 "~" H 1350 2950 50  0001 C CNN
	1    1350 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 3150 1700 3200
Connection ~ 1700 3150
Wire Wire Line
	1700 3150 2050 3150
Text GLabel 1700 3200 3    50   Input ~ 0
GND
Wire Wire Line
	1350 3100 1350 3150
Wire Wire Line
	1700 3050 1700 3150
Wire Wire Line
	2050 3100 2050 3150
Wire Wire Line
	2050 3150 2450 3150
Wire Wire Line
	2450 3150 2450 3100
Connection ~ 2050 3150
Wire Wire Line
	2000 2650 2450 2650
Wire Wire Line
	2450 2650 2450 2800
Connection ~ 2450 2650
Wire Wire Line
	2450 2650 2500 2650
Text GLabel 2500 2650 2    50   Input ~ 0
3.3V
Text GLabel 4600 3250 0    50   Input ~ 0
TFT_D0
Text GLabel 4600 3350 0    50   Input ~ 0
TFT_D1
Text GLabel 4600 3450 0    50   Input ~ 0
TFT_D2
Text GLabel 4600 3550 0    50   Input ~ 0
TFT_D3
Text GLabel 4600 3650 0    50   Input ~ 0
TFT_D4
Text GLabel 4600 3750 0    50   Input ~ 0
TFT_D5
Text GLabel 4600 3850 0    50   Input ~ 0
TFT_D6
Text GLabel 4600 3950 0    50   Input ~ 0
TFT_D7
Text GLabel 4600 4050 0    50   Input ~ 0
TFT_D8
Text GLabel 4600 4150 0    50   Input ~ 0
TFT_D9
Text GLabel 4600 4250 0    50   Input ~ 0
TFT_D10
Text GLabel 4600 4350 0    50   Input ~ 0
TFT_D11
Text GLabel 4600 4450 0    50   Input ~ 0
TFT_D12
Text GLabel 4600 4550 0    50   Input ~ 0
TFT_D13
Text GLabel 4600 4650 0    50   Input ~ 0
TFT_D14
Text GLabel 4600 4750 0    50   Input ~ 0
TFT_D15
Text GLabel 10300 1750 2    50   Input ~ 0
TFT_D0
Text GLabel 10300 1850 2    50   Input ~ 0
TFT_D1
Text GLabel 10300 1950 2    50   Input ~ 0
TFT_D2
Text GLabel 10300 2050 2    50   Input ~ 0
TFT_D3
Text GLabel 10300 2150 2    50   Input ~ 0
TFT_D4
Text GLabel 10300 2250 2    50   Input ~ 0
TFT_D5
Text GLabel 10300 2350 2    50   Input ~ 0
TFT_D6
Text GLabel 10300 2450 2    50   Input ~ 0
TFT_D7
Text GLabel 10300 2550 2    50   Input ~ 0
TFT_D8
Text GLabel 10300 2650 2    50   Input ~ 0
TFT_D9
Text GLabel 10300 2750 2    50   Input ~ 0
TFT_D10
Text GLabel 10300 2850 2    50   Input ~ 0
TFT_D11
Text GLabel 10300 2950 2    50   Input ~ 0
TFT_D12
Text GLabel 10300 3050 2    50   Input ~ 0
TFT_D13
Text GLabel 10300 3150 2    50   Input ~ 0
TFT_D14
Text GLabel 10300 3250 2    50   Input ~ 0
TFT_D15
$Comp
L Device:C C15
U 1 1 5FC75A66
P 10600 5500
F 0 "C15" H 10485 5454 50  0000 R CNN
F 1 "10u" H 10485 5545 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805" H 10638 5350 50  0001 C CNN
F 3 "~" H 10600 5500 50  0001 C CNN
	1    10600 5500
	-1   0    0    1   
$EndComp
$Comp
L Custom:PAM2804 U5
U 1 1 5FDB536F
P 9550 5400
F 0 "U5" H 9550 5450 50  0000 C CNN
F 1 "PAM2804" H 9550 5350 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:TSOT-23-5" H 9550 5400 50  0001 C CNN
F 3 "" H 9550 5400 50  0001 C CNN
	1    9550 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5900 9550 5950
Wire Wire Line
	9550 5950 10150 5950
Wire Wire Line
	10150 5950 10150 5850
Connection ~ 9550 5950
Wire Wire Line
	9550 5950 9550 6000
Wire Wire Line
	10100 5500 10150 5500
Wire Wire Line
	10150 5500 10150 5550
Connection ~ 10150 5500
Wire Wire Line
	10150 5500 10200 5500
Wire Wire Line
	10100 5300 10150 5300
Wire Wire Line
	10150 5950 10600 5950
Wire Wire Line
	10600 5950 10600 5650
Connection ~ 10150 5950
Wire Wire Line
	10600 5300 10600 5350
Connection ~ 10600 5300
Wire Wire Line
	10600 5300 10700 5300
Wire Wire Line
	10450 5300 10600 5300
Wire Wire Line
	8950 5400 9000 5400
Wire Wire Line
	9550 4850 9550 4900
$Comp
L Device:C C14
U 1 1 5FE35BC2
P 8700 1150
F 0 "C14" H 8815 1196 50  0000 L CNN
F 1 "100n" H 8815 1105 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8738 1000 50  0001 C CNN
F 3 "~" H 8700 1150 50  0001 C CNN
	1    8700 1150
	1    0    0    -1  
$EndComp
Text GLabel 8700 950  1    50   Input ~ 0
3.3V
Text GLabel 8700 1350 3    50   Input ~ 0
GND
Wire Wire Line
	8700 950  8700 1000
Wire Wire Line
	8700 1300 8700 1350
$Comp
L Device:C C12
U 1 1 5FE35E3C
P 8300 1150
F 0 "C12" H 8415 1196 50  0000 L CNN
F 1 "1u" H 8415 1105 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8338 1000 50  0001 C CNN
F 3 "~" H 8300 1150 50  0001 C CNN
	1    8300 1150
	1    0    0    -1  
$EndComp
Text GLabel 8300 1350 3    50   Input ~ 0
GND
Text GLabel 8300 950  1    50   Input ~ 0
3.3V
Wire Wire Line
	8300 950  8300 1000
Wire Wire Line
	8300 1300 8300 1350
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5FE3EB5F
P 700 1050
F 0 "J1" H 600 1450 50  0000 C CNN
F 1 "Conn_01x05" H 600 1350 50  0000 C CNN
F 2 "Connectors_JST:JST_SH_BM05B-SRSS-TB_05x1.00mm_Straight" H 700 1050 50  0001 C CNN
F 3 "~" H 700 1050 50  0001 C CNN
	1    700  1050
	-1   0    0    1   
$EndComp
Text GLabel 950  950  2    50   Input ~ 0
5V
Text GLabel 950  1250 2    50   Input ~ 0
GND
Text GLabel 2100 1300 0    50   Input ~ 0
USB_D-
Text GLabel 3000 1300 2    50   Input ~ 0
USB_D+
$Comp
L Power_Protection:USBLC6-2P6 U2
U 1 1 5FE4D2C2
P 2550 1200
F 0 "U2" H 2300 1450 50  0000 C CNN
F 1 "USBLC6-2P6" H 2550 950 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 2550 700 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/usblc6-2.pdf" H 2750 1550 50  0001 C CNN
	1    2550 1200
	-1   0    0    -1  
$EndComp
Text GLabel 2550 750  1    50   Input ~ 0
5V
Text GLabel 2550 1650 3    50   Input ~ 0
GND
Text GLabel 2100 1100 0    50   Input ~ 0
USB_US_D-
Text GLabel 3000 1100 2    50   Input ~ 0
USB_US_D+
Text GLabel 950  1150 2    50   Input ~ 0
USB_US_D+
Text GLabel 950  1050 2    50   Input ~ 0
USB_US_D-
Wire Wire Line
	900  950  950  950 
Wire Wire Line
	900  1050 950  1050
Wire Wire Line
	900  1150 950  1150
Wire Wire Line
	900  1250 950  1250
Wire Wire Line
	2100 1100 2150 1100
Wire Wire Line
	2100 1300 2150 1300
Wire Wire Line
	2550 750  2550 800 
Wire Wire Line
	2550 1600 2550 1650
Wire Wire Line
	2950 1100 3000 1100
Wire Wire Line
	2950 1300 3000 1300
Wire Wire Line
	6050 5550 6100 5550
Wire Wire Line
	6100 5550 6100 5350
Connection ~ 6100 5550
Wire Wire Line
	6100 5550 6150 5550
Connection ~ 6100 5350
$Comp
L Connector:Conn_ARM_JTAG_SWD_10 J2
U 1 1 5FD5D1FB
P 1900 4650
F 0 "J2" H 1700 5250 50  0000 R CNN
F 1 "Conn_ARM_JTAG_SWD_10" V 1600 5100 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch1.27mm_SMD" H 1900 4650 50  0001 C CNN
F 3 "http://infocenter.arm.com/help/topic/com.arm.doc.ddi0314h/DDI0314H_coresight_components_trm.pdf" V 1550 3400 50  0001 C CNN
	1    1900 4650
	1    0    0    -1  
$EndComp
Text GLabel 1800 5300 3    50   Input ~ 0
GND
Text GLabel 1900 5300 3    50   Input ~ 0
GND
Text GLabel 1900 4000 1    50   Input ~ 0
3.3V
Text GLabel 2450 4350 2    50   Input ~ 0
~RESET
Text GLabel 4600 2350 0    50   Input ~ 0
~RESET
Wire Wire Line
	4600 2350 4650 2350
Wire Wire Line
	2400 4350 2450 4350
Text GLabel 2450 4650 2    50   Input ~ 0
SWDIO
Text GLabel 2450 4550 2    50   Input ~ 0
SWDCLK
Text GLabel 4600 5450 0    50   Input ~ 0
SWDCLK
Text GLabel 4600 5550 0    50   Input ~ 0
SWDIO
Wire Wire Line
	2400 4550 2450 4550
Wire Wire Line
	2400 4650 2450 4650
Wire Wire Line
	1900 4000 1900 4050
Wire Wire Line
	1800 5250 1800 5300
Wire Wire Line
	1900 5250 1900 5300
Wire Wire Line
	4600 5450 4650 5450
Wire Wire Line
	4600 5550 4650 5550
$Comp
L Device:R R2
U 1 1 5FDA6B25
P 1250 4550
F 0 "R2" H 1320 4596 50  0000 L CNN
F 1 "10k" H 1320 4505 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 1180 4550 50  0001 C CNN
F 3 "~" H 1250 4550 50  0001 C CNN
	1    1250 4550
	1    0    0    -1  
$EndComp
Text GLabel 1250 4750 3    50   Input ~ 0
SWDCLK
Text GLabel 1250 4350 1    50   Input ~ 0
3.3V
Wire Wire Line
	1250 4350 1250 4400
Wire Wire Line
	1250 4700 1250 4750
$Comp
L Switch:SW_Push SW1
U 1 1 603D339B
P 3600 4550
F 0 "SW1" V 3646 4502 50  0000 R CNN
F 1 "SW_Push" V 3555 4502 50  0000 R CNN
F 2 "Buttons_Switches_SMD:SW_SPST_PTS810" H 3600 4750 50  0001 C CNN
F 3 "~" H 3600 4750 50  0001 C CNN
	1    3600 4550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 603D5250
P 3600 4100
F 0 "R3" H 3670 4146 50  0000 L CNN
F 1 "10k" H 3670 4055 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 3530 4100 50  0001 C CNN
F 3 "~" H 3600 4100 50  0001 C CNN
	1    3600 4100
	1    0    0    -1  
$EndComp
Text GLabel 3600 4800 3    50   Input ~ 0
GND
Text GLabel 3550 4300 0    50   Input ~ 0
~RESET
Text GLabel 3600 3900 1    50   Input ~ 0
3.3V
Wire Wire Line
	3600 3900 3600 3950
Wire Wire Line
	3600 4250 3600 4300
Wire Wire Line
	3550 4300 3600 4300
Connection ~ 3600 4300
Wire Wire Line
	3600 4300 3600 4350
Wire Wire Line
	3600 4750 3600 4800
$Comp
L Device:Crystal_GND23 Y1
U 1 1 6075063B
P 4100 2700
F 0 "Y1" V 4100 2650 50  0000 L CNN
F 1 "32.768k" V 4250 2750 50  0000 L CNN
F 2 "Crystals:Crystal_SMD_SeikoEpson_MC146-4pin_6.7x1.5mm" H 4100 2700 50  0001 C CNN
F 3 "~" H 4100 2700 50  0001 C CNN
	1    4100 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	3900 2500 4100 2500
Wire Wire Line
	3900 2900 4100 2900
Wire Wire Line
	4100 2850 4100 2900
Connection ~ 4100 2900
Wire Wire Line
	4100 2900 4600 2900
Wire Wire Line
	4100 2550 4100 2500
Connection ~ 4100 2500
Wire Wire Line
	4100 2500 4600 2500
Text GLabel 4350 2700 2    50   Input ~ 0
GND
Wire Wire Line
	3550 2900 3550 2950
Wire Wire Line
	3600 2900 3550 2900
Connection ~ 3550 2700
Wire Wire Line
	3900 2700 3550 2700
Connection ~ 3550 2900
Wire Wire Line
	3550 2500 3550 2700
Wire Wire Line
	3550 2700 3550 2900
Wire Wire Line
	4300 2700 4350 2700
$EndSCHEMATC

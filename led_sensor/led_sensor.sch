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
L led_sensor-rescue:SN74HCS151QPWRQ1-SN74HCS151QPWRQ1 HM#1
U 1 1 615917A2
P 3300 2950
F 0 "HM#1" H 3800 3215 50  0000 C CNN
F 1 "SN74HCS151QPWRQ1" H 3800 3124 50  0000 C CNN
F 2 "SOP65P640X120-16N" H 4150 3050 50  0001 L CNN
F 3 "https://www.ti.com/lit/gpn/SN74HCS151-Q1" H 4150 2950 50  0001 L CNN
F 4 "Encoders, Decoders, Multiplexers & Demultiplexers 8-to-1 multiplexer with Schmitt-trigger inputs 16-TSSOP -40 to 125" H 4150 2850 50  0001 L CNN "Description"
F 5 "1.2" H 4150 2750 50  0001 L CNN "Height"
F 6 "Texas Instruments" H 4150 2650 50  0001 L CNN "Manufacturer_Name"
F 7 "SN74HCS151QPWRQ1" H 4150 2550 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "595-SN74HCS151QPWRQ1" H 4150 2450 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/SN74HCS151QPWRQ1?qs=zW32dvEIR3vrj7NdSe9MXg%3D%3D" H 4150 2350 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 4150 2250 50  0001 L CNN "Arrow Part Number"
F 11 "" H 4150 2150 50  0001 L CNN "Arrow Price/Stock"
	1    3300 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	2700 2950 2600 2950
Connection ~ 2600 2950
Wire Wire Line
	2800 3950 2800 4700
Wire Wire Line
	2700 3950 2700 4550
Wire Wire Line
	2600 3950 2600 4400
$Comp
L led_sensor-rescue:AH3362Q-HallSensor H#0
U 1 1 615A38AC
P 4750 3350
F 0 "H#0" H 4825 3365 50  0000 C CNN
F 1 "AH3362Q" H 4825 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 4750 3350 50  0001 C CNN
F 3 "" H 4750 3350 50  0001 C CNN
	1    4750 3350
	0    1    1    0   
$EndComp
$Comp
L led_sensor-rescue:AH3362Q-HallSensor U#1
U 1 1 615A5ADB
P 5500 3350
F 0 "U#1" H 5575 3365 50  0000 C CNN
F 1 "AH3362Q" H 5575 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 5500 3350 50  0001 C CNN
F 3 "" H 5500 3350 50  0001 C CNN
	1    5500 3350
	0    1    1    0   
$EndComp
$Comp
L led_sensor-rescue:AH3362Q-HallSensor U#2
U 1 1 615A5DEF
P 6150 3350
F 0 "U#2" H 6225 3365 50  0000 C CNN
F 1 "AH3362Q" H 6225 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 6150 3350 50  0001 C CNN
F 3 "" H 6150 3350 50  0001 C CNN
	1    6150 3350
	0    1    1    0   
$EndComp
$Comp
L led_sensor-rescue:AH3362Q-HallSensor U#3
U 1 1 615A6394
P 6800 3350
F 0 "U#3" H 6875 3365 50  0000 C CNN
F 1 "AH3362Q" H 6875 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 6800 3350 50  0001 C CNN
F 3 "" H 6800 3350 50  0001 C CNN
	1    6800 3350
	0    1    1    0   
$EndComp
$Comp
L led_sensor-rescue:AH3362Q-HallSensor U#4
U 1 1 615A67A4
P 7500 3350
F 0 "U#4" H 7575 3365 50  0000 C CNN
F 1 "AH3362Q" H 7575 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 7500 3350 50  0001 C CNN
F 3 "" H 7500 3350 50  0001 C CNN
	1    7500 3350
	0    1    1    0   
$EndComp
$Comp
L led_sensor-rescue:AH3362Q-HallSensor U#5
U 1 1 615A6E92
P 8200 3350
F 0 "U#5" H 8275 3365 50  0000 C CNN
F 1 "AH3362Q" H 8275 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 8200 3350 50  0001 C CNN
F 3 "" H 8200 3350 50  0001 C CNN
	1    8200 3350
	0    1    1    0   
$EndComp
$Comp
L led_sensor-rescue:AH3362Q-HallSensor U#6
U 1 1 615A72E7
P 8850 3350
F 0 "U#6" H 8925 3365 50  0000 C CNN
F 1 "AH3362Q" H 8925 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 8850 3350 50  0001 C CNN
F 3 "" H 8850 3350 50  0001 C CNN
	1    8850 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	4200 3150 4200 1650
Wire Wire Line
	4200 1650 2600 1650
Wire Wire Line
	2600 1650 2600 2950
Connection ~ 7850 3150
Wire Wire Line
	7850 3150 7150 3150
Connection ~ 7150 3150
Wire Wire Line
	7150 3150 6450 3150
Connection ~ 6450 3150
Wire Wire Line
	6450 3150 5800 3150
Connection ~ 5800 3150
Wire Wire Line
	5800 3150 5150 3150
Connection ~ 5150 3150
Wire Wire Line
	5150 3150 4400 3150
Connection ~ 4400 3150
Wire Wire Line
	4400 3150 4200 3150
Wire Wire Line
	3000 2950 3000 1750
Wire Wire Line
	3000 1750 4100 1750
Wire Wire Line
	4100 1750 4100 3800
Wire Wire Line
	4100 3800 4500 3800
Wire Wire Line
	4500 3800 4500 3700
Wire Wire Line
	3100 2950 3100 1850
Wire Wire Line
	3100 1850 4050 1850
Wire Wire Line
	4050 1850 4050 3850
Wire Wire Line
	4050 3850 5250 3850
Wire Wire Line
	5250 3850 5250 3700
Wire Wire Line
	3200 2950 3200 1950
Wire Wire Line
	3200 1950 4000 1950
Wire Wire Line
	4000 1950 4000 3900
Wire Wire Line
	4000 3900 5900 3900
Wire Wire Line
	5900 3900 5900 3700
Wire Wire Line
	3300 2950 3300 2050
Wire Wire Line
	3300 2050 3950 2050
Wire Wire Line
	3950 2050 3950 3950
Wire Wire Line
	3950 3950 6550 3950
Wire Wire Line
	6550 3950 6550 3700
Wire Wire Line
	3200 3950 3200 4000
Wire Wire Line
	3200 4000 7250 4000
Wire Wire Line
	7250 4000 7250 3700
Wire Wire Line
	3100 3950 3100 4050
Wire Wire Line
	3100 4050 7950 4050
Wire Wire Line
	7950 4050 7950 3700
Wire Wire Line
	3000 3950 3000 4100
Wire Wire Line
	3000 4100 8600 4100
Wire Wire Line
	8600 4100 8600 3700
$Comp
L led_sensor-rescue:AH3362Q-HallSensor H#7
U 1 1 615B3333
P 9500 3350
F 0 "H#7" H 9575 3365 50  0000 C CNN
F 1 "AH3362Q" H 9575 3274 50  0000 C CNN
F 2 "Hall:AH3662Q" H 9500 3350 50  0001 C CNN
F 3 "" H 9500 3350 50  0001 C CNN
	1    9500 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	9150 3150 8500 3150
Connection ~ 8500 3150
Wire Wire Line
	8500 3150 7850 3150
Wire Wire Line
	2900 3950 2900 4150
Wire Wire Line
	2900 4150 9250 4150
Wire Wire Line
	9250 4150 9250 3700
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED0
U 1 1 6153BA49
P 3750 5100
F 0 "LED0" H 3750 5050 50  0000 R CNN
F 1 "XZCFBB53W-8VF" H 3700 4850 50  0000 R CNN
F 2 "SamacSys_Parts:HSMA-C380" H 3750 5100 50  0001 C CNN
F 3 "" H 3750 5100 50  0001 C CNN
	1    3750 5100
	0    -1   -1   0   
$EndComp
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED1
U 1 1 6153D881
P 4850 5100
F 0 "LED1" H 4900 5150 50  0000 R CNN
F 1 "XZCFBB53W-8VF" H 5100 5050 50  0000 R CNN
F 2 "SamacSys_Parts:HSMA-C380" H 4850 5100 50  0001 C CNN
F 3 "" H 4850 5100 50  0001 C CNN
	1    4850 5100
	0    -1   -1   0   
$EndComp
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED2
U 1 1 6153E4EA
P 5450 5100
F 0 "LED2" H 5404 5038 50  0000 L CNN
F 1 "XZCFBB53W-8VF" H 5050 4850 50  0000 L CNN
F 2 "SamacSys_Parts:HSMA-C380" H 5450 5100 50  0001 C CNN
F 3 "" H 5450 5100 50  0001 C CNN
	1    5450 5100
	0    -1   -1   0   
$EndComp
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED3
U 1 1 615401E5
P 6500 5100
F 0 "LED3" H 6500 5175 50  0000 C CNN
F 1 "XZCFBB53W-8VF" H 6500 5050 50  0000 C CNN
F 2 "SamacSys_Parts:HSMA-C380" H 6500 5100 50  0001 C CNN
F 3 "" H 6500 5100 50  0001 C CNN
	1    6500 5100
	0    -1   -1   0   
$EndComp
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED6
U 1 1 61544A04
P 9150 5100
F 0 "LED6" H 9104 5038 50  0000 L CNN
F 1 "XZCFBB53W-8VF" H 8800 5100 50  0000 L CNN
F 2 "SamacSys_Parts:HSMA-C380" H 9150 5100 50  0001 C CNN
F 3 "" H 9150 5100 50  0001 C CNN
	1    9150 5100
	0    -1   -1   0   
$EndComp
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED7
U 1 1 61544A0A
P 10050 5100
F 0 "LED7" H 10050 5175 50  0000 C CNN
F 1 "XZCFBB53W-8VF" H 10050 5084 50  0000 C CNN
F 2 "SamacSys_Parts:HSMA-C380" H 10050 5100 50  0001 C CNN
F 3 "" H 10050 5100 50  0001 C CNN
	1    10050 5100
	0    -1   -1   0   
$EndComp
Text Label 5000 4550 1    50   ~ 0
gin
Text Label 9300 4550 1    50   ~ 0
bin
Text Label 7700 4550 1    50   ~ 0
din
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED4
U 1 1 615449F8
P 7550 5100
F 0 "LED4" H 7600 5100 50  0000 R CNN
F 1 "XZCFBB53W-8VF" H 7700 4850 50  0000 R CNN
F 2 "SamacSys_Parts:HSMA-C380" H 7550 5100 50  0001 C CNN
F 3 "" H 7550 5100 50  0001 C CNN
	1    7550 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7150 5350 6650 5350
Connection ~ 5000 5350
Wire Wire Line
	5000 5350 3900 5350
Connection ~ 5600 5350
Wire Wire Line
	5600 5350 5000 5350
Connection ~ 6650 5350
Wire Wire Line
	6650 5350 5600 5350
Wire Wire Line
	7150 5350 7700 5350
Connection ~ 7150 5350
Connection ~ 7700 5350
Text Label 7150 4350 0    50   ~ 0
CCin
Wire Wire Line
	2600 4400 2150 4400
Wire Wire Line
	2700 4550 2150 4550
Wire Wire Line
	2150 4700 2800 4700
Text Label 3900 4550 1    50   ~ 0
dpin
Text Label 5600 4550 1    50   ~ 0
fin
Text Label 6650 4550 1    50   ~ 0
ein
Text Label 10250 4600 1    50   ~ 0
ain
Text Label 8450 4550 1    50   ~ 0
cin
Wire Wire Line
	2600 2950 1500 2950
$Comp
L power:GND #PWR0101
U 1 1 615850DC
P 1500 2950
F 0 "#PWR0101" H 1500 2700 50  0001 C CNN
F 1 "GND" H 1505 2777 50  0000 C CNN
F 2 "" H 1500 2950 50  0001 C CNN
F 3 "" H 1500 2950 50  0001 C CNN
	1    1500 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3950 3300 4250
Text Label 2150 4400 0    50   ~ 0
HALLSEL[2]
Text Label 2150 4550 0    50   ~ 0
HALLSEL[1]
Text Label 2150 4700 0    50   ~ 0
HALLSEL[0]
Wire Wire Line
	9050 4250 9050 3700
Connection ~ 3300 4250
Wire Wire Line
	3300 4250 3300 4350
Wire Wire Line
	8400 4250 8400 3700
Wire Wire Line
	3300 4250 4300 4250
Connection ~ 8400 4250
Wire Wire Line
	8400 4250 9050 4250
Wire Wire Line
	7750 4250 7750 3700
Connection ~ 7750 4250
Wire Wire Line
	7750 4250 8400 4250
Wire Wire Line
	7050 4250 7050 3700
Connection ~ 7050 4250
Wire Wire Line
	7050 4250 7750 4250
Wire Wire Line
	6350 4250 6350 3700
Connection ~ 6350 4250
Wire Wire Line
	6350 4250 7050 4250
Wire Wire Line
	5700 3700 5700 4250
Connection ~ 5700 4250
Wire Wire Line
	5700 4250 6350 4250
Wire Wire Line
	5000 4250 5000 3700
Wire Wire Line
	5000 3700 5050 3700
Connection ~ 5000 4250
Wire Wire Line
	5000 4250 5700 4250
Wire Wire Line
	4300 3700 4300 4250
Connection ~ 4300 4250
Wire Wire Line
	4300 4250 5000 4250
Connection ~ 1500 2950
Text Label 2100 2500 1    50   ~ 0
MuxOut
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 6161C7BE
P 1400 2950
F 0 "H1" V 1637 2953 50  0000 C CNN
F 1 "MountingHole_Pad" V 1546 2953 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 1400 2950 50  0001 C CNN
F 3 "~" H 1400 2950 50  0001 C CNN
	1    1400 2950
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 6161DA8A
P 2000 2500
F 0 "H2" V 2237 2503 50  0000 C CNN
F 1 "MountingHole_Pad" V 2146 2503 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 2000 2500 50  0001 C CNN
F 3 "~" H 2000 2500 50  0001 C CNN
	1    2000 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2100 2500 2800 2500
Wire Wire Line
	2800 2500 2800 2950
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 61622F7C
P 2050 4400
F 0 "H3" V 2287 4403 50  0000 C CNN
F 1 "MountingHole_Pad" V 2196 4403 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 2050 4400 50  0001 C CNN
F 3 "~" H 2050 4400 50  0001 C CNN
	1    2050 4400
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 616237C1
P 2050 4550
F 0 "H4" V 2287 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 2196 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 2050 4550 50  0001 C CNN
F 3 "~" H 2050 4550 50  0001 C CNN
	1    2050 4550
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0103
U 1 1 615EF6D2
P 3300 4350
F 0 "#PWR0103" H 3300 4200 50  0001 C CNN
F 1 "+3V3" V 3315 4478 50  0000 L CNN
F 2 "" H 3300 4350 50  0001 C CNN
F 3 "" H 3300 4350 50  0001 C CNN
	1    3300 4350
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 6162BD29
P 2050 4700
F 0 "H5" V 2287 4703 50  0000 C CNN
F 1 "MountingHole_Pad" V 2196 4703 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 2050 4700 50  0001 C CNN
F 3 "~" H 2050 4700 50  0001 C CNN
	1    2050 4700
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H6
U 1 1 6162DD50
P 3200 4350
F 0 "H6" V 3437 4353 50  0000 C CNN
F 1 "MountingHole_Pad" V 3346 4353 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 3200 4350 50  0001 C CNN
F 3 "~" H 3200 4350 50  0001 C CNN
	1    3200 4350
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H7
U 1 1 61633D8E
P 3800 4550
F 0 "H7" V 4037 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 3946 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 3800 4550 50  0001 C CNN
F 3 "~" H 3800 4550 50  0001 C CNN
	1    3800 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H8
U 1 1 61635FF3
P 4000 4550
F 0 "H8" V 4237 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 4146 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 4000 4550 50  0001 C CNN
F 3 "~" H 4000 4550 50  0001 C CNN
	1    4000 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	3900 4550 3900 4850
Connection ~ 3900 4550
Wire Wire Line
	7700 5350 8450 5350
Connection ~ 8450 5350
$Comp
L led_sensor-rescue:XZCFBB53W-8VF-xzcfbb53w-8vf LED5
U 1 1 615449FE
P 8300 5100
F 0 "LED5" H 8346 4872 50  0000 R CNN
F 1 "XZCFBB53W-8VF" H 8550 5050 50  0000 R CNN
F 2 "SamacSys_Parts:HSMA-C380" H 8300 5100 50  0001 C CNN
F 3 "" H 8300 5100 50  0001 C CNN
	1    8300 5100
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H9
U 1 1 616519FE
P 4900 4550
F 0 "H9" V 5137 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 5046 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 4900 4550 50  0001 C CNN
F 3 "~" H 4900 4550 50  0001 C CNN
	1    4900 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H10
U 1 1 61651A04
P 5100 4550
F 0 "H10" V 5337 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 5246 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 5100 4550 50  0001 C CNN
F 3 "~" H 5100 4550 50  0001 C CNN
	1    5100 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	5000 4550 5000 4850
$Comp
L Mechanical:MountingHole_Pad H11
U 1 1 61653BC4
P 5500 4550
F 0 "H11" V 5737 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 5646 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 5500 4550 50  0001 C CNN
F 3 "~" H 5500 4550 50  0001 C CNN
	1    5500 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H12
U 1 1 61653BCA
P 5700 4550
F 0 "H12" V 5937 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 5846 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 5700 4550 50  0001 C CNN
F 3 "~" H 5700 4550 50  0001 C CNN
	1    5700 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	5600 4550 5600 4850
$Comp
L Mechanical:MountingHole_Pad H13
U 1 1 616560C2
P 6550 4550
F 0 "H13" V 6787 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 6696 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 6550 4550 50  0001 C CNN
F 3 "~" H 6550 4550 50  0001 C CNN
	1    6550 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H14
U 1 1 616560C8
P 6750 4550
F 0 "H14" V 6987 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 6896 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 6750 4550 50  0001 C CNN
F 3 "~" H 6750 4550 50  0001 C CNN
	1    6750 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	6650 4550 6650 4850
$Comp
L Mechanical:MountingHole_Pad H17
U 1 1 6165A853
P 7600 4550
F 0 "H17" V 7837 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 7746 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 7600 4550 50  0001 C CNN
F 3 "~" H 7600 4550 50  0001 C CNN
	1    7600 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H18
U 1 1 6165A859
P 7800 4550
F 0 "H18" V 8037 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 7946 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 7800 4550 50  0001 C CNN
F 3 "~" H 7800 4550 50  0001 C CNN
	1    7800 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	7700 4550 7700 4850
$Comp
L Mechanical:MountingHole_Pad H19
U 1 1 6165CB72
P 8350 4550
F 0 "H19" V 8587 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 8496 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 8350 4550 50  0001 C CNN
F 3 "~" H 8350 4550 50  0001 C CNN
	1    8350 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H20
U 1 1 6165CB78
P 8550 4550
F 0 "H20" V 8787 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 8696 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 8550 4550 50  0001 C CNN
F 3 "~" H 8550 4550 50  0001 C CNN
	1    8550 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	8450 4550 8450 4850
$Comp
L Mechanical:MountingHole_Pad H21
U 1 1 6165ECEE
P 9200 4550
F 0 "H21" V 9437 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 9346 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 9200 4550 50  0001 C CNN
F 3 "~" H 9200 4550 50  0001 C CNN
	1    9200 4550
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H22
U 1 1 6165ECF4
P 9400 4550
F 0 "H22" V 9637 4553 50  0000 C CNN
F 1 "MountingHole_Pad" V 9546 4553 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 9400 4550 50  0001 C CNN
F 3 "~" H 9400 4550 50  0001 C CNN
	1    9400 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	9300 4550 9300 4850
$Comp
L Mechanical:MountingHole_Pad H23
U 1 1 61661158
P 10150 4600
F 0 "H23" V 10387 4603 50  0000 C CNN
F 1 "MountingHole_Pad" V 10296 4603 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 10150 4600 50  0001 C CNN
F 3 "~" H 10150 4600 50  0001 C CNN
	1    10150 4600
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H24
U 1 1 6166115E
P 10350 4600
F 0 "H24" V 10587 4603 50  0000 C CNN
F 1 "MountingHole_Pad" H 10496 4603 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 10350 4600 50  0001 C CNN
F 3 "~" H 10350 4600 50  0001 C CNN
	1    10350 4600
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H15
U 1 1 61663B1F
P 7050 4350
F 0 "H15" V 7287 4353 50  0000 C CNN
F 1 "MountingHole_Pad" V 7196 4353 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 7050 4350 50  0001 C CNN
F 3 "~" H 7050 4350 50  0001 C CNN
	1    7050 4350
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H16
U 1 1 61663B25
P 7250 4350
F 0 "H16" V 7487 4353 50  0000 C CNN
F 1 "MountingHole_Pad" V 7396 4353 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 7250 4350 50  0001 C CNN
F 3 "~" H 7250 4350 50  0001 C CNN
	1    7250 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 4350 7150 5350
$Comp
L Mechanical:MountingHole TL1
U 1 1 615964DE
P 900 950
F 0 "TL1" H 1000 996 50  0000 L CNN
F 1 "MountingHole" H 1000 905 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 900 950 50  0001 C CNN
F 3 "~" H 900 950 50  0001 C CNN
	1    900  950 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole TR1
U 1 1 61597718
P 1450 950
F 0 "TR1" H 1550 996 50  0000 L CNN
F 1 "MountingHole" H 1550 905 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1450 950 50  0001 C CNN
F 3 "~" H 1450 950 50  0001 C CNN
	1    1450 950 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole BL1
U 1 1 6159B4AD
P 900 1200
F 0 "BL1" H 1000 1246 50  0000 L CNN
F 1 "MountingHole" H 1000 1155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 900 1200 50  0001 C CNN
F 3 "~" H 900 1200 50  0001 C CNN
	1    900  1200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole BR1
U 1 1 6159B4B3
P 1450 1200
F 0 "BR1" H 1550 1246 50  0000 L CNN
F 1 "MountingHole" H 1550 1155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 1450 1200 50  0001 C CNN
F 3 "~" H 1450 1200 50  0001 C CNN
	1    1450 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 4600 10250 4700
Wire Wire Line
	10200 4850 10200 4700
Wire Wire Line
	10200 4700 10250 4700
Connection ~ 10250 4600
Wire Wire Line
	8450 5350 9300 5350
Connection ~ 9300 5350
Wire Wire Line
	9300 5350 10200 5350
Text Notes 1650 700  0    50   ~ 0
Mounting Equipment
Wire Notes Line
	700  1450 700  550 
Wire Notes Line
	700  550  2500 550 
Wire Notes Line
	2500 550  2500 1450
Wire Notes Line
	2500 1450 700  1450
Wire Notes Line
	700  1500 10750 1500
Wire Notes Line
	10750 1500 10750 5900
Wire Notes Line
	10750 5900 700  5900
Wire Notes Line
	700  1500 700  6000
Text Notes 8900 2150 0    197  ~ 0
Circuit
$EndSCHEMATC

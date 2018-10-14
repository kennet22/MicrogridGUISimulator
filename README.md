# MicrogridGUISimulator
This repository features a CCS project to simulate sending data via UART over Bluetooth. A Matlab App Designer project file is also in the repository connect the Bluetooth device and is responsible for processing controls and displaying data.


Steps to run the Simulator:

1. Download CCS & Matlab

2. Import Existing Project into CCS

3. Upload CCS project onto an MCU (MSP430G2553 used for prototyping)

4. Connect MCU through Bluetooth (BlueSMiRF RN-42)

5. Open Matlab and PL3GUI.mlapp through Matlab

6. In 'Code View', set app.BT equal to the Remote ID of your Bluetooth device

7. Run GUI

8. Click 'Connect' to enable Bluetooth data stream

9. Turn 'On' main power switch to begin showing the streamed data

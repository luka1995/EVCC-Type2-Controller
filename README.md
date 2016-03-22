# EVCC TYPE2 Controller

Created by Luka Penger and Duško Cvijič

Avant Car d.o.o.

## Description

The EVCC module controls the power-supply of an electric vehicle (EV) while charging at a supply equipment (EVSE). The communication between EV and EVSE complies with IEC 61851-1 Mode 3.
Potential-free switching contacts are used for load control (EV power-supply) and locking-device (for type 2 socket-outlet). Two external LEDs can be connected without additional resistors and indicate the present system status. Default and available charging current can be set via the RS485 serial interface. The available charging current can be changed while charging, limited by the default current and the rated current of the charging cable (type 2 socket-outlet only).
Start of the charging process and monitoring of the locking mechanism of the socket outlet are controlled by external switches directly connected to the module.
Using the RS485-interface several EVCC can be connected as network with an individual address assigned to each module.

Connector type 2: SAE J1772</br>
Type 2 standard: IEC 61851-1 Mode 3
Serial interface: RS485, 38400Bd, 8 data bits, 1 stop bit, no parity</br>
-> No internal termination; use 120Ω-resistor to terminate both ends of network!

##Proximity voltages:

Max Current
13 A --> Vadc = 2,57 V (R = 1550 ohms)
20 A --> Vadc = 2,22 V (R = 680 ohms)
32 A --> Vadc = 1,82 V (R = 220 ohms)
63 A --> Vadc = 1,66 V (R = 100 ohms)

Not connected --> Vadc = 3.3 V (Max 6A)
# RFID-Based Hospital Medicine Cabinet Lock System Using LPC2129

## 📌 Project Overview

This project implements an RFID-based access control system for a hospital medicine cabinet using the **LPC2129 ARM7 microcontroller**. The system authenticates RFID cards and controls cabinet access based on the scanned ID.

Authorized users can open the cabinet, and a push-button switch is used to initiate closing. Unauthorized RFID cards are denied access, and a buzzer provides an alert.

## 🎯 Objectives

- Implement RFID-based user authentication.
- Control cabinet opening and closing using a DC motor.
- Display system status and access messages on a 16×2 LCD.
- Provide an audible alert for unauthorized access.
- Learn UART communication, GPIO control, LCD interfacing, and embedded C programming.

## 🛠️ Hardware Components

- LPC2129 ARM7 microcontroller
- EM-18 RFID reader
- RFID cards or tags
- 16×2 LCD display
- L293D motor driver
- DC motor
- LED
- Buzzer
- Push-button switch
- Power supply and connecting wires

## 💻 Software and Tools

- Embedded C
- Keil µVision
- LPC2129 device header and register programming

## ⚙️ Working Principle

1. **System Initialization:** The microcontroller initializes GPIO, UART0, Timer0, and the LCD.
2. **RFID Scanning:** The EM-18 reader sends the scanned card ID to the LPC2129 through UART0.
3. **Authentication:** The received ID is compared with the predefined authorized IDs.
4. **Authorized Access:** The LCD displays an authorization message, and the motor runs to open the cabinet.
5. **Closing Operation:** After the cabinet opens, the system waits for the push-button switch. Pressing the switch initiates the closing operation.
6. **Unauthorized Access:** The LCD displays an access-denied message, and the buzzer sounds. The motor remains stopped.

## 🔌 Communication and Peripherals

- **UART0:** Receives RFID data from the EM-18 reader.
- **GPIO:** Controls the LCD, motor driver, LED, buzzer, and push-button switch.
- **Timer0:** Provides software delay timing.
- **LCD:** Displays authentication results and cabinet status.
- **L293D:** Controls the motor direction through two control inputs.

## 🔐 RFID Authentication

The program currently contains two authorized RFID IDs:

- `0003893303`
- `0987654321`

Replace these example IDs with the RFID IDs intended for your application.

## 📂 Project Structure

```text
RFID-Medicine-Cabinet/
├── main.c
└── README.md
```

The project may also include Keil project files, device configuration files, circuit diagrams, and demonstration images.

## 🚀 How to Use

1. Clone or download this repository.
2. Open the project in Keil µVision.
3. Configure the target device and project settings for the LPC2129.
4. Verify the UART baud rate and Timer0 clock settings.
5. Build the project and resolve any compiler errors.
6. Connect the RFID reader, LCD, motor driver, buzzer, LED, and switch according to the configured pin assignments.
7. Flash the compiled program to the microcontroller.
8. Scan an authorized RFID card and follow the LCD instructions.

## 🔮 Future Improvements

- Store authorized RFID IDs in non-volatile memory.
- Add an administrator mode to register or remove RFID cards.
- Add a door-position sensor to confirm whether the cabinet is open or closed.
- Add access logging with date and time.
- Improve access security and power-failure handling.

## 📚 Learning Outcomes

This project demonstrates practical use of ARM7 microcontroller programming, UART serial communication, GPIO interfacing, LCD control, timer-based delays, motor control, and RFID authentication.

## 👨‍💻 Author

**Embedded Systems Project**

Developed using Embedded C and the LPC2129 ARM7 microcontroller.

## 📄 License

This project is available for educational and learning purposes. Add a license file if you intend to specify permissions for reuse and distribution.

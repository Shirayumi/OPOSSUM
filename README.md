# 🦝 OPOSSUM
**Open-source PrOgrammable Smart System for Universal Measurements**

<!--------------------------------------------------------------------------->
## 📌 What is OPOSSUM?

**OPOSSUM** is a modular, portable, open-source device designed to simplify and automate electrical diagnostics and measurement. It is based on the [ESP32-3248S035R](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display) and [MIKROE-3116](/OUTSIDE_DOCUMENTATION/Multimeter_Click/Multimeter_Click_MIKROE-3116.pdf). OPOSSUM can take current, voltage, resistance, and capacitance measurements, allowing for data logging, and analysis. Inputs can be multiplexed with relay expansion modules.

<!--------------------------------------------------------------------------->
## ⚙️ Key Features

- 🔄 **Automated test sequencing** via JSON configuration files
- 🔌 **Universal connector interface** using M12 8-pin connector
- 🔧 **Modular input** with customizable adapters
- 📊 **Live readings, data logging**, and **go/no-go indication**
- 🖥️ **Touch display** for standalone operation
- 🔋 **Battery-powered** using removable 18650 cells
- 🧩 **Expansion modules** for custom functionality

<!--------------------------------------------------------------------------->
## Project Updates

### 13-MAY-25
- Getting the MIKROE-3116 soon so I can start making actual readings.
- Using an I2C-to-SPI bridge chip ([SC18IS604](OUTSIDE_DOCUMENTATION/SC18IS604/SC18IS604.pdf)) for coms to the MIKROE-3116.

### 03-MAY-25
- Created a simple GUI for changing channels. (0.3.0) <br>
[![OPOSSUM GUI update](https://img.youtube.com/vi/N5y217l4g18/0.jpg)](https://www.youtube.com/watch?v=N5y217l4g18)

<!--------------------------------------------------------------------------->
## 🔗 Quick Links
- [Project Roadmap](/roadmap.md)
- [To-do](/docs/to-do.md)

<!--------------------------------------------------------------------------->
## 📫 Contact
Questions? Ideas? File an [issue](https://github.com/Shirayumi/OPOSSUM/issues) or start a [discussion](https://github.com/Shirayumi/OPOSSUM/discussions).
 

Made by [Shirayumi](https://github.com/Shirayumi) and the open hardware community.


Special thanks to the [ESP32-Cheap-Yellow-Display Github](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display) for their massive collection of knowledge.

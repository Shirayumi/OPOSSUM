
# OPOSSUM Development To-Do List

## 1️⃣ Initial Research & Planning

- [x] Define **core testing functionalities** OPOSSUM must support
- [x] Finalize **connector selection** (M12 connector)
- [x] Research **multimeter interfaces** (M12 8-pin-->MIKROE-3116)
- [x] Select a **microcontroller** (ESP32-2432S028R)
- [x] Research **low-power, reliable switching** for expansion modules (relays/photorelays)

<!------------------------------------------------------------>
## 2️⃣ Firmware & Software Development

### Embedded Software (ESP32-3248S035R and MIKROE-3116)
- [x] Set up **firmware environment** (using ArduinoIDE for now)
- [x] Add **firmware versioning system**
- [ ] Develop **I2C communication with MIKROE-3116** (WIP-simulating MIKROE-3116)
- [ ] Create **touchscreen UI for manual testing** (WIP-using basic tft_eSPI)
- [ ] Develop **data logging functionality** to SD card

### Automated Testing & Configuration System
- [ ] Define **JSON test configuration file structure**
- [ ] Implement **script-based test sequencing**

<!------------------------------------------------------------>
## 3️⃣ Prototyping & Testing

### Main Unit
- [ ] Design **power supply circuit** (battery + charging system)
- [ ] Build **first full hardware prototype**
- [ ] Validate **everything works** via multi input testing

### Expansion Modules
- [ ] Define **standardized channel definitions**
- [ ] Implement **safe expansion module initialization and shutdown procedures**
- [ ] Develop **relay control logic and interlocks**
- [ ] Prototype an **expansion module**
- [ ] Validate **control logic, interlocks, and test sequencing**
- [ ] Validate **battery power performance** with expansion module

<!------------------------------------------------------------>
## 4️⃣ Hardware Design

- [x] Select **input/output expansion interface** (I2C and M12 connectors)
- [ ] Select **enclosure materials & form factor**

<!------------------------------------------------------------>
## 5️⃣ Refinements & Production

- [ ] Finalize **enclosure designs**
- [ ] Improve **user interface and testing workflow**
- [ ] Validate **automated test sequences**
- [ ] Conduct **stress testing** with real-world devices
- [ ] Develop **user documentation** and setup guides

<!------------------------------------------------------------>
## 6️⃣ Future Enhancements

### Extra Features
- [ ] Develop **automatic expansion module recognition** (Channel numbers per input)
- [ ] Add **Bluetooth/WiFi connectivity** for remote monitoring
- [ ] Implement **cloud-based test logging**
- [ ] Develop **PC-based test automation tools**
- [ ] Implement **USB test result export interface**
- [ ] Develop **visual test profile editor**
- [ ] Explore **machine learning-based anomaly detection** or other integrations

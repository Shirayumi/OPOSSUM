
# OPOSSUM Development To-Do List

## 1️⃣ Initial Research & Planning

- [x] Define **core testing functionalities** OPOSSUM must support
- [x] Finalize **connector selection** (standard + modular input panel decision)
- [x] Research **multimeter interfaces** (UART-controlled vs. direct measurement)
- [x] Select a **microcontroller** (ESP32, Raspberry Pi Zero, or alternative)
- [x] Research relay types for **low-power, reliable switching** (relays vs. SSRs)

## 2️⃣ Firmware & Software Development

### Embedded Software (ESP32-3248S035R)
- [ ] Set up **firmware environment**
- [ ] Add **firmware versioning system**
- [ ] Develop **SPI communication with multimeter**
- [ ] Create **touchscreen UI for manual testing**
- [ ] Develop **data logging functionality**

### Automated Testing & Configuration System
- [ ] Define **JSON configuration file structure**
- [ ] Implement **safe relay initialization and shutdown states**
- [ ] Develop **relay control logic and interlocks**
- [ ] Implement **script-based test sequencing**

## 3️⃣ Prototyping & Testing

- [ ] Build **first hardware prototype**
- [ ] Test **relay control logic** and interlocks
- [ ] Develop **fuse protection and safety features**

## 4️⃣ Hardware Design

- [ ] Design **power supply circuit** (battery + charging system)
- [ ] Develop **input/output expansion interface** (I2C)
- [ ] Select **enclosure materials & form factor**
- [ ] Validate **battery power performance**

## 5️⃣ Refinements & Production

- [ ] Finalize **enclosure design & assembly process**
- [ ] Improve **user interface and testing workflow**
- [ ] Validate **automated test sequences**
- [ ] Conduct **stress testing** with real-world devices
- [ ] Develop **user documentation** and testing guides

## 6️⃣ Future Enhancements

### Expansion Modules
- [ ] Define **expansion module use cases**
- [ ] Design **I2C expansion interface** and communication protocol
- [ ] Test **I2C expansion module recognition**
- [ ] Prototype an **expansion board**

### Extra Features
- [ ] Add **Bluetooth/WiFi connectivity** for remote monitoring
- [ ] Implement **cloud-based test logging**
- [ ] Develop **more expansion modules**
- [ ] Develop **PC-based test automation tools**
- [ ] Implement **USB test result export interface**
- [ ] Develop **visual test profile editor**
- [ ] Explore **machine learning-based anomaly detection** or other integrations

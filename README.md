# 🦝 OPOSSUM
**Open-source PrOgrammable Smart System for Universal Measurements**

---

## 📌 What is OPOSSUM?

**OPOSSUM** is a modular, portable, open-source device designed to simplify and automate **electrical diagnostics and measurement**. It routes power and signal lines through customizable relays, enabling remote-controlled testing, logging, and analysis of devices that use custom or industrial connectors.

Whether you're a technician, engineer, hobbyist, or educator, OPOSSUM helps you streamline complex test setups with repeatable, programmable workflows.

---

## ⚙️ Key Features

- 🔄 **Automated test sequencing** via JSON/YAML configuration files
- 🔌 **Universal connector interface** using 8-pin aviation connectors
- 🔧 **Modular input** with customizable plug adapters
- 📊 **Live readings, data logging**, and **go/no-go indicators**
- 🖥️ **Touch display** for standalone operation
- 🔋 **Battery-powered** using removable 18650 cells
- 🧩 **I2C expansion modules** for signal-level switching and more

---

## 🛠️ Project Structure

| Folder        | Description                                         |
|---------------|-----------------------------------------------------|
| [`docs/`](docs/)       | Markdown files for GitHub Pages site                |
| [`hardware/`](hardware/)   | PCB designs, schematics, and 3D enclosure files     |
| [`firmware/`](firmware/)   | ESP32/RPi microcontroller code                      |
| [`software/`](software/)   | PC-based config tools (CLI/GUI)                     |
| [`config/`](config/)     | YAML/JSON test profile examples                     |
| [`images/`](images/)     | Diagrams and project visuals                        |
| [`logs/`](logs/)       | Sample output and test logs                         |

---

## 🚀 Getting Started

### 🔧 Hardware Setup
- See [Hardware Overview](docs/hardware.md) for schematics, connector layouts, and fuse protection design.

### 💻 Firmware Installation
- Flash the microcontroller (ESP32-3248S035R) with code in [`/firmware`](firmware/).
- Configuration can be done using touchscreen or via USB-connected PC tool.

### 🧪 Run a Test Sequence
1. Connect your device to the aviation connector adapter.
2. Load or select a test profile (`.yaml` or `.json`) from `/config`.
3. Run the sequence and monitor results live or via USB logging.

See [Testing Guide](docs/testing.md) for step-by-step instructions.

---

## 🔄 Contributing

We welcome contributions! To get started:
- Read the [Contribution Guide](CONTRIBUTING.md)
- Check out [open issues](https://github.com/YOUR_USERNAME/OPOSSUM/issues)
- Submit a pull request or start a discussion

If you add a new expansion module, please document it in [`docs/expansion-modules.md`](docs/expansion-modules.md).

---

## 📅 Project Roadmap

See [`ROADMAP.md`](ROADMAP.md) for planned features including:
- ✅ Visual configuration tool (PC-based)
- 🔄 Wi-Fi syncing of test profiles
- 📦 Auto-export of logs to CSV

---

## 🧾 License

- **Software**: [MIT License](LICENSE)

By contributing to OPOSSUM, you agree to license your work under these terms.

---

## 📷 Screenshots

> _Add hardware photos, PCB layout screenshots, or UI mockups here to help users visualize the device._

---

## 📫 Contact

Questions? Ideas? File an [issue](https://github.com/Shirayumi/OPOSSUM/issues) or start a [discussion](https://github.com/Shirayumi/OPOSSUM/discussions).

---

Made by [Shirayumi](https://github.com/Shirayumi) and the open hardware community.

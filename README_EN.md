# CzhWidget

> Professional Communication Monitoring and Protocol Analysis Tool

CzhWidget is a modern communication monitoring and protocol analysis desktop application developed based on Qt 6. It provides real-time monitoring of serial port and network communications, multi-protocol parsing, data visualization and other features.


## 📋 Table of Contents

- [Features](#features)
- [Tech Stack](#tech-stack)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Development Guide](#development-guide)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## ✨ Features

### 📊 Core Features

| Module | Description |
|--------|-------------|
| **Real-time Monitoring** | Supports real-time data transmission and reception monitoring for Serial Port, TCP Client, TCP Server, and UDP communication methods |
| **Protocol Parsing** | Built-in industrial communication protocol parsing including Modbus RTU, Modbus TCP, CAN, CANopen |
| **Data Visualization** | Real-time waveform display of communication data trends |
| **Logging System** | Comprehensive logging and management with multi-level log filtering |
| **Automation Tools** | Practical tools for automatic sending, scheduled tasks, and data conversion |
| **Internationalization** | Supports Chinese, English, Japanese, and Korean UI languages |

### 🎨 UI Features

- Modern dark/light theme support
- Responsive layout design
- Smooth animation effects
- Intuitive card-based navigation

### 🔧 Technical Advantages

- **MVP Architecture**: Clear layered design for easy maintenance and expansion
- **Factory Pattern**: Flexible communication protocol and parser extension mechanism
- **Singleton Pattern**: Unified management of global services
- **Asynchronous Processing**: Efficient background data processing

## 🛠️ Tech Stack

### Core Frameworks

| Category | Technology | Version |
|----------|------------|---------|
| Framework | Qt | 6.8+ |
| Language | C++ | 20 |
| UI Library | ElaWidgetTools | Latest |
| Logging | Log4Qt | Latest |

### Supported Communication Protocols

- ✅ Serial Port
- ✅ TCP Client
- ✅ TCP Server
- ✅ UDP

### Supported Protocol Parsers

- ✅ Modbus RTU
- ✅ Modbus TCP
- ✅ CAN
- ✅ CANopen

## 🚀 Quick Start

### Prerequisites

- **OS**: Windows 10/11 (64-bit)
- **Qt Version**: Qt 6.8.3 or higher
- **Compiler**: MSVC 2022 or MinGW 11

### Installation Steps

1. **Clone Repository**
```bash
git clone https://github.com/CZHsoft/CzhWidget.git
cd CzhWidget
```

2. **Configure Dependencies**

Ensure the following dependencies are properly configured:
- Qt 6.8+ installed with environment variables configured
- ElaWidgetTools library placed in `ElaWidgetTools/` directory
- Log4Qt library placed in `Log4Qt/` directory

3. **Build Project**

Open `CzhWidget.pro` with Qt Creator, select Release build configuration and compile.

### Run Application

After successful compilation, run the generated executable `CzhWidget.exe`.

## 📁 Project Structure

```
CzhWidget/
├── ElaWidgetTools/          # ElaWidgetTools UI Library
├── Log4Qt/                  # Log4Qt Logging Framework
├── Models/                  # Data Model Layer
│   ├── AppModel.cpp/h       # Application Global Model
│   ├── MonitorModel.cpp/h   # Monitor Data Model
│   ├── AutoSendModel.cpp/h  # Auto Send Model
│   └── CommandTableModel.cpp/h # Command Table Model
├── Presenters/              # Presenter Layer
│   ├── MainPresenter.cpp/h  # Main Presenter
│   ├── MonitorPresenter.cpp/h # Monitor Presenter
│   └── AutoSendPresenter.cpp/h # Auto Send Presenter
├── Views/                   # View Layer
│   ├── MainWindow.cpp/h     # Main Window
│   ├── SplashWindow.cpp/h   # Splash Screen
│   └── Pages/               # Page Components
│       ├── DashboardPage.cpp/h    # Dashboard Page
│       ├── MonitorPage.cpp/h      # Monitor Page
│       ├── ProtocolPage.cpp/h     # Protocol Page
│       ├── ToolsPage.cpp/h        # Tools Page
│       ├── LogsPage.cpp/h         # Logs Page
│       ├── SettingPage.cpp/h      # Setting Page
│       └── AboutPage.cpp/h        # About Page
├── Services/                # Service Layer
│   ├── Comm/                # Communication Services
│   │   ├── CommManager.cpp/h      # Communication Manager
│   │   ├── CommFactory.cpp/h      # Communication Factory
│   │   ├── SerialComm.cpp/h       # Serial Communication
│   │   ├── TcpClientComm.cpp/h    # TCP Client
│   │   ├── TcpServerComm.cpp/h    # TCP Server
│   │   └── UdpComm.cpp/h          # UDP Communication
│   ├── Protocol/            # Protocol Parsing Services
│   │   ├── ProtocolParserFactory.cpp/h # Parser Factory
│   │   ├── ModbusRtuParser.cpp/h      # Modbus RTU Parser
│   │   ├── ModbusTcpParser.cpp/h      # Modbus TCP Parser
│   │   ├── CanParser.cpp/h            # CAN Parser
│   │   └── CanOpenParser.cpp/h        # CANopen Parser
│   ├── Log/                 # Log Services
│   │   └── LogService.cpp/h        # Log Manager
│   └── I18n/                # Internationalization Services
│       └── LanguageManager.cpp/h   # Language Manager
├── Widgets/                 # Custom Widgets
│   ├── FunctionNavCard.cpp/h   # Function Navigation Card
│   ├── WaveformPlotter.cpp/h   # Waveform Plotter
│   ├── ToggleSwitch.cpp/h      # Toggle Switch
│   └── ComboBox.cpp/h          # Combo Box
├── Resources/               # Resources
│   └── pic/                 # Image Resources
├── Tests/                   # Unit Tests
├── CzhWidget.pro            # Qt Project File
├── CzhWidget.iss            # Inno Setup Script
└── log4qt.properties        # Log4Qt Configuration
```

## 📖 Usage

### Basic Operation Flow

1. **Select Communication Mode**: Choose Serial Port, TCP or UDP in the Monitor page
2. **Configure Parameters**: Set port, baud rate, IP address, etc.
3. **Establish Connection**: Click connect button to establish communication
4. **Monitor Data**: View real-time transmitted/received data and waveforms
5. **Protocol Parsing**: Select protocol type in Protocol page for parsing

## 🔌 Development Guide

### Adding New Communication Protocol

1. Create new communication class in `Services/Comm/` directory, inheriting `ICommInterface`
2. Add protocol creation logic in `CommFactory.cpp`
3. Add new protocol enum in `CommProtocol.h`

### Adding New Protocol Parser

1. Create new parser class in `Services/Protocol/` directory, inheriting `ProtocolParser`
2. Register new parser in `ProtocolParserFactory.cpp`
3. Add new protocol type in `ProtocolType.h`

### Internationalization Support

1. Wrap translatable strings with `tr()` in code
2. Update `.ts` files using `lupdate` tool
3. Translate each language version using Qt Linguist
4. Generate `.qm` files using `lrelease`

## 🤝 Contributing

Welcome to contribute! Please follow these steps:

1. Fork the project
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Create Pull Request

### Code Guidelines

- Follow Qt coding conventions
- Use 4-space indentation
- File names use PascalCase
- Variable names use camelCase
- Class names use PascalCase

## 📄 License

This project is licensed under the **MIT License**. See [LICENSE](LICENSE) file for details.

## 📞 Contact

- **Project Home**: https://github.com/CZHsoft/CzhWidget
- **Author**: CZHsoft
- **Email**: chenandczh@163.com

---

**Special thanks to the following open source projects:**

- [Qt](https://www.qt.io/) - Powerful cross-platform application framework
- [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) - Modern Qt UI component library
- [Log4Qt](https://github.com/MEONMedical/Log4Qt) - Qt logging framework

---

*Made with ❤️ by CZHsoft*
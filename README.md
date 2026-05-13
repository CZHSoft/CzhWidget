# CzhWidget

> 专业的通信监控与协议分析工具

CzhWidget 是一款基于 Qt 6 开发的现代化通信监控与协议分析桌面应用程序，提供串口、网络通信实时监控、多协议解析、数据可视化等功能。


## 📋 目录

- [功能特性](#功能特性)
- [技术栈](#技术栈)
- [快速开始](#快速开始)
- [项目结构](#项目结构)
- [使用说明](#使用说明)
- [开发指南](#开发指南)
- [贡献指南](#贡献指南)
- [许可证](#许可证)
- [联系方式](#联系方式)

## ✨ 功能特性

### 📊 核心功能

| 模块 | 功能描述 |
|------|----------|
| **实时监控** | 支持串口、TCP Client、TCP Server、UDP 多种通信方式的实时数据收发监控 |
| **协议解析** | 内置 Modbus RTU、Modbus TCP、CAN、CANopen 等工业通信协议解析 |
| **数据可视化** | 通过波形图实时展示通信数据变化趋势 |
| **日志系统** | 完善的日志记录与管理，支持多级别日志过滤 |
| **自动化工具** | 支持自动发送、定时任务、数据转换等实用工具 |
| **国际化** | 支持中文、英文、日语、韩语四种语言界面 |

### 🎨 UI 特点

- 现代化深色/浅色主题支持
- 响应式布局设计
- 流畅的动画效果
- 直观的卡片式导航

### 🔧 技术优势

- **MVP 架构**：清晰的分层设计，便于维护和扩展
- **工厂模式**：灵活的通信协议和解析器扩展机制
- **单例模式**：全局服务的统一管理
- **异步处理**：高效的后台数据处理

## 🛠️ 技术栈

### 核心框架

| 类别 | 技术 | 版本 |
|------|------|------|
| 开发框架 | Qt | 6.8+ |
| 编程语言 | C++ | 20 |
| UI 组件库 | ElaWidgetTools | 最新 |
| 日志框架 | Log4Qt | 最新 |

### 支持的通信协议

- ✅ 串口通信（Serial Port）
- ✅ TCP Client
- ✅ TCP Server  
- ✅ UDP

### 支持的协议解析

- ✅ Modbus RTU
- ✅ Modbus TCP
- ✅ CAN
- ✅ CANopen

## 🚀 快速开始

### 环境要求

- **操作系统**：Windows 10/11 (64位)
- **Qt 版本**：Qt 6.8.3 或更高版本
- **编译器**：MSVC 2022 或 MinGW 11

### 安装步骤

1. **克隆项目**
```bash
git clone https://github.com/CzhWidget/CzhWidget.git
cd CzhWidget
```

2. **配置依赖**

确保以下依赖已正确配置：
- Qt 6.8+ 已安装并配置环境变量
- ElaWidgetTools 库已放入 `ElaWidgetTools/` 目录
- Log4Qt 库已放入 `Log4Qt/` 目录

3. **构建项目**

使用 Qt Creator 打开 `CzhWidget.pro` 项目文件，选择 Release 构建配置并编译。

### 运行项目

编译成功后，运行生成的可执行文件 `CzhWidget.exe`。

## 📁 项目结构

```
CzhWidget/
├── ElaWidgetTools/          # ElaWidgetTools UI组件库
├── Log4Qt/                  # Log4Qt日志框架
├── Models/                  # 数据模型层
│   ├── AppModel.cpp/h       # 应用全局模型
│   ├── MonitorModel.cpp/h   # 监控数据模型
│   ├── AutoSendModel.cpp/h  # 自动发送模型
│   └── CommandTableModel.cpp/h # 命令表模型
├── Presenters/              # 控制器层
│   ├── MainPresenter.cpp/h  # 主控制器
│   ├── MonitorPresenter.cpp/h # 监控控制器
│   └── AutoSendPresenter.cpp/h # 自动发送控制器
├── Views/                   # 视图层
│   ├── MainWindow.cpp/h     # 主窗口
│   ├── SplashWindow.cpp/h   # 启动闪屏
│   └── Pages/               # 页面组件
│       ├── DashboardPage.cpp/h    # 仪表盘首页
│       ├── MonitorPage.cpp/h      # 监控页面
│       ├── ProtocolPage.cpp/h     # 协议解析页面
│       ├── ToolsPage.cpp/h        # 工具页面
│       ├── LogsPage.cpp/h         # 日志页面
│       ├── SettingPage.cpp/h      # 设置页面
│       └── AboutPage.cpp/h        # 关于页面
├── Services/                # 服务层
│   ├── Comm/                # 通信服务
│   │   ├── CommManager.cpp/h      # 通信管理器
│   │   ├── CommFactory.cpp/h      # 通信工厂
│   │   ├── SerialComm.cpp/h       # 串口通信
│   │   ├── TcpClientComm.cpp/h    # TCP客户端
│   │   ├── TcpServerComm.cpp/h    # TCP服务端
│   │   └── UdpComm.cpp/h          # UDP通信
│   ├── Protocol/            # 协议解析服务
│   │   ├── ProtocolParserFactory.cpp/h # 解析器工厂
│   │   ├── ModbusRtuParser.cpp/h      # Modbus RTU解析
│   │   ├── ModbusTcpParser.cpp/h      # Modbus TCP解析
│   │   ├── CanParser.cpp/h            # CAN解析
│   │   └── CanOpenParser.cpp/h        # CANopen解析
│   ├── Log/                 # 日志服务
│   │   └── LogService.cpp/h        # 日志管理器
│   └── I18n/                # 国际化服务
│       └── LanguageManager.cpp/h   # 语言管理器
├── Widgets/                 # 自定义控件
│   ├── FunctionNavCard.cpp/h   # 功能导航卡片
│   ├── WaveformPlotter.cpp/h   # 波形绘制控件
│   ├── ToggleSwitch.cpp/h      # 开关控件
│   └── ComboBox.cpp/h          # 组合框控件
├── Resources/               # 资源文件
│   └── pic/                 # 图片资源
├── Tests/                   # 单元测试
├── CzhWidget.pro            # Qt项目文件
├── CzhWidget.iss            # Inno Setup安装脚本
└── log4qt.properties        # Log4Qt配置文件
```

## 📖 使用说明

### 基本操作流程

1. **选择通信方式**：在监控页面选择串口、TCP或UDP
2. **配置参数**：设置端口、波特率、IP地址等参数
3. **建立连接**：点击连接按钮建立通信
4. **监控数据**：实时查看收发数据和波形图
5. **协议解析**：在协议页面选择协议类型进行解析


## 🔌 开发指南

### 添加新的通信协议

1. 在 `Services/Comm/` 目录下创建新的通信类，继承 `ICommInterface`
2. 在 `CommFactory.cpp` 中添加协议创建逻辑
3. 在 `CommProtocol.h` 中添加新协议枚举

### 添加新的协议解析器

1. 在 `Services/Protocol/` 目录下创建新的解析器类，继承 `ProtocolParser`
2. 在 `ProtocolParserFactory.cpp` 中注册新解析器
3. 在 `ProtocolType.h` 中添加新协议类型

### 国际化支持

1. 在代码中使用 `tr()` 包裹需要翻译的字符串
2. 使用 `lupdate` 工具更新 `.ts` 文件
3. 使用 Qt Linguist 翻译各语言版本
4. 使用 `lrelease` 生成 `.qm` 文件

## 🤝 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

### 代码规范

- 遵循 Qt 编码规范
- 使用 4 空格缩进
- 文件名使用 PascalCase
- 变量名使用 camelCase
- 类名使用 PascalCase

## 📄 许可证

本项目采用 **MIT License** 开源许可证。详见 [LICENSE](LICENSE) 文件。

## 📞 联系方式

- **项目主页**：https://github.com/CZHsoft/CzhWidget
- **作者**：CZHsoft
- **邮箱**：chenandczh@163.com

---

**感谢以下开源项目的支持：**

- [Qt](https://www.qt.io/) - 强大的跨平台应用框架
- [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) - 现代化 Qt UI 组件库
- [Log4Qt](https://github.com/MEONMedical/Log4Qt) - Qt 日志框架

---

*Made with ❤️ by CZHsoft*
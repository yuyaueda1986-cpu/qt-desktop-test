# Project Specification: Dynamic Qt6 Desktop Application

## 1. Project Overview

`layout.json` を実行時にパースし、ネイティブウィジェットおよびカスタム図形を動的に配置・描画する Qt6 (C++17) アプリケーション。
`layout.json` の完全なスキーマ定義は [json_spec.md](json_spec.md) を参照。

ターゲット環境: **Linux (WSL2/WSLg 含む)**

## 2. Core Architectural Requirements

### 2.1 Window Management
- `window` オブジェクトの `title`, `width`, `height`, `background_color` に従いウィンドウを構成する。
- `--geometry WxH` コマンドライン引数で JSON のサイズをオーバーライド可能。
- `--fullscreen` で全画面起動をサポート。

### 2.2 Layout Engine
- レイアウトマネージャは不使用。全ウィジェットを `QWidget::setGeometry(x, y, width, height)` で絶対配置する。
- `widgets` 配列のインデックス順が Z-order となる (先頭が最背面、末尾が最前面)。

### 2.3 Dependencies & Build System
- **Language:** C++17
- **Build System:** CMake 3.16+
- **Libraries:** Qt6 Widgets

## 3. Supported Widget Types

`type` フィールドの値で分岐し、対応する Qt ウィジェットを生成する。

### A. ウィジェットタイプ

| type | Qt ウィジェット | props |
|------|----------------|-------|
| `Button` | `QPushButton` | `label`, `icon_name` |
| `Label` | `QLabel` | `label`, `font_size` |
| `Entry` | `QLineEdit` | `placeholder`, `text` |
| `Checkbox` | `QCheckBox` | `label`, `checked` |
| `Switch` | `QCheckBox` (styled) | `label`, `active` |
| `Combo` | `QComboBox` | `items` (カンマ区切り文字列), `active_index` |
| `Slider` | `QSlider` | `min`, `max`, `value`, `step` |
| `Spin` | `QSpinBox` | `min`, `max`, `value`, `step` |
| `Image` | `QLabel` + `QPixmap` | `file_path`, `alt_text` |
| `Progress` | `QProgressBar` | `value` (0.0-1.0), `show_text` |
| `Separator` | `QFrame` | `orientation` |

### B. 図形タイプ (QPainter によるカスタム描画)

| type | 描画方法 | props |
|------|---------|-------|
| `Line` | `drawLine` | `stroke_color`, `stroke_width`, `direction` |
| `Rect` | `drawRect` / `drawRoundedRect` | `fill_color`, `stroke_color`, `stroke_width`, `border_radius` |
| `Ellipse` | `drawEllipse` | `fill_color`, `stroke_color`, `stroke_width` |
| `Triangle` | `drawPolygon` (3点) | `fill_color`, `stroke_color`, `stroke_width`, `direction` |
| `Diamond` | `drawPolygon` (4点) | `fill_color`, `stroke_color`, `stroke_width` |
| `Arrow` | `drawLine` + `drawPolygon` | `stroke_color`, `stroke_width`, `direction` |
| `Star` | `drawPolygon` | `fill_color`, `stroke_color`, `stroke_width`, `points` |

## 4. Style

`style` オブジェクトは全ウィジェット共通で `background_color` と `color` の 2 プロパティのみ。
図形タイプでは `style` は `"transparent"` 固定とし、色情報は `props` 内の `fill_color` / `stroke_color` を使用する。

## 5. Events

`events` オブジェクトでシグナルとハンドラ名を紐付ける。

| type | イベントキー | Qt シグナル |
|------|-------------|------------|
| `Button` | `clicked` | `QPushButton::clicked` |
| `Checkbox` | `toggled` | `QCheckBox::stateChanged` |
| `Switch` | `toggled` | カスタム |
| `Combo` | `changed` | `QComboBox::currentIndexChanged` |
| `Slider` | `value_changed` | `QSlider::valueChanged` |
| `Spin` | `value_changed` | `QSpinBox::valueChanged` |
| `Entry` | `activate` | `QLineEdit::returnPressed` |

## 6. Source Structure

```
src/
  main.cpp          - エントリポイント、コマンドライン引数解析
  config.h/cpp      - JSON パーサ (WindowConfig, WidgetConfig, AppConfig)
  widgetfactory.h/cpp - ウィジェット生成ファクトリ
  shapewidget.h/cpp - 図形描画ウィジェット (QPainter)
  mainwindow.h/cpp  - メインウィンドウ、ウィジェット配置
```

# qt-desktop-test

`layout.json` を読み込み、ネイティブウィジェットとカスタム図形を動的に配置・描画する Qt6 デスクトップアプリケーション。

## 必要条件

- CMake 3.16以上
- Qt6 (Widgetsモジュール)
- C++17対応コンパイラ

## ビルド方法

```bash
cmake -S . -B build
cmake --build build
```

## 使い方

```bash
./build/qt-desktop-test --layout layout.json [OPTIONS]
```

### オプション

| オプション | 説明 |
|------------|------|
| `--layout FILE` | JSON レイアウトファイルのパス (**必須**) |
| `--geometry WxH` | ウィンドウサイズをオーバーライド (例: `--geometry 1024x768`) |
| `--fullscreen` | フルスクリーンモードで起動 |
| `--help` | ヘルプを表示 |

`--geometry` を指定しない場合、JSON の `window.width` / `window.height` がウィンドウサイズとして使用されます。

### キーボードショートカット

| キー | 動作 |
|------|------|
| F12 | プロパティダイアログを表示 |
| Ctrl+F12 | フルスクリーンモードを切り替え |

## 対応ウィジェットタイプ

### ネイティブウィジェット

Button, Label, Entry, Checkbox, Switch, Combo, Slider, Spin, Image, Progress, Separator

### 図形 (QPainter 描画)

Line, Rect, Ellipse, Triangle, Diamond, Arrow, Star

## JSON スキーマ

レイアウトファイルの完全なスキーマ定義は [docs/json_spec.md](docs/json_spec.md) を参照してください。

## ソース構成

```
src/
  main.cpp            - エントリポイント、コマンドライン引数解析
  config.h/cpp        - JSON パーサ
  widgetfactory.h/cpp - ウィジェット生成ファクトリ
  shapewidget.h/cpp   - 図形描画ウィジェット
  mainwindow.h/cpp    - メインウィンドウ
```

## 既知の制限事項

WSL2/WSLg 環境でフルスクリーンモードを使用する場合、非プライマリモニターでは正常に動作しない場合があります。これは WSLg の Wayland/X11 ブリッジの制限によるものです。

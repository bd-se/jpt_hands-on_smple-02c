# ToDo (C) — SAST/SCA 演習用サンプル

このリポジトリは、C 言語で書かれたシンプルな CLI ToDo アプリのサンプルです。学習目的で**意図的に脆弱性を含めていることがあります**。実運用・公開はしないでください。

（注: 現在ファイルは WSL 上の `/home/haruna/polaris-handson/polaris-hands-on-2` に配置されています。）

## 機能
- `todo add <task>` — タスクを追加（SQLite に保存）
- `todo view` — タスク一覧を表示
- `todo complete <id>` — 指定したタスクを完了にする

## Build
### Requirement:

事前に `sqlite3` の開発ライブラリが必要です（Ubuntu: `sudo apt install libsqlite3-dev`）。

### Pre build:

```sh
git clone https://github.com/bd-se/jpt_hands-on_smpl-02c
cd jpt_hands-on_smpl-02c
mkdir bin
```

### Build command:

```sh
make
```

### Usage:

```sh
./bin/todo add "short task"
./bin/todo view
./bin/todo complete 1
```

## Hands-On
### Coverity

```sh
cov-configure --gcc
cov-build --dir <imdir> make
cov-analyze --dir <imdir>
cov-commit-defects --dir <imdir> --stream <stream> --url <https://host:port> --user <username> --password <password>
```
### Black Duck (Detect)


## 補足

- `todo add` 実装内で**固定長バッファ (`char name[32]`) に対する `strcpy` の使用**があり、長い入力でバッファオーバーフローが発生します。これはSAST検出練習用に**意図的**に入れています。
- 将来的に SQL インジェクションやフォーマット文字列脆弱性のサンプルも追加する予定です。

## SCA（依存関係）練習
- `vendor/sqlite-amalgamation/` に古い SQLite のamalgamation を置くことで SCA ツールが警告を出すようにできます（実行環境に危険が無いことを確認してください）。

**注:** このリポジトリには現在 `vendor/sqlite-amalgamation/VERSION` と `vendor/sqlite-amalgamation/README.md`（SCA テスト用プレースホルダ）を追加しています。古い SQLite ソースを実際に配置して SCA 検出を試す場合は、配置後にリポジトリを外部に公開しないでください。

## 注意
このリポジトリの目的は学習のみです。脆弱なバージョンや脆弱性を含むビルドを外部に公開しないでください.

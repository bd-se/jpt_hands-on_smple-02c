# Manual smoke tests (WSL)

以下をWSL内で実行して、ビルドと基本操作、バッファオーバーフローの挙動確認を行ってください。

1) 依存のインストール

```sh
sudo apt update
sudo apt install -y build-essential libsqlite3-dev
```

2) ビルド

```sh
make
```

3) スモークテスト（基本操作）

```sh
./bin/todo add "short task"
./bin/todo view
./bin/todo complete 1
./bin/todo view
```

4) バッファオーバーフロー挙動の確認（意図的に長いタスク名を追加）

```sh
# 100 bytes の長い文字列を追加
LONG=$(printf '%0.sA' $(seq 1 100))
./bin/todo add "$LONG" || true
./bin/todo view
```

期待: 1) SAST ツール（後述）でこの `strcpy` は検出されるはずです。2) 実行時にクラッシュする場合があり、その場合は core dump などで解析できます（WSL の設定によりダンプ無効な場合あり）。

5) 推奨 SAST 実行例（オプション）

- cppcheck
  ```sh
  sudo apt install -y cppcheck
  cppcheck --enable=all --inconclusive --std=c11 src include
  ```

- clang static analyzer / clang-tidy
  ```sh
  sudo apt install -y clang-tidy
  clang-tidy src/*.c -- -Iinclude
  ```

- CodeQL (GitHub 上で実行する場合)：`.github/workflows/codeql-analysis.yml` を追加して CI で実行するのが簡単です。

6) SCA の練習用アプローチ

- `vendor/sqlite-amalgamation/` に既知脆弱な SQLite のソース（amalgamation）を配置すると、SCA ツール（Snyk, OWASP Dependency-Check, GitHub Dependabot）で検出できます。**注**: 脆弱なライブラリを置く場合は必ず README に「テスト目的で含めている」等の注意を書いて外部公開しないでください。

---

必要なら、テスト用のスクリプト（bash）や、CodeQL のワークフロー雛形を追加します。どちらにしますか？ (スクリプト / CodeQL / 両方 / なし)
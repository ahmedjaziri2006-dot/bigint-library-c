# 🔢 BigInt Library in C

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue.svg">
  <img src="https://img.shields.io/badge/topic-BigInt-green.svg">
  <img src="https://img.shields.io/badge/level-Algorithmic-orange.svg">
</p>

---

## 📌 Description

This project is a **C implementation of arbitrary-precision integers (BigInt)**.

It allows performing operations on integers that exceed the standard 32-bit or 64-bit limits, which is essential in fields like **cryptography (RSA)**.

📖 This project is based on an academic assignment focused on large integer manipulation .

---

## 🎯 Features

### ✅ Core operations

* Addition of large integers
* Subtraction
* Equality comparison
* Less-than comparison

### ⚙️ Advanced operations

* Naive multiplication
* Modulo operation
* Modular exponentiation (fast exponentiation)

---

## 🧠 How it works

A BigInt is represented using:

```c
typedef struct {
    int base;
    int *digits;
    int size;
    bool sign;
} BigInt;
```

👉 Each number is stored as:

* an array of digits
* a base (default = 10)
* a sign
* a size (number of digits)

This follows the idea of representing integers as arrays of digits, similar to polynomial representations .

---

## 🛠️ Compilation

```bash
gcc -Wall -Wextra -Werror -Wfatal-errors main.c -o bigint
```

---

## ▶️ Usage

Run the program:

```bash
./bigint
```

Then follow the prompts:

```text
A=12345
B=6789
ADDITION=19134
```

---

## 📊 Example

```text
A=123
B=456
ADDITION=579

A=1000
B=1
SUBTRACTION=999
```

---

## 🔐 Cryptography aspect

This project includes:

* Modular exponentiation:

```text
A^E mod N
```

👉 This is a key operation used in **RSA encryption**.

---

## ⚠️ Limitations

* Base is currently fixed to 10
* No Karatsuba multiplication (only naive)
* Memory management can be improved
* No full RSA implementation (only core math)

---

## 🚀 Possible improvements

* Implement Karatsuba multiplication
* Support different bases (100, 1000...)
* Add division algorithm
* Implement full RSA encryption/decryption
* Optimize memory usage

---

## 👨‍💻 Author

* Ahmed Jaziri

---

## 📜 License

Academic project (ESIEA).

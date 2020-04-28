# ⚡ Just Fast

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/cbf1179a0d3a42c19788210ec18efbac)](https://www.codacy.com/manual/pecorainpannacotta/just-fast?utm_source=github.com&utm_medium=referral&utm_content=GiuseppeCesarano/just-fast&utm_campaign=Badge_Grade)  

Just Fast is a CLI file manager with focus on speed in both execution times and usage. 🚀  
_Note that this is highly work in progress, so expect missing features and a lot of bugs._  

The only implemented part for now is the file explorer.  

## 📖 Info

Supported OS:

-   [x] MacOs 🍎
-   [x] Gnu/Linux 🐧
-   [ ] Windows

## Try Just Fast

If you have **git** and **cmake** installed you can use the following one line command to download and compile _Just Fast_.  
`git clone https://github.com/GiuseppeCesarano/just-fast.git && cd just-fast && cmake . -B build -DCMAKE_BUILD_TYPE=Release && cd build && cmake --build .`

## ⌨️ Keybindings

This is the complete list of default keybindings :

-   `j` or `↓`: Select next file
-   `k` or `↑`: Select previous file
-   `l` or `→`: Open selected file
-   `h` or `←`: Back to parent folder
-   `a`: Toggle hidden files
-   `q`: Quit

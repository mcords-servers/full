# MCords Base

[![Discord](https://img.shields.io/discord/1369798082195095592?label=Join%20MCords%20Discord&style=flat-square)](https://discord.gg/FdHhRYW66P)


**MCords Full** is a lightweight Minecraft proxy server written in C, designed to modify packets on the fly.  

---

> [!IMPORTANT]
> This project is licensed under the **GNU General Public License v2.0 (GPL-2.0)**.  
> You are free to use, modify, and distribute it under the terms of this license.

---

> [!WARNING]
> This version is intended to work **only on Linux**.

---

### 📂 Clone the Repository

```bash
git clone https://github.com/mcords-servers/full.git
```

---

### 🛠️ Compile mcords instance

To compile the project, run the included Python build script:

```bash
python3 build.py
```

This script will compile the source code and create the executable.

---

## FAQ  

#### Q: What is currently implemented?

A:
 - A proxy like system (Server<->Proxy<->Client)
    This can be easly toggled on by running script_cts function
 - Per fd memory managment
    You can easly allocate variables and link them to a certain fd by using mem_add(fd, ptr) function or storing vars by keyword using fds_set(fd, key, ptr) function
 - A requests header file that can be later on be used for authentification

#### Q: What is this project all about?

A: It's all about allowing minecraft players fully controll their own gmeplay and play on official minecraft servers for a single byte more than free

#### Q: When will the project be finished?

A: I can't say for sure but it will be realeased in beta in a few months if things keep going well :D

#### Q: Is this even legal?!

A: Yes It's fully legal, we don't sell any minecraft accounts we just allow players to see how it is to play with a license.

#### Q: Can I just steal your code and make my own monopoly with it?

A: You can as long as it remains OpenSource and respects **GPL-2.0 LICENSE**

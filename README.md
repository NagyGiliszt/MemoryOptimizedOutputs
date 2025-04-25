# MemoryOptimizedOutputs

**MemoryOptimizedOutputs** egy könnyű, dinamikusan méretezhető bitmezőkezelő osztály, amely kimeneti állapotok memóriahatékony tárolására és manipulálására szolgál.

---

## 🔧 Fő jellemzők

- ARDUINO környezethez!
- Bármennyi kimenet kezelése (1 - 1000+ között)
- Kimenetek be- és kikapcsolása
- Kimenet állapotának lekérdezése
- Minden kimenet egyszerre történő állítási lehetősége
- Bitmező visszaadása belső tároló változtatása nélkül:
  - Eredeti sorrendben (LSB → MSB)
  - Fordított sorrendben (MSB → LSB)
- Bitmező megfordítása (LSB → MSB) a belső tárolón

---

## 📦 Telepítés

Másold a `MemoryOptimizedOutputs.h` és `MemoryOptimizedOutputs.cpp` fájlokat a projekted könyvtárába, majd:

```cpp
#include "MemoryOptimizedOutputs.h"
```

## 🔧 Használat
[Wokwi példa](https://wokwi.com/projects/428724545689331713)

```cpp
#define OUTPUT_COUNT 16
#include <MemoryOptimizedOutputs.h>

MemoryOptimizedOutputs outputs(OUTPUT_COUNT);

outputs.setOnOff(3, true);          // Bekapcsolja a 3. kimenetet
bool current = outputs.getState(3); // Lekérdezi az állapotát
```
## 🧠 API Referencia

| Függvény | Leírás |
|----------|--------|
| `setOnOff(uint8_t pin, bool state)` | Egy adott pin be-/kikapcsolása |
| `getState(uint8_t pin)` | A pin aktuális állapotának lekérdezése |
| `toggle(uint8_t pin)` | A pin aktuális állapotának átbillentése |
| `setAll(bool state)` | Az összes kimenet be-/kikapcsolása |
| `getRawData(uint8_t* buffer, size_t size)` | Bitmező lekérése LSB → MSB sorrendben |
| `getRawDataReversed(uint8_t* buffer, size_t size)` | Bitmező lekérése fordított sorrendben (MSB → LSB) |
| `printStates()` | Soros portra kiírja az összes állapotot |
| `getBitfield()` | Bitmező állapota stringben |
| `getReversedBitfield()` | Bitmező fordított állapota stringben (LSB→MSB) |
| `reverseBits()` | Belső bitmező megfordítása (LSB→MSB), (MSB→LSB). Valódi forgatás, a változás a belső tárolón történik! |
| `getTypeStr()` | Dinamikus bitmező típusa |

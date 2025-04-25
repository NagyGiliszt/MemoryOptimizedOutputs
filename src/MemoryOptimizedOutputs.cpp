#include "MemoryOptimizedOutputs.h"


/**
 * @brief Konstruktor.
 * 
 * @param outputCount A kezelni kívánt bináris kimenetek száma.
 * 
 * @code
 * #define OUTPUT_COUNT 16
 * #include "MemoryOptimizedOutputs.h"
 * 
 * MemoryOptimizedOutputs outputs(OUTPUT_COUNT);
 * @endcode
 */
MemoryOptimizedOutputs::MemoryOptimizedOutputs(uint8_t outputCount) {

  _outputCount = outputCount;

  if (_outputCount <= 8) {
    storageType = U8;
    states.u8 = 0;
  } else if (_outputCount <= 16) {
    storageType = U16;
    states.u16 = 0;
  } else if (_outputCount <= 32) {
    storageType = U32;
    states.u32 = 0;
  } else if (_outputCount <= 64) {
    storageType = U64;
    states.u64 = 0;
  } else {
    storageType = DYN;
    uint8_t size = (_outputCount + 7) / 8;
    states.dyn = new uint8_t[size]();
  }
}
/**
 * @brief Destruktor.
 */
MemoryOptimizedOutputs::~MemoryOptimizedOutputs() {
  if (storageType == DYN) {
    delete[] states.dyn;
  }
}

/**
 * @brief Kimenet be- vagy kikapcsolása.
 * 
 * @param pin A kimeneti pozíció (1-től indulva).
 * @param state true = bekapcsol, false = kikapcsol
 * @return true Sikeres beállítás
 * @return false Hibás pin (tartományon kívül)
 */
bool MemoryOptimizedOutputs::setOnOff(uint8_t pin, bool state) {
  if (pin == 0 || pin > _outputCount) return false;
  pin -= 1;

  switch (storageType) {
    case U8:
      if (state) states.u8 |= (1 << pin);
      else states.u8 &= ~(1 << pin);
      break;
    case U16:
      if (state) states.u16 |= (1 << pin);
      else states.u16 &= ~(1 << pin);
      break;
    case U32:
      if (state) states.u32 |= (1UL << pin);
      else states.u32 &= ~(1UL << pin);
      break;
    case U64:
      if (state) states.u64 |= (1ULL << pin);
      else states.u64 &= ~(1ULL << pin);
      break;
    case DYN:
      states.dyn[pin / 8] = state ?
        (states.dyn[pin / 8] | (1 << (pin % 8))) :
        (states.dyn[pin / 8] & ~(1 << (pin % 8)));
      break;
  }
  return true;
}
/**
 * @brief Kimenet állapotának lekérdezése.
 * 
 * @param pin A lekérdezni kívánt pin (1-től indulva).
 * @return true = bekapcsolva, false = kikapcsolva
 */
bool MemoryOptimizedOutputs::getState(uint8_t pin) {
  if (pin == 0 || pin > _outputCount) return false;
  pin -= 1;

  switch (storageType) {
    case U8:
      return (states.u8 & (1 << pin)) != 0;
    case U16:
      return (states.u16 & (1 << pin)) != 0;
    case U32:
      return (states.u32 & (1UL << pin)) != 0;
    case U64:
      return (states.u64 & (1ULL << pin)) != 0;
    case DYN:
      return (states.dyn[pin / 8] & (1 << (pin % 8))) != 0;
  }
  return false;
}

/**
 * @brief Minden kimenet be- vagy kikapcsolása egyszerre.
 * 
 * @param state true = összes bekapcsolása, false = kikapcsolása
 */
void MemoryOptimizedOutputs::setAll(bool state) {
  for (int i = 1; i <= _outputCount; i++) {
    setOnOff(i, state);
  }
}


/**
 * @brief Kimenet átbillentése ellentétes állapotra.
 * 
 * @param pin A kimeneti pozíció (1-től indulva).
 * @return true Sikeres beállítás
 * @return false Hibás pin (tartományon kívül)
 */ 
bool MemoryOptimizedOutputs::toggle(uint8_t pin) {
  if (pin == 0 || pin > _outputCount) return false;
  pin -= 1;

  switch (storageType) {
    case U8:
      states.u8 ^= (1 << pin);
      break;
    case U16:
      states.u16 ^= (1 << pin);
      break;
    case U32:
      states.u32 ^= (1UL << pin);
      break;
    case U64:
      states.u64 ^= (1ULL << pin);
      break;
    case DYN:
      states.dyn[pin / 8] ^= (1 << (pin % 8));
      break;
  }
  return true;
}

/**
 * @brief Bitmező állapota stringben,  Serial.println(outputs.getBitfield());
 * 
 * @return String 
 */
String MemoryOptimizedOutputs::getBitfield() {
  String result;

  for (int i = _outputCount - 1; i >= 0; i--) {
    result += getState(i + 1) ? '1' : '0';
  }

  return result;
}


/**
 * @brief Soros portra kiírja az összes állapotot: outputs.printStates();
 * 
 * @param out
 */
void MemoryOptimizedOutputs::printStates(Stream &out) {
  out.print(F("Output States ["));
  out.print(_outputCount);
  out.print(F("]: "));
  out.println(getBitfield());
}

/**
 * @brief Belső bitmező megfordítása (LSB→MSB), (MSB→LSB). Valódi forgatás, a változás a belső tárolón történik!.
 */
void MemoryOptimizedOutputs::reverseBits() {
  int n = _outputCount;
  for (int i = 0; i < n / 2; ++i) {
    bool low = getState(i + 1);
    bool high = getState(n - i);

    setOnOff(i + 1, high);
    setOnOff(n - i, low);
  }
}
/**
 * @brief Bitmező fordított állapota stringben (LSB→MSB),  Serial.println(outputs.getReversedBitfield());
 * 
 * @return String 
 */
String MemoryOptimizedOutputs::getReversedBitfield() {
  String result;

  for (int i = 0; i < _outputCount; i++) {
    result += getState(i + 1) ? '1' : '0';
  }

  return result;
}

/**
 * @brief Belső bitmező lekérése eredeti sorrendben (LSB→MSB).
 * 
 * @param buffer Célbuffer pointere
 * @param size Buffer mérete (byte)
 */
void MemoryOptimizedOutputs::getRawData(uint8_t *buffer, size_t bufferSize) {
  memset(buffer, 0, bufferSize);

  for (uint8_t i = 0; i < _outputCount; i++) {
    if (getState(i + 1)) {
      buffer[i / 8] |= (1 << (i % 8));
    }
  }
}


/**
 * @brief Belső bitmező lekérése fordított sorrendben (MSB→LSB). Változás a belső tárolón NEM történik!
 * 
 * @param buffer Célbuffer pointere
 * @param size Buffer mérete (byte)
 */
void MemoryOptimizedOutputs::getRawDataReversed(uint8_t *buffer, size_t bufferSize) {
  memset(buffer, 0, bufferSize); // Minden bit 0, induláskor

  for (uint8_t i = 0; i < _outputCount; i++) {
    if (getState(i + 1)) {
      uint8_t revIndex = _outputCount - i - 1;
      buffer[revIndex / 8] |= (1 << (revIndex % 8));
    }
  }
}
/**
 * @brief Dinamikus bitmező típusa
 * 
 * @return const char* 
 */
const char* MemoryOptimizedOutputs::getTypeStr() {
  switch (storageType) {
    case U8: return "uint8_t";
    case U16: return "uint16_t";
    case U32: return "uint32_t";
    case U64: return "uint64_t";
    case DYN: return "uint8_t[]";
    default: return "unknown";
  }
}

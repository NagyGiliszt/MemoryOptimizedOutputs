#ifndef MemoryOptimizedOutputs_h
#define MemoryOptimizedOutputs_h

#include <Arduino.h>

/**
 * @class MemoryOptimizedOutputs
 * @brief Kimeneti bitmező osztály, memóriahatékony bináris vezérléshez.
 */
class MemoryOptimizedOutputs {
  public:
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
    MemoryOptimizedOutputs(uint8_t outputCount);
    /**
     * @brief Destruktor.
     */
    ~MemoryOptimizedOutputs();

    /**
     * @brief Kimenet be- vagy kikapcsolása.
     * 
     * @param pin A kimeneti pozíció (1-től indulva).
     * @param state true = bekapcsol, false = kikapcsol
     * @return true Sikeres beállítás
     * @return false Hibás pin (tartományon kívül)
     */
    bool setOnOff(uint8_t pin, bool state);

    /**
     * @brief Kimenet állapotának lekérdezése.
     * 
     * @param pin A lekérdezni kívánt pin (1-től indulva).
     * @return true = bekapcsolva, false = kikapcsolva
     */
    bool getState(uint8_t pin);

    /**
     * @brief Minden kimenet be- vagy kikapcsolása egyszerre.
     * 
     * @param state true = összes bekapcsolása, false = kikapcsolása
     */
    void setAll(bool state);

    /**
     * @brief Kimenet átbillentése ellentétes állapotra.
     * 
     * @param pin A kimeneti pozíció (1-től indulva).
     * @return true Sikeres beállítás
     * @return false Hibás pin (tartományon kívül)
     */    
    bool toggle(uint8_t pin);

    /**
     * @brief Belső bitmező lekérése eredeti sorrendben (LSB→MSB).
     * 
     * @param buffer Célbuffer pointere
     * @param size Buffer mérete (byte)
     */
    void getRawData(uint8_t *buffer, size_t bufferSize);

    /**
     * @brief Belső bitmező lekérése fordított sorrendben (MSB→LSB). Változás a belső tárolón NEM történik!
     * 
     * @param buffer Célbuffer pointere
     * @param size Buffer mérete (byte)
     */
    void getRawDataReversed(uint8_t *buffer, size_t bufferSize);

    /**
     * @brief Belső bitmező megfordítása (LSB→MSB), (MSB→LSB). Valódi forgatás, a változás a belső tárolón történik!.
     */
    void reverseBits();

    /**
     * @brief Soros portra kiírja az összes állapotot: outputs.printStates();
     * 
     * @param out
     */
    void printStates(Stream &out = Serial);

    /**
     * @brief Bitmező állapota stringben,  Serial.println(outputs.getBitfield());
     * 
     * @return String 
     */
    String getBitfield();

    /**
     * @brief Bitmező fordított állapota stringben (LSB→MSB),  Serial.println(outputs.getReversedBitfield());
     * 
     * @return String 
     */
    String getReversedBitfield();


    /**
     * @brief Dinamikus bitmező típusa
     * 
     * @return const char* 
     */
    const char* getTypeStr();

  private:
    uint8_t _outputCount;   // Összes kimenet száma

    union {
      uint8_t  u8;
      uint16_t u16;
      uint32_t u32;
      uint64_t u64;
      uint8_t* dyn;
    } states;                // Dinamikus bitmező típusa

    enum StorageType {
      U8, U16, U32, U64, DYN
    } storageType;
};

#endif

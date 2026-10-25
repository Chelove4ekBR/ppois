#pragma once

#include <cstdint>
#include "ByteSize.hpp"

/**
 * @brief Представляет сегмент адресного пространства памяти с флагами доступа RWX.
 */
class MemorySegment {
public:
    /**
     * @brief Создаёт сегмент памяти.
     * @param startAddress Начальный виртуальный/физический адрес.
     * @param size Размер сегмента.
     * @param readable Флаг разрешения чтения.
     * @param writable Флаг разрешения записи.
     * @param executable Флаг разрешения выполнения кода.
     */
    MemorySegment(uint64_t startAddress, ByteSize size,
                  bool readable = true, bool writable = true, bool executable = false);

    /**
     * @brief Возвращает начальный адрес сегмента.
     * @return Адрес в виде 64-битного числа.
     */
    uint64_t startAddress() const { return startAddress_; }

    /**
     * @brief Возвращает размер сегмента.
     * @return Объём ByteSize.
     */
    const ByteSize& size() const { return size_; }

    /**
     * @brief Проверяет факт аллокации сегмента.
     * @return true, если сегмент занят.
     */
    bool isAllocated() const { return allocated_; }

    /**
     * @brief Возвращает флаг чтения.
     * @return true, если чтение разрешено.
     */
    bool isReadable() const { return readable_; }

    /**
     * @brief Возвращает флаг записи.
     * @return true, если запись разрешена.
     */
    bool isWritable() const { return writable_; }

    /**
     * @brief Возвращает флаг исполнения кода.
     * @return true, если выполнение разрешено.
     */
    bool isExecutable() const { return executable_; }

    /**
     * @brief Рассчитывает конечный адрес сегмента (включительно).
     * @return Конечный адрес.
     */
    uint64_t endAddress() const;

    /**
     * @brief Помечает сегмент как аллоцированный.
     */
    void allocate();

    /**
     * @brief Освобождает сегмент.
     */
    void release();

    /**
     * @brief Проверяет права доступа к сегменту и выбрасывает исключение при нарушении.
     * @param needWrite Требуется ли запись.
     * @param needExec Требуется ли выполнение.
     */
    void checkPermission(bool needWrite, bool needExec) const;

    /**
     * @brief Проверяет пересечение диапазонов адресов с другим сегментом.
     * @param other Другой сегмент.
     * @return true, если диапазоны пересекаются.
     */
    bool overlaps(const MemorySegment& other) const;

    /**
     * @brief Проверяет, попадает ли указанный адрес внутрь сегмента.
     * @param addr Проверяемый адрес.
     * @return true, если адрес принадлежит сегменту.
     */
    bool containsAddress(uint64_t addr) const;

private:
    uint64_t startAddress_;
    ByteSize size_;
    bool allocated_;
    bool readable_;
    bool writable_;
    bool executable_;
};

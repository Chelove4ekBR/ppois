#pragma once

#include <cstdint>
#include <vector>

#include "ByteSize.hpp"
#include "MemorySegment.hpp"

/**
 * @brief Управляет виртуальной памятью, трансляцией адресов, сегментацией и файлом подкачки (swap).
 */
class VirtualMemoryManager {
public:
    /**
     * @brief Создаёт менеджер виртуальной памяти.
     * @param pageSize Размер страницы (обычно 4 KiB).
     * @param swapCapacity Объём области подкачки.
     */
    explicit VirtualMemoryManager(ByteSize pageSize = ByteSize::fromKiB(4),
                                  ByteSize swapCapacity = ByteSize::fromGiB(8));

    /**
     * @brief Возвращает размер страницы памяти.
     * @return Размер ByteSize.
     */
    const ByteSize& pageSize() const { return pageSize_; }

    /**
     * @brief Возвращает список сегментов памяти.
     * @return Вектор сегментов.
     */
    const std::vector<MemorySegment>& segments() const { return segments_; }

    /**
     * @brief Возвращает общий объём swap.
     * @return Объём ByteSize.
     */
    const ByteSize& swapCapacity() const { return swapCapacity_; }

    /**
     * @brief Возвращает объём используемого swap.
     * @return Объём ByteSize.
     */
    const ByteSize& usedSwap() const { return usedSwap_; }

    /**
     * @brief Выделяет новый виртуальный сегмент памяти с заданными правами доступа.
     * @param size Объём сегмента.
     * @param writable Разрешение на запись.
     * @param executable Разрешение на исполнение.
     * @return Начальный адрес созданного сегмента.
     */
    uint64_t allocateSegment(ByteSize size, bool writable, bool executable);

    /**
     * @brief Освобождает сегмент памяти по начальному адресу.
     * @param startAddress Начальный адрес.
     */
    void freeSegment(uint64_t startAddress);

    /**
     * @brief Обрабатывает ошибку отсутствия страницы (page fault), выделяя swap.
     * @param neededSize Необходимый объём.
     */
    void handlePageFault(ByteSize neededSize);

    /**
     * @brief Рассчитывает суммарный объём всех выделенных сегментов.
     * @return Суммарный объём ByteSize.
     */
    ByteSize totalAllocatedBytes() const;

    /**
     * @brief Находит сегмент памяти, содержащий указанный адрес.
     * @param addr Искомый адрес.
     * @return Константная ссылка на MemorySegment.
     */
    const MemorySegment& findSegment(uint64_t addr) const;

    /**
     * @brief Рассчитывает доступный остаток файла подкачки.
     * @return Доступный объём ByteSize.
     */
    ByteSize availableSwap() const;

private:
    ByteSize pageSize_;
    std::vector<MemorySegment> segments_;
    ByteSize swapCapacity_;
    ByteSize usedSwap_;
    uint64_t nextAddress_;
};
